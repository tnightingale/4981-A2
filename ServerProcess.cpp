#include "ServerProcess.h"

using namespace std;

ServerProcess::ServerProcess(Connection& connection, int client_pid, size_t num_queue_shares)
: connection_(connection), client_pid_(client_pid), qShare_(num_queue_shares) {}


long ServerProcess::GetFileLength(ifstream& filestream) {
  long length = 0;
  
  filestream.seekg (0, ios::end);
  length = filestream.tellg();
  filestream.seekg (0, ios::beg);
  
  return length;
}

bool ServerProcess::LongToString(long val, string& dest) {
  stringstream strstream;
  strstream << val;
  strstream >> dest;
  
  return true;
}

bool ServerProcess::FileAck(ifstream& file, char* filepath) {
  long length = 0;
  
  file.open(filepath, ios_base::in | ios_base::binary);
  if (!file.good()) {
    // Failed to open file.
    string error("Cannot find/open file.");
    this->Write(error.c_str(), error.length(), -1);
    
    return false;
  }
  
  string filelength;
  length = GetFileLength(file);
  LongToString(length, filelength);
  
  this->Write(filelength.c_str(), filelength.length(), 0);
  
  return true;
}

bool ServerProcess::Respond(ifstream& file) {
  char msg_str[MAXMESGDATA];
  int msg_flag;
  int remaining = qShare_;
  
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
  int client_status = 0;
  int result = 0;
  
  msg.type = client_pid_;
  msg.sender_pid = 0;
  msg.priority = msg_flag;
  msg.data_len = msg_text_len;
  strncpy(msg.data, msg_text, msg_text_len);
  
  while ((result = connection_.Write(msg, IPC_NOWAIT)) == EAGAIN) {
    if ((client_status = kill(client_pid_, 0)) < 0) {
      cout << "ServerProcess::Write(); Client (PID: " << client_pid_ << ") not responding, performing cleanup." << endl;
      connection_.Cleanup(client_pid_);
      return false;
    }
  }

  if (result == -1) {
    return false;
  }
  
  return true;
}
