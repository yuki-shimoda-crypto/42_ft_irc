#include "Replies.hpp"

// 251
const std::string Replies::RPL_LUSERCLIENT(const int &users,
                                           const int &services,
                                           const int &servers) {
  std::stringstream ss;
  std::string message;
  ss << "251";
  ss << " :There are ";
  ss << users;
  ss << " users and ";
  ss << services;
  ss << " services on ";
  ss << servers;
  ss << " servers";
  message = ss.str();
  message += "\r\n";
  return (message);
}

// 252
const std::string Replies::RPL_LUSEROP(const int &operators) {
  std::stringstream ss;
  std::string message;
  ss << "252 ";
  ss << operators;
  ss << " :operator(s) online";
  message = ss.str();
  message += "\r\n";
  return (message);
}

// 253
const std::string Replies::RPL_LUSERUNKNOWN(const int &unknown) {
  std::stringstream ss;
  std::string message;
  ss << "253 ";
  ss << unknown;
  ss << " :unknown connection(s))";
  message = ss.str();
  message += "\r\n";
  return (message);
}

// 254
const std::string Replies::RPL_LUSERCHANNELS(const int &channels) {
  std::stringstream ss;
  std::string message;
  ss << "254 ";
  ss << channels;
  ss << " :channels formed";
  message = ss.str();
  message += "\r\n";
  return (message);
}

// 255
const std::string Replies::RPL_LUSERME(const int &clients, const int &servers) {
  std::stringstream ss;
  std::string message;
  ss << "255";
  ss << " :I have ";
  ss << clients;
  ss << " clients and ";
  ss << servers;
  ss << " servers";
  message = ss.str();
  message += "\r\n";
  return (message);
}

// 256
const std::string Replies::RPL_ADMINME(const std::string &serverName) {
  std::string message;
  message += "256 ";
  message += serverName;
  message += " :Administrative info";
  message += "\r\n";
  return (message);
}

// 257
const std::string Replies::RPL_ADMINLOC1(const std::string &adminLocation) {
  std::string message;
  message += "257 ";
  message += " :";
  message += adminLocation;
  message += "\r\n";
  return (message);
}

// !
// 258
const std::string Replies::RPL_ADMINLOC2(const std::string &adminAffiliation) {
  std::string message;
  message += "258 ";
  message += " :";
  message += adminAffiliation;
  return (message);
}

// 259
const std::string Replies::RPL_ADMINEMAIL(const std::string &adminMail) {
  std::string message;
  message += "259 ";
  message += " :";
  message += adminMail;
  message += "\r\n";
  return (message);
}

// 332
const std::string Replies::RPL_TOPIC(const std::string &channelName,
                                     const std::string &topic) {
  std::string message;
  message += "332 ";
  message += channelName;
  message += " :";
  message += topic;
  message += "\r\n";
  return (message);
}

// !
// 333
const std::string Replies::RPL_TOPICWHOTIME(const std::string &channelName,
                                            const std::string &nick,
                                            const long &setat) {
  std::string message;
  message += "333 ";
  message += channelName;
  message += " ";
  message += nick;
  message += " ";
  message += setat;
  return (message);
}

// 351
const std::string Replies::RPL_VERSION(const std::string &version,
                                       const std::string &debuglevel,
                                       const std::string &server,
                                       const std::string &comments) {
  std::string message;
  message += "351 ";
  message += version;
  message += ".";
  message += debuglevel;
  message += " ";
  message += server;
  message += " :";
  message += comments;
  message += "\r\n";
  return (message);
}

/// !
// 353
const std::string Replies::RPL_NAMREPLY(const Channel &channel,
                                        const User &user) {
  std::string message;
  message += "353 ";

  // Channel mode
  if (channel.hasChannleMode(Channel::Secret)) {
    message += "@ ";
  } else if (channel.hasChannleMode(Channel::Private)) {
    message += "* ";
  } else {
    message += "= ";
  }

  // Channel name
  message += channel.getChannelName() + " :";

  // User status
  if (channel.hasUserStatus(const_cast<User &>(user), Channel::Operator)) {
    message += "@";
  } else if (channel.hasUserStatus(const_cast<User &>(user), Channel::Voice)) {
    message += "+";
  }
  message += user.getNickName();
  return (message);
}

