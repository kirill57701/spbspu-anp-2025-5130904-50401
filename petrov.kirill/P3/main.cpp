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
    int** makefixmassive(size_t a, size_t b);
    size_t calculatediagonal(int ** mtx, size_t a, size_t b);
    size_t minn(size_t a, size_t b);
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

int ** petrov::makefixmassive(size_t a, size_t b)
{
    size_t c = petrov::minn(a, b);
    int** m = static_cast<int**>(malloc(c*sizeof(int*)));
        for (size_t i = 0; i < c; ++i)
        {
            m[i] = static_cast<int*>(malloc(c*sizeof(int)));
        }
    return m;
}

int petrov::isitoneortwo(const char* a)
{
    return ((a[0] == '1' || a[0] == '2') && a[1] == '\0') ? 1 : 0;
}

size_t petrov::minn(size_t a, size_t b)
{
    return a > b ? b : a;
}

size_t petrov::calculatediagonal(int ** mtx, size_t a, size_t b)
{
    size_t i = petrov::minn(a, b) - 1, j = 0, s = 0, c = 1;
    size_t q = petrov::minn(a, b);
    bool d = 1;
    while (c < q)
    {
        while (i > q - c - 1)
        {
            if (mtx[i][j] == 0)
            {
                d = 0;
            }
            i--;
            j++;
        }
        s += d, i = q - 1, j = c, c++, d = 1;
    }
    c = 1, i = 0, j = q - 1;
    while (c < q)
    {
        while (j > q - c - 1)
        {
            if (mtx[i][j] == 0)
            {
                d = 0;
            }
            i++;
            j--;
        }
        s += d, i = q - c - 1, j = q - 1, c++, d = 1;
    }
    return s;
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
    if (argv[1][0] == '1')
    {
        int ** mtx = petrov::makefixmassive(a, b);
        size_t c = petrov::minn(a, b);
        for (size_t i = 0; i < c; ++i)
        {
            for (size_t j = 0; j < c; ++j)
            {
                in >> mtx[i][j];
            }
        }
        std::cout << petrov::calculatediagonal(mtx, a, b) << "\n";
    }
}
