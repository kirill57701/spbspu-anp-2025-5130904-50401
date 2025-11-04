#include <fstream>
#include <iostream>
#include <limits>

namespace petrov
{
    int isitnumber(const char* a);
    int isitoneortwo(const char* a);
}

int petrov::isitnumber(const char* a)
{
    int i = 0;
    while (a[i] != '\0')
    {
        if (a[i] > '9' || a[i] < '0')
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int petrov::isitoneortwo(const char* a){
    return ((a[0] == '1' || a[0] == '2') && a[1] == '\0') ? 1 : 0;
}

int main(int argc, char ** argv)
{
    if (argc < 4)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    else if (argc > 4)
    {
        std::cerr << "Too many arguments\n";
        return 1;
    }
    else if(!petrov::isitnumber(argv[1]))
    {
        std::cerr << "First parameter is not a number\n";
        return 1;
    }
    else if(!petrov::isitnumber(argv[1]))
    {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }
}
 

