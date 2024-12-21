#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>  

class QLabel;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow {   
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void onOkClicked();
    void onCancelClicked();

    ~MainWindow();                         

private:
    QLineEdit *urlLineEdit;
    QLineEdit *fileNameLineEdit;
    QLineEdit *saveLocationLineEdit;
};

#endif
