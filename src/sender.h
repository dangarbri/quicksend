/**
 * This class handles sending a file. Its purpose is to stat the provided file
 * and follow the protocol for sending the file to the server
 */

class Sender
{
public:
  /**
   * Sends a file to a server started with quicksend -r
   */
  void Send(const char* ip_addr, int port, const char* in_file);

  ~Sender();

private:
  int _sock = -1;
  /**
   * Create a connection to ip_addr on this port and return the
   * socket descriptor
   */
  int Connect(const char* ip_addr, int port);

  void CheckResult(int result);

  void ParseIpAddress(const char* ip_addr, struct in_addr* out);

  /**
   * Sends IN_FILE over the given SOCK
   */
  void SendFile(int sock, const char* in_file);

  /**
   * Sends the file size over the given sock
   */
  void SendFileSize(int sock, off_t size);

  /**
   * Returns the size of the given file stream
   */
  off_t GetFileSize(const char* file);

  /**
   * Sends the file's name over SOCK, this is the first part of the protocol
   */
  void SendFileName(int sock, const char* in_file);

  /**
   * Check if IP Address is loopback, if so, then use special
   * INADDR_LOOPBACK instead of ip address.
   */
  bool IsLoopback(const char* ip_addr);

  struct in_addr GetAddress(const char* ip_addr_host);
};
