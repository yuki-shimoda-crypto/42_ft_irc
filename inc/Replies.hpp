#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

class Replies {
public:
  // 421
  static const std::string ERR_UNKNOWNCOMMAND(const std::string &command);
  // 461
  static const std::string ERR_NEEDMOREPARAMS(const std::string &command);
  // 462
  static const std::string ERR_ALREADYREGISTRED();
};

#endif // REPLIES
