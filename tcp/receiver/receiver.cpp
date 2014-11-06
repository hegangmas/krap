/*
 * Copyright (c) 2010 Instrumentation Technologies
 * All Rights Reserved.
 *
 * $Id$
 */

// UDP includes


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include <iostream>
#include <string>
#include <vector>
#include <thread>

#include <stdint.h> 


void error(const char *msg)
{
    perror(msg);
    exit(1);
}


//funcija posluša za en port(en thred vse ponovitve)
void listen_thr(uint32_t r,uint32_t size,uint32_t port,std::string name)
{


     int sockfd, newsockfd;
     socklen_t clilen;
     char buffer[size];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(port);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)  error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,size);
	 for (int i = 0 ;i<r ;i++){
         n = read(newsockfd,buffer,size);
         if (n < 0) error("ERROR reading from socket");
                //std::cout<<n<<".";
                //if(n!=size) error("not same");
//              n = write(newsockfd,"I got your message",18);
//              if (n < 0) error("ERROR writing to socket");

        }

     close(newsockfd);
     close(sockfd);
 



}


void usage(const char *argv0)
{
    std::cout << "Usage: " << argv0 << " udp_host udp_port size iterations threads" << std::endl;
    std::cout << "       udp_host:     Hostname/IP to send data to via UDP." << std::endl;
    std::cout << "       udp_port:     Starting port number of UDP sending (increases with threads)." << std::endl;
    std::cout << "       size:         UDP Packet size." << std::endl;
    std::cout << "       iterations:   Number of iterations/atoms to process." << std::endl;
    std::cout << "       threads:      Number of parallel threads to create." << std::endl;
}


int main(int argc, char** argv)
{
	
    if (argc < 6) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string send_host =         argv[1];
    uint32_t send_port = atoi(argv[2]);
    uint32_t size =            atoi(argv[3]);
    uint32_t iterations =      atoi(argv[4]);
    uint32_t nothreads =         atoi(argv[5]);

    if (nothreads == 0) {
        exit(EXIT_SUCCESS);
    }
		
	std::vector<std::thread*> threads;

    for (unsigned short i=0; i < nothreads; i++) {
		uint32_t port= i+send_port;
		
		std::thread *t=new std::thread(listen_thr,iterations,size,port,send_host);
		threads.push_back( t);
    }
    
    for(auto& thread : threads){
        thread->join();
    }

    exit(EXIT_SUCCESS);
	
}
