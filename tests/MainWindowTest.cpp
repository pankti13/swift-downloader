#include <gtest/gtest.h>
#include <QApplication>
#include <QProgressBar>
#include <QPushButton>
#include <QTableWidget>
#include "mainwindow.h"
#include "inputform.h"
#include "downloadfile.h"
#include "downloadworker.h"

class MainWindowTest : public ::testing::Test {
protected:
    QApplication *app;
    MainWindow *mainWindow;

    void SetUp() override {
    int argc = 0;
    char **argv = nullptr;
    app = new QApplication(argc, argv);
    mainWindow = new MainWindow();
}

    void TearDown() override {
        delete mainWindow;
        delete app;
    }
};

void testWidgetConstruction(QWidget *widget, const QString &text = "") {
    ASSERT_NE(widget, nullptr);
    if (auto button = qobject_cast<QPushButton *>(widget)) {
        EXPECT_EQ(button->text(), text);
    }
}

// Test construction of InputForm
TEST_F(MainWindowTest, InputFormConstruction) {
    testWidgetConstruction(mainWindow->findChild<InputForm *>("inputForm"));
}

// Test construction of progress page
TEST_F(MainWindowTest, ProgressTableConstruction) {
    QWidget *progressPage = mainWindow->findChild<QWidget *>("progressPage");
    ASSERT_NE(progressPage, nullptr);
    
    QTableWidget *progressTable = progressPage->findChild<QTableWidget *>("progressTable");
    ASSERT_NE(progressTable, nullptr);

    EXPECT_EQ(progressTable->columnCount(), 5);
    EXPECT_EQ(progressTable->horizontalHeaderItem(0)->text(), "DateTime");
    EXPECT_EQ(progressTable->horizontalHeaderItem(1)->text(), "File Name");
    EXPECT_EQ(progressTable->horizontalHeaderItem(2)->text(), "Downloaded/Total");
    EXPECT_EQ(progressTable->horizontalHeaderItem(3)->text(), "Status");
    EXPECT_EQ(progressTable->horizontalHeaderItem(4)->text(), "Remaining Time");
}

TEST_F(MainWindowTest, ProgressBarConstruction) {
    QProgressBar *progressBar = mainWindow->findChild<QProgressBar *>("progressBar");
    ASSERT_NE(progressBar, nullptr);
    EXPECT_EQ(progressBar->minimum(), 0);
    EXPECT_EQ(progressBar->maximum(), 100);
}

TEST_F(MainWindowTest, ButtonConstruction) {
    testWidgetConstruction(mainWindow->findChild<QPushButton *>("pauseButton"), "Pause");
    testWidgetConstruction(mainWindow->findChild<QPushButton *>("stopButton"), "Stop");
    testWidgetConstruction(mainWindow->findChild<QPushButton *>("cancelButton"), "Cancel");
}

