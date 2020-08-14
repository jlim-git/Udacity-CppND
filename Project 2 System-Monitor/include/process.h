#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, std::string user, std::string cmd, std::string ram, long int upTime) :
        Pid_(pid),
        User_(user),
        Command_(cmd),
        Ram_(ram),
        UpTime_(upTime) 
  {}

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // COMPLETE: Declare any necessary private members
 private:
  int Pid_;
  std::string User_;
  std::string Command_;
  float CpuUtilization_;
  std::string Ram_;
  long int UpTime_;
};

#endif