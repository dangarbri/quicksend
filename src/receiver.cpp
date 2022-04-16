#include "receiver.h"
#include <sys/socket.h>
#include <cerrno>
#include <cstdio>
#include <cunistd>
#if DEBUG
#include <iostream>
#endif

Receiver::Receiver()
{
}

Receiver::~Receiver()
{
  if (_sock != -1)
  {
    close(_sock);
  }
}

void Receiver::Listen(int port)
{
#if DEBUG
  std::cout << "Attempting to open socket..." << std::endl;
#endif
  
  _sock = socket(PF_INET, SOCK_STREAM, 0);
  VerifySocket(_sock);
  
#if DEBUG
  std::cout << "Socket created, socket pointer is: " << _sock << std::endl;
#endif
}

void Receiver::VerifySocket(int sock)
{
  if (sock == -1)
  {
    perror("Failed to open socket");
    exit(errno);
  }    
}
