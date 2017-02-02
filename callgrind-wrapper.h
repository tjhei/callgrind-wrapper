#ifndef _perf_h_
#define _perf_h_

/*
Usage:

valgrind --tool=callgrind -q --combine-dumps=yes --callgrind-out-file=callgrind.out ./executable

 */

#include <valgrind/callgrind.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstdio>

#include <sys/types.h>       
#include <unistd.h>

namespace callgrind_wrapper
{
  

/**
 * reset the current instruction counter to zero and start instrumenting (if
 * not already happening).
 */
inline void reset()
{
	CALLGRIND_START_INSTRUMENTATION;
	CALLGRIND_ZERO_STATS;
}

/**
 * Return the number of instructions since reset() has been called last. This
 * might return 0 or an invalid number if the program is not executed under
 * callgrind with the recommended parameters.
 */
inline long current()
{
  CALLGRIND_DUMP_STATS;
  std::ifstream f("callgrind.out");
  std::string bla;
  
  long cycles = 0;
  while (f)
    {
      f >> bla;
      if (bla == "summary:")
	f >> cycles;	    
    }
  return cycles;
}

/**
 * A somewhat expensive call to check that we are running under callgrind,
 * that the parameters are set correctly, and that we can retrieve cycle
 * information.
 */
inline bool is_working()
{
  int running = RUNNING_ON_VALGRIND;
  if (running==0)
    return false;

  reset();
  CALLGRIND_DUMP_STATS;
  
  std::string token = "callgrind-wrapper-token";
  CALLGRIND_DUMP_STATS_AT(token.c_str());
  
  std::ifstream f("callgrind.out");
  std::string bla;
  long cycles = 0;
  
  bool found_token = false;
  pid_t mypid = getpid();
  long found_pid = 0;
  
  while (f)
    {
      f >> bla;
      if (bla == "pid:")
	f >> found_pid;
      if (bla == "desc:")
	{
	  
	  std::string line;
	  std::getline(f, line);
	  if (line == " Trigger: Client Request: " + token)
	    found_token = true; 
	}
      if (bla == "summary:")
	f >> cycles;
    }
  
  if (!found_token || found_pid != mypid)
    return false;
  
  return cycles > 0;
}

/**
 * A simple class that logs cycle information to std::cout when going out of
 * scope.
 */
class Instrument
{
  public:
    Instrument(const char* name)
		    :
		    name (name),
		    stopped(false)
      {
	reset();
      }

    void stop()
      {
	long cycles = current();
	stopped = true;
	
	std::cout << "> " << name << " " << cycles << std::endl;
      }

    ~Instrument()
      {
	if (!stopped)
	  stop();
      }    

  private:
    const char* name;
    bool stopped;
};
}

#endif
