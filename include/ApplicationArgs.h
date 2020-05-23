#pragma once

#include <string_view>
#include <string>
#include <map>
#include <filesystem>
#include <sstream>
#include <optional>

namespace sg
{
    class ApplicationArgs
    {
    public:

        static void CreateInstance(char** argv, char prefix = '-');
        static ApplicationArgs* GetInstance();
        static void DestroyInstance();

        const char* Get(const std::string& arg);

        template <class T>
        std::optional<T> GetAs(const std::string& arg);

        std::filesystem::path GetApplicationLocation();


    private:
        ApplicationArgs(char** argv, char prefix);
    private:
        std::map<std::string, std::string> args;
        std::filesystem::path appPath;
    private:
        static ApplicationArgs* instance;
    };

    template<class T>
    std::optional<T> ApplicationArgs::GetAs(const std::string& arg)
    {
        if (auto option = Get(arg))
        {
            T temp;
            if (std::stringstream(option) >> temp)
            {
                return temp;
            }
        }
        return std::nullopt;
    }
}