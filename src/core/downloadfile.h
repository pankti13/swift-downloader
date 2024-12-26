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
    void mergeChunks();
    void handleChunkDownloaded(QByteArray data);
    QList<QByteArray> chunks;
    qint64 totalBytes;
    int activeThreads;
    QString getUrl() const { return url; } 
    QString getSavePath() const { return savePath; }
    void setSavePath(QString savePath) {this->savePath = savePath;}
    qint64 getBytesReceived() { return bytesReceived; }

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadStatusChanged(const QString &status); 
    void errorOccurred(const QString &error);

private:
    QString url;
    QString savePath;
    qint64 bytesReceived;
    QMutex chunkMutex;
    QMutex threadMutex;
    const int maxThreads = 5;

    QSemaphore semaphore;
};

#endif
