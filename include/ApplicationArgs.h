#pragma once

#include <string_view>
#include <string>
#include <map>
#include <filesystem>

class ApplicationArgs
{
public:

    static void CreateInstance(char** argv, char prefix = '-');
    static ApplicationArgs* GetInstance();
    static void DestroyInstance();

    const char* Get(std::string_view arg);
    std::filesystem::path GetApplicationLocation();


private:
    ApplicationArgs(char** argv, char prefix);
private:
    std::map<std::string, std::string> args;
    std::filesystem::path appPath;
private:
    static ApplicationArgs* instance;
};