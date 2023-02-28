#include "hash.h"

// String folding function: https://tarjotin.cs.aalto.fi/cs-a1141/OpenDSA/Books/CS-A1141/html/HashFuncExamp.html
int hash(char *str, int nbuckets)
{
    long sum = 0, mult = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        mult = (i % 4 == 0) ? 1 : mult * 256;
        sum += str[i] * mult;
    }
    return (int)(abs(sum % nbuckets));
}