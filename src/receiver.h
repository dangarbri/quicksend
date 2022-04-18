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
