#include <config.h> // autoconf output
#include <argp.h>   // for argparsing
#include <cstdlib>  // for exit
#include <cstring>  // for strerror
#include <iostream>
#include "receiver.h"
#include "sender.h"

// These macros are defined in config.h via autoconf with configure.ac
const char* argp_program_version = PACKAGE_VERSION;
const char* argp_program_bug_address = PACKAGE_BUGREPORT;

// Forward declaration for the function that handles the options
error_t argparse(int key, char* arg, struct argp_state *state);

// This defines the program's options. See the argp manual for details.
const struct argp_option options[] = {
  {
    .name = "receive",                          // for --receive
    .key = 'r',                                 // for -r
    .flags = OPTION_ARG_OPTIONAL,               // Specify this is an optional arg
    .doc = "Receive a file."                    // Help string
  },
  {0} // Required to terminate the option list
};
const struct argp argument_parser = {
  // Options
  .options = options,
  .parser = argparse,
  .args_doc = "ipaddress port filename\n--receive",
  .doc = "Utility to simply send and receive files"                         \
  "\v"                                                                      \
  "There are two ways to use this program. As a receiver or sender."        \
  "In order to send a file, you must run the program by simply calling\n\n" \
  PACKAGE_NAME " --receive\n\n"                                             \
  "This will the info needed to retrieve a file, use this information to"   \
  "then execute\n\n"                                                        \
  PACKAGE_NAME " ipaddress port file_to_send"
};

// Main function for receiving a file
void receive_file();
void send_file(const char* ip_addr, const char* port, const char* in_file);

/**
 * Anonymous struct used for storing results from argument parsing.
 */
struct {
  // Determines if the program is running as a sender or receiver
  bool is_receiver = false;
  const char* ip_addr = "";
  const char* port = "";
  const char* filename = "";
} program_flags;

/**
 * Handles parsing the args given by argp. More specifically, this toggles
 * the send/receive flag.
 */
error_t argparse(int key, char* arg, struct argp_state *state)
{
  switch (key)
  {
  case 'r':
    program_flags.is_receiver = true;
    break;

  case ARGP_KEY_ARG:
    switch (state->arg_num) {
    case 0:
      program_flags.ip_addr = arg;
      break;
    case 1:
      program_flags.port = arg;
      break;
    case 2:
      program_flags.filename = arg;
      break;
    }
    break;

  case ARGP_KEY_END:
    if (!program_flags.is_receiver && state->arg_num < 3)
    {
      argp_usage(state);
    }
    break;
    
  default:
    return ARGP_ERR_UNKNOWN;
  }

  return 0;
}

int main(int argc, char** argv)
{
#if DEBUG
  std::cout << "Running program " << PACKAGE_NAME << std::endl;
#endif

  // Parse command line arguments and return an error if anything goes
  // wrong.
  error_t result = argp_parse(&argument_parser, argc, argv, 0, NULL, NULL);
  if (result != 0)
  {
    const char* error = strerror(result);
    std::cerr << error << std::endl;
    exit(result);
  }

#if DEBUG
  if (program_flags.is_receiver)
  {
    std::cout << "Running as receiver" << std::endl;
  }
  else
  {
    std::cout << "Running as sender" << std::endl;
  }
#endif

  // Proceed with sending/receiving files
  if (program_flags.is_receiver)
  {
    receive_file();
  }
  else
  {
    send_file(program_flags.ip_addr, program_flags.port, program_flags.filename);
  }
  
#if DEBUG
  std::cout << "Goodbye." << std::endl;
#endif
}

/**
 * Receives a file using a receiver.
 */
void receive_file()
{
  Receiver receiver;
  receiver.Listen();
}

void send_file(const char* ip_addr, const char* port, const char* file)
{
  Sender sender;
  long int int_port = strtol(port, NULL, 0);
  if (int_port > UINT16_MAX)
  {
    std::cerr << "Invalid port." << std::endl;
    exit(EXIT_FAILURE);
  }

#if DEBUG
  std::cout << "Sending file to " << ip_addr << ":" << port << std::endl;
#endif
  
  sender.Send(ip_addr, (int) int_port, file);
}
