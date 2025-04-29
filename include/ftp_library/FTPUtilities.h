/*
 * File: c:\Users\tonyw\Desktop\New folder\include\FTPUtilities.h
 * Project: c:\Users\tonyw\Desktop\ftp-client-cpp\include\ftp_library
 * Created Date: Thursday January 23rd 2025
 * Author: Tony Wiedman
 * -----
 * Last Modified: Tue April 29th 2025 6:45:10 
 * Modified By: Tony Wiedman
 * -----
 * Copyright (c) 2025 MolexWorks
 */

#ifndef FTPUTILITIES_H
#define FTPUTILITIES_H

#include <ftp_library/Framework.h>

namespace ftp_library
{

    class FTPUtilities
    {
    public:
        //! Split a string into a vector of strings based on a delimiter
        //! @param input The string to split
        //! @param delimiter The character to split the string on
        //! @return A vector of strings
        static std::vector<std::string> SplitString(const std::string &input, char delimiter);

        //! Trim whitespace from the beginning and end of a string
        //! @param input The string to trim
        //! @return The trimmed string
        static std::string Trim(const std::string &input);

        //! Convert a string to lowercase
        //! @param input The string to convert
        //! @return The lowercase string
        static std::string ToLowerCase(const std::string &input);

        //! Check if a string starts with a prefix
        //! @param input The string to check
        //! @param prefix The prefix to check for
        //! @return True if the string starts with the prefix, false otherwise
        static bool StartsWith(const std::string &input, const std::string &prefix);

        //! Check if a string ends with a suffix
        //! @param input The string to check
        //! @param suffix The suffix to check for
        //! @return True if the string ends with the suffix, false otherwise
        static bool EndsWith(const std::string &input, const std::string &suffix);

        //! Parse the response from a PASV command
        //! @param response The response to parse
        //! @return A tuple containing the IP address and port number
        static std::tuple<std::string, uint16_t> ParsePassiveModeResponse(const std::string &response);
    };
}

#endif
