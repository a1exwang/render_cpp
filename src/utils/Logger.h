//
// Created by alexwang on 6/19/16.
//

#ifndef RENDER_LOGGER_H
#define RENDER_LOGGER_H

#include <string>
#include <iostream>
#include <cxcore.h>

namespace alex {

#define Log (alex::Logger::get())

class Logger {
public:
  enum Level {
    Verbose = 20,
    Info = 15,
    Warning = 10,
    Error = 5
  };
  static Logger& get() {
    if (theInstance) {
      return *theInstance;
    }
    else {
      throw "logger uninitialized!";
    }
  }
  static void setup(std::string output = "stdout", int verboseLevel = Level::Info) {
    theInstance = new Logger;
    theInstance->resetOutput(output);
    theInstance->resetVerboseLevel(verboseLevel);
  }

  void i(std::string tag, std::string str, int indent = 0, ...) const;
  void w(std::string tag, std::string str, int indent = 0) const;
  void e(std::string tag, std::string str, int indent = 0) const;

private:
  Logger(std::ostream *os = nullptr) :os(os), needFlush(false), needDelete(false) { }

  void resetOutput(std::string output = "stdout");
  void resetVerboseLevel(int l) { this->currentLevel = l; }
  void output(int level, std::string tag, std::string str, int indent = 0) const;
  bool needFlush;
  bool needDelete;
  std::ostream *os;
  int currentLevel;

  static Logger *theInstance;
};

std::string vec3ToStr(const cv::Vec3d v);

}

#endif //RENDER_LOGGER_H
