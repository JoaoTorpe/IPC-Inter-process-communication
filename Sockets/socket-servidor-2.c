#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SOCK_PATH "unix_socket"
int sockfd;
void error(char *msg)
{
    perror(msg);
    exit(1);
}

void cleanup(int signum)
{

    close(sockfd);
    unlink(SOCK_PATH);
    exit(0);
}

int main(int argc, char *argv[])
{

     char buffer[256];
     struct sockaddr_un serv_addr, cli_addr;
     int n;


     sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");


     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sun_family = AF_UNIX;
     strcpy(serv_addr.sun_path, SOCK_PATH);


     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");


     printf("Server ready\n");


     int len = sizeof(cli_addr);
      while (1)
    {
        bzero(buffer, 256);

        len = sizeof(cli_addr);
        n = recvfrom(sockfd, buffer, 256 - 1, 0, (struct sockaddr *)&cli_addr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        buffer[n] = '\0';
        printf("%s", buffer);
    }



     unlink(SOCK_PATH);
     return 0;
}
