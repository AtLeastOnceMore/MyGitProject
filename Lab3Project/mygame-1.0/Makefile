CC = g++
CFLAGS = -Wall
LDFLAGS = -lncurses
TARGET = mygame

all: $(TARGET)

$(TARGET): mygame.cpp
	$(CC) $(CFLAGS) -o $(TARGET) mygame.cpp $(LDFLAGS)

clean:
	rm -f $(TARGET)
