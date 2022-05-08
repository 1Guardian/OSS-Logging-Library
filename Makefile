#Vars
CC=g++
CFLAGS=-I.
DEPS = LoggerLibrary.h
OBJ = LoggerLibrary.o driver.o
BIN = driver

#Make Target Object files w/ suffix rules
%.o: %.cpp $(DEPS)
	$(CC) -std=c++11 -c -o $@ $< $(CFLAGS)

#making the bin
$(BIN): $(OBJ)
	$(CC) -std=c++11 -o $@ $^ $(CFLAGS)

#Clean
clean: 
	rm $(OBJ) $(BIN)