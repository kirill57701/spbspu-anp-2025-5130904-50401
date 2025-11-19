#include <iostream>
#include <iomanip>
#include <cctype>

namespace petrov
{
    size_t getline(std::istream & in, char* s, size_t q);
    char* resize_length(char* s, size_t q1, size_t q2);
    char* RPL_SYM(std::istream& in, char* s, size_t q);
    size_t LAT_TWO(std::istream& in, char* var_2, char* var_2_help, size_t size, char* s, size_t d);
    char* sorting_a(char* a, size_t b);
    bool is_element_in_mass(char* a, size_t d, char b);
    char* resize_massive(char* a, size_t d1, size_t d2);
}

char* petrov::sorting_a(char* a, size_t b)
{
    for (size_t i = 0; i < b; ++i)
    {
        for (size_t j = 0; j < b - 1; ++j)
        {
            if (a[j] > a[j + 1])
            {
                size_t swapa = a[j];
                a[j] = a[j + 1];
                a[j + 1] = swapa;
            }
        }
    }
    return a;
}

char* petrov::resize_massive(char* a, size_t d1, size_t d2)
{
    char* new_a = new char[d2];
    for (size_t i = 0; i < d2; ++i)
    {
        new_a[i] = a[i];
    }
    delete[] a;
    return new_a;
}

bool petrov::is_element_in_mass(char* a, size_t d, char b)
{
    for (size_t i = 0; i < d; ++i)
    {
        if (a[i] == b)
        {
            return 1;
        }
    }
    return 0;
}

char* petrov::resize_length(char* s, size_t q1, size_t q2)
{
    char* new_s = new char[q2];
    for (size_t i = 0; i < q2; ++i)
    {
        new_s[i] = s[i];
    }
    delete[] s;
    return new_s;
}

size_t petrov::getline(std::istream& in, char* s, size_t q)
{
    size_t i = 0;
    for (; i < q; ++i)
    {
        in >> s[i];
        if (in.fail())
        {
            throw std::logic_error("err");
            delete[] s;
        }
        if (s[i] == '\0' && i != q - 1)
        {
            s = petrov::resize_length(s, q, i + 2);
            break;
        }
    }
    return i + 2;
}

char* petrov::RPL_SYM(std::istream& in, char* s, size_t q)
{
    std::cin >> std::skipws;
    char a, b;
    in >> a >> b;
    if (in.fail())
    {
        throw std::logic_error("err");
        delete[] s;
        return nullptr;
    }
    else
    {
        char* new_s = new char[q];
        for (size_t i = 0; i < q; ++i)
        {
            if (s[i] == a)
            {
                new_s[i] = b;
            }
            else
            {
                new_s[i] = s[i];
            }
        }
        return new_s;
    }
}

size_t petrov::LAT_TWO(std::istream& in, char* var_2, char* var_2_help, size_t size, char* s, size_t d)
{
    std::cin >> std::skipws;
    size_t q = size;
    try
    {
        q = petrov::getline(std::cin, var_2_help, size);
    }
    catch (...)
    {
        delete[] var_2;
        delete[] var_2_help;
        delete[] s;
        throw "err\n";
    }
    size_t j = 0;
    for (size_t i = 0; i < d; ++i)
    {
        if (!petrov::is_element_in_mass(var_2, 27, s[i]) && s[i] != ' ')
        {
            var_2[j] = s[i];
            j++;
        }
    }
    for (size_t i = 0; i < q; ++i)
    {
        if (!petrov::is_element_in_mass(var_2, 27, var_2_help[i]) && var_2_help[i] != ' ')
        {
            var_2[j] = var_2_help[i];
            j++;
        }
    }
    var_2 = petrov::resize_massive(var_2, 27, j + 1);
    var_2 = petrov::sorting_a(var_2, j + 1);
    return j + 1;
}

int main()
{
    const size_t size = 10000;
    char* s = new char[size];
    size_t d = 0;
    try
    {
        std::cin >> std::noskipws;
        d = petrov::getline(std::cin, s, size);
    }
    catch (...)
    {
        std::cerr << "err\n";
        delete[] s;
        return 1;
    }
    char* var_1 = new char[d];
    try
    {
        var_1 = petrov::RPL_SYM(std::cin, s, d);
    }
    catch (...)
    {
        std::cerr << "err\n";
        delete[] var_1;
        delete[] s;
        return 1;
    }
    char* var_2_help = new char[size];
    char* var_2 = new char[27];
    size_t size_of_var_2 = 0;
    try
    {
        size_of_var_2 = petrov::LAT_TWO(std::cin, var_2, var_2_help, size, s, d);
    }
    catch (...)
    {
        delete[] var_2;
        delete[] var_2_help;
        delete[] s;
        std::cerr << "err\n";
        return 1;
    }
    std::cout << var_1 << "\n" << var_2 << "\n";
    delete[] var_1;
    delete[] var_2;
    delete[] var_2_help;
    delete[] s;
    return 0;
}
