#include <iostream>
#include <cstdlib>
#include <cstddef>

namespace petrov
{
    bool isitnum(char* a);
    bool outofrangeit(char* a);
}

bool petrov::isitnum(char* a)
{
    size_t i = 0;
    while (a[i] != '\0')
    {
        if (a[i] < '0' || a[i] > '9')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

bool petrov::outofrangeit(char* a)
{
    if ((a[0] == '1' || a[0] == '2') && a[1] == '\0'){return 1;}
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    else if(argc > 4)
    {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    else if(!petrov::isitnum(argv[1]))
    {
        std::cerr << "First parameter is not a number\n";
        return 1;
    }
    else if(!petrov::outofrangeit(argv[1]))
    {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }
}
