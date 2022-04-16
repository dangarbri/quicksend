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
   * Verifies the socket was created successfully. If not, then prints
   * the error and ends the program
   */
  void VerifySocket(int sock);

  /**
   * Verifies that a port is successfully bound to a socket. If not, then
   * this will report the error and end the program
   */
  void VerifyBind(int result);
};
