#include <iostream>
using namespace std;

double random(int x)
{
    return ((double)(rand() % x));
}

int main()
{
    cout << random(3) << endl;
    system("pause");
    return 0;
}