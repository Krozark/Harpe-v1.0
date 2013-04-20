#include "Serializer.hpp"


Serializer::Serializer(unsigned int buffer_cursor_end) : _cursor_end(0),_cursor_begin(0) _buffer_size(buffer_cursor_end)
{
   _buffer = new unsigned char[buffer_cursor_end];
};

Serializer::~Serializer()
{
    delete _buffer;
    /*_buffer = 0;
    _cursor_end = 0;
    _buffer_size = 0;*/
};


/*typedef union _1oct {
//_1oct() = delete;
//_1oct(const _1oct&) = delete;
uint8_t u;
unsigned char c[1];
};

typedef union _2oct {
uint16_t u;
unsigned char c[2];
inline void swap(){
u= ((u & 0x00FF) << 8)|
((u & 0xFF00) >> 8);
}
};

typedef union _4oct {
uint32_t u;
unsigned char c[4];
inline void swap(){
u = ((u & 0x000000FF) << 8*3) |
((u & 0x0000FF00) << 8  ) |
((u & 0x00FF0000) >> 8  ) |
((u & 0xFF000000) >> 8*3);
};
};

typedef union _8oct {
uint64_t u;
unsigned char c[8];
inline void swap() {
u = ((u & 0x00000000000000FFLL) << 8*7) |
((u & 0x000000000000FF00LL) << 8*5) |
((u & 0x0000000000FF0000LL) << 8*3) |
((u & 0x00000000FF000000LL) << 8  ) |
((u & 0x000000FF00000000LL) >> 8  ) |
((u & 0x0000FF0000000000LL) >> 8*3) |
((u & 0x00FF000000000000LL) >> 8*5) |
((u & 0xFF00000000000000LL) >> 8*7);
}
};*/
