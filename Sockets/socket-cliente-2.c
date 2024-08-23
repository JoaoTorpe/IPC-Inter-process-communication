#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH "unix_socket"
#define BUFFER_SIZE 256

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_un serv_addr;
    FILE *file;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("Erro ao abrir socket");


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, SOCK_PATH);


    file = fopen(argv[1], "r");
    if (file == NULL) {
        error("Erro ao abrir o arquivo");
    }
   printf("Enviando linhas do arquivo para o servidor...\n");
     while (fgets(buffer, BUFFER_SIZE, file)) {

     n = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    }







    fclose(file);
    close(sockfd);
    return 0;
}
