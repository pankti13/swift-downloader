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

private:
    QStackedWidget *stackedWidget;
    InputForm *inputForm;
    QWidget *progressPage;
    QTableWidget *progressTable;
    QProgressBar *progressBar;
    QNetworkAccessManager *networkManager;
};

#endif
