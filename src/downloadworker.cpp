#include "downloadworker.h"
#include <QFile>
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
#include <QMutexLocker>

DownloadWorker::DownloadWorker(const QString &url, long long start, long long end, 
                               QSemaphore *semaphore, QObject *parent)
    : QObject(parent), url(url), start(start), end(end), semaphore(semaphore) {}


void DownloadWorker::process() {
    QNetworkAccessManager networkManager;

    qDebug() << "Acquiring semaphore for chunk" << start;
    semaphore->acquire(); 
    qDebug() << "Semaphore acquired for chunk" << start;

    qDebug() << "Processing chunk from" << start << "to" << end;
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QString rangeHeader = QString("bytes=%1-%2").arg(start).arg(end);
    request.setRawHeader("Range", rangeHeader.toUtf8());

    QNetworkReply *reply = networkManager.get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        emit chunkDownloaded(data); 
    } else {
        qWarning() << "Error downloading chunk:" << reply->errorString();
    }

    reply->deleteLater(); 
    semaphore->release(); 
    qDebug() << "Semaphore released for chunk" << start;
    emit finished();
}

void DownloadWorker::run() {
    qDebug() << "Inside run for chunk" << start;
    process(); 
}
