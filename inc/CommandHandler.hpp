#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

class Server;

#include "Channel.hpp"
#include "Replies.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <sstream>

class CommandHandler {
public:
  CommandHandler(Server &server);
  ~CommandHandler();
  void handleCommand(const std::string &message);

private:
  CommandHandler();
  Server &_server;


  std::string _prefix;
  std::string _command;
  std::vector<std::string> _params;


  // parseMessage
  void parseMessage(const std::string &message);
  void extractPrefix(std::istringstream &iss);
  void extractCommand(std::istringstream &iss);
  void extractParam(std::istringstream &iss);

  // executeCommand
  void executeCommand(User &user);

  // command

  // command
  const std::string PASS(User &user);
  const std::string USER(User &user);
  const std::string NICK(User &user);
  
  // nick method
  void convertChar(std::string &str);
  bool isSpecialChar(const char c);
  bool validateNick(const std::string &str);

};

#endif
