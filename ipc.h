#ifndef IPC_H
#define IPC_H

#ifdef __linux__
#include <stdio.h>
#endif

#include <sys/msg.h>
#include <iostream>
#include <errno.h>

/**
 * Define max message size (note this differs per OS).
 */
#if defined(__APPLE__)
#define MSGSIZE 2048
#else
#define MSGSIZE 4096
#endif

/**
 * Define path to be used for resource key generation.
 */
#define MSGQUEUE_TMP "/tmp/"

/**
 * Defining remaining room in message for the data section.
 */
#define MAXMESGDATA (MSGSIZE - sizeof(long) - (2 * sizeof(int)) - sizeof(size_t))

/**
 * The message structure.
 * Has the following values:
 *   - type:       Required by the SysV api. Categorizes messages and allows the 
 *                 queue to be easily filtered. 
 *   - sender_pid: The sender's process id.
 *   - priority:   The message's priority.
 *   - data:       The message's data.
 */
typedef struct _MSG_ {
  long type;   // message type
  int sender_pid;
  int priority;
  size_t data_len;  // #bytes in mesg_data
  char data[MAXMESGDATA];
} MSG;

/**
 * The connection class encapsulates the underlying message queue that is used
 * for communication between the client and server processes.
 * It provides a variety of static and object methods for managing the queue.
 *
 * @author Tom Nightingale
 */
class Connection {
private:
  /**
   * The qid of the connection.
   */
  int qid_;

public:
  /**
   * Connection constructor.
   *
   * @param key    A resource key to use for connection creation.
   * @param create A flag to either create a new connection or connect to an 
   *               existing one.
   *
   * @author Tom Nightingale
   */
  Connection(key_t& key, bool create = true);
  
  /**
   * Connection destructor.
   *
   * @author Tom Nightingale
   */ 
  virtual ~Connection() {}
  
  /**
   * Listen for messages on the message queue.
   * This is essentially a wrapper function for msgrcv().
   *
   * @param type They type to filter the queue by.
   * @param msg A buffer to place received data into.
   * @param flags Any special flags to apply to the msgrcv() call.
   *
   * @return 0 on success.
   *         -1 on error.
   *
   * @author Tom Nightingale
   */ 
  int Listen(int msg_type, MSG& msg, int flags = 0);
  
  /**
   * Write messages on to the message queue.
   * This is essentially a wrapper function for msgsnd().
   *
   * @param msg The data to send.
   * @param flags Any special flags to apply to the msgsnd() call.
   *
   * @return 0 on success.
   *         -1 on error.
   *
   * @author Tom Nightingale
   */
  int Write(MSG& msg, int flags = 0);
  
  /**
   * Will scan the queue for messages of the specified message type and remove 
   * them. This is useful if a client terminates unexpectedly and leaves 
   * messages unread in the queue.
   *
   * @param msg_type The type to filter the queue by.
   *
   * @author Tom Nightingale
   */
  void Cleanup(int msg_type);
  
  /**
   * Creates a new queue using supplied resource key.
   *
   * @param key The resource key to use to make the queue.
   *
   * @return A message queue descriptor.
   *
   * @author Tom Nightingale
   */
  static int CreateQueue(key_t& key) {
    int qid;
    if ((qid = msgget(key, 0644 | IPC_CREAT | IPC_EXCL)) == -1) {
      perror("Connection::CreateQueue: Error; Failed getting queue.");
      throw "Exception: Failed creating message queue.";
    }
  
    return qid;
  }

  /**
   * Attempts to connect to a specific queue.
   *
   * @param key The resource key used to identify the queue.
   *
   * @return 0 on success.
   *         -1 on failure.
   *
   * @author Tom Nightingale
   */
  static int ConnectQueue(key_t& key) {
    int qid;
    if ((qid = msgget(key, 0644)) == -1) {
      perror("Connection::ConnectQueue: Error; Failed getting queue.");
      throw "Exception: Failed connecting to message queue.";
    }
  
    return qid;
  }
 
  /**
   * Generates a resource key which can be used to create/get a queue.
   * Essentially a wrapper function for ftok().
   *
   * @return A resource key.
   *
   * @author Tom Nightingale.
   */  
  static key_t GetResKey() {
    key_t key;

    if ((key = ftok(MSGQUEUE_TMP, 1)) < 0) {
      perror("Connection::GetResKey(); Error: Could not create key.");
      return 1; 
    }
    
    return key;
  }
 
  /**
   * Destroys a queue.
   * Used during the server's teardown phase. 
   *
   * @param qid The queue's file descriptor.
   *
   * @return 0 on success.
   *         -1 on failure.
   *
   * @author Tom Nightingale.
   */  
  static int DestroyQueue(int qid) {
    if (msgctl(qid, IPC_RMID, 0) == -1) {        
      return -1;     
    }

    return (0);
  }
};

#endif
