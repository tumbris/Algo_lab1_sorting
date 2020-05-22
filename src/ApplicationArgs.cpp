#include <ApplicationArgs.h>

ApplicationArgs* ApplicationArgs::instance = nullptr;

void ApplicationArgs::CreateInstance(char** argc, char prefix)
{
    instance = new ApplicationArgs(argc, prefix);
}

ApplicationArgs* ApplicationArgs::GetInstance()
{
    return instance;
}

void ApplicationArgs::DestroyInstance()
{
    delete instance;
}

const char* ApplicationArgs::Get(std::string_view arg)
{
    std::string key(arg);
    if (args.find(key) != args.end())
    {
        return args.at(key).c_str();
    }
    return nullptr;
}

std::filesystem::path ApplicationArgs::GetApplicationLocation()
{
    return appPath;
}

ApplicationArgs::ApplicationArgs(char** argv, char prefix)
{
    if (argv == nullptr) return;

    appPath = *(argv);
    while (*(++argv))
    {
        if (*(argv)[0] == prefix)
        {
            const char* key = *(argv++);
            if (const char* value = *(argv))
            {
                args.emplace(key, value);
            }
        }
    }
}
