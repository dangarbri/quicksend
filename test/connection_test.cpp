#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void Check(int result, const char* message)
{
  if (result < 0)
    {
      perror(message);
      exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv)
{
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  Check(sock, "Creating socket");
  struct sockaddr_in target;
  target.sin_family = AF_INET;
  target.sin_port = htons(12345);
  target.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  int result = connect(sock, (struct sockaddr*) &target, sizeof(target));
  Check(result, "Connect");
  if (result >= 0)
    {
      printf("Connection was successful.\n");
    }

  close(sock);
}
