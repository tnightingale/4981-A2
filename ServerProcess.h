#include <fstream>
#include <string.h>
#include "ipc.h"

class ServerProcess {
public:
  ServerProcess(Connection&, int, size_t);
  virtual ~ServerProcess() {}
  
  bool Respond(char*);
  bool Write(char*, size_t, int);
  
private:
  Connection connection_;
  int client_pid_;
  size_t qShare_;
};
