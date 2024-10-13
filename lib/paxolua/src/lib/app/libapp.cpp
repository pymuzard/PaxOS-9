//
// Created by Charles on 11/09/2024.
//

#include "libapp.hpp"

namespace paxolua::lib
{
    void AppLibrary::load(LuaEnvironment *env)
    {
        libsystem::log("Loading AppLibrary");

        sol::table paxo = env->getPaxoNamespace();
        auto app = paxo["app"].get_or_create<sol::table>();

        app.set_function("quit", [&]()
                         { 
                        AppManager::quitApp();; });
    }

    void AppLibrary::update(LuaEnvironment *env)
    {
    }
}