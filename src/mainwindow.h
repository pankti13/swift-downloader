#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QLabel;

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); 
    ~MainWindow();                         

private:
    QLabel *label;
};

#endif // MAINWINDOW_H
