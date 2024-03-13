#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Server &server) : _server(server) {}
CommandHandler::~CommandHandler() {}

void CommandHandler::PASS(User &user) {
  if (this->_params.size() == 0) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NEEDMOREPARAMS(this->_command));
    return;
  }
  if (user.getState() != User::NONE && user.getState() != User::PASS) {
    this->_server.sendReply(user.getFd(),
                            Replies::Replies::ERR_ALREADYREGISTRED());
    return;
  }
  if (this->_params.at(0) != this->_server.getPassword()) {
    user.setState(User::PASS, false);
    this->_server.sendReply(user.getFd(), Replies::ERR_PASSWDMISMATCH());
    return;
  }
  user.setState(User::PASS, true);
  return;
}

void CommandHandler::USER(User &user) {

  // paramsを確認
  if (this->_params.size() < 4) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NEEDMOREPARAMS(this->_command));
    return;
  }
  // statusを確認
  if (user.getState() == User::NONE) {
    return;
  }
  // stateが4,5,7だったら弾く(4以上)
  if ((user.getState() & User::USER) != 0) {
    this->_server.sendReply(user.getFd(), Replies::ERR_ALREADYREGISTRED());
    return;
  }

  // user処理
  // params[0]のバリデート
  if (this->_params.at(0).empty() || this->_params.at(1).empty() ||
      this->_params.at(2).empty() || this->_params.at(3).empty()) {
    return;
  }

  // params[0]のバリデート user
  if (this->_params.at(0).find_first_of("@\n\r") != std::string::npos) {
    return;
  }
  // params[1]のバリデート mode
  unsigned int mode;
  std::istringstream iss(this->_params.at(1));
  iss >> mode;
  if (iss.bad() || iss.fail() || !iss.eof()) {
    return;
  }

  // params[2]のバリデート unused
  // do nothing

  // params[3]以降のバリデートrealname
  std::string realname;
  for (std::size_t i = 3; i < this->_params.size(); ++i) {
    if (i == 3 && this->_params.at(3).at(0) == ':') {
      realname = this->_params.at(3).substr(1);
    } else if (i == 3) {
      realname += this->_params.at(i);
    } else {
      realname += " ";
      realname += this->_params.at(i);
    }
    if (realname.size() > REAL_NAME_MAX_LEN)
      return;
  }

  // 全部ok setをする
  user.setUserName(this->_params.at(0));
  if ((mode & (1 << 2)) != 0) {
    user.setMode(User::Wallops, true);
  } else {
    user.setMode(User::Wallops, false);
  }
  if ((mode & (1 << 3)) != 0) {
    user.setMode(User::Invisible, true);
  } else {
    user.setMode(User::Invisible, false);
  }
  user.setRealName(realname);
  user.setState(User::USER, true);

  // 登録済みのmapに移動させる
  if (user.getState() == User::REGISTERD) {
    this->_server.addRegisterMap(user.getFd(), User(user));
    this->_server.eraseTmpMap(user.getFd());
  }

  return;
}

bool CommandHandler::isSpecialChar(const char c) {
  const std::string special = "[]\\`_^{|}";
  return special.find(c) != std::string::npos;
}

bool CommandHandler::validateNick(const std::string &str) {
  for (std::size_t i = 0; i < str.size(); ++i) {
    char c = str.at(i);
    if (i == 0) {
      if (!std::isalpha(static_cast<unsigned char>(c)) && !isSpecialChar(c))
        return (false);
    } else if (!std::isalnum(static_cast<unsigned char>(c)) &&
               !isSpecialChar(c) && c != '-') {
      return (false);
    }
  }
  return (true);
}

void CommandHandler::convertChar(std::string &str) {
  for (std::size_t i = 0; i < str.size(); ++i) {
    switch (str.at(i)) {
    case '{':
      str.at(i) = '[';
      break;
    case '}':
      str.at(i) = ']';
      break;
    case '|':
      str.at(i) = '\\';
      break;
    case '^':
      str.at(i) = '~';
      break;
    default:
      break;
    }
  }
}

bool CommandHandler::isReservedNick(const std::string &nick) {
  if (nick == OPER_USER || nick == ADMIN_NAME || nick == ANON_NAME) {
    return (true);
  }
  return (false);
}

