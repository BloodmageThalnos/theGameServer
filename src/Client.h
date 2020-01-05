#pragma once

#include <string>

enum ClientState {
  kClosed = 0,
  kConnecting,
  kWaiting,
  kGaming,
};

class Client {
 public:
  Client() {}
  ClientState state;
  std::string address;
  int id;
  std::string name;
};