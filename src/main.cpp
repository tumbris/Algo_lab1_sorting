#include <Tests.h>
#include <Utility.h>
#include <ApplicationArgs.h>

#include <iostream>

int main(int argc, char **argv)
{
    ApplicationArgs::CreateInstance(argv);

    if (auto appArgs = ApplicationArgs::GetInstance())
    {
        std::cout << appArgs->Get("-o") << ' ' << appArgs->Get("-N");
    }

    ApplicationArgs::DestroyInstance();
    return EXIT_SUCCESS;
}