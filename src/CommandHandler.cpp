#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Server &server) : _server(server) {}
CommandHandler::~CommandHandler() {}

const std::string CommandHandler::PASS(User &user) {
  if (this->_params.size() < 1)
    return Replies::ERR_NEEDMOREPARAMS(this->_command);
  if (user.getState() != User::NONE && user.getState() != User::PASS)
    return Replies::ERR_ALREADYREGISTRED();
  if (this->_params.at(0) != this->_server.getPassword()) {
    user.setState(User::PASS, false);
    return Replies::ERR_PASSWDMISMATCH();
  }
  user.setState(User::PASS, true);
  return "";
}

const std::string CommandHandler::USER(User &user) {
  const int REAL_NAME_MAX_LEN = 63;

  // paramsを確認
  if (this->_params.size() < 4)
    return Replies::ERR_NEEDMOREPARAMS(this->_command);
  // statusを確認
  if (user.getState() != User::NONE)
    return "";
  // stateが4,5,7だったら弾く(4以上)
  if ((user.getState() & User::USER) != 0)
    return Replies::ERR_ALREADYREGISTRED();

  // user処理
  // params[0]のバリデート
  if (this->_params.at(0).empty() || this->_params.at(1).empty() ||
      this->_params.at(2).empty() || this->_params.at(3).empty())
    return "";

  // params[0]のバリデート user
  // params[0]のバリデート user
  if (this->_params.at(0).find_first_of("@\n\r") != std::string::npos) {
    return "";
  }
  // params[1]のバリデート mode
  unsigned int mode;
  std::istringstream iss(this->_params.at(1));
  iss >> mode;
  if (iss.bad() || iss.fail() || !iss.eof()) {
    return "";
  }

  // params[2]のバリデート unused
  // do nothing

  // params[3]以降のバリデートrealname
  std::string realname;
  for (std::size_t i = 3; i < this->_params.size(); ++i) {
    if (i == 3 && this->_params.at(3).at(0) == ':')
      realname = this->_params.at(3).substr(1);
    else if (i == 3) {
      realname += this->_params.at(i);
    } else {
      realname += " ";
      realname += this->_params.at(i);
    }
    if (realname.size() > REAL_NAME_MAX_LEN)
      return "";
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
    this->_server.eraseTmpMap(user.getFd());
    this->_server.addRegisterMap(user.getFd(), user);
  }

  return "";
}
