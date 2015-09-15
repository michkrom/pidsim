TARGET = pidsimtest
LIBS = -lm
CC = gcc
CPP = g++
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTSCPP = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
HEADERS = $(wildcard *.h)

OBJECTSCPP += $(patsubst %.cpp, %.o, $(wildcard lib/src/*.cpp))
HEADERS += $(wildcard lib/include/*.h)

INCLUDES = -I lib/include

%.o: %.cpp $(HEADERS)
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS) $(OBJECTSCPP)
	$(CPP) $(OBJECTS) $(OBJECTSCPP) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
