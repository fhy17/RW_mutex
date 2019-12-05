CC=g++

INCLUDE = -I.

CFLAGS = -Wall

LIBFLAGS = -lpthread 

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

TARGET = run

ALL:$(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBFLAGS)

%.o:%.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(TARGET)
