//
// Created by Charles on 11/09/2024.
//

#ifndef SYSTEMLIBRAYRY_HPP
#define SYSTEMLIBRAYRY_HPP

#include <SOL2/sol.hpp>
#include <graphics.hpp>

namespace paxolua::system
{
    void load(const sol::state &lua);

    namespace config
    {
        bool getBool(const std::string &key);
        float getFloat(const std::string &key);
        int getInt(const std::string &key);
        std::string getString(const std::string &key);
        bool has(const std::string &key);

        void setBool(const std::string &key, bool value);
        void setInt(const std::string &key, const int value);
        void setFloat(const std::string &key, float value);
        void setString(const std::string &key, const std::string &value);

        // TODO: Make "write()" auto.
        void write();

    }

}

#endif // SYSTEM_HPP
