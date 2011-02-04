#ifndef IPC_H
#define IPC_H

#include <sys/msg.h>
#include <iostream>

#define MSGQUEUE_TMP "/tmp/"
#define MAXMESGDATA (2048 - sizeof(long) - (2 * sizeof(int)) - sizeof(size_t)) /* don't want sizeof(Mesg) > 4096 */

//#define MESGHDRSIZE (sizeof(Mesg) - MAXMESGDATA) /* length of mesg_len and mesg_type */

typedef struct _MSG_ {
  long type;   // message type
  int sender_pid;
  int priority;
  size_t data_len;  // #bytes in mesg_data
  char data[MAXMESGDATA];
} MSG;

class Connection
{
public:
  Connection(key_t&);
  virtual ~Connection() {}
  
  bool Create();
  bool Listen(int, MSG&);
  int Write(MSG&);

private:
  int qid_;
};

#endif