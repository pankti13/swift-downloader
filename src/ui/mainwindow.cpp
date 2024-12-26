#include "mainwindow.h"
#include "inputform.h"
#include "downloadfile.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDateTime>
#include <QPushButton>
#include <QHeaderView>
#include <QProgressBar>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      inputForm(new InputForm(this)),
      progressPage(new QWidget(this)),
      networkManager(new QNetworkAccessManager(this)),
      speedTimer(new QTimer(this)) {

    inputForm->setObjectName("inputForm");
    // Setup progress page
    progressTable = new QTableWidget(progressPage);
    progressTable->setObjectName("progressTable");
    progressTable->setColumnCount(5);
    progressTable->setHorizontalHeaderLabels({"DateTime", "File Name", "Downloaded/Total", "Status", "Remaining Time"});
    progressTable->horizontalHeader()->setStretchLastSection(true);
    progressTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    progressBar = new QProgressBar(progressPage);
    progressBar->setObjectName("progressBar");
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    pauseButton = new QPushButton("Pause", progressPage);
    pauseButton->setObjectName("pauseButton");
    pauseButton->setIcon(QIcon(":/icons/pause_icon.png"));
    stopButton = new QPushButton("Stop", progressPage);
    stopButton->setObjectName("stopButton");
    stopButton->setIcon(QIcon(":/icons/stop_icon.png"));
    cancelButton = new QPushButton("Cancel", progressPage);
    cancelButton->setObjectName("cancelButton");
    cancelButton->setIcon(QIcon(":/icons/cancel_icon.png"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *progressLayout = new QVBoxLayout(progressPage);
    progressLayout->addWidget(progressTable);
    progressLayout->addWidget(progressBar);
    progressLayout->addLayout(buttonLayout);

    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseDownload);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopDownload);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::handleFormCancellation);
    connect(speedTimer, &QTimer::timeout, this, &MainWindow::calculateSpeed);

    // Setup stacked widget
    stackedWidget->addWidget(inputForm);
    stackedWidget->addWidget(progressPage);
    setCentralWidget(stackedWidget);

    // Connect signals from input form
    connect(inputForm, &InputForm::formSubmitted, this, &MainWindow::handleFormSubmission);
    connect(inputForm, &InputForm::formCanceled, this, &MainWindow::handleFormCancellation);
    resize(1200, 600);
    progressPage->setObjectName("progressPage");
}

MainWindow::~MainWindow() {}

void MainWindow::handleFormSubmission() {
    QString url = inputForm->getUrl();
    QString fileName = inputForm->getFileName();
    QString saveLocation = inputForm->getSaveLocation();

    QTableWidget *progressTable = progressPage->findChild<QTableWidget *>();
    int row = progressTable->rowCount();
    progressTable->insertRow(row);

    progressTable->setItem(row, 0, new QTableWidgetItem(
        QDateTime::currentDateTime().toString("ddd MMM dd yyyy HH:mm:ss")
    ));
    progressTable->setItem(row, 1, new QTableWidgetItem(fileName));
    progressTable->setItem(row, 2, new QTableWidgetItem("0/0"));
    progressTable->setItem(row, 3, new QTableWidgetItem("To Be Started"));
    progressTable->setItem(row, 4, new QTableWidgetItem("N/A"));

    startDownload(url, fileName, saveLocation);
    stackedWidget->setCurrentWidget(progressPage);
}

void MainWindow::handleFormCancellation() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, "Warning", 
                                 "This will close the window. Do you want to proceed?",
                                 QMessageBox::Ok | QMessageBox::Cancel);

    if (reply == QMessageBox::Ok) {
        close();
    }
}

void MainWindow::startDownload(const QString &url, const QString &fileName, const QString &saveLocation) {
    int row = progressTable->rowCount() - 1;
    if (row < 0) return;
    DownloadFile *downloadFile = new DownloadFile(url, saveLocation + "/" + fileName, this);

    connect(downloadFile, &DownloadFile::downloadProgress, this, [this, row](qint64 bytesReceived, qint64 bytesTotal) {
        if (progressBar && progressTable) {
            int progress = (bytesTotal > 0) ? static_cast<int>((bytesReceived * 100) / bytesTotal) : 0;
            progressBar->setValue(progress);

            QString readableDownloaded = formatSize(bytesReceived);
            QString readableTotal = formatSize(bytesTotal);
            progressTable->setItem(row, 2, new QTableWidgetItem(QString("%1/%2").arg(readableDownloaded, readableTotal)));
            progressTable->setItem(row, 3, new QTableWidgetItem("Downloading"));
        }
    });

    connect(downloadFile, &DownloadFile::downloadStatusChanged, this, [this, row](const QString &status) {
        QMetaObject::invokeMethod(this, [this, row, status] {
            progressTable->setItem(row, 3, new QTableWidgetItem(status));
        });
    });

    connect(downloadFile, &DownloadFile::errorOccurred, this, [this, row, downloadFile](const QString &error) {
        progressTable->setItem(row, 3, new QTableWidgetItem("Error Occurred"));
        delete downloadFile;
    });

    QString savePath = inputForm->getSaveLocation() + "/" +  inputForm->getFileName();
    downloadFile->startDownload(url, savePath);
}

