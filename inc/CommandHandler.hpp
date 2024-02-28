#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "Replies.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <string>
#include <vector>

class CommandHandler {
public:
  void handleCommad(const std::string &command, User &user);

  CommandHandler(const Server &server);
  ~CommandHandler();
private:
  const Server &_server;

  void parseCommand(const std::string &commad);
  void executeCommand(const std::string &commandName,
                      const std::vector<std::string> &params, User &user);
  // command
  const std::string PASS(const std::string &commandName,
                         const std::vector<std::string> &params, User &user);
};

#endif
