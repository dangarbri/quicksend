/**
 * Quicksend - The extremely simple file transfer tool
 * Copyright (C) 2022 Daniel Garcia-Briseno
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
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
