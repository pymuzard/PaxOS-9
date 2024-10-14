//
// Created by Charles on 12/09/2024.
//

#ifndef GUILIBRARY_HPP
#define GUILIBRARY_HPP

#include <LuaLibrary.hpp>

#include <lua_window.hpp>

#include "ElementBase.hpp"

class Ele;

namespace paxolua::lib {
    class GUILibrary final : public LuaLibrary {
    public:
        GUILibrary();

    protected:
        void load(LuaEnvironment *env) override;

        void update(LuaEnvironment *env) override;

        void keyboard(const std::string &placeholder, const std::string &defaultText, sol::function callback);

    private:
        enum GraphicalMode
        {
            NO_GRAPHICS,
            GRAPHICS,
            KEYBOARD
        };

        LuaWindow *m_currentWindow;
        struct KeyboardData
        {
            Keyboard *m_keyboardWindow = nullptr;
            sol::function callback;     // void (std::string textWritten)
            GraphicalMode graphicalModeBackup;
        };

        KeyboardData m_keyboard;
        
        GraphicalMode graphicalMode = NO_GRAPHICS;
    };
} // paxolua::lib

#endif //GUILIBRARY_HPP
