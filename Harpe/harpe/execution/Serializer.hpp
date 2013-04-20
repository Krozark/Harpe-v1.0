#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <string.h>
#include <stdint.h>

/*
 * Se base sur .. pour les fonction en interne
__BYTE_ORDER__
__ORDER_LITTLE_ENDIAN__
__ORDER_BIG_ENDIAN__
__ORDER_PDP_ENDIAN__
*/

#if __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#error "byte orden not suported (PDP endian)"
#endif

/* Convert local data in BIG ENDIAN for network, and from big endian to local  */
class Serializer
{
    public:
        explicit Serializer(unsigned int buffer_cursor_end=255);
        ~Serializer();
        
        Serializer(Serializer&) = delete;
        Serializer& operator=(const Serializer& other) = delete;


        inline const unsigned char* const buffer(){return _buffer;};
        inline const unsigned int& size(){return _cursor_end;};

        /********* SERIALIZE (empile) *************/
        //1 oct | 8 bit
        inline Serializer& operator<<(char c){push(*reinterpret_cast<uint8_t*>(&c));return *this;};         
        //2 oct | 16 bit
        inline Serializer& operator<<(short int s){push(*reinterpret_cast<uint16_t*>(&s));return *this;};
        //4 oct | 32 bit
        inline Serializer& operator<<(int i){push(*reinterpret_cast<uint32_t*>(&i));return *this;};
        inline Serializer& operator<<(float f){push(*reinterpret_cast<uint32_t*>(&f));return *this;};
        //8 oct | 64 bit
        inline Serializer& operator<<(double d){push(*reinterpret_cast<uint64_t*>(&d));return *this;};
        inline Serializer& operator<<(long int l){push(*reinterpret_cast<uint64_t*>(&l));return *this;};
        //16 oct | 124 bit
        //inline Serializer& operator<<(long double ld){push(*reinterpret_cast<uint128_t*>(&ld);};
        //1 oct | 8 bit []
        inline Serializer& operator<<(const char* c){
            uint8_t* data = (uint8_t*)c;
            for(int i=0;c[i];++i) //exit when \0
                push(data[i]);
            return *this;
        };

        /********** UNSERIALIZE (depile) ***********/
        //1 oct | 8 bit
        inline Serializer& operator>>(char& c){pop_back(*reinterpret_cast<uint8_t*>(&c));return *this;};         
        //2 oct | 16 bit
        inline Serializer& operator>>(short int s){pop_back(*reinterpret_cast<uint16_t*>(&s));return *this;};
        //4 oct | 32 bit
        inline Serializer& operator>>(int& i){pop_back(*reinterpret_cast<uint32_t*>(&i));return *this;};
        inline Serializer& operator>>(float& f){pop_back(*reinterpret_cast<uint32_t*>(&f));return *this;};
        //8 oct | 64 bit
        inline Serializer& operator>>(double& d){pop_back(*reinterpret_cast<uint64_t*>(&d));return *this;};
        inline Serializer& operator>>(long int& l){pop_back(*reinterpret_cast<uint64_t*>(&l));return *this;};


    private:

        unsigned char* _buffer;
        unsigned int _cursor_end;
        unsigned int _cursor_begin;
        unsigned int _buffer_size;

        inline void resize(const unsigned int buffer_cursor_end)
        {
            unsigned char* buffer = new unsigned char[buffer_cursor_end];
            buffer = (unsigned char*)memcpy(buffer,_buffer,_cursor_end);

            delete _buffer;
            _buffer = buffer;
            _buffer_size = buffer_cursor_end;
        };


        /************ ADD DATAs ***************/

        inline void push(uint8_t& a){
            if(_buffer_size < _cursor_end + 1)
                resize(_buffer_size+128);

            _buffer[_cursor_end++] = a;
        };

        inline void push(uint16_t& a) {
            if(_buffer_size < _cursor_end + 2)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            _buffer[_cursor_end++] = d[0];
            _buffer[_cursor_end++] = d[1];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            _buffer[_cursor_end++] = d[1];
            _buffer[_cursor_end++] = d[0];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif

        }

