CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -static -static-libgcc -static-libstdc++
LDFLAGS = -lws2_32 -lole32 -lcomdlg32 -loleaut32 -luuid

FLTK_CXXFLAGS = $(shell fltk-config --cxxflags)
FLTK_LDFLAGS = $(shell fltk-config --ldflags)

SRC_DIR = include/ftp_library
BIN_DIR = build/bin
LIB_DIR = build/lib
OBJ_DIR = build/obj
BUILD_DIR = build

LIBRARY_SOURCES = $(SRC_DIR)/FTPClient.cpp $(SRC_DIR)/FTPResponseParser.cpp $(SRC_DIR)/FTPUtilities.cpp
OBJECTS = $(OBJ_DIR)/FTPClient.o $(OBJ_DIR)/FTPResponseParser.o $(OBJ_DIR)/FTPUtilities.o

EXECUTABLE = $(BIN_DIR)/ftpclient
EXECUTABLE_GUI = $(BIN_DIR)/ftpclient_gui
SHARED_LIB = $(LIB_DIR)/ftpclient_shared.dll

all: $(EXECUTABLE) $(EXECUTABLE_GUI) $(SHARED_LIB)

# CLI Executable
$(EXECUTABLE): $(OBJECTS) $(OBJ_DIR)/FTPClientApp.o
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# GUI Executable
$(EXECUTABLE_GUI): $(OBJECTS) $(OBJ_DIR)/FTPClientApp-GUI.o
	@mkdir -p $(BIN_DIR)
	$(CXX) -v $(CXXFLAGS) $(FLTK_CXXFLAGS) -o $@ $^ $(FLTK_LDFLAGS) $(LDFLAGS)

# Shared Library
$(SHARED_LIB): $(OBJECTS)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LDFLAGS)

# Object rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

$(OBJ_DIR)/FTPClientApp.o: FTPClientApp.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c -o $@ $<

$(OBJ_DIR)/FTPClientApp-GUI.o: FTPClientApp-GUI.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(FLTK_CXXFLAGS) -Iinclude -c -o $@ $<

# Copy shared lib
post_build: $(SHARED_LIB)
	@mkdir -p $(LIB_DIR)
	@cp $(SHARED_LIB) $(LIB_DIR)/ftpclient_shared.dll
	@echo "Shared library copied to $(LIB_DIR)"

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Cleaned up build directory"

.PHONY: all clean post_build
