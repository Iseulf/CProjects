/*这是一个复数类的实现*/
#include <iostream>
#include <iomanip>
using namespace std;
class complex
{
private:
    double _real;
    double _imga;

public:
    complex(double real, double imga);
    complex();
    ~complex();
    complex operator+(const complex &foobar) const;
    complex operator-(const complex &foobar) const;
    complex operator*(const complex &foobar) const;
    complex operator/(const complex &foobar) const;
    complex operator^(const int x) const;
    complex operator~() const;
    friend complex operator*(double x, const complex &foobar);
    friend ostream &operator<<(ostream &os, const complex &foobar);
    friend istream &operator>>(istream &in, complex &foobar);
};

complex::complex(double real, double imga)
{
    _real = real;
    _imga = imga;
}

complex::complex(){
    _real=0;
    _imga=0;
}

complex::~complex()
{
}
complex complex::operator+(const complex &foobar) const
{
    return complex(_real + foobar._real, _imga + foobar._imga);
}
complex complex::operator-(const complex &foobar) const
{
    return complex(_real - foobar._real, _imga - foobar._imga);
}
complex complex::operator~() const
{
    return complex(_real, -_imga);
}
complex operator*(double x, const complex &foobar)
{
    return complex(x * foobar._real, x * foobar._imga);
}
complex complex::operator*(const complex &foobar) const
{
    return complex(_real * foobar._real - _imga * foobar._imga, _real * foobar._imga + foobar._real * _imga);
}
complex complex::operator/(const complex &foobar) const
{
    double square = foobar._real * foobar._real + foobar._imga * foobar._imga;
    complex c = (*this) * (~foobar);
    return complex(c._real / square, c._imga / square);
}
complex complex::operator^(const int x) const
{
    complex temp = (*this);
    for (int i = 1; i <= x; i++)
        temp = temp * (*this);
    return temp;
}
ostream &operator<<(ostream &os, const complex &foobar)
{
    cout << setiosflags(ios::fixed);
    if (foobar._imga > 0)
        os << setprecision(2) << foobar._real
           << "+" << foobar._imga << "i" << endl;
    else
        os << foobar._real << foobar._imga << "i" << endl;
    return os;
}
istream &operator>>(istream &in, complex &foobar)
{
    in >> foobar._real >> foobar._imga;
    return in;
}