        inline void push(uint32_t& a){
            if(_buffer_size < _cursor_end + 4)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            _buffer[_cursor_end++] = d[0];
            _buffer[_cursor_end++] = d[1];
            _buffer[_cursor_end++] = d[2];
            _buffer[_cursor_end++] = d[3];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            _buffer[_cursor_end++] = d[3];
            _buffer[_cursor_end++] = d[2];
            _buffer[_cursor_end++] = d[1];
            _buffer[_cursor_end++] = d[0];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif

        }

        inline void push(uint64_t& a){
            if(_buffer_size < _cursor_end + 8)
                resize(_buffer_size+128);

            uint8_t *d = (uint8_t *)&a;

            #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
            _buffer[_cursor_end++] = d[0];
            _buffer[_cursor_end++] = d[1];
            _buffer[_cursor_end++] = d[2];
            _buffer[_cursor_end++] = d[3];
            _buffer[_cursor_end++] = d[4];
            _buffer[_cursor_end++] = d[5];
            _buffer[_cursor_end++] = d[6];
            _buffer[_cursor_end++] = d[7];
            #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            _buffer[_cursor_end++] = d[7];
            _buffer[_cursor_end++] = d[6];
            _buffer[_cursor_end++] = d[5];
            _buffer[_cursor_end++] = d[4];
            _buffer[_cursor_end++] = d[3];
            _buffer[_cursor_end++] = d[2];
            _buffer[_cursor_end++] = d[1];
            _buffer[_cursor_end++] = d[0];
            #else
            #error "byte orden not suported (PDP endian)"
            #endif
        }

        /***************** GET DATAs ****************/
        inline void pop_back(uint8_t& a){
            if(_cursor_end > 1)
            {
                a= _buffer[--_cursor_end];
            }
        };

        inline void pop_back(uint16_t& a){
            if(_cursor_end > 2)
            {
                uint8_t *d = (uint8_t *)&a;
                #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                d[1]= _buffer[--_cursor_end];
                d[0]= _buffer[--_cursor_end];
                #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                d[0]= _buffer[--_cursor_end];
                d[1]= _buffer[--_cursor_end];
                #else
                #error "byte orden not suported (PDP endian)"
                #endif
            }
        };

        inline void pop_back(uint32_t& a){
            if(_cursor_end > 4)
            {
                uint8_t *d = (uint8_t *)&a;
                #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                d[3]= _buffer[--_cursor_end];
                d[2]= _buffer[--_cursor_end];
                d[1]= _buffer[--_cursor_end];
                d[0]= _buffer[--_cursor_end];
                #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                d[0]= _buffer[--_cursor_end];
                d[1]= _buffer[--_cursor_end];
                d[2]= _buffer[--_cursor_end];
                d[3]= _buffer[--_cursor_end];
                #else
                #error "byte orden not suported (PDP endian)"
                #endif
            }
        };

        inline void pop_back(uint64_t& a){
            if(_cursor_end > 8)
            {
                uint8_t *d = (uint8_t *)&a;
                #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                d[7]= _buffer[--_cursor_end];
                d[6]= _buffer[--_cursor_end];
                d[5]= _buffer[--_cursor_end];
                d[4]= _buffer[--_cursor_end];
                d[3]= _buffer[--_cursor_end];
                d[2]= _buffer[--_cursor_end];
                d[1]= _buffer[--_cursor_end];
                d[0]= _buffer[--_cursor_end];
                #elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                d[0]= _buffer[--_cursor_end];
                d[1]= _buffer[--_cursor_end];
                d[2]= _buffer[--_cursor_end];
                d[3]= _buffer[--_cursor_end];
                d[4]= _buffer[--_cursor_end];
                d[5]= _buffer[--_cursor_end];
                d[6]= _buffer[--_cursor_end];
                d[7]= _buffer[--_cursor_end];
                #else
                #error "byte orden not suported (PDP endian)"
                #endif
            }
        };
};

#endif
