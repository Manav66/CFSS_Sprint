#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 13569
#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_LENGTH 8192
#define ENCRYPT_KEY 34

extern char* encrypt_string(const char*);
extern char* decrypt_string(const char*);

extern void myflush(void);
extern int create_command_helper(char[], int);
extern int login(void);
extern int admin_menu(void);
