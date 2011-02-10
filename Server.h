#ifdef __linux__
#include <stdlib.h>
#endif

#include <iostream>
#include <sys/msg.h>
#include <sys/resource.h>

#include "ipc.h"


/**
 * The server class defines a server object which is responsible for setting up
 * a connection and passing client requests on to new processes. 
 * It also manages priority requests and delegates connection share and
 * CPU time accordingly.
 *
 * @author Tom Nightingale
 */
class Server {
private:
  /**
   * The number of clients created. 
   */ 
  size_t num_clients_;

  /**
   * The resource key used to create new connections.
   */ 
  key_t key_;

  /**
   * The active connection. 
   */ 
  Connection connection_;

public:
  /**
   * Server constructor.
   *
   * @param key A resource key to use for connection creation.
   *
   * @author Tom Nightingale
   */
  explicit Server(key_t& key);
  
  /**
   * Server destructor.
   *
   * @author Tom Nightingale
   */ 
  virtual ~Server() {}
  
  /**
   * Begins listening for new client connections.
   * 
   * @return 0 for clean exit.
   *         1 if error occurs.
   *
   * @author Tom Nightingale
   */
  int Listen();

  /**
   * Process received client connection.
   * This causes the server to create a ServerProcess and pass it details of 
   * the new client's request for handling.
   *
   * @param msg The client's request message.
   * @return True on valid request and successful processing.
   *         False on invalid request or error during processing.
   *
   * @author Tom Nightingale
   */
  bool ProcessMessage(MSG&);
  /**
   * Lookup function to translate a priority into a queue share.
   *
   * @param priority The requested priority.
   * @return The size of specified priority's queue share.
   *
   * @author Tom Nightingale
   */
  size_t GetShare(int priority);

  /**
   * Lookup function to translate a priority into a process priority for use 
   * with setpriority().
   *
   * @param priority The requested priority.
   * @return The priority value to be passed to setpriority().
   *
   * @author Tom Nightingale
   */ 
  int GetProcPrio(int priority);
};
