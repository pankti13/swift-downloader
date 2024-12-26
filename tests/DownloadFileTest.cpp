#include "downloadfile.h"
#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QTemporaryFile>

TEST(DownloadFileTest, Constructor) {
    const QString url("https://example.com/test.txt");
    const QString savePath("test.txt");

    DownloadFile downloadFile(url, savePath, nullptr);

    EXPECT_EQ(downloadFile.getUrl(), url);
    EXPECT_EQ(downloadFile.getSavePath(), savePath);
    EXPECT_EQ(downloadFile.totalBytes, 0);
    EXPECT_EQ(downloadFile.getBytesReceived(), 0);
    EXPECT_EQ(downloadFile.activeThreads, 0);
}

TEST(DownloadFileTest, HandleChunkDownloaded) {
    DownloadFile downloadFile("https://example.com/test.txt", "test_download.txt", nullptr);

    QSignalSpy downloadProgressSpy(&downloadFile, &DownloadFile::downloadProgress);
    QSignalSpy downloadStatusChangedSpy(&downloadFile, &DownloadFile::downloadStatusChanged);

    QByteArray data = "test_chunk";

    // Simulate a download chunk
    downloadFile.handleChunkDownloaded(data);
    ASSERT_EQ(downloadProgressSpy.count(), 1);
    QList<QVariant> arguments = downloadProgressSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toLongLong(), data.size());
    EXPECT_EQ(downloadFile.totalBytes, 0);

    downloadFile.activeThreads = 1;
    downloadFile.handleChunkDownloaded(data);

    ASSERT_EQ(downloadStatusChangedSpy.count(), 1);
    arguments = downloadStatusChangedSpy.takeFirst();
    EXPECT_EQ(arguments.at(0).toString(), "Completed");
}

TEST(DownloadFileTest, MergeChunks) {
    DownloadFile downloadFile("https://example.com/test.txt", "test.txt", nullptr);

    // Simulate downloaded chunks
    QList<QByteArray> chunks = {"chunk1", "chunk2", "chunk3"};
    downloadFile.chunks = chunks;

    QTemporaryFile tempFile;
    ASSERT_TRUE(tempFile.open());
    QString savePath = tempFile.fileName();
    downloadFile.setSavePath(savePath); 

    downloadFile.mergeChunks();
    EXPECT_TRUE(downloadFile.chunks.empty());
    tempFile.seek(0);
    QByteArray fileContent = tempFile.readAll();
    EXPECT_EQ(fileContent, "chunk1chunk2chunk3");
}
