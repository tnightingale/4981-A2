#include <string>
#include <iostream>

class Client {
public:
  Client(key_t& key);
  virtual ~Client () {}
  
  bool Request();
  int Write(std::string&, int);
  bool Listen();

private:
  key_t key_;
  Connection connection_;
};