//
// Created by Charles on 11/09/2024.
//

#ifndef APP_HPP
#define APP_HPP

#include <libsystem.hpp>
#include <LuaEnvironment.hpp>
#include <SOL2/sol.hpp>
#include "app.hpp"

namespace paxolua::lib
{
    class AppLibrary final : public LuaLibrary
    {
    protected:
        void load(LuaEnvironment *env) override;

        void update(LuaEnvironment *env) override;
    };
} // paxolua::lib

#endif // APP_HPP
