/*
 * File: c:\Users\tonyw\Desktop\New folder\src\FTPUtilities.cpp
 * Project: c:\Users\tonyw\Desktop\New folder\src
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:37:00 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "FTPUtilities.h"
#include <algorithm>
#include <cctype>
#include <locale>
#include <sstream>

namespace ftp_library {

    //! Convert a string to lowercase
    // @param input The string to convert
    // @return The lowercase version of the input string
    std::string FTPUtilities::trim(const std::string& input) {
        std::string result = input;
        result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), result.end());
        return result;
    }

    //! Convert a string to lowercase
    // @param input The string to convert
    // @return The lowercase version of the input string
    std::tuple<std::string, uint16_t> FTPUtilities::parsePassiveModeResponse(const std::string& response) {
        size_t start = response.find('(');
        size_t end = response.find(')');
        
        if (start == std::string::npos || end == std::string::npos || end <= start) {
            throw std::invalid_argument("Invalid passive mode response format.");
        }

        std::string data = response.substr(start + 1, end - start - 1);
        auto parts = splitString(data, ',');
        if (parts.size() != 6) {
            throw std::invalid_argument("Invalid passive mode data.");
        }

        std::string ip = parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3];
        uint16_t port = std::stoi(parts[4]) * 256 + std::stoi(parts[5]);

        return {ip, port};
    }

    //! Convert a string to lowercase
    // @param input The string to convert
    // @return The lowercase version of the input string
    std::vector<std::string> FTPUtilities::splitString(const std::string& input, char delimiter) {
        std::vector<std::string> result;
        std::string token;
        std::istringstream tokenStream(input);
        
        while (std::getline(tokenStream, token, delimiter)) {
            result.push_back(token);
        }
        
        return result;
    }

}
