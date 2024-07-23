// Backend functions for handling client-server interactions and operations in the server file.
#include"server.h"
#include"sqlite3.h"

sqlite3* db;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char logMsg[128];
    
    // Fetch current date and time (to name the log file).
    char logFileName[50] = "server-log-";
    char* initTime = get_current_time();
    sprintf(logFileName + strlen(logFileName), "%s.txt", initTime);

    // Open the log file for the server
    FILE* logger = fopen(logFileName, "a");
    
    system("clear");

    // Special ASCII Art for the server program
    // From: https://patorjk.com/software/taag/#p=display&f=ANSI%20Shadow&t=ForwardIt
    printf("\n");
    printf("   ██████╗███████╗███████╗███████╗  \n");
    printf("  ██╔════╝██╔════╝██╔════╝██╔════╝  \n");
    printf("  ██║     █████╗  ███████╗███████╗  \n");
    printf("  ██║     ██╔══╝  ╚════██║╚════██║  \n");
    printf("  ╚██████╗██║     ███████║███████║  \n");
    printf("   ╚═════╝╚═╝     ╚══════╝╚══════╝  \n");
    printf("                                        Call Forwarding System - Simulator\n\n");

    sprintf(logMsg, "[INFO] Starting server\n");
    printf("[INFO] Starting server\n");
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);

    sprintf(logMsg, "[INFO] Time: %s\n\n", initTime);
    printf("[INFO] Time: %s\n\n", initTime);
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
    free(initTime);

    // Initialize SQLite database
    sprintf(logMsg, "[INFO] Opening database\n");
    printf("[INFO] Opening database\n");
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
    if(open_database()) 
    {
        sprintf(logMsg, "[FATAL] Failed to open the database and set the required tables\n");
        printf("[FATAL] Failed to open the database and set the required tables\n");
        fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
        exit(1);
    }
    
    // Creating socket file descriptor which helps in opening the socket on the server side.
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        char* errorLogMsg = "[FATAL] Socket failed";
        perror(errorLogMsg);
        fwrite(errorLogMsg, sizeof(char), strlen(errorLogMsg), logger);
        fclose(logger);
        close_database();
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attaching socket to the port number 13569.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        char* errorLogMsg = "[FATAL] Failed to attach socket";
        perror(errorLogMsg);
        fwrite(errorLogMsg, sizeof(char), strlen(errorLogMsg), logger);
        fclose(logger);
        close_database();
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure.
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Attempt to bind the server socket to the specified localhost address and port (13569).
    // If the binding fails, log a fatal error, close resources (logger, database connection),
    // and terminate the program with EXIT_FAILURE.
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        char* errorLogMsg = "[FATAL] Bind failed";
        perror(errorLogMsg);
        fwrite(errorLogMsg, sizeof(char), strlen(errorLogMsg), logger);
        fclose(logger);
        close_database();
        exit(EXIT_FAILURE);
    }
    
    // Start listening on the server socket for incoming client connections, allowing up to MAX_CLIENTS pending connections.
    // If listening fails, log a fatal error, close resources (logger, database connection),
    // and terminate the program with EXIT_FAILURE.
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        char* errorLogMsg = "[FATAL] Failed to initiate listening";
        perror(errorLogMsg);
        fwrite(errorLogMsg, sizeof(char), strlen(errorLogMsg), logger);
        fclose(logger);
        close_database();
        exit(EXIT_FAILURE);
    }
    
    // If listening is succesfull, start listening on Port 13569.
    sprintf(logMsg, "[INFO] Server listening on port %d...\n", PORT);
    printf("[INFO] Server listening on port %d...\n", PORT);
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
    
    fclose(logger);

    // Close the server if user presses Ctrl + C. Signal is passed to interrupt_handler which handles this.
    signal(SIGINT, interrupt_handler);
    
    while (1) {
        logger = fopen(logFileName, "a");
        // Accept an incoming client connection on the server socket.
        // If accepting the connection fails, log a fatal error, close resources (logger),
        // and terminate the program with EXIT_FAILURE.
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            char* errorLogMsg = "[FATAL] Failed to accept incoming connection";
            perror(errorLogMsg);
            fwrite(errorLogMsg, sizeof(char), strlen(errorLogMsg), logger);
            fclose(logger);
            exit(EXIT_FAILURE);
        }
        
        // Handle client communication
        sprintf(logMsg, "[INFO] Handling incoming client communication\n");
        printf("[INFO] Handling incoming client communication\n");
        fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
        
        while(handle_client(new_socket, logFileName)==0);
        
        // Close the socket after handling the client
        sprintf(logMsg, "[INFO] Closing the socket\n");
        printf("[INFO] Closing the socket\n");
        fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
        close(new_socket);

        if (fseek(logger, 0, SEEK_END) != 0) {
            perror("[ERROR] Error seeking to end of log file");
            sprintf(logMsg, "[ERROR] Error seeking to end of log file. Expect missing logs in the server log.");
            fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
        }
        fclose(logger);
    }
    
    // Close the socket and close the server.
    close(new_socket);
    close(server_fd);
    sprintf(logMsg, "[INFO] Closing server\n");
    printf("[INFO] Closing server\n");
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);

    sprintf(logMsg, "[INFO] Closing SQLite database connection\n");
    printf("[INFO] Closing SQLite database connection\n");
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
    close_database(); // Close SQLite database  

    sprintf(logMsg, "[INFO] Closing the logger\n");
    printf("[INFO] Closing the logger\n");
    fwrite(logMsg, sizeof(char), strlen(logMsg), logger);
    fclose(logger);

    return 0;
}
