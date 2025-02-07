CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
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

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(SHARED_LIB): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

$(OBJ_DIR)/FTPClientApp.o: FTPClientApp.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

post_build: $(SHARED_LIB)
	@mkdir -p $(LIB_DIR)
	@cp $(SHARED_LIB) $(LIB_DIR)/ftpclient_shared.dll
	@echo "Shared library copied to $(LIB_DIR)"

clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned up build directory"

.PHONY: all post_build clean
