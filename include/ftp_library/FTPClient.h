/*
 * File: c:\Users\tonyw\Desktop\New folder\include\FTPClient.h
 * Project: c:\Users\tonyw\Desktop\ftp-client-cpp\include\ftp_library
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Tue April 29th 2025 6:45:43 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <ftp_library/Framework.h>

namespace ftp_library
{

    class FTPException : public std::runtime_error
    {
    public:
        explicit FTPException(const std::string &message) : std::runtime_error(message) {}
    };

    class FTPResponseParser;

    class FTPClient
    {
    public:
        //! Constructor and destructor
        FTPClient();
        virtual ~FTPClient();

        //! Prevent copy construction and assignment
        FTPClient(const FTPClient &) = delete;
        FTPClient &operator=(const FTPClient &) = delete;

        //! Move constructor and move assignment
        FTPClient(FTPClient &&) noexcept;
        FTPClient &operator=(FTPClient &&) noexcept;

        bool IsConnected() const
        {
            return m_connected;
        }

        //! Connect to an FTP server
        //! @param host The hostname or IP address of the server
        //! @param port The port number to connect to (default is 21)
        virtual void Connect(const std::string &host, uint16_t port = 21);

        //! Authenticate with username and password
        //! @param username The username to authenticate with
        //! @param password The password to authenticate with
        virtual void Authenticate(const std::string &username, const std::string &password);

        //! Get a list of files and directories in the current working directory
        //! @return A vector of strings containing the names of the files and directories
        virtual std::vector<std::string> ListDirectory(const std::string &remoteDir = "/");

        //! Download a file from the server
        //! @param remoteFilePath The path to the file on the server
        //! @param localFilePath The path to save the file locally
        virtual void DownloadFile(const std::string &remoteFilePath, const std::string localFilePath);

        //! Upload a file to the server
        //! @param localFilePath The path to the file to upload
        //! @param remoteFilePath The path to save the file on the server
        virtual void UploadFile(const std::string &localFilePath, const std::string &remoteFilePath);

        //! Disconnect from the FTP server
        virtual void Disconnect();

    private:
        //! Helper methods --
        bool InitializeWinsock();                          //* Initialize Winsock
        void CleanupWinsock();                             //* Clean up Winsock
        void SendCommand(const std::string &command);      //* Send a command to the server
        std::string ReceiveResponse();                     //* Receive and return server response
        void ValidateResponse(const std::string &response, //* Validate server response
                              const std::vector<int> &expectedCodes);

        //! Data members
        int m_socket;                                        //* Socket descriptor for the connection
        bool m_connected;                                    //* Connection status
        std::string m_host;                                  //* Server host
        uint16_t m_port;                                     //* Server port
        std::string m_remoteDir;                             //* Current remote directory (default is "/")
        std::unique_ptr<FTPResponseParser> m_responseParser; //* Response parser (composition)
    };

}

#endif
