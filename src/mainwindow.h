#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class InputForm;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleFormSubmission();
    void handleFormCancellation();

private:
    QStackedWidget *stackedWidget;
    InputForm *inputForm;
    QWidget *progressPage;
};

#endif
