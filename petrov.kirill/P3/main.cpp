#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>
#include <stdexcept>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    int* makemtx(char* a);
    void fillincway(char* a, char* b, int* mtx, size_t n);
    void cntnzrdig(char* a, char* b, int* mtx, size_t n);
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

int* petrov::makemtx(char* a)
{
    std::ifstream in(a);
    if (!in.is_open())
    {
        throw std::logic_error("err");
    }
    size_t b, c;
    in >> b >> c;
    if (in.fail() || in.eof())
    {
        throw std::logic_error("err");
    }
    if (b == 0 || c == 0)
    {
        return nullptr;
    }
    size_t n = (b > c) ? c : b;
    int* mtx = reinterpret_cast<int*>(malloc(sizeof(int) * n * n));
    if (mtx == nullptr)
    {
        throw std::logic_error("err");
    }
    try
    {
        for (size_t i = 0; i < n * n; ++i)
        {
            if (in.eof())
            {
                free(mtx);
                throw std::logic_error("err");
            }
            in >> mtx[i];
            if (in.fail())
            {
                free(mtx);
                throw std::logic_error("err");
            }
        }
    }
    catch (...)
    {
        free(mtx);
        throw;
    }
    in.close();
    return mtx;
}

void petrov::fillincway(char* a, char* b, int* mtx, size_t n)
{
    if (mtx == nullptr || n == 0)
    {
        std::ofstream ou(b);
        ou << 0;
        ou.close();
        return;
    }
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
            if (mtx[i* n + j] == 0){iszero = 0;}
            j++, i--;
        }
        q++, s += iszero, i = n - 1 - q, j = q, iszero = 1;
    }
    std::ofstream ou(b);
    ou << s;
    ou.close();
}

void petrov::cntnzrdig(char* a, char* b, int* mtx, size_t n)
{
    if (mtx == nullptr || n == 0)
    {
        std::ofstream ou(b, std::ios::app);
        ou << "\n" << 0 << " " << 0 << " ";
        ou.close();
        return;
    }
    size_t d = 1;
    while (d < n + 1)
    {
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (i >= d - 1 && i < n - d + 1 && j >= d - 1 && j < n - d + 1)
                {
                    mtx[i*n + j]++;
                }
            }
        }
        d++;
    }
    std::ofstream ou(b, std::ios::app);
    ou << "\n" << n << " " << n << " ";
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            ou << mtx[i*n + j] << " ";
        }
    }
    ou.close();
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
    if (c == 1)
    {
        int* mtx = nullptr;
        size_t n = 0;
        try
        {
            mtx = petrov::makemtx(argv[2]);
            std::ifstream in(argv[2]);
            if (in.is_open())
            {
                size_t b, c;
                in >> b >> c;
                if (!in.fail())
                {
                    n = (b > c) ? c : b;
                }
                in.close();
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << "err\n";
            return 2;
        }
        catch(...)
        {
            std::cerr << "err\n";
            return 2;
        }
        petrov::fillincway(argv[2], argv[3], mtx, n);
        petrov::cntnzrdig(argv[2], argv[3], mtx, n);
        if (mtx != nullptr)
        {
            free(mtx);
        }
        return 0;
    }
    else if (c == 2)
    {
        std::ofstream ou(argv[3]);
        ou << 0;
        ou << "\n" << 0 << " " << 0 << " ";
        ou.close();
        return 0;
    }
}
