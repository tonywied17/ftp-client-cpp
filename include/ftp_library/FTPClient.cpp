/*
 * File: c:\Users\tonyw\Desktop\New folder\src\FTPClient.cpp
 * Project: c:\Users\tonyw\Desktop\ftp-client-cpp\include\ftp_library
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Tue April 29th 2025 6:45:43 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include <ftp_library/FTPClient.h>

namespace ftp_library
{

    //! Constructor
    FTPClient::FTPClient() : m_socket(-1), m_connected(false), m_port(21), m_remoteDir("/")
    {
#if defined(_WIN32) || defined(_WIN64)
        if (!InitializeWinsock())
        {
            throw FTPException("Failed to initialize Winsock.");
        }
#endif
    }

    //! Destructor
    FTPClient::~FTPClient()
    {
        if (m_connected)
        {
            Disconnect();
        }
#if defined(_WIN32) || defined(_WIN64)
        CleanupWinsock();
#endif
    }

    //! Move constructor
    FTPClient::FTPClient(FTPClient &&other) noexcept
        : m_socket(other.m_socket), m_connected(other.m_connected), m_host(std::move(other.m_host)),
          m_port(other.m_port), m_remoteDir(std::move(other.m_remoteDir))
    {
        other.m_socket = -1;
        other.m_connected = false;
    }

    //! Move assignment
    FTPClient &FTPClient::operator=(FTPClient &&other) noexcept
    {
        if (this != &other)
        {
            if (m_connected)
            {
                Disconnect();
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
    bool FTPClient::InitializeWinsock()
    {
#if defined(_WIN32) || defined(_WIN64)
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        return result == 0;
#else
        return true;
#endif
    }

    //! Clean up Winsock
    void FTPClient::CleanupWinsock()
    {
#if defined(_WIN32) || defined(_WIN64)
        WSACleanup();
#endif
    }

    //! Connect to an FTP server
    //@ param host The hostname or IP address of the server
    //@ param port The port number to connect to (default is 21)
    void FTPClient::Connect(const std::string &host, uint16_t port)
    {
        m_host = FTPUtilities::Trim(host);
        m_port = port;

        addrinfo hints = {};
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        addrinfo *result = nullptr;
        int res = getaddrinfo(m_host.c_str(), std::to_string(m_port).c_str(), &hints, &result);
        if (res != 0 || result == nullptr)
        {
            throw FTPException("Failed to resolve server address.");
        }

        m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (m_socket < 0)
        {
            freeaddrinfo(result);
            throw FTPException("Failed to create socket.");
        }

        res = ::connect(m_socket, result->ai_addr, result->ai_addrlen);
        freeaddrinfo(result);
        if (res < 0)
        {
            throw FTPException("Failed to connect to the server.");
        }

        std::string response = ReceiveResponse();
        response = FTPUtilities::Trim(response);
        m_connected = true;
        ValidateResponse(response, {220});
        std::cout << "Connected to FTP server: " << m_host << std::endl;
    }

    //! Authenticate with username and password
    //@ param username The username to authenticate with
    //@ param password The password to authenticate with
    void FTPClient::Authenticate(const std::string &username, const std::string &password)
    {
        SendCommand("USER " + FTPUtilities::Trim(username));
        std::string response = ReceiveResponse();
        ValidateResponse(response, {331});

        SendCommand("PASS " + FTPUtilities::Trim(password));
        response = ReceiveResponse();
        ValidateResponse(response, {230});
        std::cout << "Authentication successful." << std::endl;
    }

    //! Get a list of files and directories in the current working directory
    //@ param remoteDir The directory to list (default is root)
    //@ return A vector of strings containing the names of the files and directories
    std::vector<std::string> FTPClient::ListDirectory(const std::string &remoteDir)
    {
        m_remoteDir = FTPUtilities::Trim(remoteDir.empty() ? "/" : remoteDir);

        SendCommand("PASV");
        std::string response = ReceiveResponse();
        ValidateResponse(response, {227});

        auto [ip, port] = FTPUtilities::ParsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0)
        {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr{};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr *)&dataAddr, sizeof(dataAddr)) < 0)
        {
            closesocket(dataSocket);
            throw FTPException("Failed to connect to passive mode address.");
        }

        SendCommand("CWD " + FTPUtilities::ToLowerCase(m_remoteDir));
        response = ReceiveResponse();
        ValidateResponse(response, {250});

        SendCommand("LIST");
        response = ReceiveResponse();
        ValidateResponse(response, {150});

        char buffer[1024];
        std::string directoryListing;
        ssize_t bytesRead;

        while ((bytesRead = recv(dataSocket, buffer, sizeof(buffer) - 1, 0)) > 0)
        {
            directoryListing.append(buffer, bytesRead);
        }

        closesocket(dataSocket);

        response = ReceiveResponse();
        ValidateResponse(response, {226});

        return FTPUtilities::SplitString(directoryListing, '\n');
    }

    //! Download a file from the server
    //@ param remoteFilePath The path to the file on the server
    //@ param localFilePath The path to save the file locally
    void FTPClient::DownloadFile(const std::string &remoteFilePath, const std::string localFilePath)
    {
        std::string fileName = remoteFilePath.substr(remoteFilePath.find_last_of("/\\") + 1);
        std::string resolvedPath = localFilePath.empty() ? "./" + fileName : localFilePath;

        if (!FTPUtilities::EndsWith(resolvedPath, fileName))
        {
            resolvedPath += "/" + fileName;
        }

        SendCommand("PASV");
        std::string response = ReceiveResponse();
        ValidateResponse(response, {227});

        auto [ip, port] = FTPUtilities::ParsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0)
        {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr{};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr *)&dataAddr, sizeof(dataAddr)) < 0)
        {
            closesocket(dataSocket);
            throw FTPException("Failed to connect to passive mode address.");
        }

        SendCommand("RETR " + FTPUtilities::Trim(remoteFilePath));
        response = ReceiveResponse();
        ValidateResponse(response, {150});

        FILE *localFile = fopen(resolvedPath.c_str(), "wb");
        if (!localFile)
        {
            closesocket(dataSocket);
            throw FTPException("Failed to open local file for writing: " + resolvedPath);
        }

        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = recv(dataSocket, buffer, sizeof(buffer), 0)) > 0)
        {
            fwrite(buffer, 1, bytesRead, localFile);
        }

        fclose(localFile);
        closesocket(dataSocket);

        response = ReceiveResponse();
        ValidateResponse(response, {226});
        std::cout << "File downloaded successfully: " << resolvedPath << std::endl;
    }

    //! Upload a file to the server
    //@ param localFilePath The path to the file to upload
    //@ param remoteFilePath The path to save the file on the server
    void FTPClient::UploadFile(const std::string &localFilePath, const std::string &remoteFilePath)
    {
        SendCommand("PASV");
        std::string response = ReceiveResponse();
        ValidateResponse(response, {227});

        auto [ip, port] = FTPUtilities::ParsePassiveModeResponse(response);
        int dataSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (dataSocket < 0)
        {
            throw FTPException("Failed to create data socket.");
        }

        sockaddr_in dataAddr{};
        dataAddr.sin_family = AF_INET;
        dataAddr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &dataAddr.sin_addr);

        if (::connect(dataSocket, (struct sockaddr *)&dataAddr, sizeof(dataAddr)) < 0)
        {
            closesocket(dataSocket);
            throw FTPException("Failed to connect to passive mode address.");
        }

        SendCommand("STOR " + FTPUtilities::ToLowerCase(remoteFilePath));
        response = ReceiveResponse();
        ValidateResponse(response, {150});

        FILE *localFile = fopen(localFilePath.c_str(), "rb");
        if (!localFile)
        {
            closesocket(dataSocket);
            throw FTPException("Failed to open local file for reading: " + localFilePath);
        }

        char buffer[1024];
        ssize_t bytesRead;

        while ((bytesRead = fread(buffer, 1, sizeof(buffer), localFile)) > 0)
        {
            if (send(dataSocket, buffer, bytesRead, 0) < 0)
            {
                fclose(localFile);
                closesocket(dataSocket);
                throw FTPException("Failed to send file data.");
            }
        }

        fclose(localFile);
        closesocket(dataSocket);

        response = ReceiveResponse();
        ValidateResponse(response, {226});
        std::cout << "File uploaded successfully: " << remoteFilePath << std::endl;
    }

    //! Disconnect from the FTP server
    void FTPClient::Disconnect()
    {
        if (m_connected)
        {
            SendCommand("QUIT");
            closesocket(m_socket);
            m_connected = false;
            std::cout << "Disconnected from FTP server." << std::endl;
        }
    }

    //! Send a command to the server
    //@ param command The command to send
    void FTPClient::SendCommand(const std::string &command)
    {
        std::string commandWithCRLF = FTPUtilities::Trim(command) + "\r\n";
        if (send(m_socket, commandWithCRLF.c_str(), commandWithCRLF.size(), 0) < 0)
        {
            throw FTPException("Failed to send command: " + command);
        }
    }

    //! Receive and return server response
    //@ return The server response
    std::string FTPClient::ReceiveResponse()
    {
        char buffer[1024];
        ssize_t bytesRead = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead < 0)
        {
            throw FTPException("Failed to receive response.");
        }
        std::string response(buffer, bytesRead);
        return FTPUtilities::Trim(response);
    }

    //! Validate server response
    //@ param response The server response
    //@ param expectedCodes The expected response codes
    void FTPClient::ValidateResponse(const std::string &response, const std::vector<int> &expectedCodes)
    {
        auto [code, message] = FTPResponseParser::ParseResponse(response);

        if (!FTPResponseParser::IsExpectedCode(response, code))
        {
            throw FTPException("Unexpected response code: " + std::to_string(code) + " - " + message);
        }

        if (std::find(expectedCodes.begin(), expectedCodes.end(), code) == expectedCodes.end())
        {
            throw FTPException("Unexpected response code: " + std::to_string(code) + " - " + message);
        }
    }

}
