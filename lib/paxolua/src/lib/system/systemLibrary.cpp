//
// Created by Charles on 11/09/2024.
//
#include <SOL2/sol.hpp>
#include "systemLibrary.hpp"
#include <libsystem.hpp>

void paxolua::system::load(sol::state &lua)
{
    auto paxo = lua["paxo"].get<sol::table>();

    // paxo.system
    auto system = paxo["system"].get_or_create<sol::table>(sol::new_table());

    // paxo.system.config
    auto systemConfig = system["config"].get_or_create<sol::table>(sol::new_table());

    // paxo.system.config.get()
    systemConfig.set_function("get", sol::overload(
                                         &config::getBool,
                                         &config::getFloat,
                                         &config::getInt,
                                         &config::getString));

    // paxo.system.config.set()
    systemConfig.set_function("set", sol::overload(
                                         &config::setBool,
                                         &config::setFloat,
                                         &config::setInt,
                                         &config::setString));

    // paxo.system.config.write() / paxo.system.config.save()
    systemConfig.set_function("has", &config::has);
    systemConfig.set_function("write", &config::write);

    // paxo.system.settings
    auto systemSettings = system["settings"].get_or_create<sol::table>(sol::new_table());

    systemSettings.set_function("getBrightness", &libsystem::paxoConfig::getBrightness);
    systemSettings.set_function("setBrightness", &libsystem::paxoConfig::setBrightness);
    systemSettings.set_function("setStandBySleepTime", &libsystem::paxoConfig::setStandBySleepTime);
    systemSettings.set_function("getStandBySleepTime", &libsystem::paxoConfig::getStandBySleepTime);

    systemSettings.set_function("getOSVersion", &libsystem::paxoConfig::getOSVersion);

    systemSettings.set_function("getConnectedWifi", &libsystem::paxoConfig::getConnectedWifi);
    systemSettings.set_function("connectWifi", &libsystem::paxoConfig::connectWifi);
    systemSettings.set_function("getAvailableWifiSSID", [&]() -> sol::table
                                {
        std::vector<std::string> lstSSID = libsystem::paxoConfig::getAvailableWifiSSID();

        sol::table result = lua.create_table();
        for (const auto elem : lstSSID) {
            result.add(elem);
        }
        return result; });

    systemSettings.set_function("getBackgroundColor", &libsystem::paxoConfig::getBackgroundColor);
    systemSettings.set_function("getTextColor", &libsystem::paxoConfig::getTextColor);
    systemSettings.set_function("getBorderColor", &libsystem::paxoConfig::getBorderColor);
    systemSettings.set_function("setBackgroundColor", &libsystem::paxoConfig::setBackgroundColor);
    systemSettings.set_function("setTextColor", &libsystem::paxoConfig::setTextColor);
    systemSettings.set_function("setBorderColor", &libsystem::paxoConfig::setBorderColor);
}

bool paxolua::system::config::has(const std::string &key)
{
    libsystem::log("has: " + key);
    return libsystem::getSystemConfig().has(key);
}

bool paxolua::system::config::getBool(const std::string &key)
{
    libsystem::log("getBool: " + key);
    return libsystem::getSystemConfig().get<bool>(key);
}

float paxolua::system::config::getFloat(const std::string &key)
{
    libsystem::log("getFloat: " + key);
    return libsystem::getSystemConfig().get<float>(key);
}

int paxolua::system::config::getInt(const std::string &key)
{
    libsystem::log("getInt: " + key);
    return libsystem::getSystemConfig().get<int>(key);
}

std::string paxolua::system::config::getString(const std::string &key)
{
    libsystem::log("getString: " + key);
    return libsystem::getSystemConfig().get<std::string>(key);
}

void paxolua::system::config::setBool(const std::string &key, const bool value)
{
    libsystem::log("setBool: " + key + ", " + std::to_string(value));
    libsystem::getSystemConfig().set<bool>(key, value);
}

void paxolua::system::config::setInt(const std::string &key, const int value)
{
    libsystem::log("setInt: " + key + ", " + std::to_string(value));
    libsystem::getSystemConfig().set<int>(key, value);
}

void paxolua::system::config::setFloat(const std::string &key, const float value)
{
    libsystem::log("setFloat: " + key + ", " + std::to_string(value));
    libsystem::getSystemConfig().set<float>(key, value);
}

void paxolua::system::config::setString(const std::string &key, const std::string &value)
{
    libsystem::log("setString: " + key + ", " + value);
    libsystem::getSystemConfig().set<std::string>(key, value);
}

void paxolua::system::config::write()
{
    libsystem::log("write");
    libsystem::getSystemConfig().write();
}
