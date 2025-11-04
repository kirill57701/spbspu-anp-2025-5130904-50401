#include <fstream>
#include <iostream>
#include <limits>
#include <cstdlib>

/*
Memory: malloc/free
Var-1: CNT-NZR-DIG
Var-2: FLL-INC-WAV
*/

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
    std::ifstream in(argv[2]);
    size_t a, b;
    in >> a >> b;
    if (in.fail())
    {
        std::cout << "Error of getting massive\n";
        return 2;
    }
    if (argv[1][0] == '2')
    {
        int ** mtx = (int**)malloc(a*sizeof(int*));
        for (int i = 0; i < a; ++i)
        {
            mtx[i] = (int*)malloc(b*sizeof(int));
        }
        for (size_t i = 0; i < a; ++i)
        {
            for (size_t j = 0; j < b; ++j)
            {
                in >> mtx[i][j];
            }
        }
    }
}
