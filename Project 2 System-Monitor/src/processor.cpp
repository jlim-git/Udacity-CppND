#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::stof;

// COMPLETE: Return the aggregate CPU utilization
float Processor::Utilization() { 
  float totalJiffies = LinuxParser::Jiffies();
  float activeJiffies = LinuxParser::ActiveJiffies();

  float cpuUsage = activeJiffies / totalJiffies;
  
  return cpuUsage; 
  }