/*
 * File: c:\Users\tonyw\Desktop\New folder\src\FTPClient.cpp
 * Project: c:\Users\tonyw\Desktop\New folder\src
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:36:40 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "FTPClient.h"
#include "FTPResponseParser.h"
#include "FTPUtilities.h"
#include <iostream>
#include <stdexcept>
#include <cstring> 
#include <winsock2.h> 
#include <ws2tcpip.h>
#include <unistd.h>
#include <algorithm>

namespace ftp_library {

    //! Constructor
    FTPClient::FTPClient() : m_socket(-1), m_connected(false), m_port(21), m_remoteDir("/") {
        if (!initializeWinsock()) {
            throw FTPException("Failed to initialize Winsock.");
        }
    }

    //! Destructor
    FTPClient::~FTPClient() {
        if (m_connected) {
            disconnect();
        }
        cleanupWinsock(); 
    }

    //! Move Constructor
    // @param other The FTPClient object to move
    FTPClient::FTPClient(FTPClient&& other) noexcept
        : m_socket(other.m_socket), m_connected(other.m_connected), m_host(std::move(other.m_host)), 
        m_port(other.m_port), m_remoteDir(std::move(other.m_remoteDir)) {
        other.m_socket = -1;
        other.m_connected = false;
    }

    //! Move Assignment Operator
    // @param other The FTPClient object to move
    // @return A reference to the moved FTPClient object
    FTPClient& FTPClient::operator=(FTPClient&& other) noexcept {
        if (this != &other) {
            if (m_connected) {
                disconnect();
            }

            m_socket = other.m_socket;
            m_connected = other.m_connected;
            m_host = std::move(other.m_host);
            m_port = other.m_port;
            m_remoteDir = std::move(other.m_remoteDir);

            other.m_socket = -1;
            other.m_connected = false;
        }
        return *this;
    }

    //! Initialize Winsock
    // @return True if Winsock was initialized successfully, false otherwise
    bool FTPClient::initializeWinsock() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        return result == 0;
    }

    //! Clean up Winsock
    void FTPClient::cleanupWinsock() {
        WSACleanup();
    }

    //! Connect to the FTP server
    // @param host The hostname or IP address of the FTP server
    // @param port The port number of the FTP server (default is 21)
    void FTPClient::connect(const std::string& host, uint16_t port) {
        m_host = host;
        m_port = port;

        addrinfo hints = {};
        hints.ai_family = AF_INET;  //? IPv4
        hints.ai_socktype = SOCK_STREAM;  //? TCP
        hints.ai_protocol = IPPROTO_TCP;

        addrinfo* result = nullptr;
        int res = getaddrinfo(m_host.c_str(), std::to_string(m_port).c_str(), &hints, &result);
        if (res != 0 || result == nullptr) {
            throw FTPException("Failed to resolve server address.");
        }

        //* Create a socket
        m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (m_socket < 0) {
            freeaddrinfo(result);
            throw FTPException("Failed to create socket.");
        }

        //* Connect to the server
        res = ::connect(m_socket, result->ai_addr, result->ai_addrlen);
        freeaddrinfo(result); 
        if (res < 0) {
            throw FTPException("Failed to connect to the server.");
        }

        //* Receive the welcome message
        std::string response = receiveResponse();
        m_connected = true;

        validateResponse(response, {220});
        std::cout << "Connected to FTP server: " << m_host << std::endl;
    }

    //! Authenticate with username and password
    // @param username The username to authenticate with
    // @param password The password to authenticate with
    void FTPClient::authenticate(const std::string& username, const std::string& password) {
        //* Send USER command
        sendCommand("USER " + username);
        std::string response = receiveResponse();
        validateResponse(response, {331});

        //* Send PASS command
        sendCommand("PASS " + password);
        response = receiveResponse();
        validateResponse(response, {230});

        std::cout << "Authentication successful." << std::endl;
    }

    //! Get a list of files and directories from the current working directory or specified remoteDir
    // @param remoteDir The remote directory to list (default is "/")
    // @return A vector of strings containing the directory listing
    std::vector<std::string> FTPClient::listDirectory(const std::string& remoteDir) {

        m_remoteDir = remoteDir.empty() ? "/" : remoteDir;

        sendCommand("PASV");
        std::string response = receiveResponse();
        validateResponse(response, {227});
        auto [ip, port] = FTPUtilities::parsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0) {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr {};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr*)&dataAddr, sizeof(dataAddr)) < 0) {
            throw FTPException("Failed to connect to passive mode address.");
        }

        //* Change the remote directory
        sendCommand("CWD " + m_remoteDir);
        response = receiveResponse();
        validateResponse(response, {250});

        //* Send LIST command
        sendCommand("LIST");
        response = receiveResponse();
        validateResponse(response, {150});

        //* Receive the directory listing
        char buffer[1024] = {0};
        std::string directoryListing;
        ssize_t bytesRead;

        while ((bytesRead = recv(dataSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
            directoryListing.append(buffer, bytesRead);
        }

        close(dataSocket);

        response = receiveResponse();
        validateResponse(response, {226});

        return FTPUtilities::splitString(directoryListing, '\n');
    }

    //! Download a file
    // @param remoteFilePath The path of the file to download
    // @param localFilePath The path to save the downloaded file
    void FTPClient::downloadFile(const std::string& remoteFilePath, std::string localFilePath) {
        std::string fileName = remoteFilePath.substr(remoteFilePath.find_last_of("/\\") + 1);

        //* If localFilePath is empty or ".", use the current directory
        if (localFilePath == "." || localFilePath.empty()) {
            localFilePath = "./" + fileName;  
        } else {
            //* If a directory is provided append the filename to it
            // @TODO: Ensure provided path is a directory otherwise allow filename to be specified
            if (localFilePath.back() != '\\' && localFilePath.back() != '/') {
                localFilePath += "/"; 
            }
            localFilePath += fileName; 
        }

        sendCommand("PASV");
        std::string response = receiveResponse();
        validateResponse(response, {227});
        auto [ip, port] = FTPUtilities::parsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0) {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr {};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr*)&dataAddr, sizeof(dataAddr)) < 0) {
            throw FTPException("Failed to connect to passive mode address.");
        }

        //* Send RETR to retrieve the file
        sendCommand("RETR " + remoteFilePath);
        response = receiveResponse();
        validateResponse(response, {150});

        //* Open the local file for writing
        FILE* localFile = fopen(localFilePath.c_str(), "wb");
        if (!localFile) {
            close(dataSocket);
            throw FTPException("Failed to open local file for writing: " + localFilePath);
        }

        //* Receive the file data
        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = recv(dataSocket, buffer, sizeof(buffer), 0)) > 0) {
            fwrite(buffer, 1, bytesRead, localFile);
        }

        fclose(localFile);
        close(dataSocket);

        response = receiveResponse();
        validateResponse(response, {226});

        std::cout << "File downloaded successfully: " << localFilePath << std::endl;
    }

    //! Upload a file
    // @param localFilePath The path of the file to upload
    // @param remoteFilePath The path to save the uploaded file on the server
    void FTPClient::uploadFile(const std::string& localFilePath, const std::string& remoteFilePath) {
        sendCommand("PASV");
        std::string response = receiveResponse();
        validateResponse(response, {227});
        auto [ip, port] = FTPUtilities::parsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0) {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr {};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr*)&dataAddr, sizeof(dataAddr)) < 0) {
            throw FTPException("Failed to connect to passive mode address.");
        }

        //* Send STOR command
        sendCommand("STOR " + remoteFilePath);
        response = receiveResponse();
        validateResponse(response, {150});

        //* Open
        FILE* localFile = fopen(localFilePath.c_str(), "rb");
        if (!localFile) {
            close(dataSocket);
            throw FTPException("Failed to open local file for reading: " + localFilePath);
        }

        //* Send
        char buffer[1024];
        ssize_t bytesRead;
        while ((bytesRead = fread(buffer, 1, sizeof(buffer), localFile)) > 0) {
            if (send(dataSocket, buffer, bytesRead, 0) < 0) {
                fclose(localFile);
                close(dataSocket);
                throw FTPException("Failed to send file data.");
            }
        }

        // @TODO: Fix closing after upload

        fclose(localFile);
        close(dataSocket);

        response = receiveResponse();
        validateResponse(response, {226});

        sendCommand("QUIT");
        response = receiveResponse();
        validateResponse(response, {221});

        close(m_socket);

        std::cout << "File uploaded successfully: " << remoteFilePath << std::endl;
    }


    //! Disconnect from the server
    void FTPClient::disconnect() {
        if (m_connected) {
            sendCommand("QUIT");
            close(m_socket);
            m_connected = false;
            std::cout << "Disconnected from FTP server." << std::endl;
        }
    }

    //! Private Helper: Send a command to the server
    // @param command The command to send
    void FTPClient::sendCommand(const std::string& command) {
        std::string commandWithCRLF = command + "\r\n";
        if (send(m_socket, commandWithCRLF.c_str(), commandWithCRLF.size(), 0) < 0) {
            throw FTPException("Failed to send command: " + command);
        }
    }

    //! Private Helper: Receive a response from the server
    // @return The server response as a string
    std::string FTPClient::receiveResponse() {
        char buffer[1024] = {0};
        ssize_t bytesRead = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead < 0) {
            throw FTPException("Failed to receive response.");
        }
        return std::string(buffer, bytesRead);
    }

    //! Private Helper: Validate server responses
    // @param response The server response to validate
    // @param expectedCodes A vector of expected response codes
    void FTPClient::validateResponse(const std::string& response, const std::vector<int>& expectedCodes) {
        auto [code, message] = FTPResponseParser::parseResponse(response);
        std::cout << "Received response: " << code << " - " << message << std::endl;

        if (std::find(expectedCodes.begin(), expectedCodes.end(), code) == expectedCodes.end()) {
            throw FTPException("Unexpected response code: " + std::to_string(code) + " - " + message);
        }
    }

} 