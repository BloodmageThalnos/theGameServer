#pragma once

enum ClientState {
  kClosed = 0,
  kWaiting,
  kGaming,
};

class Client {
 public:
  Client() {}

 private:
  ClientState state;
};