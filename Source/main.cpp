#include <iostream>


#include "Application.h"

int main(const int argc, char* argv[])
{
    try
    {
        Application application{argc, argv};
        application.run();
    }
    catch (const Nimbus::Utils::Error::ArgParseError& argErr)
    {
        std::cerr << argErr.what();
        return 1;
    }
    return 0;
}
