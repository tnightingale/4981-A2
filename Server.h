#include <iostream>
#include <sys/msg.h>
#include <sys/resource.h>

#include "ipc.h"

#define MAX_CONNECTIONS 10

class Server {
public:
  explicit Server(key_t&);
  virtual ~Server() {}
  
  int Listen();
  bool ProcessMessage(MSG&);
  size_t GetShare(int);
  int GetProcPrio(int);

private:
  size_t num_clients_;
  key_t key_;
  Connection connection_;
};