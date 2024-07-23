// Program to run the client code. Functions are called here and implemented in client_funct.h
#include <client.h>

char user_no[11] = {0};

int main() {

    int choice;

    system("clear");

    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    // Create socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Prepare server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on %s:%d\n", SERVER_IP, PORT);

    while (1) {
        choice = main_menu();

        if (choice == 1) {

            char local_no[11] = {0};

            printf("Enter number you want to make a phone call from: \n");
            scanf("%s", local_no);
            myflush();
            strcpy(user_no, local_no);

            int flag = 0;

            check(command);

            send_recv_query(client_fd, command, buffer);


            memset(command, '\0', sizeof(command));
            memset(buffer, '\0', sizeof(buffer));

            call(command);

            char command_copy[40];
            strcpy(command_copy, command);

            char *call_no = strtok(command_copy, " ");
            call_no = strtok(NULL, " ");

            send_recv_query(client_fd, command, buffer);

            if (buffer[0] == 'U' || buffer[0] == 'N') flag = 1;
            if (flag == 1) {
                // system("clear");
                printf("Calling number: %s\n\n", call_no);

            } else {
                char *token = strtok(buffer, " ");
                call_no = strtok(NULL, " ");
                int forward_type = atoi(strtok(NULL, " "));

                printf("Forwarding call to: %s\n", call_no);

                if (forward_type == 1) {
                    printf("Call connected\n\n");
                } else if (forward_type == 3) {
                    printf("The number you called is BUSY\n\n");
                } else {
                    sleep(3);
                    printf("The number is NOT RESPONDING\n\n");
                }

            }
            memset(user_no, '\0', sizeof(user_no));

        } else if (choice == 2) {

            char local_no[11] = {0};
            printf("Enter number you want to register with : \n");
            scanf("%s", local_no);
            myflush();
            strcpy(user_no, local_no);

            check(command);

            send_recv_query(client_fd, command, buffer);

            if (buffer[0] == 'F' || buffer[0] == 'N') {

                memset(command, '\0', sizeof(command));
                memset(buffer, '\0', sizeof(command));

                char comm[11] = "NA 1 -1 O";

                strcpy(command, "UPD_USER ");

                int i = 9;

                for (int j = 0; j < strlen(user_no); j++) {
                    command[i++] = user_no[j];
                }
                command[i++] = ' ';

                for (int j = 0; j < strlen(comm); j++) {
                    command[i++] = comm[j];
                }

                send_recv_query(client_fd, command, buffer);

                memset(command, '\0', sizeof(command));
                memset(buffer, '\0', sizeof(command));
                memset(user_no, '\0', sizeof(user_no));

                printf("\nPress ENTER KEY to continue:");
                myflush();

                continue;
            }

            memset(command, '\0', sizeof(command));
            memset(buffer, '\0', sizeof(command));

            register_user_pass(command);

            send_recv_query(client_fd, command, buffer);

            char *token = strtok(buffer, " ");

            memset(command, '\0', sizeof(command));

            if (strcmp(token, "[ERROR]") == 0) {

                printf("\nUser already Registered\nLogin from the Main Menu\n");

                printf("\nPress ENTER KEY to continue: ");
                memset(buffer, '\0', sizeof(buffer));

                myflush();
                continue;
            }

            memset(buffer, '\0', sizeof(buffer));

            register_user(command);

            send_recv_query(client_fd, command, buffer);
            myflush();

            while (1) {
                int ch = user_menu();
                if (ch >= 1 && ch <= 4) {
                    create_update_cmd(command, ch);
                } else if (ch == 5) {
                    printf("Making call from: %s\n", user_no);
                    int flag = 0;

                    check(command);

                    send_recv_query(client_fd, command, buffer);

                    memset(command, '\0', sizeof(command));
                    memset(buffer, '\0', sizeof(buffer));
                    
                    char call_number[11];
                            int i = 0;                            
                       
                            printf("Enter the number to call: ");
                            scanf("%s", call_number);
                            myflush();
                   
                            strcpy(command, "CFS_CODE ");
                       
                            i = 9;
                            for(int j=0; j<strlen(call_number); j++){
                                    command[i++] = call_number[j];
                            }
                
                    char command_copy[40];
                    strcpy(command_copy, command);
                    char *call_no = strtok(command_copy, " ");
                    call_no = strtok(NULL, " ");

                    send_recv_query(client_fd, command, buffer);

                    if (buffer[0] == 'U' || buffer[0] == 'N') {
                        // system("clear");
                        printf("Calling number: %s\n\n", call_no);
                    } else {
                        char *token = strtok(buffer, " ");
                        call_no = strtok(NULL, " ");
                        int forward_type = atoi(strtok(NULL, " "));

                        printf("Forwarding call to: %s\n", call_no);

                        if (forward_type == 1) {
                            printf("Call connected\n\n");
                        } else if (forward_type == 3) {
                            printf("The number you called is BUSY\n\n");
                        } else {
                            sleep(3);
                            printf("The number is NOT RESPONDING\n\n");
                        }

                    }
                } else if (ch == 6) {
                    break;
                } else if (ch == 7) {

                    send_recv_query(client_fd, encrypt_string("EXIT"), buffer);
                    close(client_fd);
                    return 0;
                }

                if (ch >= 1 && ch <= 5) {  
                    if(ch <= 4){
                        send_recv_query(client_fd, command, buffer);
                    }
                    printf("\nPress ENTER KEY to continue: ");
                    myflush();
                }
            }

        } else if (choice == 3) {
            int ch;
            int try = 3;
            int f = 0;
            while (try--) {

                if (try < 2) printf("Wrong password. Try again\n\n");

                login(command);

                send_recv_query(client_fd, command, buffer);

                if (strcmp(buffer, "AUTH_SUCCESS\n") == 0) {
                    f = 0;
                    break;
                } else {
                    f = 1;
                    continue;
                }
            }

            if (f) {
                printf("Authentication failed! Redirecting to menu\n");

                memset(command, '\0', sizeof(command));
                memset(buffer, '\0', sizeof(buffer));

                printf("\nPress ENTER KEY to continue: ");
                myflush();

                continue;
            }

            memset(command, '\0', sizeof(command));
            memset(buffer, '\0', sizeof(buffer));

            while (1) {
                ch = user_menu();
                if (ch >= 1 && ch <= 4) {
                    create_update_cmd(command, ch);
                } else if (ch == 5) {
                    
                    printf("Making call from: %s\n", user_no);
                    int flag = 0;
                    check(command);

                    send_recv_query(client_fd, command, buffer);

                    memset(command, '\0', sizeof(command));
                    memset(buffer, '\0', sizeof(buffer));
                    
                    char call_number[11];
                            int i = 0;                            
                        
                            printf("Enter the number to call: ");
                            scanf("%s", call_number);
                            myflush();
                    
                            strcpy(command, "CFS_CODE ");
                        
                            i = 9;
                            for(int j=0; j<strlen(call_number); j++){
                                    command[i++] = call_number[j];
                            }
 
                    
                    char command_copy[40];
                    strcpy(command_copy, command);
                    char *call_no = strtok(command_copy, " ");
                    call_no = strtok(NULL, " ");

                    send_recv_query(client_fd, command, buffer);
                    if (buffer[0] == 'U' || buffer[0] == 'N') {
                        // system("clear");
                        printf("Calling number: %s\n\n", call_no);

                    } else {
                        char *token = strtok(buffer, " ");
                        call_no = strtok(NULL, " ");
                        int forward_type = atoi(strtok(NULL, " "));

                        printf("Forwarding call to: %s\n", call_no);

                        if (forward_type == 1) {
                            printf("Call connected\n\n");
                        } else if (forward_type == 3) {
                            printf("The number you called is BUSY\n\n");
                        } else {
                            sleep(3);
                            printf("The number is NOT RESPONDING\n\n");
                        }

                    }
                } else if (ch == 6) {
                    break;
                } else if (ch == 7) {
                    send_recv_query(client_fd, encrypt_string("EXIT"), buffer);
                    close(client_fd);
                    return 0;
                }
                
                if (ch >= 1 && ch <= 5) {
                    if(ch <= 4){
                        send_recv_query(client_fd, command, buffer);
                    }
                    printf("\nPress ENTER KEY to continue: ");
                    myflush();
                }
            }

        } else {
            send_recv_query(client_fd, encrypt_string("EXIT"), buffer);
            close(client_fd);
            return 0;
        }

        memset(command, '\0', sizeof(command));
        memset(buffer, '\0', sizeof(buffer));

        printf("\nPress ENTER KEY to continue: ");
        myflush();

    }
    close(client_fd);
}


