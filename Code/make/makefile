CC = gcc
VG = valgrind
BIN = ../bin
SRC = ../src
INC = ../inc
OBJ = ../obj

VFLAGS = (--tool = memcheck --leak-check = full)
CFLAGS = -Wall -g -o
IFLAGS = -I $(INC)
LDFLAGS = -c

# Required flags for the SQLite: POSIX Threads (libpthread), Dynamic library (libdl)
SQFLAGS = -lpthread -lm -pthread
LDLFLAGS = -ldl

admin:
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/admin.c -o $(OBJ)/admin.o
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/admin_funct.c -o $(OBJ)/admin_funct.o 
	$(CC) $(CFLAGS) $(BIN)/admin $(OBJ)/admin.o $(OBJ)/admin_funct.o


client:
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/client.c -o $(OBJ)/client.o
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/client_funct.c -o $(OBJ)/client_funct.o 
	$(CC) $(CFLAGS) $(BIN)/client $(OBJ)/client.o $(OBJ)/client_funct.o

server:
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/server.c $(SRC)/sqlite3.c $(SQFLAGS) $(LDLFLAGS)
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/server_funct.c -o $(OBJ)/server_funct.o 
	$(CC) $(CFLAGS) $(BIN)/server ./server.o ./sqlite3.o $(SQFLAGS) $(LDLFLAGS) $(OBJ)/server_funct.o
	mv *.o ../obj

sqlite3:
	$(CC) $(LDFLAGS) $(IFLAGS) $(SRC)/sqlite3.c $(SQFLAGS) -o $(OBJ)/sqlite3.o 
		
move:
	mv *.txt ./data
	mv *.db ./data

clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*
	rm *.txt

