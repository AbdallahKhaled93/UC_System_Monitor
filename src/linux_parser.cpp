#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iomanip>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line, memEntryKey, memTotalValue, memFreeValue;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> memEntryKey >> memTotalValue;
    std::getline(stream, line);
    lineStream.str(line);
    lineStream >> memEntryKey >> memFreeValue;
  }

  return (std::stod(memTotalValue) - std::stod(memFreeValue)) /
         std::stod(memTotalValue);
}

long LinuxParser::UpTime() {
  string line;
  long int uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> uptime;
  }

  return uptime;
}

long LinuxParser::Jiffies() {
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[0]) + std::stol(cpuTimes[1]) +
          std::stol(cpuTimes[2]) + std::stol(cpuTimes[3]) +
          std::stol(cpuTimes[4]) + std::stol(cpuTimes[5]) +
          std::stol(cpuTimes[6]) + std::stol(cpuTimes[7]));
}

long LinuxParser::ActiveJiffies(int pid) {
  long int userTime, systemTime, childUserTime, childSystemTime;
  string line, skip;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);

    // skip reading untill the 14th element
    for (int i = 0; i < 13; i++) {
      lineStream >> skip;
    }

    // read from 14th to the 17th element
    lineStream >> userTime >> systemTime >> childUserTime >> childSystemTime;
  }

  return userTime + systemTime + childUserTime + childSystemTime;
}

long LinuxParser::ActiveJiffies() {
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[0]) + std::stol(cpuTimes[1]) +
          std::stol(cpuTimes[2]) + std::stol(cpuTimes[5]) +
          std::stol(cpuTimes[6]) + std::stol(cpuTimes[7]));
}

long LinuxParser::IdleJiffies() {
  vector<string> cpuTimes = CpuUtilization();

  return (std::stol(cpuTimes[3]) + std::stol(cpuTimes[4]));
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuTimes(10);
  string line, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);
    lineStream >> cpu >> cpuTimes[0] >> cpuTimes[1] >> cpuTimes[2] >>
        cpuTimes[3] >> cpuTimes[4] >> cpuTimes[5] >> cpuTimes[6] >>
        cpuTimes[7] >> cpuTimes[8] >> cpuTimes[9];
  }

  return cpuTimes;
}

int LinuxParser::TotalProcesses() {
  string line, processes;
  int totalProcesses = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (1) {
      std::getline(stream, line);
      std::istringstream lineStream(line);
      lineStream >> processes;
      if ("processes" == processes) {
        lineStream >> totalProcesses;
        break;
      }
    }
  }

  return totalProcesses;
}

int LinuxParser::RunningProcesses() {
  string line, processes;
  int runningProcesses = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (1) {
      std::getline(stream, line);
      std::istringstream lineStream(line);
      lineStream >> processes;
      if ("procs_running" == processes) {
        lineStream >> runningProcesses;
        break;
      }
    }
  }

  return runningProcesses;
}

string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if(stream.is_open())
  {
    std::getline(stream, command);
  }

  return command;
}

string LinuxParser::Ram(int pid) {

  string line, vmSize;
  int ramConsupmtion = 0;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream lineStream(line);
      lineStream >> vmSize;
      if ("VmSize:" == vmSize) {
        lineStream >> ramConsupmtion;
        break;
      }
    }
  }

  // set 2 digits after floating point to have a proper display
  std::ostringstream out;
  out << std::setprecision(2) << ramConsupmtion / 1000.0f;
  return (out.str());
}

string LinuxParser::Uid(int pid) {

  string line, skip, UID;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream lineStream(line);
      lineStream >> skip;
      if ("Uid:" == skip) {
        lineStream >> UID;
        break;
      }
    }
  }

  return UID;
}

string LinuxParser::User(int pid) {

  string line, UID, user, stringStreamOutput;
  std::ifstream stream(kPasswordPath);

  if(stream.is_open()){
    UID = Uid(pid);
    while (std::getline(stream, line)) {
      std::istringstream lineStream(line);
      // read first field, the user
      std::getline(lineStream, user, ':');
      // skip password field
      std::getline(lineStream, stringStreamOutput, ':');
      // read UID and compare it to the UID of the process
      std::getline(lineStream, stringStreamOutput, ':');
      if(stringStreamOutput == UID)
        return user;

    }
  }

  return user;
}

long LinuxParser::UpTime(int pid) {
  long int overallUptime = UpTime();
  long int processStartTime;
  string line, skip;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream lineStream(line);

    // skip reading untill the 22nd element
    for (int i = 0; i < 21; i++) {
      lineStream >> skip;
    }

    // read the 22nd element
    lineStream >> processStartTime;
  }

  return overallUptime - (processStartTime / sysconf(_SC_CLK_TCK));
}