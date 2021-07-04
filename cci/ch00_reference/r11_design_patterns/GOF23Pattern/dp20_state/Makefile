#
# Simple Makefile
# 

TARGET = run
LIBS = -lm
#CC = gcc
CC = g++

# for posix thread
#CFLAGS = -g -Wall -lpthread -std=c++11 # for posix thread

# for c++11 std::thread
CFLAGS = -g -Wall -pthread -std=c++11

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cc, %.o, $(wildcard *.cc))
HEADERS = $(wildcard *.h)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
#	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
