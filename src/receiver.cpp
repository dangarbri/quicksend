#include <config.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include "receiver.h"
#if DEBUG
#include <iostream>
#endif

Receiver::Receiver()
{
#if DEBUG
  std::cout << "Constructing receiver." << std::endl;
#endif
}

Receiver::~Receiver()
{
#if DEBUG
  std::cout << "Shutting down the receiver. _sock = " << _sock << std::endl;
#endif

  if (_sock != -1)
  {
#if DEBUG
    std::cout << "Closing socket " << _sock << std::endl;
#endif
    close(_sock);
  }
}

void Receiver::Listen()
{
#if DEBUG
  std::cout << "Attempting to open socket..." << std::endl;
#endif

  // Create the socket and verify that it was opened successfully.
  _sock = socket(PF_INET, SOCK_STREAM, 0);
  VerifySocket(_sock);

  // Name the socket
  struct sockaddr_in name = {
    .sin_family = AF_INET,
    .sin_port = htons(12345), // TODO: Find an open port if this fails
    .sin_addr = { .s_addr = htonl(INADDR_ANY) }
  };
  int result = bind(_sock, (struct sockaddr *) &name, sizeof(name));
  VerifyBind(result);

  std::cout << PACKAGE_NAME << " is listening on address TODO and port 12345" << std::endl;
  std::cout << "Send a file to it with " << std::endl;
  std::cout << PACKAGE_NAME << " TODO 12345 filename" << std::endl;
  
#if DEBUG
  std::cout << "Socket created, socket pointer is: " << _sock << std::endl;
#endif
}

void Receiver::VerifySocket(int sock)
{
  if (sock < 0)
  {
    perror("Failed to open socket");
    exit(errno);
  }    
}

void Receiver::VerifyBind(int result)
{
  if (result < 0)
  {
    perror("Failed to bind port to socket");
    exit(result);
  }
}
