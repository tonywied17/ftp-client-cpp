/*
 * File: c:\Users\tonyw\Desktop\New folder\include\FTPClient.h
 * Project: c:\Users\tonyw\Desktop\New folder\include
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:35:32 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace ftp_library {

    class FTPException : public std::runtime_error {
    public:
        explicit FTPException(const std::string& message) : std::runtime_error(message) {}
    };

    class FTPResponseParser;

    class FTPClient {
    public:
        
        //! Constructor and destructor
        FTPClient();
        ~FTPClient();

        //! Prevent copy construction and assignment
        FTPClient(const FTPClient&) = delete;
        FTPClient& operator=(const FTPClient&) = delete;

        //! Move constructor and move assignment
        FTPClient(FTPClient&&) noexcept;
        FTPClient& operator=(FTPClient&&) noexcept;

        //! Connect to an FTP server
        //! @param host The hostname or IP address of the server
        //! @param port The port number to connect to (default is 21)
        void connect(const std::string& host, uint16_t port = 21);

        //! Authenticate with username and password
        //! @param username The username to authenticate with
        //! @param password The password to authenticate with
        void authenticate(const std::string& username, const std::string& password);

        //! Get a list of files and directories in the current working directory
        //! @return A vector of strings containing the names of the files and directories
        std::vector<std::string> listDirectory(const std::string& remoteDir = "/");

        //! Download a file from the server
        //! @param remoteFilePath The path to the file on the server
        //! @param localFilePath The path to save the file locally
        void downloadFile(const std::string& remoteFilePath, const std::string localFilePath);

        //! Upload a file to the server
        //! @param localFilePath The path to the file to upload
        //! @param remoteFilePath The path to save the file on the server
        void uploadFile(const std::string& localFilePath, const std::string& remoteFilePath);

        //! Disconnect from the FTP server
        void disconnect();

    private:
        //! Helper methods --
        bool initializeWinsock();                          //* Initialize Winsock
        void cleanupWinsock();                             //* Clean up Winsock
        void sendCommand(const std::string& command);      //* Send a command to the server
        std::string receiveResponse();                     //* Receive and return server response
        void validateResponse(const std::string& response, //* Validate server response
                            const std::vector<int>& expectedCodes);

        //! Data members
        int m_socket;                      //* Socket descriptor for the connection
        bool m_connected;                  //* Connection status
        std::string m_host;                //* Server host
        uint16_t m_port;                   //* Server port
        std::string m_remoteDir;           //* Current remote directory (default is "/")
        std::unique_ptr<FTPResponseParser> m_responseParser; //* Response parser (composition)
    };

}

#endif