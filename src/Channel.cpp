#include "Channel.hpp"

// constructor
Channel::Channel()
    : _channelName(""), _users(), _userStatus(), _topic(""), _topicSetUser(""),
      _topicSetAt(0), _channelModeFlag(Channel::None), _channelKey(""),
      _userLimit(INT_MAX), _banMasks(), _exceptionMasks(), _invitationMasks() {}

Channel::Channel(const std::string &channelName)
    : _channelName(channelName), _users(), _userStatus(), _topic(""),
      _topicSetUser(""), _topicSetAt(0), _channelModeFlag(Channel::None),
      _channelKey(""), _userLimit(INT_MAX), _banMasks(), _exceptionMasks(),
      _invitationMasks() {}

Channel::Channel(const std::string &channelName, const std::string &key)
    : _channelName(channelName), _users(), _userStatus(), _topic(""),
      _topicSetUser(""), _topicSetAt(0), _channelModeFlag(Channel::None),
      _channelKey(key), _userLimit(INT_MAX), _banMasks(), _exceptionMasks(),
      _invitationMasks() {
  if (!key.empty()) {
    this->_channelModeFlag |= Channel::Key;
  }
}

// destructor
Channel::~Channel() {}

// getter
const std::string &Channel::getChannelName() const {
  return (this->_channelName);
}

// const std::map<User *, unsigned int> &Channel::getUserStatus() const {
//   return (this->_userStatus);
// }

// user
void Channel::addUser(User &user) {
  if (this->_users.find(&user) == this->_users.end()) {
    this->_users.insert(&user);
    this->_userStatus[&user] = Channel::Normal;
    user.incrementJoinedChannelCount();
  }
}

void Channel::removeUser(User &user) {
  if (this->_users.find(&user) == this->_users.end()) {
    return ;
  }
  this->_users.erase(&user);
  this->_userStatus.erase(&user);
  user.decrementJoinedChannelCount();
}

int Channel::userNum() const { return (this->_users.size()); }

std::set<User *>::const_iterator Channel::getUserBegin() const {
  return (this->_users.begin());
}

std::set<User *>::const_iterator Channel::getUserEnd() const {
  return (this->_users.end());
}

bool Channel::isUserInChannel(const User &user) const {
  if (this->_users.find(const_cast<User *>(&user)) != this->_users.end()) {
    return (true);
  }
  return (false);
}

// user status
void Channel::setUserStatus(User &user, UserStatusFlags status, bool enable) {
  unsigned int &userStatus = this->_userStatus.at(&user);
  if (enable) {
    userStatus |= status;
  } else {
    userStatus &= ~status;
  }
}

bool Channel::hasUserStatus(User &user, const UserStatusFlags status) const {
  if ((this->_userStatus.at(&user) & status) == 1) {
    return (true);
  }
  return (false);
}

Channel::UserStatusFlags Channel::getUserStatus(User &user) const {
  if (this->_userStatus.find(&user) != this->_userStatus.end()) {
    return (static_cast<Channel::UserStatusFlags>(this->_userStatus.at(&user)));
  }
  return (Channel::Normal);
}

// topic
void Channel::setTopic(const std::string &topic, const std::string &nick) {
  this->_topic = topic;
  this->_topicSetUser = nick;
  this->_topicSetAt = getCurrentUnixTimestamp();
}

const std::string &Channel::getTopic() const { return (this->_topic); }

const std::string &Channel::getTopicSetUser() const {
  return (this->_topicSetUser);
}

const long &Channel::getTopicSetAt() const { return (this->_topicSetAt); }

// // channel mode
void Channel::setChannelMode(const ChannelModeFlags flag, bool enable) {
  if (enable) {
    this->_channelModeFlag |= flag;
  } else {
    this->_channelModeFlag &= ~flag;
  }
}

bool Channel::hasChannleMode(const ChannelModeFlags flag) const {
  if ((this->_channelModeFlag & flag) == 1) {
    return (true);
  }
  return (false);
}

// k flag
void Channel::setKey(const std::string &key) { this->_channelKey = key; }

const std::string &Channel::getKey() const { return (this->_channelKey); }

// l flag
void Channel::setUserLimit(int limit) { this->_userLimit = limit; }

int Channel::getUserLimit() const { return (this->_userLimit); }

// b flag
void Channel::addBanMask(const std::string &mask) {
  this->_banMasks.insert(mask);
}

void Channel::removeBanMask(const std::string &mask) {
  this->_banMasks.erase(mask);
}

bool Channel::isBanned(const std::string &mask) const {
  if (this->_banMasks.find(mask) != this->_banMasks.end()) {
    return (true);
  }
  return (false);
}

// e flag
void Channel::addExceptionMask(const std::string &mask) {
  this->_exceptionMasks.insert(mask);
}

void Channel::removeExceptionMask(const std::string &mask) {
  this->_exceptionMasks.erase(mask);
}

bool Channel::hasException(const std::string &mask) const {
  if (this->_exceptionMasks.find(mask) != this->_exceptionMasks.end()) {
    return (true);
  }
  return (false);
}

// // I flag
void Channel::addInvitationMask(const std::string &mask) {
  this->_invitationMasks.insert(mask);
}

void Channel::removeInvitationMask(const std::string &mask) {
  this->_invitationMasks.erase(mask);
}

bool Channel::isInvited(const std::string &mask) const {
  if (this->_invitationMasks.find(mask) != this->_invitationMasks.end()) {
    return (true);
  }
  return (false);
}

// func
long Channel::getCurrentUnixTimestamp() {
  std::time_t now = std::time(NULL);
  return (static_cast<long>(now));
}
