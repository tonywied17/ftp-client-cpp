#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <stdexcept>
#include <ftp_library/FTPResponseParser.h>
#include <ftp_library/FTPClient.h>
#include <ftp_library/FTPUtilities.h>

namespace ftp_library
{

    //! Mock FTPClient class
    class MockFTPClient : public FTPClient
    {
    public:
        MOCK_METHOD(void, connect, (const std::string &host, uint16_t port), (override));
        MOCK_METHOD(void, authenticate, (const std::string &username, const std::string &password), (override));
        MOCK_METHOD(std::vector<std::string>, listDirectory, (const std::string &remoteDir), (override));
        MOCK_METHOD(void, downloadFile, (const std::string &remoteFilePath, const std::string localFilePath), (override));
        MOCK_METHOD(void, uploadFile, (const std::string &localFilePath, const std::string &remoteFilePath), (override));
        MOCK_METHOD(void, disconnect, (), (override));
    };

    //! Test for connection to the FTP server using the mock
    TEST(FTPClientTest, ConnectToServer)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            ASSERT_TRUE(mockFtpClient.isConnected());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Connection failed: " << e.what();
        }
    }

    //! Test for authentication (mocked)
    TEST(FTPClientTest, Authenticate)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, authenticate("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu")).Times(1);

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            mockFtpClient.authenticate("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");
            ASSERT_TRUE(mockFtpClient.isConnected());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Authentication failed: " << e.what();
        }
    }

    //! Test for listing directory using the mock
    TEST(FTPClientTest, ListDirectory)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, listDirectory("/")).WillOnce(testing::Return(std::vector<std::string>{"file1.txt", "file2.txt"}));

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            std::vector<std::string> files = mockFtpClient.listDirectory("/");
            ASSERT_FALSE(files.empty());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Directory listing failed: " << e.what();
        }
    }

    //! Test for uploading a file using the mock
    TEST(FTPClientTest, UploadFile)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, uploadFile("./local_file.txt", "./file.txt")).Times(1);

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            mockFtpClient.uploadFile("./local_file.txt", "./file.txt");
            ASSERT_TRUE(mockFtpClient.isConnected());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Upload failed: " << e.what();
        }
    }

    //! Test for downloading a file using the mock
    TEST(FTPClientTest, DownloadFile)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, downloadFile("./file.txt", "./local_file.txt")).Times(1);

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            mockFtpClient.downloadFile("./file.txt", "./local_file.txt");
            ASSERT_TRUE(mockFtpClient.isConnected());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Download failed: " << e.what();
        }
    }

    //! Test for disconnecting from the FTP server using the mock
    TEST(FTPClientTest, DisconnectFromServer)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, disconnect()).Times(1);

        try
        {
            mockFtpClient.connect("ftp.dlptest.com", 21);
            mockFtpClient.disconnect();
            ASSERT_FALSE(mockFtpClient.isConnected());
        }
        catch (const std::exception &e)
        {
            FAIL() << "Disconnect failed: " << e.what();
        }
    }

}

//! Main entry point for Google Test
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