// !
// 366
const std::string Replies::RPL_ENDOFNAMES(const std::string &channelName) {
  std::string message;
  message += "366 ";
  message += channelName;
  message += " :End of NAMES list";
  return (message);
}

// 371
const std::string Replies::RPL_INFO(const std::string detail,
                                    const std::string &str) {
  std::string message;
  message += "371";
  message += " :";
  message += detail;
  message += " ";
  message += str;
  message += "\r\n";
  return (message);
}

// 372
const std::string Replies::RPL_MOTD() {
  std::string message;
  message += "372";
  message += " :Today's message!!";
  message += "\r\n";
  return (message);
}

// 374
const std::string Replies::RPL_ENDOFINFO() {
  std::string message;
  message += "374";
  message += " :End of INFO list";
  message += "\r\n";
  return (message);
}

// 375
const std::string Replies::RPL_MOTDSTART() {
  std::string message;
  message += "375";
  message += " :- Message of the day - ";
  message += "\r\n";
  return (message);
}

// 376
const std::string Replies::RPL_ENDOFMOTD() {
  std::string message;
  message += "376";
  message += " :End of MOTD command";
  message += "\r\n";
  return (message);
}

// 381
const std::string Replies::RPL_YOUREOPER() {
  std::string message;
  message += "381 ";
  message += ":You are now an IRC operator";
  message += "\r\n";
  return (message);
}

// 391
const std::string Replies::RPL_TIME(const std::string &serverName,
                                    const std::string &time) {
  std::string message;
  message += "391 ";
  message += serverName;
  message += " :";
  message += time;
  return (message);
}

// 402
const std::string Replies::ERR_NOSUCHSERVER(const std::string &serverName) {
  std::string message;
  message += "402";
  message += serverName;
  message += " :No such server";
  message += "\r\n";
  return (message);
}

// !
// 403
const std::string Replies::ERR_NOSUCHCHANNEL(const std::string &channelName) {
  std::string message;
  message += "403 ";
  message += channelName;
  message += " :No such channel";
  return (message);
}

// !
// 405
const std::string Replies::ERR_TOOMANYCHANNELS(const std::string &channelName) {
  std::string message;
  message += "405 ";
  message += channelName;
  message += " :You have joined too many channels";
  return (message);
}

// 421
const std::string Replies::ERR_UNKNOWNCOMMAND(const std::string &command) {
  std::string message;
  message += "421 ";
  message += command;
  message += " :Unknown command";
  message += "\r\n";
  return (message);
}

// 422
const std::string Replies::ERR_NOMOTD() {
  std::string message;
  message += "422";
  message += " :MOTD File is missing";
  message += "\r\n";
  return (message);
}

// 431
const std::string Replies::ERR_NONICKNAMEGIVEN() {
  std::string message;
  message += "431 ";
  message += ":No nickname given";
  message += "\r\n";
  return (message);
}

// 432
const std::string Replies::ERR_ERRONEUSNICKNAME(const std::string &nick) {
  std::string message;
  message += "432 ";
  message += nick;
  message += " :Erroneous nickname";
  message += "\r\n";
  return message;
}

// 433
const std::string Replies::ERR_NICKNAMEINUSE(const std::string &nick) {
  std::string message;
  message += "433 ";
  message += nick;
  message += " :Nickname is already in use";
  message += "\r\n";
  return message;
}

// 436
const std::string Replies::ERR_NICKCOLLISION(const std::string &nick,
                                             const std::string &user,
                                             const std::string &host) {
  std::string message;
  message += "436 ";
  message += nick;
  message += " :Nickname collision KILL from ";
  if (user.empty())
    message += "<user>";
  else
    message += user;
  message += host;
  message += "\r\n";
  return message;
}

