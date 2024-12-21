#include "mainwindow.h"
#include <QMessageBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QBoxLayout>
#include <QThread>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
 : QMainWindow(parent) {

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QLineEdit *urlLineEdit = new QLineEdit(this);
    QLineEdit *fileNameLineEdit = new QLineEdit(this);
    QLineEdit *saveLocationLineEdit = new QLineEdit(this);

    this->urlLineEdit = urlLineEdit;
    this->fileNameLineEdit = fileNameLineEdit;
    this->saveLocationLineEdit = saveLocationLineEdit;

    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    // Connect button signals to slots
    connect(okButton, &QPushButton::clicked, this, &MainWindow::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MainWindow::onCancelClicked);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("URL:", urlLineEdit);
    formLayout->addRow("File Name:", fileNameLineEdit);
    formLayout->addRow("Save Location:", saveLocationLineEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);  
    centralWidget->setLayout(mainLayout);

    resize(700, 350);
    setWindowTitle("Swift Downloader");
}

MainWindow::~MainWindow() {}

void MainWindow::onOkClicked() {
    QString url = urlLineEdit->text();
    QString fileName = fileNameLineEdit->text();
    QString saveLocation = saveLocationLineEdit->text();

    if (url.isEmpty() || fileName.isEmpty() || saveLocation.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled!");
        return;
    }
}

void MainWindow::onCancelClicked() {
    QMessageBox::information(this, "Canceled", "Operation canceled!");
    close();
}
