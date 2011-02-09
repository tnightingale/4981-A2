#ifdef __linux__ 
#include <string.h>
#endif

#include <string>
#include <iostream>

#define OPT_NO_OPTION 1
#define OPT_FILENAME 2
#define OPT_FILENAME_PRIORITY 3


class Client {
public:
  Client(key_t& key);
  virtual ~Client () {}
  
  bool Request(std::string&, int = 5);
  bool Write(std::string&, int);
  bool Listen();

private:
  key_t key_;
  Connection connection_;
};
