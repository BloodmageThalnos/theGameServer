#pragma once

class NetworkManager {
 public:
  NetworkManager() {}
  ~NetworkManager() {}

  virtual void startListen(unsigned short port) = 0;
};