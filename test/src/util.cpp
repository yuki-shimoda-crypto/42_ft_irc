#include "util.hpp"

void Util::start_announce() {
  std::cout
      << "\033[1;34m"
      << " __  __         ___ ____   ____   ____                           \n"
         "|  \\/  |_   _  |_ _|  _ \\ / ___| / ___|  ___ _ ____   _____ _ __ \n"
         "| |\\/| | | | |  | || |_) | |     \\___ \\ / _ \\ '__\\ \\ / / _ \\ "
         "'__|\n"
         "| |  | | |_| |  | ||  _ <| |___   ___) |  __/ |   \\ V /  __/ |   \n"
         "|_|  |_|\\__, | |___|_| \\_\\____| |____/ \\___|_|    \\_/ \\___|_|  "
         " \n"
         "        |___/                                                     \n"
         "     _             _           _                                  \n"
         " ___| |_ __ _ _ __| |_ ___  __| |                                 \n"
         "/ __| __/ _` | '__| __/ _ \\/ _` |                                 \n"
         "\\__ \\ || (_| | |  | ||  __/ (_| |  _ _ _ _                        "
         "\n"
         "|___/\\__\\__,_|_|   \\__\\___|\\__,_| (_|_|_|_)                     "
         "  \n"
      << "\033[0m";
}