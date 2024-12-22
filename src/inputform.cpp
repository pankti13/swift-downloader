#include "inputform.h"
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QUrl>
#include <QRegExpValidator>
#include <QFileDialog>

InputForm::InputForm(QWidget *parent)
    : QWidget(parent),
      urlLineEdit(new QLineEdit(this)),
      fileNameLineEdit(new QLineEdit(this)),
      saveLocationLineEdit(new QLineEdit(this)),
      okButton(new QPushButton("OK", this)),
      cancelButton(new QPushButton("Cancel", this)),
      browseButton(new QPushButton("Browse", this)) {

    okButton->setIcon(QIcon(":/icons/ok_icon.png"));
    cancelButton->setIcon(QIcon(":/icons/cancel_icon.png"));

    QHBoxLayout *saveLocationLayout = new QHBoxLayout;
    saveLocationLayout->addWidget(saveLocationLineEdit);
    saveLocationLayout->addWidget(browseButton);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("URL:", urlLineEdit);
    formLayout->addRow("File Name:", fileNameLineEdit);
    formLayout->addRow("Save Location:", saveLocationLayout);

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
    connect(browseButton, &QPushButton::clicked, this, &InputForm::browseForSaveLocation);
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

void InputForm::browseForSaveLocation() {
    QString directory = QFileDialog::getExistingDirectory(this, "Select Save Location");
    if (!directory.isEmpty()) {
        saveLocationLineEdit->setText(directory);
    }
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
