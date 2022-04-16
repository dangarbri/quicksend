#include "receiver.h"
#if DEBUG
#include <iostream>
#endif

int main(int argc, char** argv)
{
#if DEBUG
  std::cout << "Goodbye." << std::endl;
#endif
}
