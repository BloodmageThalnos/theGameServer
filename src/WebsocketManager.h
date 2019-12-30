#pragma once

#include "NetworkManager.h"

class WebsocketManager : public NetworkManager {
 public:
  WebsocketManager() {}
  ~WebsocketManager() {}

  void listen(){}
};