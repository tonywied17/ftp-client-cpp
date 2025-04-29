#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <ftp_library/Framework.h>

namespace ftp_library
{

    //! Mock FTPClient class
    class MockFTPClient : public FTPClient
    {
    public:
        MOCK_METHOD(void, Connect, (const std::string &host, uint16_t port), (override));
        MOCK_METHOD(void, Authenticate, (const std::string &username, const std::string &password), (override));
        MOCK_METHOD(std::vector<std::string>, ListDirectory, (const std::string &remoteDir), (override));
        MOCK_METHOD(void, DownloadFile, (const std::string &remoteFilePath, const std::string localFilePath), (override));
        MOCK_METHOD(void, UploadFile, (const std::string &localFilePath, const std::string &remoteFilePath), (override));
        MOCK_METHOD(void, disconnect, (), (override));
    };

    //! Test for connection to the FTP server using the mock
    TEST(FTPClientTest, ConnectToServer)
    {
        MockFTPClient mockFtpClient;

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            ASSERT_TRUE(mockFtpClient.IsConnected());
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

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, Authenticate("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu")).Times(1);

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            mockFtpClient.Authenticate("dlpuser", "rNrKYTX9g7z3RgJRmxWuGHbeu");
            ASSERT_TRUE(mockFtpClient.IsConnected());
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

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, ListDirectory("/")).WillOnce(testing::Return(std::vector<std::string>{"file1.txt", "file2.txt"}));

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            std::vector<std::string> files = mockFtpClient.ListDirectory("/");
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

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, UploadFile("./local_file.txt", "./file.txt")).Times(1);

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            mockFtpClient.UploadFile("./local_file.txt", "./file.txt");
            ASSERT_TRUE(mockFtpClient.IsConnected());
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

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, DownloadFile("./file.txt", "./local_file.txt")).Times(1);

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            mockFtpClient.DownloadFile("./file.txt", "./local_file.txt");
            ASSERT_TRUE(mockFtpClient.IsConnected());
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

        EXPECT_CALL(mockFtpClient, Connect("ftp.dlptest.com", 21)).Times(1);
        EXPECT_CALL(mockFtpClient, Disconnect()).Times(1);

        try
        {
            mockFtpClient.Connect("ftp.dlptest.com", 21);
            mockFtpClient.Disconnect();
            ASSERT_FALSE(mockFtpClient.IsConnected());
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
