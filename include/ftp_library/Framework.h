//
#include <ftp_library/FTPClient.h>
#include <ftp_library/FTPResponseParser.h>
#include <ftp_library/FTPUtilities.h>

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

#if defined(_WIN32) || defined(_WIN64)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <unistd.h>
#endif

// 
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

