#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool is_it_num(char* a);
    int get_type_mass(char* a);
    void make_stat_mtx(std::ifstream& in, size_t r, size_t c, int * statmtx);
    int* make_mtx(std::ifstream& in, size_t r, size_t c);
    void fll_inc_way(std::ofstream& ou, int* mtx, size_t r, size_t c);
    void cnt_nzr_dig(std::ofstream& ou, int* mtx, size_t r, size_t c);
}

bool petrov::is_it_num(char* a)
{
    size_t i = 0;
    while (a[i] != '\0')
    {
        if (!isdigit(a[i])) 
        {
            return 0;
        }
        i++;
    }
    return 1;
}

int petrov::get_type_mass(char* a)
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

void petrov::make_stat_mtx(std::ifstream& in, size_t r, size_t c, int * statmtx)
{
    r = std::min(c, r);
    if (r == 0)
    {
        throw std::runtime_error("err");
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

int* petrov::make_mtx(std::ifstream& in, size_t r, size_t c)
{
    int* mtx;
    int q;
    size_t w = r*c;
    r = std::min(r, c);
    if (r == 0)
    {
        throw std::runtime_error("err");
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
                free(mtx);
                throw std::logic_error("err");
            }
            in >> mtx[i*r + j];
            s++;
        }
    }
    for (size_t i = r*r; i < w; ++i)
    {
        if (in.eof())
        {
            free(mtx);
            throw std::logic_error("err");
        }
        in >> q;
    }
    if (in.fail())
    {
        free(mtx);
        throw std::logic_error("err");
    }
    return mtx;
}

void petrov::fll_inc_way(std::ofstream& ou, int* mtx, size_t r, size_t c)
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

void petrov::cnt_nzr_dig(std::ofstream& ou, int* mtx, size_t r, size_t c)
{
    r = std::min(r, c);
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
    else if(!petrov::is_it_num(argv[1]))
    {
        std::cerr << "First parameter is not a number\n";
        return 1;
    }
    int c = petrov::get_type_mass(argv[1]);
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
            petrov::make_stat_mtx(in, rows, cols, statmtx);
        }
        catch(const std::runtime_error&)
        {
            return 0;
        }
        catch(...)
        {
            std::cerr << "err\n";
            return 2;
        }
    }
    else
    {
        try
        {
            mtx = petrov::make_mtx(in, rows, cols);
        }
        catch(const std::runtime_error&)
        {
            free(mtx);
            return 0;
        }
        catch(...)
        {
            free(mtx);
            std::cerr << "err\n";
            return 2;
        }
    }
    in.close();
    std::ofstream ou(argv[3]);
    petrov::fll_inc_way(ou, (c == 1 ? statmtx : mtx), rows, cols);
    petrov::cnt_nzr_dig(ou, (c == 1 ? statmtx : mtx), rows, cols);
    if (c == 2)
    {
        free(mtx);
    }
    return 0;
}
