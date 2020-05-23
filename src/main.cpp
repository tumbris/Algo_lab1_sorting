#include <Tests.h>
#include <ApplicationArgs.h>
#include <fstream>
#include <iostream>

#pragma optimize("", off)

int main(int argc, char **argv)
{
    sg::ApplicationArgs::CreateInstance(argv);

    auto tests_res = sg::RunTests(sg::GenerateTestConfigClusters());

    auto appArgs = sg::ApplicationArgs::GetInstance();

    if (auto filepath = appArgs->Get("-o"))
    {
        std::ofstream f(filepath);
        f << sg::GetTestResultsAsString(tests_res);
    }
    else
    {
        std::cout << sg::GetTestResultsAsString(tests_res);
    }

    sg::ApplicationArgs::DestroyInstance();
    return EXIT_SUCCESS;
}