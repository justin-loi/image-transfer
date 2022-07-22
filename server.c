/*****************************************************************
//
//  NAME:        Justin Loi
//
//  HOMEWORK:    Assignment 4 Programming 3
//
//  CLASS:       ICS 451
//
//  INSTRUCTOR:  Ravi Narayan
//
//  DATE:        October 5th, 2021
//
//  FILE:        server.c
//
//  DESCRIPTION:
//   This file sends a file once a client connects and closes the message.
//   However the server will continue to run and accept new connections. 
//
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

/*****************************************************************
//
//  Function name: main
//
//  DESCRIPTION:   The server code to send a image once a client connects.
//
//  Parameters:    argc    (int)  : contains the number of arguments
//                                  passed into the program
//                 *argv[] (char) : the listing of all arguments 
//
//  Return value:  0 : The message sending was successful.
//                -1 : The message sending was unsuccessful.
//
****************************************************************/

int main(int argc, char *argv[]) {

    /* Declaring variables */ 
    FILE *server_output;
    FILE *picture;
    int server_socket;
    int client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int portNumber = atoi(argv[1]);
    int exitCounter = 0;
    int client_size;
    int user_input;
    char trashStream[200];
    int bytes_read;
    int currently_read;
    int sizeofPicture;
    char send_buffer[50000];
    char sizeMessage[100];
    
    /* Setting counts to 0 */
    bytes_read = 0;
    currently_read = 0;
    
    /* Opening output and picture files */
    server_output = fopen("output_server.txt", "w");
    picture = fopen("file_server.jpg", "rb");
    
    /* Create socket */
    printf("Attempting to create server socket\n");
    fprintf(server_output, "Attempting to create server socket\n");

    /* SOURCE FROM URL: https://www.geeksforgeeks.org/socket-programming-cc/ */
    server_socket = socket(AF_INET, SOCK_STREAM,0);

    if (server_socket < 0) 
    {
        printf("Error when creating socket\n");
        fprintf(server_output, "Error when creating socket\n");
    }
    else 
    {
        printf("Socket created successfully\n");
        fprintf(server_output, "Socket created successfully\n");
    }

    /* Setting port and IP */
    printf("Setting up the server address at localhost and port %d\n", portNumber);
    fprintf(server_output, "Setting up the server address at localhost and port %d\n", portNumber);

    /* SOURCE FROM URL: https://www.geeksforgeeks.org/socket-programming-cc/ */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portNumber);
    server_address.sin_addr.s_addr = INADDR_ANY;

    printf("Set up complete\n");
    fprintf(server_output, "Set up complete\n");

    /* Binding */
    printf("Attempting to bind socket and address\n");
    fprintf(server_output, "Attempting to bind socket and address\n");    

    /* SOURCE to understand bind() function FROM URL: https://www.geeksforgeeks.org/socket-programming-cc/ */
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0 ) 
    {
        printf("Binding unsuccessful\n");
        fprintf(server_output, "Binding unsuccessful\n");
    }
    else 
    {
        printf("Binding successfully\n");
        fprintf(server_output, "Binding successfully\n");
    }

    while (exitCounter != -1)
    {

        /* Listen */
        /* SOURCE to understand listen() function FROM URL: https://www.geeksforgeeks.org/socket-programming-cc/ */
        if (listen(server_socket, 4) < 0) 
        {
            printf("Listening unsuccessful\n");
            fprintf(server_output, "Listening unsuccessful\n");
        } 
        else 
        {
           printf("Listening Successful\n");
            fprintf(server_output, "Listening Successful\n");        
        }

        /* Accept */
        client_size = sizeof(client_address);
        /* SOURCE to understand accept() function FROM URL: https://www.geeksforgeeks.org/socket-programming-cc/ */
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, (socklen_t*)&client_size);

        if (client_socket < 0)
        {
            printf("Accepting Unsuccessful\n");
            fprintf(server_output, "Accepting Unsuccessful\n");  
        }
        else
        {
            printf("Listening Successful\n");
            fprintf(server_output, "Accepting Successful\n");  
        }

        /* Message Client */
        
        /* Getting size of picture */
        printf("Getting the size of the image\n");
        fprintf(server_output, "Getting the size of the image\n");
        fseek(picture, 0, SEEK_END);
        sizeofPicture = ftell(picture);
        fseek(picture,0,SEEK_SET);
        sprintf(sizeMessage, "%d", sizeofPicture);
        
        printf("Preparing to send the size of the image to the client socket\n");
        fprintf(server_output, "Preparing to send the size of the image to the client socket\n");

        
        if (send(client_socket, sizeMessage, strlen(sizeMessage), 0) < 0)
        {
            printf("Size of Image Message Sent Unsuccessfully\n");
            fprintf(server_output, "Size of Image Message Sent Unsuccessfully\n");
        }
        else
        {
            printf("Size of Image Message Sent Successfully\n");
            fprintf(server_output, "Size of Image Message Sent Successfully\n");
        }
        
        /* Send Picture as Byte Array (without need of a buffer as large as the image file) */
        printf("Preparing To Send Picture As Byte Array\n");
        fprintf(server_output, "Preparing To Send Picture As Byte Array\n");
        
        while(bytes_read < sizeof(picture)) {
            currently_read = fread(send_buffer, 1, sizeof(send_buffer), picture);
            write(server_socket, send_buffer, bytes_read);
            bytes_read += currently_read;
        }
        
        printf("Picture Stored Into Byte Array Successfully\n");
        fprintf(server_output, "Picture Stored Into Byte Array Successfully\n");
        
        if (send(client_socket, send_buffer, sizeof(send_buffer), 0) < 0)
        {
            printf("Picture was sent unsuccessfully\n");
            fprintf(server_output, "Picture was sent unsuccessfully\n");
        }
        else
        {
            printf("Picture Sent Successfully\n");
            fprintf(server_output, "Picture Sent Successfully\n");
        }

        /* Ask if they want to end the server*/
        printf("Asking if server should continue: ");
        fprintf(server_output, "Asking if server should continue: ");        

        printf("If you want the program to end, enter -1. Otherwise, type any number (other than -1) to keep the server running:");
        scanf("%d", &user_input);
        
        if (user_input == -1) 
        {
            exitCounter = -1;
            printf("Stopping the server\n");
            fprintf(server_output, "NO\n");
        }
        else 
        {
            exitCounter = 0; 
            printf("Continuing the server\n");
            fprintf(server_output, "YES\n");
            fgets(trashStream, 10, stdin);
        }
    }


    /* Closing file and ending program */
    fprintf(server_output, "Closing the server\n");
    printf("Closing the server\n");
    fclose(server_output);
    close(server_socket);

    return 0;
}
