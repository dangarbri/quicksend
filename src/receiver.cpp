#include <config.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdint>
#include <cstring>
#include "receiver.h"
#if DEBUG
#include <iostream>
#endif

#define CHUNK_SIZE 1500

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
  CheckResult(_sock);


#if DEBUG
  std::cout << "Socket created, socket pointer is: " << _sock << std::endl;
#endif

  // Name the socket
  struct sockaddr_in name = {
    .sin_family = AF_INET,
    .sin_port = htons(12345), // TODO: Find an open port if this fails
    .sin_addr = { .s_addr = htonl(INADDR_ANY) }
  };
  int result = bind(_sock, (struct sockaddr *) &name, sizeof(name));
  CheckResult(result);

  // Listens for a single connection.
  result = listen(_sock, 1);
  CheckResult(result);

  std::cout << PACKAGE_NAME << " is listening on address TODO and port 12345" << std::endl;
  std::cout << "Send a file to it with " << std::endl;
  std::cout << PACKAGE_NAME << " TODO 12345 filename" << std::endl;

  // Now the socket is ready, wait for a connection from a client
  WaitForFile();
}

void Receiver::WaitForFile()
{
#if DEBUG
  std::cout << "Waiting for an incoming connection." << std::endl;
#endif

  struct sockaddr_in client = {};
  socklen_t size = sizeof(client);
  int connection = accept(_sock, (struct sockaddr*) &client, &size);
  CheckResult(connection);

#if DEBUG
  std::cout << "Client connected from address: " << inet_ntoa(client.sin_addr) << " on port " << ntohs(client.sin_port) << std::endl;
#endif

  ReceiveFile(connection);
}

void Receiver::ReceiveFile(int conn)
{
  char buffer[255];
  // Read the filename being received
  ssize_t bytes_read = read(conn, buffer, sizeof(buffer));
  CheckResult(bytes_read);
 
  // Acknowledge the file by echoing the file name
  ssize_t bytes_written = write(conn, buffer, bytes_read);
  CheckResult(bytes_written);

  // Store the name of the file being read
  char* in_file = new char[bytes_read];
  strncpy(in_file, buffer, bytes_read);
  // the buffer contains the new line character that we need to remove.
  // Null terminate the input file at the newline char.
  in_file[bytes_read - 1] = '\0';
  

  // The next line should be the size of the file
  bytes_read = read(conn, buffer, sizeof(buffer));
  CheckResult(bytes_read);

  // Again write this back to acknowledge it
  bytes_written = write(conn, buffer, bytes_read);
  CheckResult(bytes_written);

  // Get the size of the file as a number
  errno = 0;
  long int size = strtol(buffer, NULL, 0);
  CheckResult(errno);

  // Now we're ready to receive the file
  std::cout << "Receiving file " << in_file << " with size " << size << std::endl;

  CreateFileFromSocket(conn, in_file, size);

  delete in_file;
}

void Receiver::CreateFileFromSocket(int sock, const char* in_file, long int size)
{
  // TODO: Check if in_file already exists and confirm if the user wants to
  //       overwrite it or not. (Maybe use x option and check for error)
  errno = 0;
  FILE* new_file = fopen(in_file, "wb");
  CheckResult(errno);
  // Verify file was opened successfully.
  if (new_file != NULL)
  {
    long int bytes_left = size;
    uint8_t buffer[CHUNK_SIZE];
    // Continue reading until all bytes have been transferred
    while (bytes_left > 0)
    {
      // Set chunk size to CHUNK_SIZE or the remaining size
      int chunk_size = bytes_left > CHUNK_SIZE ? CHUNK_SIZE : bytes_left;

      // read a chunk
      ssize_t bytes_read = read(sock, buffer, chunk_size);
      if (bytes_read < 0)
      {
	// If there was a problem reading, attempt to gracefully close
	// before exiting.
	fclose(new_file);
      }
      CheckResult(bytes_read);

      // Exit if we didn't read the expected number of bytes... 
      if (bytes_read != chunk_size)
      {
	std::cerr << "Failed reading bytes over the network, is something wrong with the client?" << std::endl;
	exit(EXIT_FAILURE);
      }

      // stream the chunk that was just read to the file.
      fwrite(buffer, sizeof(uint8_t), bytes_read, new_file);

      // update the remaining bytes
      bytes_left -= bytes_read;
    }

    // Transfer complete, close the file.
    fclose(new_file);

    std::cout << "Successfully received " << in_file << std::endl;
  }
}

void Receiver::CheckResult(int result)
{
  if (result < 0)
  {
    perror("Error");
    exit(errno);
  }    
}
