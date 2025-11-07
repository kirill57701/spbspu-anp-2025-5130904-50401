#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    int** makemtx(char* argv, size_t& b, size_t& c);
    void freemtx(int** a, size_t b);
    void cntnzrdig(size_t b, size_t c, int** mtx, char* o);
    void fllincway(size_t b, size_t c, int** mtx, char* o);
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
    else if(a[0] == '2' && a[1] == '\0'){return 2;}
    else{return -1;}
}

int** petrov::makemtx(char* agrv, size_t& a, size_t& b)
{
    std::ifstream in(agrv);
    if (!in.is_open()){throw std::logic_error("err");}

    in >> a >> b;
    if (in.fail()){throw std::invalid_argument("err");}
    if (a == 0 || b == 0){throw std::invalid_argument("err");}

    int** c = reinterpret_cast<int**>(malloc(a * sizeof(int*)));
    if (!c){throw std::logic_error("err");}

    for (size_t i = 0; i < a; ++i)
    {
        c[i] = reinterpret_cast<int*>(malloc(b * sizeof(int)));
        if (!c[i])
        {
            for (size_t j = 0; j < i; ++j){free(c[j]);}
            free(c);
            throw std::logic_error("err");
        }
    }

    for (size_t i = 0; i < a; ++i)
    {
        for (size_t j = 0; j < b; ++j)
        {
            if (in.eof())
            {
                for (size_t k = 0; k < a; ++k){free(c[k]);}
                free(c);
                throw std::logic_error("err");
            }
            in >> c[i][j];
            if (in.fail())
            {
                for (size_t k = 0; k < a; ++k){free(c[k]);}
                free(c);
                throw std::invalid_argument("err");
            }
        }
    }
    in.close();
    return c;
}

void petrov::freemtx(int** a, size_t b)
{
    for (size_t i = 0; i < b; ++i)
    {
        free(a[i]);
    }
    free(a);
}

void petrov::fllincway(size_t b, size_t c, int** mtx, char* o)
{
    size_t n = (b < c) ? b : c;
    size_t s = 0, q = 0;
    size_t i = 0, j = n - 1;
    bool iszero = 1;
    while (q < n - 1)
    {
        while (i < n - 1)
        {
            if (mtx[i][j] == 0){iszero = 0;}
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
            if (mtx[i][j] == 0){iszero = 0;}
            j++, i--;
        }
        q++, s += iszero, i = n - 1 - q, j = q, iszero = 1;
    }
    std::ofstream ou(o);
    ou << s << "\n";
}

void petrov::cntnzrdig(size_t b, size_t c, int** mtx, char* o)
{
    size_t n = (b < c) ? b : c;
    size_t cnt = 0;
    while (cnt < n/2)
    {
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (i == cnt || i == n - cnt - 1 || j == cnt || j == n - cnt - 1)
                {
                    mtx[i][j] = mtx[i][j] + cnt + 1;
                }
            }
        }
        cnt++;
    }
    if (n % 2 != 0){mtx[n/2][n/2] += cnt + 1;}

    std::ofstream ou(o, std::ios::app);
    ou << b << " " << c << " ";
    for (size_t i = 0; i < b; ++i)
    {
        for (size_t j = 0; j < c; ++j)
        {
            ou << mtx[i][j] << " ";
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
    if (c == 2)
    {
        try{
            size_t rows, cols;
            int** d = petrov::makemtx(argv[2], rows, cols);
            petrov::fllincway(rows, cols, d, argv[3]);
            petrov::cntnzrdig(rows, cols, d, argv[3]);
            petrov::freemtx(d, rows);
            return 0;
        }catch(...){
            std::cerr << "error\n";
            return 2;
        }
    }
    else
    {
        try
        {
            std::ifstream in(argv[2]);
            if (!in.is_open())
            {
                std::cerr << "Cannot open input file\n";
                return 1;
            }
            size_t rows, cols;
            in >> rows >> cols;
            if (in.fail())
            {
                std::cerr << "Invalid matrix dimensions\n";
                return 1;
            }
            int temp;
            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    if (in.eof())
                    {
                        std::cerr << "Not enough data in file\n";
                        return 1;
                    }
                    in >> temp;
                    if (in.fail())
                    {
                        std::cerr << "Invalid data in file\n";
                        return 1;
                    }
                }
            }
            return 0;
        }
        catch(...)
        {
            std::cerr << "Error processing static array\n";
            return 1;
        }
    }
}
