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
//  FILE:        client.c
//
//  DESCRIPTION:
//   This file receives a file once it connects and closes the connection.
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
//  DESCRIPTION:   The client code to receive a image once it connects and closes the connection.
//
//  Parameters:    argc    (int)  : contains the number of arguments
//                                  passed into the program
//                 *argv[] (char) : the listing of all arguments
//
//  Return value:  0 : The message receiving was successful.
//                -1 : The message receiving was unsuccessful.
//
****************************************************************/

int main(int argc, char *argv[]) {
{
    int client_socket;
    int portNumber = atoi(argv[1]);
    struct sockaddr_in server_addr;
    FILE *client_output;
    char parray[50000];
    char sizeMessage[2000];
    FILE *image;
    int bytesReceived;
    int currentlyRead;
    int size;
    
    /* Setting counts to 0 */
    currentlyRead = 0;
    bytesReceived = 0;
    size = 0;
    
    /* Opening output file */
    client_output = fopen("output_client.txt", "w");
    
    /* Create Socket */
    client_socket = socket(AF_INET, SOCK_STREAM,0);

    if (client_socket < 0)
    {
        printf("Socket unsuccessfully created\n");
        fprintf(client_output, "Socket unsuccessfully created\n");
    }
    else
    {
        printf("Socket created successfully\n");
        fprintf(client_output, "Socket created successfully\n");
    }

    /* Setting port and IP */
    printf("Setting up the server address at localhost and port %d\n", portNumber);
    fprintf(client_output, "Setting up the server address at localhost and port %d\n", portNumber);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(portNumber);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    printf("Set up complete\n");
    fprintf(client_output, "Set up complete\n");
    
    /* Send connection request to server */
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Connection with server unsuccessful\n");
        fprintf(client_output, "Connection with server unsuccessful\n");
    }
    else
    {
        printf("Connection with server successful\n");
        fprintf(client_output, "Connection with server successful\n");
    }
    
    if (recv(client_socket, sizeMessage, sizeof(sizeMessage), 0) < 0)
    {
        printf("Size of Picture Message Was Not Received\n");
        fprintf(client_output, "Size of Picture Message Was Not Received\n");
    }
    else
    {
        printf("Size of Picture Message Was Received\n");
        fprintf(client_output, "Size of Picture Message Was Received\n");
    }
    ;

    /* Converting message to int */
    size = atoi(sizeMessage);
    
    /* Receive the server's image */
    printf("Preparing to receive the image from the server by opening file_clinet.jpg\n");
    fprintf(client_output, "Preparing to receive the image from the server by opening file_clinet.jpg\n");
    
    /* Opening image file */
    image = fopen("file_client.jpg", "wb");
    
    /* Reading the received packets and writing it to the file_client.jpg file */
    while ( currentlyRead < size)
    {
        bytesReceived = read(client_socket, parray, 1 /*sizeof(parray)*/);
        currentlyRead += bytesReceived;
        fwrite(parray, 1, bytesReceived, image);
    }
    fclose(image);
    printf("The picture named file_clinet.jpg was received successfully\n");
    fprintf(client_output, "The picture named file_clinet.jpg was received successfully\n");
    
    /* Close the socket */
    close(client_socket);
    printf("Closing the socket\n");
    fprintf(client_output, "Closing the socket\n");
    fclose(client_output);
    
    return 0;
    }
}
