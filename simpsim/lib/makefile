# library makefile

TARGET = libsim.a
CC = gcc
CPP = g++
#AR = gcc-ar
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTSCPP = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

%.o: %.cpp $(HEADERS)
	$(CPP) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(HEADERS) $(OBJECTS) $(OBJECTSCPP)
	$(RM) $(output)
	$(AR) cr $@ $(OBJECTS) $(OBJECTSCPP)

clean:
	-rm -rf *.o
	-rm -rf *.a
	-rm -f $(TARGET)
