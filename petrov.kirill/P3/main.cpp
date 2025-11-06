#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    int** makemtx(char* argv);
    void freemtx(int** a, size_t b);
    void cntnzrdig(char* a, int** mtx, char* o);
    void fllincway(char* a, int** mtx, char* o);
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

int** petrov::makemtx(char* agrv)
{
    std::ifstream in(agrv);
    size_t a, b;
    in >> a >> b;
    int** c = reinterpret_cast<int**>(malloc(a*sizeof(int*)));
    for (size_t i = 0; i < a; ++i)
    {
        c[i] = reinterpret_cast<int*>(malloc(b*sizeof(int)));
    }
    for (size_t i = 0; i < a; ++i)
    {
        for (size_t j = 0; j < b; ++j)
        {
            if (in.eof()){throw std::logic_error("too small args");}
            in >> c[i][j];
        }   
    }
    if (in.fail())
    {
        throw std::invalid_argument("err");
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

void petrov::fllincway(char* a, int** mtx, char* o)
{
    std::ifstream in(a);
    size_t s = 0, c, b, q = 0;
    in >> c >> b;
    in.close();
    c > b ? c = b : c = c;
    size_t i = 0, j = c - 1;
    bool iszero = 1;
    while (q < c - 1)
    {
        while (i < c - 1)
        {
            if (mtx[i][j] == 0)
            {
                iszero = 0;
            }
            i++;
            j--;
        }
        q++, s += iszero, i = q, j = c - q - 1, iszero = 1;
    }
    i = c - 1, q = 0, j = 0, iszero = 1;
    while (q < c - 1)
    {
        while (j < c - 1)
        {
            if (mtx[i][j] == 0)
            {
                iszero = 0;
            }
            j++, i--;
        }
        q++, s += iszero, i = c - 1 - q, j = q, iszero = 1;
    }
    std::ofstream ou(o);
    ou << s << "\n";
}

void petrov::cntnzrdig(char* a, int** mtx, char* o)
{
    size_t c = 0, b, q;
    std::ifstream in(a);
    in >> b >> q;
    in.close();
    q > b ? q = b : q = q;
    while (c < b/2)
    {
        for (size_t i = 0; i < q; ++i)
        {
            for (size_t j = 0; j < q; ++j)
            {
                if (i == c || i == q - c - 1 || j == c || j == q - c - 1)
                {
                    mtx[i][j] = mtx[i][j] + c + 1;
                }
            }
        }
        c++;
    }
    std::ofstream ou(o, std::ios::app);
    for (size_t i = 0; i < q; ++i)
    {
        for (size_t j = 0; j < q; ++j)
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
    if (c == 1)
    {
        try{
            int** d = petrov::makemtx(argv[2]);
            size_t rows;
            std::ifstream in(argv[2]);
            in >> rows;
            petrov::fllincway(argv[2], d, argv[3]);
            petrov::cntnzrdig(argv[2], d, argv[3]);
            petrov::freemtx(d, rows);
            in.close();
            return 0;
        }catch(...){
            std::cerr << "error\n";
            return 2;
        }
    }
  else
    {
    return 0;
    }
}
