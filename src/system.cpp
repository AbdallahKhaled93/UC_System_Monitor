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

  // Sort by the 10 most cpu usage processes
  std::sort(processes_.begin(), processes_.end());
  std::reverse(processes_.begin(), processes_.end());

  return processes_;
}

std::string System::Kernel() const { return LinuxParser::Kernel(); }

float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }