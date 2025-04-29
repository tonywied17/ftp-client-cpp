//
// Project headers
//
#include <ftp_library/FTPClient.h>
#include <ftp_library/FTPResponseParser.h>
#include <ftp_library/FTPUtilities.h>

//
// Standard library headers
//
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <cstring>
#include <cstdint>
#include <string>
#include <algorithm>
#include <memory>
#include <vector>
#include <locale>
#include <sstream>
#include <utility>
#include <objbase.h>
#include <ole2.h> 

//
// Platform-specific headers
//
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
// #include <windows.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif
