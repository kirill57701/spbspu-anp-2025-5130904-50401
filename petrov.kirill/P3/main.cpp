#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    void makestatmtx(std::ifstream& in, size_t r, size_t c, int * statmtx);
    int* makemtx(std::ifstream& in, size_t r, size_t c);
    void fillincway(std::ofstream& ou, int* mtx, size_t r, size_t c);
    void cntnzrdig(std::ofstream& ou, int* mtx, size_t r, size_t c);
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
    if (a[0] == '1' && a[1] == '\0')
    {
        return 1;
    }
    else if(a[0] == '2' && a[1] == '\0')
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

void petrov::makestatmtx(std::ifstream& in, size_t r, size_t c, int * statmtx)
{
    r = r > c ? c : r;
    if (r == 0)
    {
        throw std::logic_error("err");
    }
    size_t s = 0;
    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < r; ++j)
        {
            if (in.eof())
            {
                throw std::logic_error("err");
            }
            in >> statmtx[i*r + j];
            s++;
        }
    }
    if (s < r*r)
    {
        throw std::logic_error("err");
    }
    if (in.fail())
    {
        throw std::logic_error("err");
    }
}

int* petrov::makemtx(std::ifstream& in, size_t r, size_t c)
{
    int* mtx;
    r = r > c ? c : r;
    if (r == 0)
    {
        throw std::logic_error("err");
    }
    mtx = reinterpret_cast<int*>(malloc(sizeof(int)*r*r));
    if (mtx == nullptr)
    {
        throw std::bad_alloc();
    }
    size_t s = 0;
    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < r; ++j)
        {
            if (in.eof())
            {
                throw std::logic_error("err");
            }
            in >> mtx[i*r + j];
            s++;
        }
    }
    if (s < r*r)
    {
        throw std::logic_error("err");
    }
    if (in.fail())
    {
        throw std::logic_error("err");
    }
    return mtx;
}

void petrov::fillincway(std::ofstream& ou, int* mtx, size_t r, size_t c)
{
    size_t n = r > c ? c : r;
    size_t s = 0, q = 0;
    size_t i = 0, j = n - 1;
    bool iszero = 1;
    while (q < n - 1)
    {
        while (i < n - 1)
        {
            if (mtx[i*n + j] == 0){iszero = 0;}
            i++;
            j--;
        }
        q++, s += iszero, i = q, j = n - q - 1, iszero = 1;
    }
    i = n - 1, q = 0, j = 0, iszero = 1;
    while (q < n - 1)
    {
        while (j < n - 1)
        {
            if (mtx[i*n + j] == 0){iszero = 0;}
            j++, i--;
        }
        q++, s += iszero, i = n - 1 - q, j = q, iszero = 1;
    }
    ou << s;
}

void petrov::cntnzrdig(std::ofstream& ou, int* mtx, size_t r, size_t c)
{
    r = r > c ? c : r;
    size_t d = 1;
    while (d < r + 1)
    {
        for (size_t i = 0; i < r; ++i)
        {
            for (size_t j = 0; j < r; ++j)
            {
                if (i >= d - 1 && i < r - d + 1 && j >= d - 1 && j < r - d + 1)
                {
                    mtx[i*r + j]++;
                }
            }
        }
        d++;
    }
    ou << "\n" << r << " " << r << " ";
    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < r; ++j)
        {
            ou << mtx[i*r + j] << " ";
        }
    }
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
    if (in.fail())
    {
        std::cerr << "err\n";
        return 2;
    }
    int statmtx[10000];
    int * mtx = nullptr;
    if (c == 1)
    {
        try
        {
            petrov::makestatmtx(in, rows, cols, statmtx);
        }
        catch(const std::logic_error&)
        {
            return 0;
        }
        catch(...)
        {
            std::cerr << "err\n";
            return 2;
        }
        in.close();
        std::ofstream ou(argv[3]);
        petrov::fillincway(ou, statmtx, rows, cols);
        petrov::cntnzrdig(ou, statmtx, rows, cols);
    }
    else
    {
        try
        {
            mtx = petrov::makemtx(in, rows, cols);
        }
        catch(const std::logic_error&)
        {
            return 0;
        }
        catch(const std::bad_alloc&)
        {
            free(mtx);
            std::cerr << "err\n";
            return 3;
        }
        catch(...)
        {
            free(mtx);
            std::cerr << "err\n";
            return 2;
        }
        in.close();
        std::ofstream ou(argv[3]);
        petrov::fillincway(ou, mtx, rows, cols);
        petrov::cntnzrdig(ou, mtx, rows, cols);
    }
    if (c == 2)
    {
        free(mtx);
    }
    return 0;
}
