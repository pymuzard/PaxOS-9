#ifndef GSM_LUA_MODULE
#define GSM_LUA_MODULE

#include <string>
#include <cstdint>
#include <vector>
#include <SOL2/sol.hpp>
#include "gsm.hpp"
#include "conversation.hpp"

namespace LuaGSM
{
    void newMessage(std::string number, std::string message);
    void newCall(std::string number);
    void endCall();
    void acceptCall();
    void rejectCall();
    std::string getNumber();
    uint8_t getCallState();
    sol::table getMessages(const std::string &number, sol::state& lua);
};

#endif
