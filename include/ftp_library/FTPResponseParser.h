/*
 * File: c:\Users\tonyw\Desktop\New folder\include\FTPResponseParser.h
 * Project: c:\Users\tonyw\Desktop\ftp-client-cpp\include\ftp_library
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Tue April 29th 2025 6:43:24 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef FTPRESPONSEPARSER_H
#define FTPRESPONSEPARSER_H

#include <ftp_library/Framework.h>

namespace ftp_library
{

    //! Exception for FTP response errors
    class FTPResponseException : public std::runtime_error
    {
    public:
        explicit FTPResponseException(const std::string &message) : std::runtime_error(message) {}
    };

    //! Parser for FTP responses
    class FTPResponseParser
    {
    public:
        //! Parse an FTP response into code and message
        //! @param response The response to parse
        //! @return A pair containing the response code and message
        static std::pair<int, std::string> ParseResponse(const std::string &response);

        //! Extract the response code
        //! @param response The response to extract the code from
        //! @return The response code as an integer
        static int ExtractCode(const std::string &response);

        //! Extract the response message
        //! @param response The response to extract the message from
        //! @return The response message as a string
        static std::string ExtractMessage(const std::string &response);

        //! Check if a response code matches the expected code
        //! @param response The server's response
        //! @param expectedCode The expected response code
        //! @return True if the response code matches, false otherwise
        static bool IsExpectedCode(const std::string &response, int expectedCode);
    };

}

#endif
