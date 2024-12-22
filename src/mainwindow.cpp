#include "mainwindow.h"
#include "inputform.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDateTime>
#include <QPushButton>
#include <QHeaderView>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      inputForm(new InputForm(this)),
      progressPage(new QWidget(this)),
      networkManager(new QNetworkAccessManager(this)) {

    // Setup progress page
    progressTable = new QTableWidget(progressPage);
    progressTable->setColumnCount(5);
    progressTable->setHorizontalHeaderLabels({"DateTime", "File Name", "Downloaded/Total", "Status", "Remaining Time"});
    progressTable->horizontalHeader()->setStretchLastSection(true);
    progressTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    progressBar = new QProgressBar(progressPage);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    QPushButton *pauseButton = new QPushButton("Pause", progressPage);
    QPushButton *stopButton = new QPushButton("Stop", progressPage);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(stopButton);

    QPushButton *backButton = new QPushButton("Back", progressPage);
    QVBoxLayout *progressLayout = new QVBoxLayout(progressPage);
    progressLayout->addWidget(progressTable);
    progressLayout->addWidget(progressBar);
    progressLayout->addLayout(buttonLayout);
    progressLayout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(inputForm);
    });

    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseDownload);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopDownload);

    // Setup stacked widget
    stackedWidget->addWidget(inputForm);
    stackedWidget->addWidget(progressPage);
    setCentralWidget(stackedWidget);

    // Connect signals from input form
    connect(inputForm, &InputForm::formSubmitted, this, &MainWindow::handleFormSubmission);
    connect(inputForm, &InputForm::formCanceled, this, &MainWindow::handleFormCancellation);
    resize(1200, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::handleFormSubmission() {
    QString url = inputForm->getUrl();
    QString fileName = inputForm->getFileName();
    QString saveLocation = inputForm->getSaveLocation();

    QTableWidget *progressTable = progressPage->findChild<QTableWidget *>();
    int row = progressTable->rowCount();
    progressTable->insertRow(row);

    progressTable->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString()));
    progressTable->setItem(row, 1, new QTableWidgetItem(fileName));
    progressTable->setItem(row, 2, new QTableWidgetItem("0/0"));
    progressTable->setItem(row, 3, new QTableWidgetItem("To Be Started"));
    progressTable->setItem(row, 4, new QTableWidgetItem("N/A"));

    startDownload(url, fileName, saveLocation);
    stackedWidget->setCurrentWidget(progressPage);
}

void MainWindow::handleFormCancellation() {
    close();
}

void MainWindow::updateStatus(const QString &status) {
    int row = progressTable->rowCount() - 1; 
    progressTable->setItem(row, 3, new QTableWidgetItem(status));
}

void MainWindow::startDownload(const QString &url, const QString &fileName, const QString &saveLocation) {
    QNetworkReply *reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::downloadProgress, this, &MainWindow::updateProgress);
    connect(reply, &QNetworkReply::finished, this, [this, reply, fileName, saveLocation]() {
        if (reply->error() == QNetworkReply::NoError) {
            QFile file(saveLocation + "/" + fileName);
            if (file.open(QIODevice::WriteOnly)) {
                file.write(reply->readAll());
                file.close();
            }
            updateStatus("Completed");
        } else {
            updateStatus("Error Occurred");
        }
        reply->deleteLater();
    });
}

void MainWindow::updateProgress(qint64 bytesReceived, qint64 bytesTotal) {
    if (progressBar && progressTable) {
        int progress = (bytesTotal > 0) ? static_cast<int>((bytesReceived * 100) / bytesTotal) : 0;
        progressBar->setValue(progress);

        int row = progressTable->rowCount() - 1; 
        progressTable->setItem(row, 2, new QTableWidgetItem(QString("%1/%2").arg(bytesReceived).arg(bytesTotal)));
        progressTable->setItem(row, 3, new QTableWidgetItem("Downloading"));
        progressBar->show();
    }
}

void MainWindow::pauseDownload() {
    updateStatus("Paused");
}

void MainWindow::stopDownload() {
    updateStatus("Stopped");
}