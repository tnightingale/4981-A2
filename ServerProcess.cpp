#include "ServerProcess.h"

using namespace std;

ServerProcess::ServerProcess(Connection& connection, int client_pid, size_t num_queue_shares)
: connection_(connection), client_pid_(client_pid), qShare_(num_queue_shares) {}


bool ServerProcess::Respond(char* filepath) {
  char msg_str[MAXMESGDATA];
  int msg_flag;
  int remaining = qShare_;
  long length = 0;
  
  ifstream file(filepath, ios_base::in | ios_base::binary);
  if (!file) {
    // Failed to open file.
    string error("Cannot find/open file.");
    this->Write(error.c_str(), error.length(), -1);
    return false;
  } 
  
  else {
    file.seekg (0, ios::end);
    length = file.tellg();
    file.seekg (0, ios::beg);
    
    string filelength;
    stringstream strstream;
    strstream << length;
    strstream >> filelength;
    
    this->Write(filelength.c_str(), filelength.length(), 0);
  }
  
  while (!file.eof()) {
    remaining = qShare_;
    while (remaining-- > 0 && !file.eof()) {
      file.read(msg_str, MAXMESGDATA);
      
      msg_flag = (file.eof()) ? -1 : 0;
      if (!this->Write(msg_str, file.gcount(), msg_flag)) {
        return false;
      }
    }
    sched_yield();
  }
  return true;
}


bool ServerProcess::Write(const char* msg_text, size_t msg_text_len, int msg_flag) {
  MSG msg;
  
  msg.type = client_pid_;
  msg.sender_pid = 0;
  msg.priority = msg_flag;
  msg.data_len = msg_text_len;
  strncpy(msg.data, msg_text, msg_text_len);
  
  if (!connection_.Write(msg)) {
    return false;
  }
  
  return true;
}