
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
    - `Makefile`: Makefile for building the project using make.
    - `FTPClientApp.cpp`: Console-based example application demonstrating the FTP client.
    - `FTPClientApp-GUI.cpp`: GUI-based example application built using FLTK to demonstrate the FTP client visually.

- **build/bin/**: Directory where the compiled executable (`ftpclient.exe`) and the GUI application (`ftpclient-gui.exe`) are placed.
  
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

This will create the executable `ftpclient.exe` and `ftpclient-gui.exe` in the `bin/` directory and the shared library `ftpclient_shared.dll` in the `lib/` directory.

## Usage

To use the FTP client library, include the headers in your project:
```cpp
#include <ftp_library/FTPClient.h>
```

Example usage is provided in the FTPClientApp.cpp (CLI) and FTPClientApp-GUI.cpp (GUI) files. These examples demonstrate how to use the FTP client to connect to an FTP server and perform various operations via the command line and a graphical interface, respectively.

## Example Applications

![FTP Client Example 1](https://raw.githubusercontent.com/tonywied17/ftp-client-cpp/refs/heads/main/assets/use5.png)
![FTP Client Example 2](https://raw.githubusercontent.com/tonywied17/ftp-client-cpp/refs/heads/main/assets/use1.png)
![FTP Client Example 3](https://raw.githubusercontent.com/tonywied17/ftp-client-cpp/refs/heads/main/assets/use2.png)
<!-- ![FTP Client Example 4](https://raw.githubusercontent.com/tonywied17/ftp-client-cpp/refs/heads/main/assets/use3.png) -->
![FTP Client Example 5](https://raw.githubusercontent.com/tonywied17/ftp-client-cpp/refs/heads/main/assets/use4.png)


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.