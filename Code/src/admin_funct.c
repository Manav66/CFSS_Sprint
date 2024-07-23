#include <admin.h>  // Include header file where admin-related functions are declared
#define USERID "admin"  // Define admin user ID
#define PWD "admin@123"  // Define admin password
 
// Function prototypes
int login(void);
int admin_menu(void);
void myflush(void);
int create_command_helper(char[], int);
 
// Static function prototypes
static int update_menu(void);
static void create_command(char[], int);
 
// Function to flush input buffer
void myflush(void) {
    while (getchar() != '\n');
}
 
// Function to encrypt a string with an offset value
char* encrypt_string(const char* string) {
    size_t len = strlen(string);
    char* newString = (char*)malloc((len + 1) * sizeof(char));
    strcpy(newString, string);
    // Encryption algorithm (offset by 34)
    for (int i = 0; i < len; i++) {
        newString[i] += 34;
    }
    return newString;
}
 
// Function to decrypt a string with an offset value
char* decrypt_string(const char* string) {
    size_t len = strlen(string);
    char* newString = (char*)malloc((len + 1) * sizeof(char));
    strcpy(newString, string);
    // Decryption algorithm (offset by 34)
    for (int i = 0; i < len; i++) {
        newString[i] -= 34;
    }
    return newString;
}
 
// Function to authenticate admin login
int login() {
    char user[10];
    char pas[20];
    printf("Enter User ID: ");
    scanf("%s", user);
    printf("Enter Password: ");
    scanf("%s", pas);
    // Check if user ID and password match predefined values
    if (strcmp(user, USERID) == 0 && strcmp(pas, PWD) == 0) {
        return 1;  // Successful login
    } else {
        return 0;  // Failed login
    }
}
 
// Function to create a command based on user input
void create_command(char command[], int ch) {
    char client_number[11];
    char forwarding_number[11];
    int registered = 0;
    int forward_activated = 0;
    char forwarding_type;
    int i = 0;
    printf("\nEnter client number: ");
    scanf("%s", client_number);
    myflush();
    if (ch == 0 || ch == 1) {
        printf("\nEnter forwarding number: ");
        scanf(" %s", forwarding_number);
        myflush();
    } else {
        strcpy(forwarding_number, "NA");
    }
 
    if (ch == 0) {
        printf("\nRegister/Unregister user(0/1): ");
        scanf("%d", &registered);
        myflush();
    } else if (ch == 4) {
        registered = 0;
    } else {
        registered = -1;
    }
 
    if (ch == 0 || ch == 3) {
        printf("\nActivate Service(0/1): ");
        scanf("%d", &forward_activated);
        myflush();
    } else {
        forward_activated = -1;
    }
 
    if (ch == 2 || ch == 0) {
        printf("\nForward type\nUnconditional - U\nNo reply - N\nBusy - B\n(U/N/B): ");
        scanf(" %c", &forwarding_type);
        myflush();
    } else {
        forwarding_type = 'O';
    }
 
    if (ch == 0) {
        strcpy(command, "ADD_USER");
        i += 8;
    } else {
        strcpy(command, "UPD_USER");
        i += 8;
    }
 
    command[i++] = ' ';
 
    for (int j = 0; j < strlen(client_number); j++) {
        command[i++] = client_number[j];
    }
    command[i++] = ' ';
    for (int j = 0; j < strlen(forwarding_number); j++) {
        command[i++] = forwarding_number[j];
    }
    command[i++] = ' ';
 
    if (registered != -1) {
        command[i++] = '0' + registered;
    } else { 
        command[i++] = '-';
        command[i++] = '1';
    }
    command[i++] = ' ';
 
    if (forward_activated != -1) {
        command[i++] = '0' + forward_activated;
    } else { 
        command[i++] = '-';
        command[i++] = '1';
    }
    command[i++] = ' ';
 
    command[i++] = forwarding_type;
}
 
// Function to display admin menu and get user choice
int admin_menu(void) {
    int try = 4;
    int choice;
 
    while (try--) {
        system("clear");
 
        printf("******************************************\n");
        printf("          Welcome to Admin Menu            \n");
        printf("******************************************\n\n");
        printf("\t1. View Record\n");
        printf("\t2. Add Record\n");
        printf("\t3. Update Record\n");
        printf("\t4. Delete Record\n");
        printf("\t5. Exit\n");
        printf("\n******************************************\n");
        if (try < 3) {
            printf("\nWrong input. You have %d tries left. Try Again", try + 1);
        }
        printf("\nEnter your choice (1-5): ");
        scanf("%d", &choice);
 
        if (choice < 1 || choice > 5) {
            continue;  // Retry if input is out of range
        } else {
            break;  // Exit loop on valid input
        }
    }
 
    return choice;
}
 
// Static function to display update menu and get user choice
static int update_menu() {
    int try = 3;
    int choice;
 
    while (try--) {
        system("clear");
 
        printf("******************************************\n");
        printf("               Update Menu                \n");
        printf("******************************************\n\n");
        printf("\t1. Forwarding number\n");
        printf("\t2. Forwarding type\n");
        printf("\t3. Enable/Disable forwarding\n");
        printf("\t4. Unregister for CFS\n");
        printf("\t5. Back to main menu\n");
        printf("\n******************************************\n");
 
        if (try < 2) {
            printf("\nWrong input. You have %d tries left. Try Again", try + 1);
        }
        printf("\nEnter your choice (1-5): ");
 
        scanf("%d", &choice);
 
        if (choice < 1 || choice > 5) {
            continue;  // Retry if input is out of range
        } else {
            break;  // Exit loop on valid input
        }
    }
 
    return choice;
}
 
// Function to determine and create command based on admin menu choice
int create_command_helper(char command[], int choice) {
    int ch;
    char del_user[11];
 
    switch (choice) {
        case 1:
            strcpy(command, "VIEW_USER");
            break;
 
        case 2:
            create_command(command, 0);
            break;
 
        case 3:
            ch = update_menu();
            if (ch == 5)
                return 10;  // Return 10 to indicate back to main menu
            create_command(command, ch);
            break;	
 
        case 4:
            printf("Enter client number to delete: ");
            scanf("%s", del_user);
            myflush();
            strcpy(command, "DEL_USER ");
            for (int i = 0; i < strlen(del_user); i++) {
                command[i + 9] = del_user[i];	
            }
            break;
 
        case 5:
            return 0; 
    }
 
    return 1;  
}