// 437
const std::string Replies::ERR_UNAVAILRESOURCE(const std::string &str) {
  std::string message;
  message += "437 ";
  message += str;
  message += " :Nick/channel is temporarily unavailable";
  message += "\r\n";
  return message;
}

// 441
const std::string Replies::ERR_USERNOTINCHANNEL(const std::string &nick,
                                              const std::string &channel) {
  std::string message;
  message += "441 ";
  message += nick;
  message += " ";
  message += channel;
  message += " :They aren't on that channel";
  message += "\r\n";
  return (message);
}

// 442
const std::string Replies::ERR_NOTONCHANNEL(const std::string &channel) {
  std::string message;
  message += "442 ";
  message += channel;
  message += " :You're not on that channel";
  message += "\r\n";
  return (message);
}

// 443
const std::string Replies::ERR_USERONCHANNEL(const std::string &nick,
                                           const std::string &channel) {
  std::string message;
  message += "443 ";
  message += nick;
  message += " ";
  message += channel;
  message += " :is already on channel";
  message += "\r\n";
  return (message);
}

// 451
const std::string Replies::ERR_NOTREGISTERED() {
  std::string message;
  message += "451 ";
  message += ":You have not registered";
  message += "\r\n";
  return (message);
}

// 461
const std::string Replies::ERR_NEEDMOREPARAMS(const std::string &command) {
  std::string message;
  message += "461 ";
  message += command;
  message += " :Not enough parameters";
  message += "\r\n";
  return (message);
}

// 462
const std::string Replies::ERR_ALREADYREGISTRED() {
  std::string message;
  message += "462 ";
  message += ":Unauthorized command (already registered)";
  message += "\r\n";
  return (message);
}

// 464
const std::string Replies::ERR_PASSWDMISMATCH() {
  std::string message;
  message += "464 ";
  message += ":Password incorrect";
  message += "\r\n";
  return (message);
}

// 471
const std::string Replies::ERR_CHANNELISFULL(const std::string &channelName) {
  std::string message;
  message += "471 ";
  message += channelName;
  message += " :Cannot join channel (+l)";
  message += "\r\n";
  return (message);
}

// 473
const std::string Replies::ERR_INVITEONLYCHAN(const std::string &channelName) {
  std::string message;
  message += "473 ";
  message += channelName;
  message += " :Cannot join channel (+i)";
  message += "\r\n";
  return (message);
}

// 474
const std::string Replies::ERR_BANNEDFROMCHAN(const std::string &channelName) {
  std::string message;
  message += "474 ";
  message += channelName;
  message += " :Cannot join channel (+b)";
  message += "\r\n";
  return (message);
}

// 475
const std::string Replies::ERR_BADCHANNELKEY(const std::string &channelName) {
  std::string message;
  message += "475 ";
  message += channelName;
  message += " :Cannot join channel (+k)";
  message += "\r\n";
  return (message);
}

// !
// 476
const std::string Replies::ERR_BADCHANMASK(const std::string &channelName) {
  std::string message;
  message += "476 ";
  message += channelName;
  message += " :Bad Channel Mask";
  return (message);
}

// 481
const std::string Replies::ERR_NOPRIVILEGES() {
  std::string message;
  message += "481";
  message += " :Permission Denied- You're not an IRC operator";
  message += "\r\n";
  return (message);
}

// 482
const std::string Replies::ERR_CHANOPRIVSNEEDED(const std::string &channel) {
  std::string message;
  message += "482 ";
  message += channel;
  message += " :You're not channel operator";
  message += "\r\n";
  return (message);
}

// 484
const std::string Replies::ERR_RESTRICTED() {
  std::string message;
  message += "484 ";
  message += ":Your connection is restricted!";
  message += "\r\n";
  return message;
}

// 491
const std::string Replies::ERR_NOOPERHOST() {
  std::string message;
  message += "491 ";
  message += ":No O-lines for your host";
  message += "\r\n";
  return (message);
}


// RPL_LISTSTART(321)
// RPL_LIST(322)
// RPL_LISTEND(323)
// RPL_NOTOPIC(331)
// RPL_INVITING(341)
