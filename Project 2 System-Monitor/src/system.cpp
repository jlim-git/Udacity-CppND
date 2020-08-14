#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// COMPLETE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// COMPLETE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  int pid = 0;
  string user;
  string cmd;
  string ram;
  long int upTime = 0;

  processes_.clear();

  for (auto i : LinuxParser::Pids()) {
      pid = i;
      user = LinuxParser::User(pid);
      cmd = LinuxParser::Command(pid);
      ram = LinuxParser::Ram(pid);
      upTime = LinuxParser::UpTime(pid);
      
      Process newProc(pid, user, cmd, ram, upTime);
      newProc.CpuUtilization();
      processes_.push_back(newProc);
  }

  std::sort(processes_.begin(), processes_.end());

  return processes_;
}

// COMPLETE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// COMPLETE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// COMPLETE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// COMPLETE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// COMPLETE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// COMPLETE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }