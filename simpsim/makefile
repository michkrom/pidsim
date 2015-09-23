TARGET = pidsim
LIBS = -lm -Llib -lsim
INCS = -I lib
CC = gcc
CPP = g++
CFLAGS = -g -Wall $(INCS)
CD = cd

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

$(TARGET): $(OBJECTS) $(OBJECTSCPP) sim.a
	$(CPP) $(OBJECTS) $(OBJECTSCPP) $(LIBS) -o $@

# this target is here for convinience, normally .a would be build separately
sim.a:
	$(CD) lib;$(MAKE)

clean:
	-rm -rf *.o
	-rm -rf *.a	
	-rm -f $(TARGET)
	$(CD) lib
	$(MAKE) clean
