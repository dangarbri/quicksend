/**
 * This file defines the class for a file receiver. A simple server
 * that opens a socket and listens for a connection. When the
 * connection is made this will read the bytes off the wire into a
 * file.
 */

class Receiver
{
public:
  Receiver();
  ~Receiver();
  /**
   * Begin listening on the specified port
   */
  void Listen();
private:
  /**
   * The socket descriptor
   */
  int _sock = -1;

  /**
   * Closes the open socket.
   */
  void cleanup();

  /**
   * Verifies that a call was successful. If result is < 0, then prints
   * the error via perror and exits the program
   */
  void CheckResult(int result);
  
  /**
   * Waits for a socket connection after binding the address to the socket.
   */
  void WaitForFile();

  /**
   * Handles receiving a file from the client connection on socket SOCK
   */
  void ReceiveFile(int sock);

  /**
   * Reads SIZE bytes from SOCK and stores them in FILE
   */
  void CreateFileFromSocket(int sock, const char* file, long int size);
};
