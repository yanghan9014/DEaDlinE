#include "bit_counter.h"


bit_counter::bit_counter()
{
    bit = 0;
}

int bit_counter::getbit()
{
    return bit;
}

bit_counter* bit_counter::operator--()
{
    if( bit == 5)bit = 6;
    else if(bit == 6)bit = 2;
    else
    {
        bit += 3;
        if ( bit >= 6 )bit -= 6;
    }

    return this;
}

bit_counter* bit_counter::operator ++()
{
    if( bit == 2)bit = 6;
    else if(bit == 6)bit = 5;
    else
    {
        bit -= 3;
        if ( bit < 0 )bit += 6;
    }
    return this;
}

bit_counter* bit_counter::operator +()
{
    bit += 1;
    if(bit == 3)bit = 0;
    else if (bit == 6)bit = 3;

    return this;
}

bit_counter* bit_counter::operator -()
{
    bit -= 1;
    if(bit == -1)bit = 2;
    else if (bit == 2)bit = 5;

    return this;
}

bit_counter* bit_counter::operator !()
{
    bit -= 1;
    if(bit < 0)bit += 6;

    return this;
}

bit_counter* bit_counter::operator ~()
{
    bit += 1;
    if(bit > 5)bit -= 6;

    return this;
}

void bit_counter::reset()
{
    bit = 0;
}
