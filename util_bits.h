    const uint8_t aReverseNibble[ 16 ] =
    {
         0x0 // 0 0000 -> 0000
        ,0x8 // 1 0001 -> 1000
        ,0x4 // 2 0010 -> 0100
        ,0xC // 3 0011 -> 1100
        ,0x2 // 4 0100 -> 0010
        ,0XA // 5 0101 -> 1010
        ,0x6 // 6 0110 -> 0110
        ,0xE // 7 0111 -> 1110
        ,0x1 // 8 1000 -> 0001
        ,0x9 // 9 1001 -> 1001
        ,0x5 // A 1010 -> 0101
        ,0xD // B 1011 -> 1101
        ,0x3 // C 1100 -> 0011
        ,0xB // D 1101 -> 1011
        ,0x7 // E 1110 -> 0111
        ,0xF // F 1111 -> 1111
    };

// ======================================================================== 
inline
uint8_t ReverseBits4( uint8_t n )
{
    return aReverseNibble[ n & 0xF ];
}

// Example:
// 1    2    -> 4    8
// 0001_0010 -> 0100_1000
// ======================================================================== 
inline
uint8_t ReverseBits8( uint8_t n )
{
    return (aReverseNibble[ (n >> 0) & 0xF ] << 4)
    |      (aReverseNibble[ (n >> 4) & 0xF ] << 0);
}

// ======================================================================== 
inline
uint16_t ReverseBits16( uint16_t n )
{
    return (ReverseBits8( n >> 0 ) << 8)
    |      (ReverseBits8( n >> 8 ) << 0);
}

inline
uint16_t ReverseNibbles( uint16_t n )
{
    return 0
        | (aReverseNibble[(glyph >> 12) & 0xF] <<  0)
        | (aReverseNibble[(glyph >>  8) & 0xF] <<  4)
        | (aReverseNibble[(glyph >>  4) & 0xF] <<  8)
        | (aReverseNibble[(glyph >>  0) & 0xF] << 12);
}

