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

int Process::Pid() const { return pid; }

float Process::CpuUtilization() const {

  long int processActiveTime = (LinuxParser::ActiveJiffies(this->pid)) /sysconf(_SC_CLK_TCK);
  long int processUptime = this->UpTime();

  return (float)processActiveTime / (float)processUptime;
}

string Process::Command() const { return LinuxParser::Command(this->pid); }

string Process::Ram() const { return LinuxParser::Ram(this->pid); }

string Process::User() const { return LinuxParser::User(this->pid); }

long int Process::UpTime() const { return LinuxParser::UpTime(this->pid); }

bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization() ? true : false;
}