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

/* Error reporting */
void error(std::string msg, int err)
{
    std::string ex = msg;

    if (err) {
        ex += ": " + std::string(strerror(err));
    }

    throw;
}

class UdpTx
{
public:
    UdpTx(const std::string& hostname, unsigned short port, int flags = 0);
    ~UdpTx();

    int Send(const void *buf, size_t len);

private:
    int m_sd;
    struct sockaddr_in m_dest_addr;
    int m_flags;
};

/* Constructor - Open UDP connection */
UdpTx::UdpTx(const std::string& a_hostname, unsigned short a_port, int a_flags) :
    m_flags(a_flags)
{

    // Create a socket
    if ( (m_sd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        error("Client: socket()", errno);
    }

    // Reuse address
    const int on = 1;
    if ( setsockopt(m_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ) {
        error("setsockopt(): SO_REUSEADDR", errno);
    }

    memset(static_cast<void*>(&m_dest_addr), 0, sizeof(m_dest_addr));
    m_dest_addr.sin_family = AF_INET;
    m_dest_addr.sin_port = htons(a_port);

    // Get host info
    struct hostent *hp = gethostbyname(a_hostname.c_str());
    if (hp == NULL) {
        std::string err = "Unknown host: " + std::string(a_hostname);
        error(err, 0);
    }
    memcpy(&m_dest_addr.sin_addr, hp->h_addr, hp->h_length);
}

/* Destructor - close socket */
UdpTx::~UdpTx()
{
    close(m_sd);
}

/* Send UDP data */
int UdpTx::Send(const void *buf, size_t len)
{
    int ret;

    ret = sendto(m_sd, buf, len, m_flags,
                 reinterpret_cast<struct sockaddr *>(&m_dest_addr),
                 sizeof(m_dest_addr));

    if (ret < 0) error("send()", errno);

    return ret;
}


/* Transmitter wrapper */
class Tx : public UdpTx
{
 public:
    Tx(uint32_t a_packet_size, uint32_t a_iter, const std::string& a_hostname = "1.2.3.4", unsigned short a_port = 2048,uint32_t slee=100);
    ~Tx();

    void operator()();
    void Join();
    void Enable();

 private:

    bool                  m_enable;
    uint32_t              m_size;
    uint32_t              m_iter;
 
    std::string           m_hostname;
    unsigned short        m_port;
   uint32_t			  m_sleep_for;
    std::thread           m_thread;
};

Tx::Tx(uint32_t a_packet_size, uint32_t a_iter, const std::string& a_hostname, unsigned short a_port,uint32_t slee) : 
    UdpTx(a_hostname, a_port),
    m_enable(false),
    m_size(a_packet_size),
    m_iter(a_iter),
    m_hostname(a_hostname),
    m_port(a_port),
    m_sleep_for(slee)
{
    m_thread = std::thread(std::ref(*this));
}


Tx::~Tx()
{
    Join();
}


void Tx::Join()
{
    if (m_thread.joinable()) {
        m_thread.join();
    }
}


void Tx::operator()()
{
    //std::cerr << "Thread created." << std::endl;

    while (!m_enable) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    char buffer[m_size];
    for(uint32_t i=0; i < m_iter; i++) {
        //std::cerr << "Sending " << m_size << " bytes to " << m_hostname << ":" << m_port << std::endl;
        buffer[0]=m_iter;
        if(m_size!=Send(buffer, m_size)){
			std::cout<<"ERROR"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(m_sleep_for));
        //std::this_thread::sleep_for(std::chrono::microseconds(400));
    }

    //std::cerr << "Thread finishing - end of UDP sending." << std::endl;
}

void Tx::Enable()
{
    m_enable = true;
}




void usage(const char *argv0)
{
    std::cout << "Usage: " << argv0 << " udp_host udp_port size iterations threads" << std::endl;
    std::cout << "       udp_host:     Hostname/IP to send data to via UDP." << std::endl;
    std::cout << "       udp_port:     Starting port number of UDP sending (increases with threads)." << std::endl;
    std::cout << "       size:         UDP Packet size." << std::endl;
    std::cout << "       iterations:   Number of iterations/atoms to process." << std::endl;
    std::cout << "       threads:      Number of parallel threads to create." << std::endl;
    std::cout << "       sleep:      Sleep after send in microseconds." << std::endl;
    
}


int main(int argc, char **argv)
{
    if (argc < 6) {
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    std::string send_host =         argv[1];
    unsigned short send_port = atoi(argv[2]);
    uint32_t size =            atoi(argv[3]);
    uint32_t iterations =      atoi(argv[4]);
    uint32_t threads =         atoi(argv[5]);
    uint32_t sleep_ =	atoi(argv[6]);

    if (threads == 0) {
        exit(EXIT_SUCCESS);
    }

    std::vector<Tx *>  tx;
    for (uint32_t i=0; i < threads; i++) {
        Tx *t = new Tx(size, iterations, send_host, send_port + static_cast<unsigned short>(i),sleep_);
        tx.push_back(t);
    }

    for (uint32_t i=0; i < threads; i++) {
        tx[i]->Enable();
    }

    for (uint32_t i=0; i < threads; i++) {
        tx[i]->Join();
    }
    
    for (uint32_t i=0; i < threads; i++) {
        if (tx[i]) {
            delete tx[i];
            tx[i] = NULL;
        }
    }

    exit(EXIT_SUCCESS);
}
