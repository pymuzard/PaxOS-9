//
// Created by Charles on 12/09/2024.
//

#ifndef GUILIBRARY_HPP
#define GUILIBRARY_HPP

#include <LuaLibrary.hpp>

#include <lua_window.hpp>

namespace paxolua::lib
{
    class GUILibrary final : public LuaLibrary
    {
    public:
        GUILibrary();

        static std::string keyboard(const std::string &placeholder, const std::string &defaultText);
        // void del(LuaWidget *widget);

    protected:
        void load(LuaEnvironment *env) override;
        void update(LuaEnvironment *env) override;

    private:
        LuaWindow *m_currentWindow;
    };
} // paxolua::lib

#endif // GUILIBRARY_HPP
