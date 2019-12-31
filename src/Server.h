#pragma once

#include "Client.h"
#include "Common.h"

class Server : public Singleton<Server> {
 public:
  Server() {}
  ~Server() {}

 private:
  std::string version = "1.0";
};