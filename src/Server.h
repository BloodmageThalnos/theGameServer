#pragma once

#include "Client.h"
#include "Common.h"

class Server : public Singleton<Server> {
 public:
  Server() {}
  void listenNewClient();

 private:
  std::string version = "1.0";
  std::vector<Client> clients;
};