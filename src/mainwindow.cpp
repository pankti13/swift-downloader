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
    resize(900, 450);
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

    stackedWidget->setCurrentWidget(progressPage);
}

void MainWindow::handleFormCancellation() {
    close();
}

void MainWindow::updateStatus(const QString &status) {
    int row = progressTable->rowCount() - 1; 
    progressTable->setItem(row, 3, new QTableWidgetItem(status));
}

void MainWindow::pauseDownload() {
    updateStatus("Paused");
}

void MainWindow::stopDownload() {
    updateStatus("Stopped");
}