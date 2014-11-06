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





//funcija posluša za en port(en thred vse ponovitve)
void listen_thr(uint32_t r,uint32_t size,uint32_t port,std::string name)
{
	
	//socket()
	//bind()
//	int sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//	if(!sockfd)return -1;
//	int flags=
//	struct sockaddr *src_addr=
	
//	socklen_t *addrlen =
//	char buf[size];
	
	
	
	int flags=0;
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	char mesg[size];

	sockfd=socket(AF_INET,SOCK_DGRAM,0);

	bzero(&servaddr,sizeof(servaddr));
	bzero(&cliaddr,sizeof(cliaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(port);
	bind(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	
	for (uint32_t i =0 ; i<r; i++){
		ssize_t re= recvfrom(sockfd, mesg,size, flags, (struct sockaddr *)&cliaddr, &len);
		if(re!=size)std::cout<<"!"/*<<mesg[0]*/;
	}

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
