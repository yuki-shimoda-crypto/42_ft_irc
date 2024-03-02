#ifndef REPLIES_HPP
#define REPLIES_HPP

#include <string>

class Replies {
public:
  // 421
  static const std::string ERR_UNKNOWNCOMMAND(const std::string &command);

  // 451
  static const std::string ERR_NOTREGISTERED();

  // 431
  static const std::string ERR_NONICKNAMEGIVEN();
  // 432
  static const std::string ERR_ERRONEUSNICKNAME(const std::string &nick);
  // 433
  static const std::string ERR_NICKNAMEINUSE(const std::string &nick);
  // 436
  static const std::string ERR_NICKCOLLISION(const std::string &nick,
                                             const std::string &user,
                                             const std::string &host);
  // 437
  static const std::string ERR_UNAVAILRESOURCE(const std::string &str);
  // 484
  static const std::string ERR_RESTRICTED();

  // 461
  static const std::string ERR_NEEDMOREPARAMS(const std::string &command);
  // 462
  static const std::string ERR_ALREADYREGISTRED();
  // 464
  static const std::string ERR_PASSWDMISMATCH();
};

#endif // REPLIES
