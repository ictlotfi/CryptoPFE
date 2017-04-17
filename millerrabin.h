#ifndef MILLERRABIN_H
#define MILLERRABIN_H
#include <bits/stdc++.h>


using namespace std;

class MillerRabin
{

public:
    MillerRabin();
    int power(int x, unsigned int y, int p);
    bool miillerTest(int d, int n);
    bool isPrime(int n);
private:
    int k;
};

#endif // MILLERRABIN_H
