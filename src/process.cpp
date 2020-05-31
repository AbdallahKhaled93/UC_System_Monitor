#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid; }

float Process::CpuUtilization() {
  float processActiveTime = (float)(LinuxParser::ActiveJiffies(this->pid)) /
                            (float)sysconf(_SC_CLK_TCK);
  long int processUptime = this->UpTime();

  cpuUtilization = 100.0f * (processActiveTime / processUptime);

  return cpuUtilization;
}

string Process::Command() { return LinuxParser::Command(this->pid); }

string Process::Ram() { return LinuxParser::Ram(this->pid); }

string Process::User() { return LinuxParser::User(this->pid); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid); }

bool Process::operator<(Process const& a) const {
  return this->cpuUtilization < a.cpuUtilization ? true : false;
}