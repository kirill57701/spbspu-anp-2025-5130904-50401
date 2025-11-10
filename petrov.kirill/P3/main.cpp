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
    void fillincway(char* b, int* mtx, size_t n);
    void cntnzrdig(char* b, int* mtx, size_t n);
    bool processStaticArray(char* inputFile, char* outputFile);
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
        in.close();
        throw std::logic_error("err");
    }
    if (b == 0 || c == 0)
    {
        in.close();
        return nullptr;
    }
    size_t n = (b > c) ? c : b;
    int* mtx = reinterpret_cast<int*>(malloc(sizeof(int) * n * n));
    if (mtx == nullptr)
    {
        in.close();
        throw std::logic_error("err");
    }
    for (size_t i = 0; i < n * n; ++i)
    {
        if (in.eof())
        {
            free(mtx);
            in.close();
            throw std::logic_error("err");
        }
        in >> mtx[i];
        if (in.fail())
        {
            free(mtx);
            in.close();
            throw std::logic_error("err");
        }
    }
    in.close();
    return mtx;
}

void petrov::fillincway(char* b, int* mtx, size_t n)
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

void petrov::cntnzrdig(char* b, int* mtx, size_t n)
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

bool petrov::processStaticArray(char* inputFile, char* outputFile)
{
    std::ifstream in(inputFile);
    if (!in.is_open())
    {
        return false;
    }
    size_t b, c;
    in >> b >> c;
    if (in.fail() || in.eof())
    {
        in.close();
        return false;
    }
    if (b == 0 || c == 0)
    {
        std::ofstream ou(outputFile);
        ou << 0;
        ou << "\n" << 0 << " " << 0 << " ";
        ou.close();
        in.close();
        return true;
    }
    size_t n = (b > c) ? c : b;
    const size_t MAX_SIZE = 1000;
    if (n > MAX_SIZE)
    {
        in.close();
        return false;
    }
    int staticMtx[MAX_SIZE * MAX_SIZE];
    for (size_t i = 0; i < n * n; ++i)
    {
        if (in.eof())
        {
            in.close();
            return false;
        }
        in >> staticMtx[i];
        if (in.fail())
        {
            in.close();
            return false;
        }
    }
    in.close();
    fillincway(outputFile, staticMtx, n);
    cntnzrdig(outputFile, staticMtx, n);
    return true;
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
        bool success = false;
        try
        {
            mtx = petrov::makemtx(argv[2]);
            std::ifstream in(argv[2]);
            if (in.is_open())
            {
                size_t b, c_val;
                in >> b >> c_val;
                if (!in.fail())
                {
                    n = (b > c_val) ? c_val : b;
                }
                in.close();
            }
            petrov::fillincway(argv[3], mtx, n);
            petrov::cntnzrdig(argv[3], mtx, n);
            success = true;
        }
        catch(const std::exception& e)
        {
            std::cerr << "err\n";
        }
        catch(...)
        {
            std::cerr << "err\n";
        }
        if (mtx != nullptr)
        {
            free(mtx);
        }
        return success ? 0 : 2;
    }
    else if (c == 2)
    {
        if (!petrov::processStaticArray(argv[2], argv[3]))
        {
            std::cerr << "err\n";
            return 2;
        }
        return 0;
    }
}
