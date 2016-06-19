//
// Created by alexwang on 6/19/16.
//

#include "Logger.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

alex::Logger *alex::Logger::theInstance = nullptr;

void alex::Logger::resetOutput(std::string output) {
  if (needDelete) {
    delete os;
  }

  if (output == "stdout") {
    os = &(std::cout);
    needDelete = false;
    needFlush = false;
  }
  else if (output == "") {
    os = nullptr;
    needDelete = false;
    needFlush = false;
  }
  else {
    os = new std::fstream(output);
    needDelete = true;
    needFlush = true;
  }
}

static std::string getLevelStr(int level) {
  switch(level) {
  case alex::Logger::Level::Error:
    return "E";
  case alex::Logger::Level::Warning:
    return "W";
  case alex::Logger::Level::Info:
    return "I";
  case alex::Logger::Level::Verbose:
    return "V";
  default:
    return "?";
  }
}

void alex::Logger::output(int level, std::string tag, std::string str, int indent) const {
  if (level <= currentLevel && os != nullptr) {
    std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream ss;
    ss << getLevelStr(level) << " ";
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S   ");
    ss << tag << "\t";
    for (int i = 0; i < indent; ++i)
      ss << " ";
    std::string headString = ss.str();

    std::stringstream anotherSS(str);
    std::string line;

    bool isFirstLine = true;
    while(std::getline(anotherSS, line, '\n')){
      (*os) << headString;
      if (isFirstLine) {
        isFirstLine = false;
      }
      else {
        (*os) << "  ";
      }
      (*os) << line << std::endl;
    }
  }
}

void alex::Logger::i(std::string tag, std::string str, int indent) const {
  output(Level::Info, tag, str, indent);
}
void alex::Logger::w(std::string tag, std::string str, int indent) const {
  output(Level::Warning, tag, str, indent);
}
void alex::Logger::e(std::string tag, std::string str, int indent) const {
  output(Level::Error, tag, str, indent);
}



