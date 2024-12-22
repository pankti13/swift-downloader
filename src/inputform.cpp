#include "inputform.h"
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QUrl>
#include <QRegExpValidator>

InputForm::InputForm(QWidget *parent)
    : QWidget(parent),
      urlLineEdit(new QLineEdit(this)),
      fileNameLineEdit(new QLineEdit(this)),
      saveLocationLineEdit(new QLineEdit(this)),
      okButton(new QPushButton("OK", this)),
      cancelButton(new QPushButton("Cancel", this)) {

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

    setLayout(mainLayout);
    urlLineEdit->setPlaceholderText("http://example.com");
    fileNameLineEdit->setPlaceholderText("Example Dataset");
    saveLocationLineEdit->setPlaceholderText("/home/user/Downloads");

    // Connect buttons
    connect(okButton, &QPushButton::clicked, [this]() {
        if (!validateInputs()) {
            return;
        }
        emit formSubmitted();
    });
    connect(cancelButton, &QPushButton::clicked, this, &InputForm::formCanceled);
}

bool InputForm::validateInputs() {
    QRegExp urlRegex("(https?|ftp)://[\\w\\-]+(\\.[\\w\\-]+)+[/#?]?.*");
    QRegExpValidator *urlValidator = new QRegExpValidator(urlRegex, this);
    urlLineEdit->setValidator(urlValidator);

    if (urlLineEdit->text().isEmpty() ||
        fileNameLineEdit->text().isEmpty() ||
        saveLocationLineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "All fields must be filled!");
        return false;
    }
    QUrl url(urlLineEdit->text());
    if (!url.isValid() || url.scheme().isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid URL.");
        return false;
    }
    return true;
}

QString InputForm::getUrl() const {
    return urlLineEdit->text();
}

QString InputForm::getFileName() const {
    return fileNameLineEdit->text();
}

QString InputForm::getSaveLocation() const {
    return saveLocationLineEdit->text();
}
