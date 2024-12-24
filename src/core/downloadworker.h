#ifndef DOWNLOADWORKER_H
#define DOWNLOADWORKER_H

#include <QObject>
#include <QThread>
#include <QSemaphore>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DownloadWorker : public QObject {
    Q_OBJECT

public:
    DownloadWorker(const QString &url, long long start, long long end, QSemaphore *semaphore, QObject *parent = nullptr);
    void process();
    void run();

signals:
    void startDownloadRequest(const QString &url, long long start, long long end);
    void chunkDownloaded(QByteArray data);
    void downloadStatusChanged(const QString &status); 
    void finished();

private:
    QString url;
    qint64 start;
    qint64 end;
    QSemaphore *semaphore;
    QNetworkAccessManager networkManager;
};

#endif
