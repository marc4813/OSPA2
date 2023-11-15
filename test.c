/**
 * @file   test.c
 * @author Original: Derek Molloy
 * @author Current: Derek Aguirre, Kayla Funchess, Marcus Simmonds, Nikole Solano
 * @date   Original: 7 April 2015
 * @date   Current: 14 November 2023
 * @version 1.0
 * @brief  A Linux user space program that communicates with the charkmod.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/charkmod.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
 *
 * Adapted for COP 4600 by Dr. John Aedo
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFFER_LENGTH 256           ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH]; ///< The receive buffer from the LKM

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: test <path to device>\n");
        exit(0);
    }
    char *devicepath = argv[1];

    int ret, fd, len = 0;
    char stringToSend[BUFFER_LENGTH];
    
    strncpy(stringToSend, "", sizeof(stringToSend));
    printf("Starting device test code example...\n");
    fd = open(devicepath, O_RDWR); // Open the device with read/write access
    
    if (fd < 0) {
        perror("Failed to open the device...");
        return errno;
    }
  
    do {
        char input;
        
        printf("Options: \n r : read \n w : write \n e : exit\n");
        input = getchar();
        printf("\n");

        switch(input) {
            case 'R':
            case 'r':
                printf("Press ENTER to read back from the device\n");
                getchar();

                printf("Reading from the device...\n");
                ret = read(fd, receive, len); // Read the response from the LKM
    
                if (ret < 0) {
                    printf("The Buffer is empty\n");
                } else {
                    printf("[%s]\n", receive);
                }

                break;

            case 'W':
            case 'w':
                getchar();

                printf("Type in a short string to send to the kernel module:\n");
                scanf("%[^\n]%*c", stringToSend); // Read in a string (with spaces)
                printf("Writing message to the device [%s].\n", stringToSend);

                if(strlen(stringToSend) > 255) {
                    printf("String is too long\n");
                } else {
                    strcat(stringToSend, "&");

                    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
                    len = strlen(stringToSend);

                    if (ret < 0) {
                        printf("The Message is too long\n");
                    } 
                }

                break;  

            case 'E':
            case 'e':
                printf("Goodbye\n");

                return 1;

            default:
                printf("Invalid input, please try again\n");
                break;

        }

        printf("\n");
    } while(true);

    return 1;
}