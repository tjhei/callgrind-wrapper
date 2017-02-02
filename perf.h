#ifndef _perf_h_
#define _perf_h_

/*
Usage:

valgrind --tool=callgrind -q --combine-dumps=yes --callgrind-out-file=callgrind.out

 */


#include <valgrind/callgrind.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

class Instrument
{
  public:
    Instrument(const char* name)
		    :
		    name (name),
		    stopped(false)
      {
	CALLGRIND_START_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;
      }

    void stop()
      {
	if (stopped)
	  return;
	
	stopped = true;
	int ret;
	//ret = system("rm -f log*");
	CALLGRIND_DUMP_STATS_AT(name);
	
	ret = system("cat log* | grep summary >log.summary");

	std::ifstream f("log.summary");
	std::string bla;
	long cycles = 0;
	f >> bla >> cycles;
	std::cout << "> " << name << " " << cycles << std::endl;
	
	//ret = system("rm -f log*");
      }

    ~Instrument()
      {
	stop();
      }    

  private:
    const char* name;
    bool stopped;
};

#endif
