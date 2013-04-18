#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

class Serializer
{
    public:
        Serializer();
        ~Serializer();
        
        Serializer(Serializer&) = delete;
        Serializer& operator=(const Serializer& other) = delete;


        inline const char const* buffer(){return _buffer;};
        inline const unsigned int& size(){return size;};

        /********* SERIALIZE *************/
        Serializer& operator<<(char c);       //1
        Serializer& operator<<(short s);      //2
        Serializer& operator<<(int i);        //4
        Serializer& operator<<(float f);      //4
        Serializer& operator<<(double d);     //8
        Serializer& operator<<(long l);       //8
        Serializer& operator<<(long double ld); //16

        /********** UNSERIALIZE ***********/
        Serializer& operator>>(int i);



    private:
        char* _buffer;
        unsigned int _size;
};

#endif
