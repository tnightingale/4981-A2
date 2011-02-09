#include <fstream>
#include <sstream>
#include <string.h>
#include "ipc.h"

class ServerProcess {
public:
  ServerProcess(Connection&, int, size_t);
  virtual ~ServerProcess() {}
  
  /**
   * Open file for reading.
   * Pass contents to Write() for sending.
   */
  bool Respond(char*);
  
  /**
   * Prepare message object.
   * Write X message segments to queue then yeild (where X is qShare).
   * Repeat until entire message has been sent.
   */
  bool Write(const char*, size_t, int);
  
private:
  Connection connection_;
  int client_pid_;
  size_t qShare_;
};
