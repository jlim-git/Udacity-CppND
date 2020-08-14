#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// COMPLETE: Return this process's ID
int Process::Pid() { return Pid_; }

// CHECK: Return this process's CPU utilization
float Process::CpuUtilization() {
  string line;
  const string pidStr = std::to_string(Pid_);

  vector<string> val;  // [14=utime, 15=stime= 16=cutime, 17=cstime]
  long totalTime = 0;
  val.clear();

  std::ifstream filestream(LinuxParser::kProcDirectory + pidStr + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      for (int i = 0; i < 18; i++) {
        std::istringstream linestream(line);
        string tempVal;
        linestream >> tempVal;
        val.push_back(tempVal);
      }
    }
  }

  for (int i = 14; i < 18; i++) {
    long tempTime = stol(val[i]) / sysconf(_SC_CLK_TCK);
    totalTime += tempTime;
  }

  // long divide = totalTime / LinuxParser::UpTime(Pid_);

  CpuUtilization_ = (float) (1.0 * totalTime / LinuxParser::UpTime(Pid_));

  return CpuUtilization_;
}

// COMPLETE: Return the command that generated this process
string Process::Command() { return Command_; }

// COMPLETE: Return this process's memory utilization
string Process::Ram() { return Ram_; }

// COMPLETE: Return the user (name) that generated this process
string Process::User() { return User_; }

// COMPLETE: Return the age of this process (in seconds)
long int Process::UpTime() { return UpTime_; }

// COMPLETE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return (a.CpuUtilization_ < this->CpuUtilization_); 
}