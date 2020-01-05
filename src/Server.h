#pragma once

#include <unordered_map>
#include "Client.h"
#include "Common.h"
#include "Message.h"
#include "NetworkManager.h"

class Server : public Singleton<Server> {
 public:
  Server(NetworkManager* _nm) : networkManager(_nm) {}
  ~Server() {}
  void init();
  void run();
  int newClient();
  void processMessage(int cid, Message msg);

 private:
  std::string version = "1.0";
  NetworkManager* networkManager;
  std::unordered_map<int, Client> clients;
  int generateClientId();
};