#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    int* makemtx(char* a);
    void fillincway(char* a, char* b, int* mtx);
    void cntnzrdig(char* a, char* b, int* mtx);
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
    size_t b, c;
    in >> b >> c;
    if (in.fail())
    {
        throw std::logic_error("err");
    }
    b = (b > c) ? c : b;
    int* mtx;
    mtx = reinterpret_cast<int*>(malloc(sizeof(int)*b*b));
    if (mtx == nullptr)
    {
        throw std::logic_error("err");
    }
    try
    {
        for (size_t i = 0; i < b*b; ++i)
        {
            if (in.eof() && i != b*b - 1)
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

void petrov::fillincway(char* a, char* b, int* mtx)
{
    size_t b1, c;
    std::ifstream in(a);
    in >> b1 >> c;
    in.close();
    size_t n = (b1 < c) ? b1 : c;
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

void petrov::cntnzrdig(char* a, char* b, int* mtx)
{
    size_t r, c;
    std::ifstream in(a);
    in >> r >> c;
    r = r > c ? c : r;
    in.close();
    size_t d = 1;
    while (d < r)
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
    std::ofstream ou(b, std::ios::app);
    ou << "\n" << r << " " << r << " ";
    for (size_t i = 0; i < r; ++i)
    {
        for (size_t j = 0; j < r; ++j)
        {
            ou << mtx[i*r + j] << " ";
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
        try
        {
            mtx = petrov::makemtx(argv[2]);
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
        if (mtx != nullptr)
        {
            petrov::fillincway(argv[2], argv[3], mtx);
            petrov::cntnzrdig(argv[2], argv[3], mtx);
            free(mtx);
        }
        return 0;
    }
    else if (c == 2)
    {
        return 0;
    }
}
