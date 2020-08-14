#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::stol;
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

// COMPLETE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memType, memVal, memKB;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  float MemTotal = 0, MemFree = 0, MemUsed = 0, MemUsage = 0;
  string::size_type sz;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> memType >> memVal >> memKB;

      if (memType == "MemTotal:") {
        MemTotal = stof(memVal, &sz);
        line.clear();
      }

      else if (memType == "MemFree:") {
        MemFree = stof(memVal, &sz);
      }
    }
  }

  MemUsed = MemTotal - MemFree;
  MemUsage = MemUsed / MemTotal;
  return MemUsage;
}

// COMPLETE: Read and return the system uptime
long LinuxParser::UpTime() {
  string upTimeStr, idleTimeStr;
  string::size_type sz;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTimeStr >> idleTimeStr;
  }

  long upTime = stol(upTimeStr, &sz);

  return upTime;
}

// CHECK: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpuUsage =
      CpuUtilization();  // {0=user, 1=nice, 2=system, 3=idle, 4=iowait,
                         // 5=irq, 6=softirq, 7=steal, 8=guest, 9=guest_nice}

  long totalJiffies = 0;

  for (auto i : cpuUsage) {
    long jiffy = stol(i);
    totalJiffies += jiffy;
  }

  return totalJiffies;
}

// CHECK: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line;
  const string pidStr = std::to_string(pid);
  vector<string> val;

  val.clear();

  std::ifstream filestream(kProcDirectory + pidStr + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      for (int i = 0; i < 22; i++) {
        std::istringstream linestream(line);
        string tempVal;
        linestream >> tempVal;
        val.push_back(tempVal);
      }
    }
  }

  long pidActiveJiffies = stol(val[21]);
  return pidActiveJiffies;
}

// CHECK: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpuUsage =
      CpuUtilization();  // {0=user, 1=nice, 2=system, 3=idle, 4=iowait,
                         // 5=irq, 6=softirq, 7=steal, 8=guest, 9=guest_nice}

  std::ifstream filestream(kProcDirectory + kStatFilename);

  long activeJiffies = Jiffies() - IdleJiffies();
  return activeJiffies;
}

// CHECK: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpuUsage =
      CpuUtilization();  // {0=user, 1=nice, 2=system, 3=idle, 4=iowait,
                         // 5=irq, 6=softirq, 7=steal, 8=guest, 9=guest_nice}

  long idleJiffies = stol(cpuUsage[3]) + stol(cpuUsage[4]);
  return idleJiffies;
}

// COMPLETE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key;
  vector<string> cpuUsage;  // {0=user, 1=nice, 2=system, 3=idle, 4=iowait,
                            // 5=irq, 6=softirq, 7=steal, 8=guest, 9=guest_nice}

  std::ifstream filestream(kProcDirectory + kStatFilename);

  cpuUsage.clear();

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key) {
        if (key == "cpu") {
          for (int i = 0; i < 10; i++) {
            string tempStr;
            linestream >> tempStr;
            cpuUsage.push_back(tempStr);
          }
        }
      }
    }
  }

  return cpuUsage;
}

// COMPLETE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  int intValue;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "processes") {
          linestream >> value;
          intValue = std::stoi(value);
          return intValue;
        }
      }
    }
  }

  return 0;
}

// COMPLETE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  int intValue;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "procs_running") {
          linestream >> value;
          intValue = std::stoi(value);
          return intValue;
        }
      }
    }
  }

  return 0;
}

// COMPLETE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string key, value;
  string cmdLine;

  const string pidStr = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + pidStr + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      return line;
    }
  }

  return string();
}

// COMPLETE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  string vmSize;
  int vmSizeMB;

  const string pidStr = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + pidStr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;

      if (key == "VmSize:") {
        linestream >> vmSize;
        vmSizeMB = stoi(vmSize) / 1000;
        vmSize = std::to_string(vmSizeMB);
        return vmSize;
      }
    }
  }

  return string();
}

// COMPLETE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key, uid;

  const string pidStr = std::to_string(pid);

  std::ifstream filestream(kProcDirectory + pidStr + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;

      if (key == "Uid:") {
        return uid;
      }
    }
  }

  return "uidError";
}

// COMPLETE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;

  string uid = LinuxParser::Uid(pid);

  if (uid != "uidError") {
    string searchUID = ":x:" + uid;
    string username;

    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        auto pos = line.find(searchUID);
        username = line.substr(0, pos);
        return username;
      }
    }
  }

  return "userError";
}

// COMPLETE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  long startTime =
      LinuxParser::UpTime() - (ActiveJiffies(pid) / sysconf(_SC_CLK_TCK));

  return startTime;
}