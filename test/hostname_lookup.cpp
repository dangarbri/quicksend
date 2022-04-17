#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_host(const char*);
void dump_host(struct hostent*);
void print_address(char* addr_data, int length);

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      printf("Invalid number of args.\n");
      exit(EXIT_FAILURE);
    }
  
  printf("Looking up host: %s\n", argv[1]);

  print_host(argv[1]);
}

void print_host(const char* hostname)
{
  struct hostent* host = gethostbyname(hostname);
  dump_host(host);
}

void dump_host(struct hostent* host)
{
  if (host == NULL)
    {
      puts("No host found.");
      exit(EXIT_FAILURE);
    }

  printf("h_name: %s\n", host->h_name);
  printf("aliases: \n");
  for (int idx = 0; host->h_aliases[idx] != NULL; idx++)
    {
      printf("  %d. %s\n", idx, host->h_aliases[idx]);
    }
    
  printf("Address Type: ");
  if (host->h_addrtype == AF_INET)
    {
      printf("AF_INET\n");
    }
  if (host->h_addrtype == AF_INET6)
    {
      printf("AF_INET6\n");
    }
  printf("Address Length: %d\n", host->h_length);
  printf("Default Address: %s\n", host->h_addr);
  printf("Addresses: \n");
  for (int idx = 0; host->h_addr_list[idx] != NULL; idx++)
    {
      print_address(host->h_addr_list[idx], host->h_length);
    }
}

void print_address(char* data, int length)
{
  uint32_t* pdata = (uint32_t*) data;
  
  struct in_addr addr;
  addr.s_addr = *pdata;
  
  char* ip_str = inet_ntoa(addr);
  printf("   - %s\n", ip_str);
}
