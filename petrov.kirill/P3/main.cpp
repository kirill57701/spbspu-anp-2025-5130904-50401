#include <iostream>
#include <memory>
#include <cstddef>
#include <fstream>

namespace petrov
{
    bool isitnum(char* a);
    int gettypemass(char* a);
    int** makemtx(char* argv, size_t& rows, size_t& cols);
    void freemtx(int** a, size_t b);
    void fllincway(char* a, int** mtx, char* o);
    void cntnzrdig(char* a, int** mtx, char* o);
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

int** petrov::makemtx(char* argv, size_t& rows, size_t& cols)
{
    std::ifstream in(argv);
    if (!in.is_open())
    {
        throw std::invalid_argument("Cannot open file");
    }
    in >> rows >> cols;
    if (rows == 0 || cols == 0)
    {
        in.close();
        return nullptr;
    }
    int** matrix = new int*[rows];
    for (size_t i = 0; i < rows; ++i)
    {
        matrix[i] = new int[cols];
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < cols; ++j)
        {
            if (in.eof())
            {
                for (size_t k = 0; k <= i; ++k)
                {
                    delete[] matrix[k];
                }
                delete[] matrix;
                in.close();
                throw std::logic_error("too small args");
            }
            in >> matrix[i][j];
        }
    }
    if (in.fail())
    {
        for (size_t i = 0; i < rows; ++i)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
        in.close();
        throw std::invalid_argument("err");
    }
    in.close();
    return matrix;
}

void petrov::freemtx(int** a, size_t b)
{
    if (a == nullptr) return;
    for (size_t i = 0; i < b; ++i)
    {
        delete[] a[i];
    }
    delete[] a;
}

void petrov::fllincway(char* a, int** mtx, char* o)
{
    std::ifstream in(a);
    size_t rows = 0, cols = 0;
    in >> rows >> cols;
    in.close();
    if (mtx == nullptr || rows == 0 || cols == 0)
    {
        std::ofstream ou(o);
        ou << "0\n";
        ou.close();
        return;
    }
    size_t min_dim = (rows < cols) ? rows : cols;
    size_t count = 0;
    size_t q = 0;
    size_t i = 0, j = min_dim - 1;
    bool isNonZero = true;
    while (q < min_dim - 1)
    {
        while (i < min_dim - 1 && j > 0)
        {
            if (mtx[i][j] == 0)
            {
                isNonZero = false;
            }
            i++;
            j--;
        }
        if (isNonZero) count++;
        q++;
        i = q;
        j = min_dim - q - 1;
        isNonZero = true;
    }
    i = min_dim - 1;
    q = 0;
    j = 0;
    isNonZero = true;
    while (q < min_dim - 1)
    {
        while (j < min_dim - 1 && i > 0)
        {
            if (mtx[i][j] == 0)
            {
                isNonZero = false;
            }
            j++;
            i--;
        }
        if (isNonZero) count++;
        q++;
        i = min_dim - 1 - q;
        j = q;
        isNonZero = true;
    }
    std::ofstream ou(o);
    ou << count << "\n";
    ou.close();
}

void petrov::cntnzrdig(char* a, int** mtx, char* o)
{
    size_t rows = 0, cols = 0;
    std::ifstream in(a);
    in >> rows >> cols;
    in.close();
    if (mtx == nullptr || rows == 0 || cols == 0)
    {
        std::ofstream ou(o, std::ios::app);
        ou.close();
        return;
    }
    size_t min_dim = (rows < cols) ? rows : cols;
    size_t layer = 0;
    while (layer < min_dim / 2)
    {
        for (size_t i = 0; i < min_dim; ++i)
        {
            for (size_t j = 0; j < min_dim; ++j)
            {
                if (i == layer || i == min_dim - layer - 1 || j == layer || j == min_dim - layer - 1)
                {
                    mtx[i][j] = mtx[i][j] + layer + 1;
                }
            }
        }
        layer++;
    }
    std::ofstream ou(o, std::ios::app);
    for (size_t i = 0; i < min_dim; ++i)
    {
        for (size_t j = 0; j < min_dim; ++j)
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
    int array_type = petrov::gettypemass(argv[1]);
    if (array_type != 1 && array_type != 2)
    {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }
    if (array_type == 1)
    {
        try
        {
            size_t rows = 0, cols = 0;
            int** matrix = petrov::makemtx(argv[2], rows, cols);
            petrov::fllincway(argv[2], matrix, argv[3]);
            petrov::cntnzrdig(argv[2], matrix, argv[3]);
            petrov::freemtx(matrix, rows);
            return 0;
        }
        catch(const std::exception& e)
        {
            std::cerr << "error: " << e.what() << "\n";
            return 2;
        }
        catch(...)
        {
            std::cerr << "unknown error\n";
            return 2;
        }
    }
    else
    {
        return 0;
    }
}
