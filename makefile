CC = g++
CFLAGS = -Wall

TARGET1 = server
TARGET2 = client

all: $(TARGET1) $(TARGET2)

$(TARGET1): server.o buffer.o
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET2): client.o buffer.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp buffer.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f *.o $(TARGET1) $(TARGET2)


