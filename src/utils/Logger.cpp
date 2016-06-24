//
// Created by alexwang on 6/19/16.
//

#include "Logger.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <sys/time.h>
#include <cstdarg>
#include <memory>

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
    os = new std::fstream(output, std::ios::trunc | std::ios::out);
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

void alex::Logger::output(int level, std::string tag, std::string str, int indent) {
  if (level <= currentLevel && os != nullptr) {
    std::stringstream ss;
    ss << getLevelStr(level) << " ";

    // format time
    char fmt[64];
    char timeBuf[64];
    struct timeval tv;
    struct tm *tm;
    gettimeofday (&tv, NULL);
    tm = localtime (&tv.tv_sec);
    strftime(fmt, sizeof (fmt), "%Y-%m-%d %H:%M:%S.%%06u  ", tm);
    snprintf(timeBuf, sizeof(timeBuf), fmt, tv.tv_usec);
    ss << timeBuf;

    // format tag
    char tagBuf[16];
    snprintf(tagBuf, sizeof(tagBuf), "%-15s", tag.c_str());
    ss << tagBuf << "     ";

    // print indent
    for (int i = 0; i < indent; ++i)
      ss << " ";
    std::string headString = ss.str();

    // print other lines
    std::stringstream anotherSS(str);
    std::string line;
    bool isFirstLine = true;

    mu.lock();
    {
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
      os->flush();
    }
    mu.unlock();
  }
}
void alex::Logger::v(std::string tag, std::string str, int indent, ...) {
#ifdef ALEX_DEBUG
  va_list args;
  va_start(args, indent);

  int bufSize = vsnprintf(NULL, 0, str.c_str(), args);
  std::shared_ptr<char> buf(new char[bufSize + 1]);
  snprintf(buf.get(), (size_t)bufSize + 1, str.c_str(), args);

  va_end(args);

  output(Level::Verbose, tag, buf.get(), indent);
#endif
}


void alex::Logger::i(std::string tag, std::string str, int indent, ...) {
#ifdef ALEX_DEBUG
  va_list args;
  va_start(args, indent);

  int bufSize = vsnprintf(NULL, 0, str.c_str(), args);
  std::shared_ptr<char> buf(new char[bufSize + 1]);
  snprintf(buf.get(), (size_t)bufSize + 1, str.c_str(), args);

  va_end(args);

  output(Level::Info, tag, buf.get(), indent);
#endif
}

void alex::Logger::w(std::string tag, std::string str, int indent, ...) {
#ifdef ALEX_DEBUG
  va_list args;
  va_start(args, indent);

  int bufSize = vsnprintf(NULL, 0, str.c_str(), args);
  std::shared_ptr<char> buf(new char[bufSize + 1]);
  snprintf(buf.get(), (size_t)bufSize + 1, str.c_str(), args);

  va_end(args);

  output(Level::Warning, tag, buf.get(), indent);
#endif
}
void alex::Logger::e(std::string tag, std::string str, int indent, ...) {
#ifdef ALEX_DEBUG
  va_list args;
  va_start(args, indent);

  int bufSize = vsnprintf(NULL, 0, str.c_str(), args);
  std::shared_ptr<char> buf(new char[bufSize + 1]);
  snprintf(buf.get(), (size_t)bufSize + 1, str.c_str(), args);

  va_end(args);

  output(Level::Error, tag, buf.get(), indent);
#endif
}


std::string alex::vec3ToStr(const cv::Vec3d v) {
  std::stringstream ss;
  ss << v;
  return ss.str();
}

