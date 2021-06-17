#ifndef BIT_COUNTER_H
#define BIT_COUNTER_H


class bit_counter
{
    public:

        bit_counter();

        void reset();

        int getbit();

        //use for scharactor and smap
        bit_counter* operator ++(); //up
        bit_counter* operator --(); //down
        bit_counter* operator +();  //right
        bit_counter* operator -();  //left

        //use for smain
        bit_counter* operator !(); //up
        bit_counter* operator ~(); //down

    private:

        int bit ;
};

#endif // BIT_COUNTER_H
