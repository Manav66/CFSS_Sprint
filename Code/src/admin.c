// Program to run the admin code. Functions are called here and implemented in admin_funct.h
 
#include "admin.h"  
 
int main()
{
    system("clear");  // Clear the terminal screen
 
    int try = 3, login_status = 0;  
 
    // Loop for login attempts
    while (try--)
    {
        // Check login authentication
        if (login() == 1)
        {
            login_status = 1;
            break;  // Exit loop if login successful
        }
 
        system("clear");  // Clear screen
        printf("You have %d tries left\n\n", try);  // Prompt remaining attempts
    }
 
    // If all login attempts fail, exit the program
    if (login_status == 0)
    {
        printf("\nWrong credentials. Exiting the program. \n\n");
        return 0;
    }
 
    int sock = 0, valread;  // Socket descriptor and read value
    struct sockaddr_in serv_addr;  // Server address structure
    char command[1024] = {0};  // Buffer for commands
    char buffer[BUFFER_LENGTH] = {0};  // Buffer for server responses
 
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }
 
    // Initialize server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return -1;
    }
 
    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }
 
    printf("Connected to server on port %d\n", PORT);
 
    // Main loop for interacting with the server
    while (1) {
        // Display admin menu and get user choice
        int choice = admin_menu();
        myflush();  // Flush input buffer
 
        // If user chooses to exit
        if (choice == 5) {
            send(sock, encrypt_string("EXIT"), 4, 0);  // Send exit command to server
            valread = read(sock, buffer, BUFFER_LENGTH);  // Read server response
 
            close(sock); 
            return 0; 
        }
 
        // Prepare command based on user choice
        int ch = create_command_helper(command, choice);
 
        // If create_command_helper indicates exit
        if (ch == 0) {
            send(sock, encrypt_string("EXIT"), 4, 0);  // Send exit command to server
            valread = read(sock, buffer, BUFFER_LENGTH);  // Read server response
 
            close(sock);  // Close socket
            return 0;  // Exit program
        }
 
        // If create_command_helper indicates reset
        if (ch == 10) {
            memset(command, '\0', sizeof(command));  // Clear command buffer
            memset(buffer, '\0', sizeof(buffer));    // Clear buffer
            continue;  // Skip sending command to server
        }
 
        // Encrypt the command and send to server
        char encr_command[1024];
        strcpy(encr_command, encrypt_string(command));
        send(sock, encr_command, strlen(encr_command), 0);
        printf("Sent command: %s\n\n", command);  // Display sent command
 
        
        valread = read(sock, buffer, BUFFER_LENGTH);
 
        if (valread > 0) {
            char decr_buffer[BUFFER_LENGTH];
            strcpy(decr_buffer, decrypt_string(buffer));  // Decrypt server response
            strcpy(buffer, decr_buffer);  // Copy decrypted response to buffer
            printf("Server response: %s\n\n", buffer);  // Display server response
        }
 
        // Clear command and buffer for next iteration
        memset(command, '\0', sizeof(command));
        memset(buffer, '\0', sizeof(buffer));
 
        printf("\nPress ENTER KEY to continue: ");
        myflush();  // Flush input buffer
    }
 
    close(sock);  // Close socket
 
    return 0;  // Exit program
}