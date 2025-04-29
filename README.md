
# FTP Client Library Project

![GitHub repo size](https://img.shields.io/github/repo-size/tonywied17/ftp-client-cpp?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/top/tonywied17/ftp-client-cpp?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/tonywied17/ftp-client-cpp?style=for-the-badge)

This project contains an FTP client library and an example application demonstrating its usage.

## Project Structure

- **include/ftp_library/**: Contains the header files for the FTP client library.
    - `FTPClient.h`: Header for the FTP client class.
    - `FTPResponseParser.h`: Header for the FTP response parser class.
    - `FTPUtilities.h`: Header for utility functions used in the library.
    - `FTPClient.cpp`: Contains the implementation of the FTP client class.
    - `FTPResponseParser.cpp`: Contains the implementation of the FTP response parser class.
    - `FTPUtilities.cpp`: Contains the implementation of utility functions.

- **./**:
    - `CMakeLists.txt`: CMake configuration file for the project.
    - `FTPClientApp.cpp`: Example application that demonstrates the usage of the FTP client library.

- **build/bin/**: Directory where the compiled executable (`ftpclient.exe`) is placed.
  
- **build/lib/**: Directory where the shared library (`ftpclient_shared.dll`) is placed.

## Building the FTP Client

To build the project, use the following commands:

1. Navigate to the root directory of the project.
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```
3. Run CMake to configure the project:
   ```bash
   cmake ..
   ```
4. Build the project:
   ```bash
   cmake --build .
   ```
--

Optionally, you can use make to build the project:

```bash
   make
```

This will create the executable `ftpclient.exe` in the `bin/` directory and the shared library `ftpclient_shared.dll` in the `lib/` directory.

## Usage

To use the FTP client library, include the headers in your project:
```cpp
#include <ftp_library/FTPClient.h>

```

Example usage is provided in the `FTPClientApp.cpp` file. This example demonstrates how to use the FTP client to connect to an FTP server and perform various operations.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.