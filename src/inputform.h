#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class InputForm : public QWidget {
    Q_OBJECT

public:
    explicit InputForm(QWidget *parent = nullptr);
    bool validateInputs();
    void browseForSaveLocation();

    QString getUrl() const;
    QString getFileName() const;
    QString getSaveLocation() const;

signals:
    void formSubmitted();
    void formCanceled();

private:
    QLineEdit *urlLineEdit;
    QLineEdit *fileNameLineEdit;
    QLineEdit *saveLocationLineEdit;

    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *browseButton;
};

#endif
