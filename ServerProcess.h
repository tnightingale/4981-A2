#include <fstream>
#include <sstream>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "ipc.h"

class ServerProcess {
public:
  ServerProcess(Connection&, int, size_t);
  virtual ~ServerProcess() {}
  
  bool FileAck(std::ifstream& file, char* filepath);
  
  /**
   * Open file for reading.
   * Pass contents to Write() for sending.
   */
  bool Respond(std::ifstream& file);
  
  /**
   * Prepare message object.
   * Write X message segments to queue then yeild (where X is qShare).
   * Repeat until entire message has been sent.
   */
  bool Write(const char*, size_t, int);
  
  long GetFileLength(std::ifstream& filestream);
  bool LongToString(long val, std::string& dest);
  
private:
  Connection connection_;
  int client_pid_;
  size_t qShare_;
};
