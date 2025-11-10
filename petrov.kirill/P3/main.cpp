#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    void makestatmtx(std::ifstream& in, size_t r, size_t c, long long * statmtx);
    long long* makemtx(size_t , size_t c);
    void freemtx(long long* mtx);
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

int petrov::gettypemass(char* a)
{
    if (a[0] == '1' && a[1] == '\0'){return 1;}
    else if(a[0] == '2' && a[1] == '\0'){return 2;}
    else{return -1;}
}

void petrov::makestatmtx(std::ifstream& in, size_t r, size_t c, long long * statmtx)
{
    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < c; ++j)
        {
            in >> statmtx[i*r + j];
        }
    }
}

long long* petrov::makemtx(size_t r, size_t c)
{
    long long* matrix = reinterpret_cast<long long*>(malloc(sizeof(long long)*r*c));
    if (matrix == nullptr)
    {
        throw std::bad_alloc();
    }
    return matrix;
}

void petrov::freemtx(long long* mtx)
{
    free(mtx);
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
    int c = petrov::gettypemass(argv[1]);
    if (c != 1 && c != 2)
    {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }
    size_t rows = 0, cols = 0;
    std::ifstream in(argv[2]);
    in >> rows >> cols;
    long long statmtx[10000];
    long long * mtx = nullptr;
    if (c == 1)
    {
        petrov::makestatmtx(in, rows, cols, statmtx);
        if (!in)
        {
            std::cerr << "err\n";
            return 2;
        }
    }
    else
    {
        try
        {
            mtx = petrov::makemtx(rows, cols);
        }
        catch(const std::bad_alloc&)
        {
            std::cerr << "exception, err\n";
            petrov::freemtx(mtx);
            return 2;
        }
    }
    if (c == 2)
    {
        petrov::freemtx(mtx);
    }
}
