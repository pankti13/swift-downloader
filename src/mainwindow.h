#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>
#include <QList>
#include <QSemaphore>

class InputForm;
class QWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleFormSubmission();
    void handleFormCancellation();

    void startDownload(const QString &url, const QString &fileName, const QString &saveLocation);
    void updateProgress(qint64 bytesReceived, qint64 bytesTotal);
    void updateStatus(const QString &status);
    void pauseDownload();
    void stopDownload();
    void calculateSpeed();
    qint64 parseSize(const QString &sizeStr);
    QString formatSize(qint64 size);
    QString formatTime(int seconds);

private:
    QStackedWidget *stackedWidget;
    InputForm *inputForm;
    QNetworkAccessManager *networkManager;

    QWidget *progressPage;
    QTableWidget *progressTable;
    QProgressBar *progressBar;

    qint64 lastBytesReceived = 0;
    QTimer *speedTimer;
    double downloadSpeed = 0.0;
    qint64 totalTime;
    QList<qint64> downloadSpeeds;
    const int maxSpeedHistory = 5;

    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *cancelButton;
    bool isPaused = false;
    bool isStopped = false;
    qint64 pausedBytesReceived = 0;

    qint64 totalBytes = 0;                              
    qint64 totalBytesReceived = 0;                      
    int maxThreads = 4;                               
    int activeThreads = 0;                              
    QList<QPair<qint64, qint64>> pausedChunks;         
    QSemaphore semaphore;                               
};

#endif
