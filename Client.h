#ifdef __linux__ 
#include <stdlib.h>
#include <string.h>
#endif

#include <pthread.h>
#include <string>
#include <iostream>

/**
 * The client class defines all client objects.
 * Its lifecycle involves the following three steps.
 *   1. Open a connection to the server.
 *   2. Send file request to server and listen for response.
 *   3. On successful response, begin receiving messages from server and 
 *      printing to stdout.
 *
 * @author Tom Nightingale
 */
class Client {
private:
  /**
   * The resource key used to create new connections.
   */ 
  key_t key_;

  /**
   * This client's process id.
   */
  pid_t pid_;
  
  /**
   * The process id of the server.
   */ 
  pid_t serverProcPid_;
  
  /**
   * The active connection. 
   */ 
  Connection connection_;

  /**
   * The size of the file requested.
   */
  long filesize_;
  
  /**
   * The client's obligatory but entirely useless thread :).
   */ 
  pthread_t thread_;
public:
  /**
   * Client constructor.
   *
   * @param key A resource key to use for connection creation.
   *
   * @author Tom Nightingale
   */
  Client(key_t& key);

  /**
   * Client destructor.
   *
   * @author Tom Nightingale
   */ 
  virtual ~Client () {}
  
  /**
   * Make a request to the server.
   *
   * @param filepath The path to the requested file.
   * @param priority The requested priority.
   *
   * @return True on success.
   *         False on error.
   *
   * @author Tom Nightingale
   */ 
  bool Request(std::string& filepath, int priority = 5);
  
  /**
   * Send message to the server.
   *
   * @param msg The data to send to the server.
   * @param priority The requested priority.
   *
   * @return True on success.
   *         False on error.
   *
   * @author Tom Nightingale
   */ 
  bool Write(std::string& msg, int priority);
  
  /**
   * Listen for response to intial request to server.
   *
   * @return True on success.
   *         False on error.
   *
   * @author Tom Nightingale
   */ 
  bool WaitForResponse();
  
  /**
   * Begin receiving messages from the server and printing them to stdout.
   * This will loop until a message with priority set to -1 is received.
   *
   * @return True on success.
   *         False on error.
   *
   * @author Tom Nightingale
   */ 
  bool Receive();
  
  /**
   * This method has been included purely to give the obligatory thread 
   * something to do. As such, it just reminds us of its sad existence 
   * every three seconds.
   * 
   * @author Tom Nightingale
   */ 
  static void* PrintMessage(void * arg) {
    while (true) {
      for (int i = 0; i < 10000; i++) {
        std::cout << "Hey! I'm still here!!" << std::endl;
      }
      sleep(1);
    }
    
    return arg;
  }
};
