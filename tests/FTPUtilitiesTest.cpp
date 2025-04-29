#include "gtest/gtest.h"
#include <ftp_library/Framework.h>

//! Test for trimming whitespace
TEST(FTPUtilitiesTest, Trim)
{
    std::string input = "  Hello, world!  ";
    std::string expected = "Hello, world!";
    ASSERT_EQ(ftp_library::FTPUtilities::Trim(input), expected);
}

//! Test for converting a string to lowercase
TEST(FTPUtilitiesTest, ToLowerCase)
{
    std::string input = "HeLLo WoRLd";
    std::string expected = "hello world";
    ASSERT_EQ(ftp_library::FTPUtilities::ToLowerCase(input), expected);
}

//! Test for StartsWith function
TEST(FTPUtilitiesTest, StartsWith)
{
    std::string input = "Hello, world!";
    std::string prefix = "Hello";
    ASSERT_TRUE(ftp_library::FTPUtilities::StartsWith(input, prefix));

    prefix = "world";
    ASSERT_FALSE(ftp_library::FTPUtilities::StartsWith(input, prefix));
}

//! Test for EndsWith function
TEST(FTPUtilitiesTest, EndsWith)
{
    std::string input = "Hello, world!";
    std::string suffix = "world!";
    ASSERT_TRUE(ftp_library::FTPUtilities::EndsWith(input, suffix));

    suffix = "Hello";
    ASSERT_FALSE(ftp_library::FTPUtilities::EndsWith(input, suffix));
}

//! Test for parsing passive mode response
TEST(FTPUtilitiesTest, ParsePassiveModeResponse)
{
    std::string response = "227 Entering passive mode (192,168,0,1,19,81).";
    try
    {
        auto [ip, port] = ftp_library::FTPUtilities::ParsePassiveModeResponse(response);
        ASSERT_EQ(ip, "192.168.0.1");
        ASSERT_EQ(port, 4945); //* expected port calculation (19 * 256 + 81)
    }
    catch (const std::exception &e)
    {
        FAIL() << "Parsing failed: " << e.what();
    }
}

//! Test for splitString function
TEST(FTPUtilitiesTest, SplitString)
{
    std::string input = "one,two,three";
    char delimiter = ',';
    auto result = ftp_library::FTPUtilities::SplitString(input, delimiter);
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result[0], "one");
    ASSERT_EQ(result[1], "two");
    ASSERT_EQ(result[2], "three");
}
