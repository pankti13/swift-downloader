#include "mainwindow.h"
#include "inputform.h"
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDateTime>
#include <QPushButton>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      stackedWidget(new QStackedWidget(this)),
      inputForm(new InputForm(this)),
      progressPage(new QWidget(this)) {

    // Setup progress page
    QTableWidget *progressTable = new QTableWidget(progressPage);
    progressTable->setColumnCount(4);
    progressTable->setHorizontalHeaderLabels({"DateTime", "URL", "File Name", "Save Location"});
    progressTable->horizontalHeader()->setStretchLastSection(true);

    QVBoxLayout *progressLayout = new QVBoxLayout(progressPage);
    progressLayout->addWidget(progressTable);
    QPushButton *backButton = new QPushButton("Back", progressPage);
    progressLayout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(inputForm);
    });

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
    progressTable->setItem(row, 1, new QTableWidgetItem(url));
    progressTable->setItem(row, 2, new QTableWidgetItem(fileName));
    progressTable->setItem(row, 3, new QTableWidgetItem(saveLocation));

    stackedWidget->setCurrentWidget(progressPage);
}

void MainWindow::handleFormCancellation() {
    close();
}
