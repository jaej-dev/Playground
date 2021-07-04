#
# Simple Makefile
# added "-std=c++0x" to use "to_string()"

TARGET = run
LIBS = -lm
#CC = gcc
CC = g++ 
CFLAGS = -g -Wall -std=c++0x

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.cc, %.o, $(wildcard *.cc))
HEADERS = $(wildcard *.h)

%.o: %.cc $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
