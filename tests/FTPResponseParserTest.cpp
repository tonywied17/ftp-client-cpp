#include "gtest/gtest.h"
#include <ftp_library/Framework.h>

//! Test for parsing a valid response
TEST(FTPResponseParserTest, ParseValidResponse)
{
    std::string response = "230 User logged in, proceed.";
    try
    {
        auto [code, message] = ftp_library::FTPResponseParser::parseResponse(response);
        ASSERT_EQ(code, 230);
        ASSERT_EQ(message, "User logged in, proceed.");
    }
    catch (const std::exception &e)
    {
        FAIL() << "Parsing failed: " << e.what();
    }
}

//! Test for parsing an invalid response
TEST(FTPResponseParserTest, ParseInvalidResponse)
{
    std::string response = "Invalid response";
    try
    {
        ftp_library::FTPResponseParser::parseResponse(response);
        FAIL() << "Expected exception for invalid response";
    }
    catch (const ftp_library::FTPResponseException &e)
    {
        ASSERT_STREQ(e.what(), "Failed to parse response code.");
    }
}

//! Test for checking expected response code
TEST(FTPResponseParserTest, IsExpectedCode)
{
    std::string response = "227 Entering passive mode.";
    int expectedCode = 227;
    ASSERT_TRUE(ftp_library::FTPResponseParser::isExpectedCode(response, expectedCode));

    expectedCode = 200;
    ASSERT_FALSE(ftp_library::FTPResponseParser::isExpectedCode(response, expectedCode));
}
