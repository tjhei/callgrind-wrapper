#include "callgrind-wrapper.h"
#include <iostream>

int main()
{
  if (!callgrind_wrapper::is_working())
    {
      std::cout << "callgrind not detected. Please run as\n"
                << "    valgrind --tool=callgrind -q --combine-dumps=yes --callgrind-out-file=callgrind.out ./a.out"<< std::endl;
      return -1;
    }
  std::cout << "running correctly under callgrind..." << std::endl;

  {
    callgrind_wrapper::Instrument a("nothing");
  }
  {
    callgrind_wrapper::Instrument a("something");
    int t = 0;
    for (unsigned int i=0; i<1000; ++i)
      for (unsigned int j=0; j<1000; ++j)
        t =+ (i+j)%10;
    std::cout << "computing 8 = " <<  t << std::endl;
  }


}
