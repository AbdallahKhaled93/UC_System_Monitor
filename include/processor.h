#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
 public:
  float Utilization();

 private:
  long int prevIdle, prevActive;
};

#endif