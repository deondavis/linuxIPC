#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 80
#define PORT 8080

void func(int connfd)
{
   char buff[MAX];
   int n;
   // infinite loop for chat
   for (;;)
   {
      bzero(buff, MAX);

      // read the message from client and copy it in buffer
      read(connfd, buff, sizeof(buff));
      // print buffer which contains the client contents
      printf("From client: %s\t To client : ", buff);
      bzero(buff, MAX);
      n = 0;
      // copy server message in the buffer
      while ((buff[n++] = getchar()) != '\n')
         ;

      // and send that buffer to client
      write(connfd, buff, sizeof(buff));

      // if msg contains "Exit" then server exit and chat ended.
      if (strncmp("exit", buff, 4) == 0)
      {
         printf("Server Exit...\n");
         break;
      }
   }
}

int main()
{
   int sockfd, connfd, len;
   struct sockaddr_in servaddr, cli;

   // socket creation
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd == -1)
   {
      printf("socket creation failed...\n");
      exit(0);
   }
   else
      printf("Socket successfully created..\n");
   bzero(&servaddr, sizeof(servaddr));

   /* diffrence between struct sockaddr and struct sockaddr_in ?
      struct sockaddr is the generic structure expected by socket APIs
      sockaddr_in is specific to AF_INET, created with intention to fill the address data field
      in struct sockaddr.
      check the size of both the struct its same, so can be type casted without error */
   // assign IP and PORT
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = INADDR_ANY;
   /* why INADDR_ANY ?
      since server we are binding to all available interfaces eg: wlan eth etc
      reference: https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming */
   servaddr.sin_port = htons(PORT);
   /* why htons ?
      most modern CPU architectures use a little endian format, network protocols use big endian
      htons --> host to network */

   /* setsockopt() if required called before bind
      used to set various socket options like timeout, port reuse etc settings */
   // Binding newly created socket to given IP
   if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
   {
      printf("socket bind failed...\n");
      exit(0);
   }
   else
      printf("Socket successfully binded..\n");

   /* since TCP connection
      converts the socket to a connection-oriented server socket
      with a designated request queue */
   if ((listen(sockfd, 5)) != 0)
   {
      printf("Listen failed...\n");
      exit(0);
   }
   else
      printf("Server listening..\n");
   len = sizeof(cli);

   /* When a new request arrives, accept() performs the
      server side of the 3-way TCP handshake to establish the connection,
      storing information about the client in the address and address_len fields*/

   /* if u check the cli struct fields the clients port will not  be 8080 (#define PORT 8080)
      Instead, client sockets are assigned an ephemeral port,
      which is a pseudo-randomly selected integer in the range 1024 - 65535,
      when they are being set up
      if you want a specific port binding use bind() call after socket creation at the client side*/
   connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
   if (connfd < 0)
   {
      printf("server accept failed...\n");
      exit(0);
   }
   else
      printf("server accept the client...\n");

   // read write sequence procedure as per application requirements
   func(connfd);

   // close the socket after use
   close(sockfd);
}
