/*
 * File: c:\Users\tonyw\Desktop\New folder\main.cpp
 * Project: c:\Users\tonyw\Desktop\New folder
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:34:49 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include "FTPClient.h"
#include "FTPUtilities.h"

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
        //* Instantiate the FTP client
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
                    std::cout << "Enter server host: ";
                    std::getline(std::cin, host);
                    ftpClient.connect(host, port);
                    connected = true;
                    break;
                }

                case 2: {
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* Authenticate with the server
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    ftpClient.authenticate(username, password);
                    break;
                }

                case 3: {
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* List directory
                    std::vector<std::string> files = ftpClient.listDirectory(remoteDir);
                    std::cout << "Files and directories:\n";
                    for (const auto& file : files) {
                        std::cout << file << "\n";
                    }
                    break;
                }

                case 4: {
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* Download file
                    std::string remoteFile, localFile;
                    std::cout << "Enter the remote file path to download: ";
                    std::getline(std::cin, remoteFile); 
                    std::cout << "Enter the local file path to save (use . or leave blank for current directory): ";
                    std::getline(std::cin, localFile);

                    //* If a directory is provided append the filename to it
                    // @TODO: Ensure provided path is a directory otherwise allow filename to be specified
                    if (localFile == ".") {
                        localFile = "./" + remoteFile.substr(remoteFile.find_last_of("/") + 1);
                    }
                    ftpClient.downloadFile(remoteFile, localFile);
                    break;
                }

                case 5: {
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* Upload file
                    std::string localFile, remoteFile;
                    std::cout << "Enter the local file path to upload: ";
                    std::getline(std::cin, localFile);
                    std::cout << "Enter the remote file path to save as: ";
                    std::getline(std::cin, remoteFile); 
                    ftpClient.uploadFile(localFile, remoteFile);
                    break;
                }

                case 6: {
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* Disconnect from the server
                    ftpClient.disconnect();
                    connected = false;
                    break;
                }

                case 7: { 
                    if (!connected) {
                        std::cout << "You must connect to a server first.\n";
                        break;
                    }

                    //* Change remote directory
                    std::cout << "Enter the new remote directory: ";
                    std::getline(std::cin, remoteDir);
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
