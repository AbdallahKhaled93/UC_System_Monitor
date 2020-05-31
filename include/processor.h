#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  std::vector<long int> cpuPrevTimes{0,0,0,0,0,0,0,0,0,0};
};

#endif