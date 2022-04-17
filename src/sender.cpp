#include <config.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include "sender.h"

#if DEBUG
// For debugging hostname lookups
void dump_host(struct hostent*);
void print_address(char* addr_data, int length);
#endif

// TODO: Make this common with receiver.cpp
#define CHUNK_SIZE 1500

void Sender::Send(const char* ip_addr, int port, const char* in_file)
{
  int conn = Connect(ip_addr, port);
  SendFile(conn, in_file);
}

int Sender::Connect(const char* ip_addr, int port)
{
  // Create the socket
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  CheckResult(sock);

  // Convert ip_addr string into an IP address to use with sockets
  // TODO: Add hostname support
  struct in_addr addr;
  
  // Create the connection to the target
  struct sockaddr_in target;
  target.sin_family = AF_INET;
  target.sin_addr = GetAddress(ip_addr);
  target.sin_port = htons(port);

#if DEBUG
  std::cout << "Attempting connection to " << ip_addr << ":" << port << std::endl;
#endif

  int result = connect(sock, (struct sockaddr*) &target, sizeof(target));
#if DEBUG
  std::cout << "Connection complete." << std::endl;
#endif
  
  CheckResult(result);

#if DEBUG
  std::cout << "Connected to " << ip_addr << ":" << port << std::endl;
#endif

  return sock;
}

void Sender::CheckResult(int result)
{
  if (result < 0)
  {
    perror("Error");
    exit(errno);
  }    
}

void Sender::SendFile(int sock, const char* in_file)
{
  FILE* file = fopen(in_file, "rb");
  off_t size = GetFileSize(in_file);
  off_t bytes_left = size;

  // First send the name of the file that's being sent
  SendFileName(sock, in_file);
  SendFileSize(sock, size);

  // Send data in chunks until it's all been sent.
  char* buffer = new char[CHUNK_SIZE];
  while (bytes_left > 0)
  {
    // Get the amount to send, use CHUNK_SIZE, if bytes_left is less than
    // chunk size, then just send the remaining bytes
    int chunk_size = bytes_left > CHUNK_SIZE ? CHUNK_SIZE : bytes_left;

    // Read the data from the file
    int bytes_read = fread(buffer, sizeof(char), chunk_size, file);
    if (bytes_read < chunk_size)
    {
      std::cerr << "Failed to read from file." << std::endl;
      fclose(file);
      exit(EXIT_FAILURE);
    }

    // Send the data over the wire
    int bytes_sent = write(sock, buffer, chunk_size);
    CheckResult(bytes_sent);

    bytes_left -= chunk_size;
  }

  delete buffer;
}

off_t Sender::GetFileSize(const char* file)
{
  struct stat file_stats = {};
  int result = stat(file, &file_stats);
  CheckResult(result);
  // I don't think this supports symbolic links since the libc manual
  // states "For symbolic links this specifies the length of the
  // file name the link refers to" so...
  // TODO: Add support for symbolic links.
  return file_stats.st_size;
}

void Sender::SendFileName(int sock, const char* in_file)
{
  int bytes_sent = write(sock, in_file, strlen(in_file));
  CheckResult(bytes_sent);

  // Verify the server received the file name
  char buffer[255];
  int bytes_read = read(sock, buffer, sizeof(buffer));
  CheckResult(bytes_read);

  int result = strncmp(in_file, buffer, strlen(in_file));
  if (result != 0)
  {
    std::cerr << "Failed to communicate filename to the target server." << std::endl;
    exit(EXIT_FAILURE);
  }
}

Sender::~Sender()
{
  if (_sock != -1)
  {
#if DEBUG
    std::cout << "Closing client socket " << _sock << std::endl;
#endif
    
    close(_sock);
  }
}

void Sender::SendFileSize(int sock, off_t size)
{
  // Convert size to a string to send over the socket
  char buffer[255];
  snprintf(buffer, sizeof(buffer), "%ul", size);
  // Limit the buffer, just in case.
  buffer[254] = '\0';

  // Sending the size of the file.
  int bytes_sent = write(sock, buffer, strlen(buffer));
  CheckResult(bytes_sent);

  // Read back the size of the file to confirm it's acknowledged
  int bytes_read = read(sock, buffer, sizeof(buffer));
  CheckResult(bytes_read);
  if (bytes_sent != bytes_read)
  {
    std::cerr << "Unexpected bytes read when transferring file size." << std::endl;
    exit(EXIT_FAILURE);
  }

  // Verify size is the expected value
  long int read_back = strtol(buffer, NULL, 0);
  if (read_back != size)
  {
    std::cerr << "Got unexpected response from the server. Read " << read_back << " expected " << size << std::endl;
    exit(EXIT_FAILURE);
  }
}

bool Sender::IsLoopback(const char* ip_addr)
{
  const char* loopback = "127.0.0.1";
  int result = strncmp(ip_addr, loopback, strlen(loopback));
  return result == 0;
}

struct in_addr Sender::GetAddress(const char* ip_addr_host)
{
  struct in_addr addr;
  if (IsLoopback(ip_addr_host))
  {
    // Not sure if INADDR_LOOPBACK is the same as inet_atol("127.0.0.1")
    // So just use INADDR_LOOPBACK to be safe.
    addr.s_addr = htonl(INADDR_LOOPBACK);
  }
  else
  {
    struct hostent* host = gethostbyname(ip_addr_host);
#if DEBUG
    dump_host(host);
#endif
    // h_addr is already in network byte order
    addr.s_addr = *((uint32_t*) host->h_addr);
  }

  return addr;
}


#if DEBUG
// Note, these functions are for debugging hostname lookups.
// The original program wasn't properly translating localhost
// to 127.0.0.1
void print_address(char* data, int length)
{
  uint32_t* pdata = (uint32_t*) data;
  
  struct in_addr addr;
  addr.s_addr = *pdata;
  
  char* ip_str = inet_ntoa(addr);
  printf("   - %s\n", ip_str);
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
#endif
