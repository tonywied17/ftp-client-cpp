/*
 * File: c:\Users\tonyw\Desktop\New folder\src\FTPResponseParser.cpp
 * Project: c:\Users\tonyw\Desktop\ftp-client-cpp\include\ftp_library
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Tue April 29th 2025 6:44:18 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#include <ftp_library/FTPResponseParser.h>

namespace ftp_library
{

    //! Parse the server's FTP response into a code and message
    // @param response The response string from the server
    // @return A pair containing the response code and message
    std::pair<int, std::string> FTPResponseParser::ParseResponse(const std::string &response)
    {
        if (response.empty())
        {
            throw FTPResponseException("Received an empty response.");
        }

        int code = ExtractCode(response);

        std::string message = ExtractMessage(response);

        message = FTPUtilities::Trim(message);

        return {code, message};
    }

    //! Extract the response code
    // @param response The response string
    // @return The response code as an integer
    int FTPResponseParser::ExtractCode(const std::string &response)
    {
        if (response.size() < 3)
        {
            throw FTPResponseException("Response is too short to extract code.");
        }

        int code = 0;
        std::istringstream codeStream(response.substr(0, 3));
        if (!(codeStream >> code))
        {
            throw FTPResponseException("Failed to parse response code.");
        }

        return code;
    }

    //! Extract the response message
    // @param response The response string
    // @return The response message as a string
    std::string FTPResponseParser::ExtractMessage(const std::string &response)
    {
        if (response.size() <= 3)
        {
            return "";
        }
        return response.substr(3);
    }

    //! Check if the response code matches the expected code
    // @param response The server's response
    // @param expectedCode The expected response code
    // @return True if the response code matches, false otherwise
    bool FTPResponseParser::IsExpectedCode(const std::string &response, int expectedCode)
    {
        try
        {
            int code = ExtractCode(response);
            return code == expectedCode;
        }
        catch (const FTPResponseException &e)
        {
            return false;
        }
    }

}
