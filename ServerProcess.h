#include <fstream>
#include <sstream>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "ipc.h"

class ServerProcess {
public:
  ServerProcess(Connection&, int, size_t);
  virtual ~ServerProcess();
  
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
  bool Write(const char* msg, size_t msg_len, int flags, long type);
  
  long GetFileLength(std::ifstream& filestream);
  bool LongToString(long val, std::string& dest);
 
  static void CatchSigIntChild(int) {
    pid_t pid = getpid();

    key_t key = Connection::GetResKey();
    int qid = Connection::ConnectQueue(key);
    MSG msg;
    std::string data("\nReceived terminate signal from server, TERMINATING.");

    msg.type = pid;
    msg.priority = -1;
    strncpy(msg.data, data.c_str(), data.length());
    msg.data_len = data.length();
    msgsnd(qid, &msg, sizeof(MSG) - sizeof(msg.type), IPC_NOWAIT);

    std::cout << "\tServer process " << pid << " stopped." << std::endl;    
    exit(0);
  } 
private:
  Connection connection_;
  pid_t pid_;
  pid_t client_pid_;
  size_t qShare_;
};
