#pragma once

class NetworkManager {
 public:
  NetworkManager() {}
  ~NetworkManager() {}

  virtual void listen();
};