CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -static -static-libgcc -static-libstdc++
LDFLAGS = -lws2_32

SRC_DIR = include/ftp_library
BIN_DIR = build/bin
LIB_DIR = build/lib
OBJ_DIR = build/obj
BUILD_DIR = build
LIBRARY_SOURCES = $(SRC_DIR)/FTPClient.cpp $(SRC_DIR)/FTPResponseParser.cpp $(SRC_DIR)/FTPUtilities.cpp
EXEC_SOURCES = FTPClientApp.cpp $(LIBRARY_SOURCES)

EXECUTABLE = $(BIN_DIR)/ftpclient
SHARED_LIB = $(LIB_DIR)/ftpclient_shared.dll

OBJECTS = $(OBJ_DIR)/FTPClient.o $(OBJ_DIR)/FTPResponseParser.o $(OBJ_DIR)/FTPUtilities.o $(OBJ_DIR)/FTPClientApp.o

all: $(EXECUTABLE) $(SHARED_LIB)

# executable
$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BIN_DIR)  # Ensure the BIN_DIR exists
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# shared library
$(SHARED_LIB): $(OBJECTS)
	@mkdir -p $(LIB_DIR)  # Ensure the LIB_DIR exists
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

# object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure the OBJ_DIR exists
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

# main application object file
$(OBJ_DIR)/FTPClientApp.o: FTPClientApp.cpp
	@mkdir -p $(OBJ_DIR)  # Ensure the OBJ_DIR exists
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

# copy shared library to LIB_DIR
post_build: $(SHARED_LIB)
	@mkdir -p $(LIB_DIR)  # Ensure the LIB_DIR exists
	@cp $(SHARED_LIB) $(LIB_DIR)/ftpclient_shared.dll
	@echo "Shared library copied to $(LIB_DIR)"

# Clean
clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned up build directory"

.PHONY: all post_build clean
