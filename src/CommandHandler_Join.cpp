#include "CommandHandler.hpp"

// v3 JOIN message
// 1. ERR_NEEDMOREPARAMS (461)
// 1. ERR_NOSUCHCHANNEL (403)
// 1. ERR_TOOMANYCHANNELS (405)
// 1. ERR_BADCHANNELKEY (475)
// 1. ERR_BANNEDFROMCHAN (474)
// 1. ERR_CHANNELISFULL (471)
// 1. ERR_INVITEONLYCHAN (473)
// 1. ERR_BADCHANMASK (476)
// 1. RPL_TOPIC (332)
// 1. RPL_TOPICWHOTIME (333)
// 1. RPL_NAMREPLY (353)
// 1. RPL_ENDOFNAMES (366)

void CommandHandler::JOIN(User &user) {
  std::vector<std::string> channelNames;
  std::vector<std::string> keys;

  if (this->_params.at(0).empty()) {
    this->_server.sendReply(user.getFd(),
                            Replies::ERR_NEEDMOREPARAMS(this->_command));
  }

  splitChannelAndKey(channelNames, keys);

  for (std::size_t i = 0; i < channelNames.size(); i++) {

    if (channelNames.at(i).empty() || !isValidChannelName(channelNames.at(i))) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_NOSUCHCHANNEL(channelNames.at(i)));
      continue;
    } else if (hasReachedChannelLimit(user)) {
      this->_server.sendReply(user.getFd(),
                              Replies::ERR_TOOMANYCHANNELS(channelNames.at(i)));
      continue;
    }

    if (!this->_server.isExistChannel(channelNames.at(i))) {
      if (keys.at(i).empty()) {
        this->_server.addChannel(channelNames.at(i));
      } else {
        this->_server.addChannel(channelNames.at(i), keys.at(i));
      }
    }

    const Channel &channel = this->_server.getChannel(channelNames.at(i));
    if (!evaluateChannelJoinCondition(user, channel, keys.at(i))) {
      continue;
    }

    addUserToChannel(user, const_cast<Channel &>(channel));
    sendJoinResponses(user, channel);
  }
}

void CommandHandler::splitChannelAndKey(std::vector<std::string> &channels,
                                        std::vector<std::string> &keys) const {
  std::string token;

  std::istringstream issChannel(this->_params.at(0));
  while (std::getline(issChannel, token, ',')) {
    channels.push_back(token);
  }

  if (this->_params.size() > 1 && !this->_params.at(1).empty()) {
    std::istringstream issKey(this->_params.at(1));
    while (std::getline(issKey, token, ',')) {
      keys.push_back(token);
    }
  }
}

bool CommandHandler::isValidChannelName(const std::string &channelName) const {
  if (channelName.find_first_of("\a\n\r ,:") != std::string::npos) {
    return (false);
  } else if (channelName.at(0) != '#' || channelName.at(0) != '+' ||
             channelName.at(0) != '!' || channelName.at(0) != '&') {
    return (false);
  }
  return (true);
}

bool CommandHandler::hasReachedChannelLimit(const User &user) const {
  if (USER_CHANNEL_LIMIT < user.getJoinedChannelCount()) {
    return (true);
  }
  return (false);
}

bool CommandHandler::verifyChannelKey(const Channel &channel,
                                      const std::string &key) const {
  if (!channel.hasChannleMode(Channel::Key)) {
    return (true);
  }
  if (channel.getKey() != key) {
    return (false);
  }
  return (true);
}

bool CommandHandler::checkBanStatus(const Channel &channel,
                                    const std::string &nickname) const {
  if (!channel.hasChannleMode(Channel::BanMask)) {
    return (true);
  }
  if (channel.isBanned(nickname)) {
    return (false);
  }
  return (true);
}

bool CommandHandler::checkChannelCapacity(const Channel &channel) const {
  if (!channel.hasChannleMode(Channel::Limit)) {
    return (true);
  }
  if (channel.userNum() < channel.getUserLimit()) {
    return (true);
  }
  return (false);
}

bool CommandHandler::checkInviteOnlyStatus(const Channel &channel,
                                           const std::string &nickname) const {
  if (!channel.hasChannleMode(Channel::InviteOnly)) {
    return (true);
  }
  if (channel.isInvited(nickname)) {
    return (true);
  }
  return (false);
}

bool CommandHandler::evaluateChannelJoinCondition(
    const User &user, const Channel &channel, const std::string &key) const {
  if (!verifyChannelKey(channel, key)) {
    this->_server.sendReply(
        user.getFd(), Replies::ERR_BADCHANNELKEY(channel.getChannelName()));
    return (false);
  } else if (!checkBanStatus(channel, user.getNickName())) {
    this->_server.sendReply(
        user.getFd(), Replies::ERR_BANNEDFROMCHAN(channel.getChannelName()));
    return (false);
  } else if (!checkChannelCapacity(channel)) {
    this->_server.sendReply(
        user.getFd(), Replies::ERR_CHANNELISFULL(channel.getChannelName()));
    return (false);
  } else if (!checkInviteOnlyStatus(channel, user.getNickName())) {
    this->_server.sendReply(
        user.getFd(), Replies::ERR_INVITEONLYCHAN(channel.getChannelName()));
    return (false);
  }
  return (true);
}

void CommandHandler::addUserToChannel(User &user, Channel &channel) const {
  channel.addUser(user);
}

void CommandHandler::sendTopicReply(const User &user,
                                    const Channel &channel) const {
  // RPL_TOPIC (332)
  this->_server.sendReply(
      user.getFd(),
      Replies::RPL_TOPIC(channel.getChannelName(), channel.getTopic()));
  // RPL_TOPICWHOTIME (333)
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_TOPICWHOTIME(channel.getChannelName(),
                                                    user.getNickName(),
                                                    channel.getTopicSetAt()));
}

void CommandHandler::sendNamReply(const User &user,
                                  const Channel &channel) const {
  // RPL_NAMREPLY (353)
  for (std::set<User *>::const_iterator it = channel.getUserBegin();
       it != channel.getUserEnd(); it++) {
    this->_server.sendReply(user.getFd(), Replies::RPL_NAMREPLY(channel, **it));
  }
}

void CommandHandler::sendEndOfNamesReply(const User &user,
                                         const Channel &channel) const {
  // RPL_ENDOFNAMES (366)
  this->_server.sendReply(user.getFd(),
                          Replies::RPL_ENDOFNAMES(channel.getChannelName()));
}

void CommandHandler::sendJoinResponses(const User &user,
                                       const Channel &channel) const {
  sendTopicReply(user, channel);
  sendNamReply(user, channel);
  sendEndOfNamesReply(user, channel);
}
