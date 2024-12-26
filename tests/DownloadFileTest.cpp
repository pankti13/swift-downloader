#include "downloadfile.h"
#include <gtest/gtest.h>
#include <QtTest>

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

  QSignalSpy downloadProgressSpy(&downloadFile, SIGNAL(downloadProgress(qint64, qint64)));
  QSignalSpy downloadStatusChangedSpy(&downloadFile, SIGNAL(downloadStatusChanged(const QString&)));

  QByteArray data = "test_chunk";

  downloadFile.handleChunkDownloaded(data);
  QCOMPARE(downloadProgressSpy.count(), 1); 
  QList<QVariant> arguments = downloadProgressSpy.takeFirst();
  EXPECT_EQ(arguments.at(0).toLongLong(), data.size());
  EXPECT_EQ(downloadFile.totalBytes, 0);

  QCOMPARE(downloadStatusChangedSpy.count(), 0); 
  downloadFile.activeThreads = 1; 
  downloadFile.handleChunkDownloaded(data); 

  QCOMPARE(downloadStatusChangedSpy.count(), 1);
  arguments = downloadStatusChangedSpy.takeFirst();
  EXPECT_EQ(arguments.at(0).toString(), "Completed"); 
}

TEST(DownloadFileTest, MergeChunks) {
  DownloadFile downloadFile("https://example.com/test.txt", "test.txt", nullptr); 

  // Simulate downloaded chunks
  QList<QByteArray> chunks;
  chunks.push_back("chunk1");
  chunks.push_back("chunk2");
  chunks.push_back("chunk3");

  downloadFile.chunks = chunks; 
  downloadFile.getSavePath() = "test_file.txt"; 
  downloadFile.mergeChunks();
  EXPECT_TRUE(downloadFile.chunks.empty());
}