void MainWindow::updateProgress(qint64 bytesReceived, qint64 bytesTotal) {
    if (isPaused || isStopped) return;
    qint64 totalBytesReceived = bytesReceived + pausedBytesReceived;

    if (progressBar && progressTable) {
        int progress = (bytesTotal > 0) ? static_cast<int>((totalBytesReceived * 100) / bytesTotal) : 0;
        progressBar->setValue(progress);

        int row = progressTable->rowCount() - 1; 
        QString readableDownloaded = formatSize(totalBytesReceived);
        QString readableTotal = formatSize(bytesTotal);
        progressTable->setItem(row, 2, new QTableWidgetItem(QString("%1/%2").arg(readableDownloaded, readableTotal)));
        progressTable->setItem(row, 3, new QTableWidgetItem("Downloading"));
        progressBar->show();
    }
}

QString MainWindow::formatSize(qint64 size) {
    if (size < 1024)
        return QString::number(size) + " B";
    if (size < 1024 * 1024)
        return QString::number(size / 1024.0, 'f', 2) + " KB";
    if (size < 1024 * 1024 * 1024)
        return QString::number(size / (1024.0 * 1024.0), 'f', 2) + " MB";
    return QString::number(size / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
}

void MainWindow::calculateSpeed() {
    if (isPaused || isStopped) return;
    int row = progressTable->rowCount() - 1;
    if (row < 0) return;

    QTableWidgetItem *item = progressTable->item(row, 2);
    QStringList downloadedTotal = item->text().split("/");
    if (downloadedTotal.size() != 2) return;

    qint64 bytesReceived = parseSize(downloadedTotal[0]);
    qint64 bytesTotal = parseSize(downloadedTotal[1]);

    if (bytesTotal == 0) return;
    if (totalTime == 0 && downloadSpeed > 0) {
        totalTime = static_cast<double>(bytesTotal) / downloadSpeed;
    }
    downloadSpeed = bytesReceived - lastBytesReceived;

    downloadSpeeds.append(downloadSpeed);
    if (downloadSpeeds.size() > maxSpeedHistory) {
        downloadSpeeds.pop_front();
    }
    qint64 smoothedSpeed = 0;
    for (qint64 speed : downloadSpeeds) {
        smoothedSpeed += speed;
    }
    smoothedSpeed /= downloadSpeeds.size();
    lastBytesReceived = bytesReceived;
    double remainingTime = (smoothedSpeed > 0) ? (bytesTotal - bytesReceived) / smoothedSpeed : 0;
    
    if (remainingTime < 0) {
        remainingTime = 0;
    }

    QString totalTimeStr = formatTime(static_cast<int>(totalTime));
    QString remainingTimeStr = formatTime(static_cast<int>(remainingTime));
    if (progressTable->item(row, 4)) {
        progressTable->setItem(row, 4, new QTableWidgetItem(
            QString("%1/%2").arg(remainingTimeStr, totalTimeStr)));
    }
}

qint64 MainWindow::parseSize(const QString &sizeStr) {
    QString size = sizeStr.trimmed();
    double value = size.left(size.size() - 2).toDouble();
    QString unit = size.right(2).trimmed();

    if (unit == "KB")
        return static_cast<qint64>(value * 1024);
    else if (unit == "MB")
        return static_cast<qint64>(value * 1024 * 1024);
    else if (unit == "GB")
        return static_cast<qint64>(value * 1024 * 1024 * 1024);
    else
        return static_cast<qint64>(value);
}

QString MainWindow::formatTime(int seconds) {
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;

    return QString("%1:%2:%3")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void MainWindow::pauseDownload() {
    int row = progressTable->rowCount() - 1;
    if (row < 0) return;

    if (isPaused) {
        isPaused = false;
        pauseButton->setText("Pause");
        pauseButton->setIcon(QIcon(":/icons/pause_icon.png"));

        QString url = inputForm->getUrl();
        QString fileName = inputForm->getFileName();
        QString saveLocation = inputForm->getSaveLocation();
        startDownload(url, fileName, saveLocation); 

    } else {
        pausedBytesReceived = lastBytesReceived; 
        isPaused = true;
        pauseButton->setText("Play");
        pauseButton->setIcon(QIcon(":/icons/play_icon.png"));
        networkManager->clearAccessCache(); 
    }
}

void MainWindow::stopDownload() {
    int row = progressTable->rowCount() - 1;
    if (row < 0) return;

    if (isStopped) {
        isStopped = false;
        stopButton->setText("Stop");
        stopButton->setIcon(QIcon(":/icons/stop_icon.png"));

        lastBytesReceived = 0;
        pausedBytesReceived = 0;
        progressBar->setValue(0); 
        totalTime = 0;
        QString url = inputForm->getUrl();
        QString fileName = inputForm->getFileName();
        QString saveLocation = inputForm->getSaveLocation();        
        startDownload(url, fileName, saveLocation); 

    } else {
        pausedBytesReceived = lastBytesReceived;
        isStopped = true;
        stopButton->setText("Retry");
        stopButton->setIcon(QIcon(":/icons/retry_icon.png"));
        networkManager->clearAccessCache(); 
    }
}
