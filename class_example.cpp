/*一个简单的类实现*/
#include "complex0.h"
int main()
{
    complex c1 = {1.2, 3.4};
    complex c2 = {0, 3};
    cout << c1 << c2;
    complex c3 = c1 * c2;
    cout << "please input a complex:";
    cin >> c3;
    cout << c3;
    c1 = c3 / c2;
    cout << (c1^2);
    return 0;
}
