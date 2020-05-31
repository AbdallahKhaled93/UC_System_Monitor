#include <vector>
#include <string>
#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;

float Processor::Utilization() {

    vector<string> cpuCurrTimesString = LinuxParser::CpuUtilization();
    vector<long int> cpuCurrTimes{std::stol(cpuCurrTimesString[0]),
                                  std::stol(cpuCurrTimesString[1]),
                                  std::stol(cpuCurrTimesString[2]),
                                  std::stol(cpuCurrTimesString[3]),
                                  std::stol(cpuCurrTimesString[4]),
                                  std::stol(cpuCurrTimesString[5]),
                                  std::stol(cpuCurrTimesString[6]),
                                  std::stol(cpuCurrTimesString[7]),
                                  std::stol(cpuCurrTimesString[8]),
                                  std::stol(cpuCurrTimesString[9]),
                                 };


    /* Use formula from https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux */
    /* This formula is used in htop to have a more accurate cpu usage calculation */
    long int prevIdle = cpuPrevTimes[3] + cpuPrevTimes[4];
    long int currIdle = cpuCurrTimes[3] + cpuCurrTimes[4];
    long int prevNonIdle = cpuPrevTimes[0] + cpuPrevTimes[1] + cpuPrevTimes[2] +
        cpuPrevTimes[5] + cpuPrevTimes[6] + cpuPrevTimes[7];
    long int currNonIdle = cpuCurrTimes[0] + cpuCurrTimes[1] + cpuCurrTimes[2] +
        cpuCurrTimes[5] + cpuCurrTimes[6] + cpuCurrTimes[7];

    long int prevTotal = prevIdle + prevNonIdle;
    long int currTotal = currIdle + currNonIdle;

    long int totalDiff  = currTotal - prevTotal;
    long int idleDiff = currIdle - prevIdle;

    /* update the previous times to be used in the next iteration */
    cpuPrevTimes = cpuCurrTimes;

    return (double)(totalDiff - idleDiff)/(double)totalDiff;
}