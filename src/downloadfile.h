#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QThread>
#include <QSemaphore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMutex>

class DownloadFile : public QObject {
    Q_OBJECT

public:
    explicit DownloadFile(const QString &url, const QString &savePath, QObject *parent = nullptr);
    void startDownload(const QString &url, const QString &savePath);
    void handleDownloadRequest(const QString &url, long long start, long long end);
    void updateStatus(const QString &status);

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void errorOccurred(const QString &error);
    void finished();

private slots:
    void handleChunkDownloaded(QByteArray data);

private:
    void mergeChunks();

    QString url;
    QString savePath;
    QList<QByteArray> chunks;
    qint64 totalBytes;
    qint64 bytesReceived;
    int activeThreads;
    QMutex chunkMutex;
    QMutex threadMutex;
    const int maxThreads = 4;

    QSemaphore semaphore;
};

#endif
