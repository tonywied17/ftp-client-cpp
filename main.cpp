/*
 * File: c:\Users\tonyw\Desktop\New folder\main.cpp
 * Project: c:\Users\tonyw\Desktop\New folder
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 10:50:10 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include "FTPClient.h"
#include "FTPUtilities.h"

//! Check if the client is connected to the server
//! @param connected True if connected, false otherwise
//! @return True if connected, false otherwise
bool isConnected(bool connected) {
    if (!connected) {
        std::cout << "You must connect to a server first.\n";
        return false;
    }
    return true;
}

//! Get a file path from the user./F
//! @param filePath The file path to get
//! @param prompt The prompt to display to the user
void getFilePath(std::string& filePath, const std::string& prompt) {
    std::cout << prompt;
    std::getline(std::cin, filePath);
}

//! Print the menu options
void printMenu() {
    std::cout << "FTP Client Menu:\n";
    std::cout << "1. Connect to server\n";
    std::cout << "2. Authenticate\n";
    std::cout << "3. List Directory\n";
    std::cout << "4. Download File\n";
    std::cout << "5. Upload File\n";
    std::cout << "6. Disconnect\n";
    std::cout << "7. Change Remote Directory\n"; 
    std::cout << "8. Quit\n";
}

//! Main function
int main() {
    try {
        ftp_library::FTPClient ftpClient;
        std::string host;
        uint16_t port = 21;
        std::string username, password;
        std::string remoteDir = "/";
        int choice;
        bool connected = false;

        while (true) {
            printMenu();
            std::cout << "Enter your choice: ";
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1: {
                    //* Connect to the FTP server
                    getFilePath(host, "Enter server host: ");
                    ftpClient.connect(host, port);
                    connected = true;
                    break;
                }

                case 2: {
                    if (!isConnected(connected)) break;

                    //* Authenticate with the server
                    getFilePath(username, "Enter username: ");
                    getFilePath(password, "Enter password: ");
                    ftpClient.authenticate(username, password);
                    break;
                }

                case 3: {
                    if (!isConnected(connected)) break;

                    //* List directory
                    std::vector<std::string> files = ftpClient.listDirectory(remoteDir);
                    std::cout << "Files and directories:\n";
                    for (const auto& file : files) {
                        std::cout << file << "\n";
                    }
                    break;
                }

                case 4: {
                    if (!isConnected(connected)) break;

                    //* Download file
                    std::string remoteFile, localFile;
                    getFilePath(remoteFile, "Enter the remote file path to download: ");
                    getFilePath(localFile, "Enter the local file path to save (use . or leave blank for current directory): ");
                    if (localFile == ".") {
                        localFile = "./" + remoteFile.substr(remoteFile.find_last_of("/") + 1);
                    }
                    ftpClient.downloadFile(remoteFile, localFile);
                    break;
                }

                case 5: {
                    if (!isConnected(connected)) break;

                    //* Upload file
                    std::string localFile, remoteFile;
                    getFilePath(localFile, "Enter the local file path to upload: ");
                    getFilePath(remoteFile, "Enter the remote file path to save as: ");
                    ftpClient.uploadFile(localFile, remoteFile);
                    break;
                }

                case 6: {
                    if (!isConnected(connected)) break;

                    //* Disconnect from the server
                    ftpClient.disconnect();
                    connected = false;
                    break;
                }

                case 7: {
                    if (!isConnected(connected)) break;

                    //* Change remote directory
                    getFilePath(remoteDir, "Enter the new remote directory: ");
                    std::cout << "Changed remote directory to: " << remoteDir << std::endl;
                    break;
                }

                case 8:
                    //* Quit
                    std::cout << "Exiting FTP client.\n";
                    return 0;

                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
