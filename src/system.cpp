#include <unistd.h>
#include <algorithm>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  // clear the vector to populate with a new batch of processes
  processes_.clear();

  // Add processes with pids read from linux parser
  for (int pid : pids) {
    processes_.push_back(Process(pid));
  }
  return processes_;
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }