#pragma once

enum MessageType {
  kHello = 1,
  kOK = 2,
};

struct Message {
  MessageType type;
  char buf[];
};
