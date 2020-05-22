#include <Utility.h>

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

namespace sg
{
    std::ostream& GetOutputStream(char** argv, std::ofstream& fileStream)
    {
        std::vector<std::string> args;
        while (*argv) args.emplace_back(*(argv++));
        for (auto argIt = args.cbegin(); argIt != args.cend(); ++argIt)
        {
            if (*argIt == "-o")
            {
                if (++argIt != args.cend())
                {
                    if (fileStream.is_open()) fileStream.close();
                    fileStream.open(*argIt);
                    return fileStream;
                }
            }
        }
        return std::cout;
    }
}