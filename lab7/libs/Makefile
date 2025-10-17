CC=gcc
CFLAGS=-I. -std=c99
DYNAMIC_LIB=-shared -fPIC
BUILD_DIR=../build
UNAME=$(shell uname)

ifeq ($(UNAME),Darwin)
	SONAME_FLAG=-Wl,-install_name,@rpath/
	SO_EXTENSION=so
else
	SONAME_FLAG=-Wl,-soname,
	SO_EXTENSION=so
endif

all: $(BUILD_DIR) $(BUILD_DIR)/libtcpserver_arguments.a $(BUILD_DIR)/libudpserver_arguments.a $(BUILD_DIR)/libclient_arguments.$(SO_EXTENSION)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/libtcpserver_arguments.a: $(BUILD_DIR)/tcpserver_arguments.o
	ar rcs $@ $<

$(BUILD_DIR)/tcpserver_arguments.o: tcpserver_arguments.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/libudpserver_arguments.a: $(BUILD_DIR)/udpserver_arguments.o
	ar rcs $@ $<

$(BUILD_DIR)/udpserver_arguments.o: udpserver_arguments.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/libclient_arguments.$(SO_EXTENSION): client_arguments.c
	$(CC) $(CFLAGS) $(DYNAMIC_LIB) $< -o $@ $(SONAME_FLAG)libclient_arguments.$(SO_EXTENSION)

clean:
	rm $(BUILD_DIR)/*.$(SO_EXTENSION) $(BUILD_DIR)/*.a $(BUILD_DIR)/*.o
