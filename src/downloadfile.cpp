#include "downloadfile.h"
#include "downloadworker.h"
#include <QFile>
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
#include <QMutexLocker>

DownloadFile::DownloadFile(const QString &url, const QString &savePath, QObject *parent)
    : QObject(parent), url(url), savePath(savePath), totalBytes(0), bytesReceived(0), activeThreads(0) {
}

void DownloadFile::startDownload(const QString &url, const QString &savePath) {
    this->savePath = savePath;
    QSemaphore *semaphore = new QSemaphore(maxThreads);

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkReply *reply = manager->head(request);

    connect(reply, &QNetworkReply::finished, this, [this, url, reply, semaphore]() {
        if (reply->error() == QNetworkReply::NoError) {
            totalBytes = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
            qDebug() << "Total bytes from Content-Length header:" << totalBytes;
            qint64 chunkSize = totalBytes / maxThreads;

            for (int i = 0; i < maxThreads; ++i) {
                qint64 start = i * chunkSize;
                qint64 end = (i == maxThreads - 1) ? totalBytes - 1 : (start + chunkSize - 1);

                qDebug() << "Assigned chunk" << i << "start:" << start << "end:" << end;
                QThread *workerThread = new QThread;
                DownloadWorker *worker = new DownloadWorker(url, start, end, semaphore);
                worker->moveToThread(workerThread);

                connect(workerThread, &QThread::started, worker, &DownloadWorker::run);
                connect(worker, &DownloadWorker::chunkDownloaded, this, &DownloadFile::handleChunkDownloaded);
                connect(worker, &DownloadWorker::finished, workerThread, &QThread::quit);
                connect(workerThread, &QThread::finished, workerThread, &QObject::deleteLater);
                connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);

                {
                    QMutexLocker locker(&threadMutex);
                    ++activeThreads;
                }
                workerThread->start();
                qDebug() << "Starting worker thread for chunk" << i;
            }
        } else {
            qDebug() << "Error occurred while initiating download.";
            reply->deleteLater();
            return;
        }
        reply->deleteLater();
    });
}

void DownloadFile::handleChunkDownloaded(QByteArray data) {
    if (data.isNull() || data.isEmpty()) {
        qDebug() << "Error occurred while initiating download.";
        return;
    }
    {
        QMutexLocker locker(&chunkMutex);
        chunks.append(data);
        bytesReceived += data.size();
    }
    emit downloadProgress(bytesReceived, totalBytes);
    {
        QMutexLocker locker(&threadMutex);
        --activeThreads;
        if (activeThreads == 0) {
            qDebug() << "Merging chunks";
            mergeChunks();
        }
    }
}

void DownloadFile::mergeChunks() {
    QFile file(savePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qWarning() << "Failed to open file for writing:" << file.errorString();
        return;
    }
    {
        QMutexLocker locker(&chunkMutex);
        for (const QByteArray &chunk : chunks) {
            qDebug() << "Writing chunk of size" << chunk.size();
            file.write(chunk);
        }
        chunks.clear();
    }
    file.close();
}