void CommandHandler::NICK(User &user) {
  // ok
  if (this->_params.size() == 0) {
    this->_server.sendReply(user.getFd(), Replies::ERR_NONICKNAMEGIVEN());
    return;
  }

  // state の確認
  if ((user.getState() & User::PASS) == 0) {
    return;
  }

  // ok
  // nick paramのバリデーション
  if (this->_params.at(0).size() > 9 || !validateNick(this->_params.at(0))) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_ERRONEUSNICKNAME(this->_params.at(0)));
    return;
  }

  // ok
  // 変換 { →[, } → ], | → \, ^ → ~
  convertChar(this->_params.at(0));

  // historyに存在するかどうか,this->serverにnickHistoryのsetが存在する
  if (this->_server.isNick(this->_params.at(0)) != 0) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NICKNAMEINUSE(this->_params.at(0)));
    return;
  }

  // 予約された名前の拒否
  if (isReservedNick(this->_params.at(0))) {
    this->_server.sendReply(user.getFd(), Replies::ERR_NICKNAMEINUSE(this->_params.at(0)));
    return;
  }

  if (user.hasMode(User::Restricted)) {
    this->_server.sendReply(user.getFd(), Replies::ERR_RESTRICTED());
    return;
  }

  // 初期登録かどうかの判別
  if (user.getState() != User::REGISTERD) {
    user.setState(User::NICK, true);
    if (user.getState() == User::REGISTERD) {
      this->_server.addRegisterMap(user.getFd(), user);
      this->_server.eraseTmpMap(user.getFd());
    }
  }
  // set
  user.setNickName(this->_params.at(0));
  this->_server.setNickHistory(user.getNickName());
  return;
}

void CommandHandler::OPER(User &user) {
  if (this->_params.size() < 2) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NEEDMOREPARAMS(this->_command));
    return;
  }

  if (this->_params.at(1) != OPER_PASSWORD) {
    this->_server.sendReply(user.getFd(), Replies::ERR_PASSWDMISMATCH());
    return;
  }

  if (this->_params.at(0) != OPER_USER) {
    this->_server.sendReply(user.getFd(), Replies::ERR_NOOPERHOST());
    return;
  }

  user.setMode(User::Operator, true);
  this->_server.sendReply(user.getFd(), Replies::RPL_YOUREOPER());
}

void CommandHandler::MOTD(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  this->_server.sendReply(user.getFd(), Replies::RPL_MOTDSTART());
  this->_server.sendReply(user.getFd(), Replies::RPL_MOTD());
  this->_server.sendReply(user.getFd(), Replies::RPL_ENDOFMOTD());
}

void CommandHandler::LUSERS(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName() ||
        this->_params.at(1) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  int users = this->_server.numOfUser();
  int services = 0;
  int operators = this->_server.numOfOpeUser();
  int unknown = 0;
  int channnels = this->_server.numOfChannel();
  int clients = users;
  int servers = 1;

  this->_server.sendReply(user.getFd(),
                          Replies::RPL_LUSERCLIENT(users, services, servers));
  this->_server.sendReply(user.getFd(), Replies::RPL_LUSEROP(operators));
  this->_server.sendReply(user.getFd(), Replies::RPL_LUSERUNKNOWN(unknown));
  this->_server.sendReply(user.getFd(), Replies::RPL_LUSERCHANNELS(channnels));
  this->_server.sendReply(user.getFd(), Replies::RPL_LUSERME(clients, servers));
}

void CommandHandler::VERSION(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_VERSION(SERVER_VERSION, DEBUG_LEVEL,
                                               this->_server.getServerName(),
                                               SERVER_VERSION_COMMENT));
}
void CommandHandler::LINKS(User &user) {
  this->_server.sendReply(user.getFd(),
                          Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
}

void CommandHandler::TIME(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  std::stringstream ss;
  std::string time;

  std::time_t result = std::time(NULL);
  ss << std::ctime(&result);
  time = ss.str();
  this->_server.sendReply(
      user.getFd(), Replies::RPL_TIME(this->_server.getServerName(), time));
}

void CommandHandler::CONNECT(User &user) {
  if (this->_params.size() < 2) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NEEDMOREPARAMS(this->_command));
    return;
  }
  if (!user.hasMode(User::Operator)) {
    this->_server.sendReply(user.getFd(), Replies::ERR_NOPRIVILEGES());
    return;
  }
  this->_server.sendReply(user.getFd(),
                          Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
}

void CommandHandler::TRACE(User &user) {
  if (!(this->_params.size() == 0)) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
  } else {
    this->_server.sendReply(
        user.getFd(), Replies::ERR_NOSUCHSERVER(this->_server.getServerName()));
  }
}

void CommandHandler::ADMIN(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_ADMINME(this->_server.getServerName()));
  this->_server.sendReply(user.getFd(), Replies::RPL_ADMINLOC1(ADMIN_LOCATION));
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_ADMINLOC2(ADMIN_AFFILIATION));
  this->_server.sendReply(user.getFd(), Replies::RPL_ADMINEMAIL(ADMIN_MAIL));
}

void CommandHandler::INFO(User &user) {
  if (!(this->_params.size() == 0)) {
    if (this->_params.at(0) != this->_server.getServerName()) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHSERVER(this->_params.at(0)));
      return;
    }
  }
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_INFO("server version", SERVER_VERSION));
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_INFO("patch level", PATCH_LEVEL));
  this->_server.sendReply(
      user.getFd(),
      Replies::RPL_INFO("start day", this->_server.getStartDay()));
  this->_server.sendReply(user.getFd(), Replies::RPL_ENDOFINFO());
}
