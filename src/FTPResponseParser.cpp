/*
 * File: c:\Users\tonyw\Desktop\New folder\src\FTPResponseParser.cpp
 * Project: c:\Users\tonyw\Desktop\New folder\src
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:36:50 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include "FTPResponseParser.h"
#include "FTPUtilities.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

namespace ftp_library {

//! Parse the server's FTP response into a code and message
//@ param response The response string from the server
//@ return A pair containing the response code and message
std::pair<int, std::string> FTPResponseParser::parseResponse(const std::string& response) {
    if (response.empty()) {
        throw FTPResponseException("Received an empty response.");
    }

    //* Extract the numeric response code
    int code = 0;
    std::istringstream codeStream(response.substr(0, 3));
    if (!(codeStream >> code)) {
        throw FTPResponseException("Failed to parse response code.");
    }

    //* Extract the message (everything after the code)
    std::string message = response.substr(3);
    message = FTPUtilities::trim(message); 

    return {code, message};
}


//@ TODO
int FTPResponseParser::extractCode(const std::string &response)
{
    return 0;
}
std::string FTPResponseParser::extractMessage(const std::string &response)
{
    return std::string();
}
}
