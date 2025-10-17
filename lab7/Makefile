CC=gcc
CFLAGS=-I. -pthread -std=c99
SOURCE_DIR=src
BUILD_DIR=build
LIB_DIR=libs
ABS_BUILD_DIR=$(shell cd $(BUILD_DIR) && pwd)
DYNAMIC_CFLAGS=-Wl,-rpath,$(ABS_BUILD_DIR)

all: $(BUILD_DIR) $(BUILD_DIR)/tcpclient $(BUILD_DIR)/tcpserver $(BUILD_DIR)/udpclient $(BUILD_DIR)/udpserver

$(BUILD_DIR):
	mkdir -p build

$(BUILD_DIR)/tcpclient: $(SOURCE_DIR)/tcpclient.c
	$(CC) $(CFLAGS) $< -o $@ -L$(BUILD_DIR) -lclient_arguments $(DYNAMIC_CFLAGS)

$(BUILD_DIR)/tcpserver: $(SOURCE_DIR)/tcpserver.c
	$(CC) $(CFLAGS) $< -o $@ -L$(BUILD_DIR) -ltcpserver_arguments $(DYNAMIC_CFLAGS)

$(BUILD_DIR)/udpclient: $(SOURCE_DIR)/udpclient.c
	$(CC) $(CFLAGS) $< -o $@ -L$(BUILD_DIR) -lclient_arguments $(DYNAMIC_CFLAGS)

$(BUILD_DIR)/udpserver: $(SOURCE_DIR)/udpserver.c
	$(CC) $(CFLAGS) $< -o $@ -L$(BUILD_DIR) -ludpserver_arguments $(DYNAMIC_CFLAGS)

clean:
	rm $(BUILD_DIR)/*
