/*
 * File: c:\Users\tonyw\Desktop\New folder\include\FTPResponseParser.h
 * Project: c:\Users\tonyw\Desktop\New folder\include
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Thu January 23rd 2025 6:35:44 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef FTPRESPONSEPARSER_H
#define FTPRESPONSEPARSER_H

#include <string>
#include <utility>
#include <stdexcept>

namespace ftp_library {
    class FTPResponseException : public std::runtime_error {
    public:
        explicit FTPResponseException(const std::string& message) : std::runtime_error(message) {}
    };

    class FTPResponseParser {
    public:
        //! Parse an FTP response
        //! @param response The response to parse
        //! @return A pair containing the response code and message
        static std::pair<int, std::string> parseResponse(const std::string& response);

    private:
        
        //! Helper method to extract the response code
        //! @param response The response to extract the code from
        //! @return The response code
        static int extractCode(const std::string& response);

        //! Helper method to extract the response message
        //! @param response The response to extract the message from
        //! @return The response message
        static std::string extractMessage(const std::string& response);
    };

} 

#endif
