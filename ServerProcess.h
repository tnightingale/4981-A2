#include <fstream>
#include <sstream>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "ipc.h"

/**
 * The server process class is responsible for responding to a client's 
 * request. It will read the requested file and send its contents in message
 * segments over the established connection.
 *
 * @author Tom Nightingale
 */
class ServerProcess {
private:
  /**
   * The current active connection.
   */ 
  Connection connection_;

  /**
   * This server process's pid.
   */
  pid_t pid_;

  /**
   * The process id of the client.
   */
  pid_t client_pid_;

  /**
   * The share of the queue allocated to this client based on its priority.
   */
  size_t qShare_;

public:
  /**
   * Constructor.
   *
   * @param connection The active connection which will be used to communicate 
   *                   with the client.
   * @param client_pid The pid of the client.
   * @param qShare     The share of the queue allocated to the client.
   *
   * @author Tom Nightingale
   */
  ServerProcess(Connection& connection, int client_pid, size_t qShare);
  
  /**
   * Destructor.
   *
   * @author Tom Nightingale
   */
  virtual ~ServerProcess() {};
  
  /**
   * Acknowledge an initial request from the client if server can read/find
   * requested file.
   *
   * @param file     A filestream object to use to open the requested file with.
   * @param filepath The path to the file requested by the client.
   *
   * @return True on success.
   *         False on error.
   *
   * @author Tom Nightingale
   */
  bool FileAck(std::ifstream& file, char* filepath);
  
  /**
   * Reads blocks of file and sends them to the client.
   *
   * @param file A filestream object to use to open the requested file with.
   *
   * @return True on sucess
   *         False on error.
   *
   * @author Tom Nightingale
   */
  bool Respond(std::ifstream& file);
  
  /**
   * Prepares message object to be then written to the connection.
   * This method split the entire file into blocks and the loop through them,
   * writing them to the connection.
   *
   * @param msg     The data block of the file.
   * @param msg_len The length of the data block in bytes.
   * @param flags   Any special flags to pass to the connection.
   * @param type    The message type.
   *
   * @return True on successful write.
   *         False on error.
   *
   * @author Tom Nightingale
   */
  bool Write(const char* msg, size_t msg_len, int flags, long type);
  
  /**
   * Helper function to get the length of a file from that file's filestream.
   *
   * @param filestream The filestream handle.
   *
   * @return The length of the file.
   *
   * @author Tom Nightingale
   */
  long GetFileLength(std::ifstream& filestream);
  
  /**
   * Helper function to convert a long to a string that can be passed in the 
   * message's data section. 
   *
   * @param val  The long to be converted.
   * @param dest The string to place the long into.
   * 
   * @return True on sucessful conversion.
   *         False on error.
   *
   * @author Tom Nightingale
   */
  bool LongToString(long val, std::string& dest);
 
  /**
   * (SIGINT) Signal handler.
   * This function performs the tear-down tasks that are required when a server
   * shuts down unexpectedly.
   *
   * @author Tom Nightingale
   */
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
};
