#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <assert.h>

#define MAX 80
#define PORT 8080

void func(int sockfd)
{
   char buff[MAX];
   int n;
   for (;;)
   {
      bzero(buff, sizeof(buff));
      printf("Enter the string : ");
      n = 0;
      while ((buff[n++] = getchar()) != '\n')
         ;
      write(sockfd, buff, sizeof(buff));
      bzero(buff, sizeof(buff));
      read(sockfd, buff, sizeof(buff));
      printf("From Server : %s", buff);
      if ((strncmp(buff, "exit", 4)) == 0)
      {
         printf("Client Exit...\n");
         break;
      }
   }
}

int main()
{
   int sockfd, connfd;
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

   // assign IP, PORT
   servaddr.sin_family = AF_INET;
   servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
   /* why inet_addr ?
      convert the human readable IP notation to hexadecimal value used by system*/
   servaddr.sin_port = htons(PORT);

#ifdef DNS_LOOK_UP
   /* consider if server address is provided as a URL
      eg: https://www.google.co.in/ how to change this to IP ? */
   struct addrinfo hints, *server_list = NULL, *server = NULL;
   memset(&hints, 0, sizeof(hints));
   /* hints used to filter the IP list as per our specific needs */
   hints.ai_family = AF_INET6;      /* change to AF_INET for IPv4 */
   hints.ai_socktype = SOCK_STREAM; /* limit to byte-streams */
   hints.ai_protocol = IPPROTO_TCP; /* create as a TCP socket */
   /* Get a list of addresses at hostname that serve HTTP */
   getaddrinfo("google.co.in", "https", &hints, &server_list);
   /* Traverse through the linked list of results */
   for (server = server_list; server != NULL; server = server->ai_next)
   {
      if (server->ai_family == AF_INET6)
      {
         /* Cast ai_addr to an IPv6 socket address */
         struct sockaddr_in6 *addr = (struct sockaddr_in6 *)server->ai_addr;

         /* Allocate a buffer to store the IPv6 string */
         char in6addr[INET6_ADDRSTRLEN];
         assert(inet_ntop(AF_INET6, &addr->sin6_addr, in6addr, sizeof(in6addr)) != NULL);
         printf("IPv6 address of google india: %s\n\n", in6addr);
      }
   }
   freeaddrinfo(server_list); /* Free allocated linked list data */
#endif

   /* connect() performs the
      client side of the 3-way TCP handshake to establish the connection,
      storing information about the server in the address and address_len fields*/
   if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
   {
      printf("connection with the server failed...\n");
      exit(0);
   }
   else
      printf("connected to the server..\n");

   // read write sequence procedure as per application requirements
   func(sockfd);

   // close the socket
   close(sockfd);
}
