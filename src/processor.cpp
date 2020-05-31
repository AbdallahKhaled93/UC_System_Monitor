#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;

float Processor::Utilization() {
  long int currIdle = LinuxParser::IdleJiffies();
  long int currActive = LinuxParser::ActiveJiffies();

  /* Use formula from
   * https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
   */
  long int prevTotal = prevIdle + prevActive;
  long int currTotal = currIdle + currActive;

  long int totalDiff = currTotal - prevTotal;
  long int idleDiff = currIdle - prevIdle;

  /* update the previous times to be used in the next iteration */
  prevIdle = currIdle;
  prevActive = currActive;

  return (double)(totalDiff - idleDiff) / (double)totalDiff;
}