#include "mainwindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    label = new QLabel("Test Label");

    // Set up layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);

    setWindowTitle("Swift Downloader");
    resize(700, 350);
}

MainWindow::~MainWindow() {
   
}
