#pragma once

#include <string>

enum MessageType {
  kHello = 1,
  kOK = 2,
};

struct Message {
  MessageType type;
  void* buf;
  int len;
};

struct HelloMessage {
  HelloMessage(void* buf, int len) : name((char*)buf, (char*)buf + len) {}
  std::string name;
};