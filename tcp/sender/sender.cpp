/*
 * Copyright (c) 2010 Instrumentation Technologies
 * All Rights Reserved.
 *
 * $Id$
 */

// UDP includes

using namespace std;

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

   #include <cstdlib>
    #include <cstring>
    #include <unistd.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


//funcija posluša za en port(en thred vse ponovitve)
void send_thr(uint32_t r,uint32_t size,uint32_t port,std::string name)
{


   int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[size];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(name.c_str());
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
	if (connect(sockfd,(const sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    bzero(buffer,size);
    for (int i=0;i <r;i++){ 
		buffer[0]=i;
		n = write(sockfd,buffer,size/*strlen(buffer)*/);
		if (n < 0) 
			error("ERROR writing to socket");
		//bzero(buffer,size);
		//n = read(sockfd,buffer,size);
		//if (n < 0) 
		//	error("ERROR reading from socket");
         
	 }
    close(sockfd);





}


void usage(const char *argv0)
{
    std::cout << "Usage: " << argv0 << " tcp_host tcp_port size iterations threads" << std::endl;
    std::cout << "       tcp_host:     Hostname/IP to send data to via tcp." << std::endl;
    std::cout << "       tcp_port:     Starting port number of tcp sending (increases with threads)." << std::endl;
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
		
		std::thread *t=new std::thread(send_thr,iterations,size,port,send_host);
		threads.push_back( t);
    }
    
    for(auto& thread : threads){
        thread->join();
    }

    exit(EXIT_SUCCESS);
	
}
