/*  Nano Font 3x4 by Michaelangel007 aka MysticReddit
    License: Creative Commons Zero v1.0
        https://creativecommons.org/publicdomain/zero/1.0/   

    A working example of clean code showing font rendering in less then
    700 lines of code including data!  It is the world's smallest 3x4 font
    that pushes the boundry of readability. :-)  Yes, it even includes 
    lower case and the full ASCII character set!

      Uppercase only = 3x3 with 1 pixel leading (spacer)
      with Lowercase = 3x4 !

Compile:
    g++ nanofont3x4.cpp -o nanofont3x4 -Wall -Wextra

Usage:
    nanofont3x4 -u    # To force uppercase
    nanofont3x3

Output:
    Will save output to: output.bmp

Notes:
    This source code is wider then 80 characters. Stop living in the 1970's.

Related work:
    Ken Perlin makes the bogus claim
        "as far as I know this font is the smallest readable screen font."
    * http://blog.kenperlin.com/?p=6804

    There are numerous problems with this claim:
        1. Where is the source cdoe + data to verify this claim??
           This is extremely SLOPPY and unprofessional Science.
        2. His font is 4x6 (including 1 px leading); 
           while it looks great on LCD and takes advantage 
           of color fringes, it is a far cry from the world's smallest font.
           Apparently he is not aware of Anders de Flon 3x3 Font in 2005.
           https://en.wikipedia.org/wiki/3x3
*/

// Includes
    #include <stdio.h>
    #include <stdint.h> // uint8_t uint16_t uint32_t
    #include <string.h>

// Types
    // ======================================================================== 
    struct Image
    {
        int      _width;
        int      _height;
        uint8_t *_pixels;

        int      _size;

        void init()
        {
            _width = _height = _size = 0;
            _pixels = NULL;
        }

        void alloc()
        {
            _size = (_width * _height);
            _pixels = new uint8_t [ _size ];
            memset( _pixels, 0, _size );
        }

        void dump() // DEBUG
        {
            printf( "%d x %d\n", _width, _height );
            for( int y = 0; y < _height; y++ )
            {
                for( int x = 0; x < _width; x++ )
                     printf( "%c", *getOffset( x, y )
                         ? '*'
                         : '.' );
                printf( "\n" );
            }
        }

        Image()
        {
            init();
        }

        Image( const int width, const int height )
        {
            _width  = width;
            _height = height;
            alloc();
        }

        ~Image()
        {
            delete [] _pixels;
        }

        uint8_t* getOffset( int x, int y ) const
        {
            if (x < 0)       return NULL; // x = 0;
            if (y < 0)       return NULL; // y = 0;
            if (x > _width ) return NULL; // x = _width;
            if (y > _height) return NULL; // y = _height;
            return (_pixels + x + (y * _width));
        }
    };


    // ======================================================================== 
    struct Font : public Image
    {
        int _cellWidth;
        int _cellHeight;

        uint8_t _aGlyphWidths[ 256 ];
        uint8_t _aGlyphHeight[ 256 ];

        Font()
        {
            for( int i = 0; i < 256; i++ )
            {
                _aGlyphWidths[ i ] = 0;
                _aGlyphHeight[ i ] = 0;
            }
        }

        int  getGlyphsWidth ( const int glyph ) const { return _aGlyphWidths[ glyph ]; }
        int  getGlyphsHeight( const int glyph ) const { return _aGlyphHeight[ glyph ]; }
        void setGlyphsWidth ( const int glyph, int width  )  { _aGlyphWidths[ glyph ] = width;  }
        void setGlyphsHeight( const int glyph, int height )  { _aGlyphHeight[ glyph ] = height; }

        int  getCellWidth () const { return _cellWidth ; }
        int  getCellHeight() const { return _cellHeight; }
        void setCellWidth ( int width  )  { _cellWidth  = width;  }
        void setCellHeight( int height )  { _cellHeight = height; }

        bool doesWordWrap( int len, const char * text, int i, int cursorX, int rightMarginX, bool forceUppercase );
        void render( Image & texture, int x, int y, const char * text, int length = 0, bool forceUppercase = false, int rightMargin = 0 );
    };

// Implementation _____________________________________________________________

// Save image as Windows .BMP (Bitmap File)
// ======================================================================== 
bool BMP_Write( const char *filename, int bitsPerPixel, unsigned int width, unsigned int height, const uint8_t *pixels, int outBitsPerPixel = 0 )
{
    const uint16_t _16 = 0xFFFF; // 64K-1
    if ((width > _16) || (height > _16)) // Sanity Check: 64K * 64K = 4 GB
        return false;

    if( !width || !height)
        return false;

    // https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376%28v=vs.85%29.aspx
    // https://en.wikipedia.org/wiki/BMP_file_format
    const int BITMAPFILEHEADER_SIZE = 14; // 14 = 0x0E = BITMAPFILEHEADER size
    const int BITMAPINFOHEADER_SIZE = 40; // 40 = 0x28 = BITMAPINFOHEADER size
    const int BITMAP_HEADER_SIZE    = BITMAPFILEHEADER_SIZE + BITMAPINFOHEADER_SIZE;

    if( !outBitsPerPixel )
         outBitsPerPixel = bitsPerPixel;

    const int      sPalette = (outBitsPerPixel <= 8) ? (1 << outBitsPerPixel) * 4 : 0; // sizeof
    const int      nPalette = sPalette >> 2;                                           // array elements

    uint16_t aHeader[ BITMAP_HEADER_SIZE/2 ]; // 54 bytes / 2 = 27 x 16-bit

    const uint32_t nWidthBytes    = (width * outBitsPerPixel) / 8;
    const uint32_t nExtraBytes    = (width * 3              ) % 4;
    const uint32_t nScanLineBytes = nWidthBytes + nExtraBytes;
    const uint32_t nPaddedSize    = nScanLineBytes * height;
    const uint32_t nTexelOffset   = BITMAP_HEADER_SIZE + sPalette              ; // offset to palette start: &aHeader[27] - &aHeader[0]; 
    const uint32_t nTotalSize     = BITMAP_HEADER_SIZE + sPalette + nPaddedSize; // offset to texel start

    aHeader[ 0] = ((int)'B' << 0) | ((int)'M' << 8); // Magic File Type 0x424D "BM"
    aHeader[ 1] = nTotalSize & _16;      // bfSize          Total file size (0x14 + 0x40 + palette + texels)
    aHeader[ 2] = nTotalSize >> 16;      //            
    aHeader[ 3] = 0;                     // bfReserved 
    aHeader[ 4] = 0;                     //            
    aHeader[ 5] = nTexelOffset & _16;    // bfOffbits       Offset of texels
    aHeader[ 6] = nTexelOffset >> 16;    //            

    aHeader[ 7] = BITMAPINFOHEADER_SIZE; // biSize          Size of this header (0x40)
    aHeader[ 8] = 0;                     //
    aHeader[ 9] = width  & _16;          // biWidth
    aHeader[10] = width  >> 16;          //
    aHeader[11] = height & _16;          // biHeight
    aHeader[12] = height >> 16;          //
    aHeader[13] = 1;                     // biPlanes        Number of color planes
    aHeader[14] = outBitsPerPixel;       // biBitCount      Bits per pixel
    aHeader[15] = 0;                     // biCompression   0=uncompressed
    aHeader[16] = 0;                     //
    aHeader[17] = nPaddedSize & _16;     // biSizeImage     Total texel size
    aHeader[18] = nPaddedSize >> 16;     //
    aHeader[19] = 0;                     // biXPelsPerMeter Horz resolution
    aHeader[20] = 0;                     //
    aHeader[21] = 0;                     // biYPelsPerMeter Vert resolution
    aHeader[22] = 0;                     //
    aHeader[23] = nPalette & _16;        // biClrUsed       Colors in palette
    aHeader[24] = nPalette >> 16;        //
    aHeader[25] = nPalette & _16;        // biClrImportant  Important colors
    aHeader[26] = nPalette >> 16;        //

    FILE * file = fopen( filename, "w+b" );
    bool   pass = (file != NULL);
    if( file )
    {
        fwrite( aHeader, sizeof( aHeader ), 1, file ); // assumes Little Endian

        // Write palette for 8-bpp (Defaults to grayscale)
        if( nPalette )
        {
            uint8_t aPalette[ 1024 ]; // 8-bit RGBA[256]

            for( int iPalette = 0; iPalette < nPalette; iPalette++ )
            {
                aPalette[ iPalette*4+0 ] = iPalette; // B
                aPalette[ iPalette*4+1 ] = iPalette; // G
                aPalette[ iPalette*4+2 ] = iPalette; // R
                aPalette[ iPalette*4+3 ] = 0       ; // reserved -- Stupid early Window's BMP format can't use alpha
            }
            fwrite( aPalette, sPalette, 1, file );
        }

        if( bitsPerPixel == outBitsPerPixel )
        { 
            const uint8_t *data = pixels + nWidthBytes*height - nWidthBytes; // Stupid Windows .BMP are upside down
            for( int y = height; y > 0; --y, data -= nWidthBytes )
            {
                fwrite( data, nWidthBytes, 1, file );

                // BMP scan lines must be a multiple of 4 bytes
                for (uint32_t iExtraBytes = 0; iExtraBytes < nExtraBytes; iExtraBytes++ )
                    fprintf( file, "%c", 0x00 );
            }
        }
        else
        if ((bitsPerPixel == 8) && (outBitsPerPixel == 24)) // transcode from 8-bpp to 24-bpp
        {
            uint8_t rgb[ 65536 ]; // 64K/3 = 21,845 max monochrome columns

            const unsigned w2 = width << 1;
            const unsigned w3 = width + w2;
            const uint8_t *data = pixels + (width*(height-1)); // Stupid Windows .BMP are upside down
            for( unsigned int y = 0; y < height; y++ )
            {
                for( unsigned int dst = 0; dst < w3; data++ )
                {
                    rgb[ dst++ ] = *data;
                    rgb[ dst++ ] = *data;
                    rgb[ dst++ ] = *data;
                }
                data -= w2; // move up one scan line
                fwrite( rgb, 3*width, 1, file );

                // See above - BMP lines must be of lengths divisible by 4
                for (uint32_t iExtraBytes = 0; iExtraBytes < nExtraBytes; iExtraBytes++ )
                    fprintf( file, "%c", 0x00 );
            }
        }
		else
			printf( "Unsported BPP: %d != %d\n", bitsPerPixel, outBitsPerPixel );

        fclose( file );
    }
    return pass;
}

// Save RAW Image without any meta-data
// ======================================================================== 
bool Image_Write( const char *filename, int w, int h, const uint8_t *pixels )
{
    FILE * file = fopen( filename, "wb" );
    bool   pass = (file != NULL);
    if( file )
    {
        const uint8_t *data = pixels;
        for( int y = 0; y < h; y++, data += w )
            fwrite( data, w, 1, file ); // ptr, size, count, file

        fclose( file );
    }
    return pass;
}

// This entire font is copyleft since virtual "ownership" of bits (numbers) is retarded.
//
// 3x4 Monochrome Font Glyphs within 4x4 cell
// Each nibble is one row, with the bits stored in reverse order: msb = left column, lsb = right column
// The bit layout for the 4x4 cell format is uppercase A is:
//   abcd    Row0  _X__  0100
//   efgh    Row1  XXX_  1110
//   ijkl    Row2  X_X_  1010
//   mnop    Row3  ____  0000
// There are 64K permutations of a 4x4 monochrome cell.
// ======================================================================== 
uint16_t aGlyphs3x4[] =
{           // Hex    [1] [2] [3] [4] Scanlines
    0x0000, // 0x20
    0x4400, // 0x21 ! 010 010 000
    0xAA00, // 0x22 " 101 101 000
    0xAEE0, // 0x23 # 101 111 111
    0x64C4, // 0x24 $ 011 010 110 010
    0xCE60, // 0x25 % 110 111 011
    0x4C60, // 0x26 & 010 110 011
    0x4000, // 0x27 ' 010 000 000
    0x4840, // 0x28 ( 010 100 010
    0x4240, // 0x29 ) 010 001 010
    0x6600, // 0x2A * 011 011 000
    0x4E40, // 0x2B + 010 111 010
    0x0088, // 0x2C , 000 000 100 100
    0x0E00, // 0x2D - 000 111 000
    0x0080, // 0x2E . 000 000 100
    0x2480, // 0x2F / 001 010 100

    0x4A40, // 0x30 0 010 101 010
    0x8440, // 0x31 1 100 010 010
    0xC460, // 0x32 2 110 010 011
    0xE6E0, // 0x33 3 111 011 111
    0xAE20, // 0x34 4 101 111 001
    0x64C0, // 0x35 5 011 010 110
    0x8EE0, // 0x36 6 100 111 111
    0xE220, // 0x37 7 111 001 001
    0x6EC0, // 0x38 8 011 111 110
    0xEE20, // 0x39 9 111 111 001
    0x4040, // 0x3A : 010 000 010
    0x0448, // 0x3B ; 000 010 010 100 
    0x4840, // 0x3C < 010 100 010
    0xE0E0, // 0x3D = 111 000 111
    0x4240, // 0x3E > 010 001 010
    0x6240, // 0x3F ? 011 001 010

    0xCC20, // 0x40 @ 110 110 001 // 0 = 000 _
    0x4EA0, // 0x41 A 010 111 101 // 2 = 001 _
    0xCEE0, // 0x42 B 110 111 111 // 4 = 010 _
    0x6860, // 0x43 C 011 100 011 // 6 = 011 _
    0xCAC0, // 0x44 D 110 101 110 // 8 = 100 _
    0xECE0, // 0x45 E 111 110 111 // A = 101 _
    0xEC80, // 0x46 F 111 110 100 // C = 110 _
    0xCAE0, // 0x47 G 110 101 111 // E = 111 _
    0xAEA0, // 0x48 H 101 111 101
    0x4440, // 0x49 I 010 010 010
    0x22C0, // 0x4A J 001 001 110
    0xACA0, // 0x4B K 101 110 101
    0x88E0, // 0x4C L 100 100 111
    0xEEA0, // 0x4D M 111 111 101
    0xEAA0, // 0x4E N 111 101 101
    0xEAE0, // 0x4F O 111 101 111

    0xEE80, // 0x50 P 111 111 100
    0xEAC0, // 0x51 Q 111 101 110
    0xCEA0, // 0x52 R 110 111 101
    0x64C0, // 0x53 S 011 010 110
    0xE440, // 0x54 T 111 010 010
    0xAAE0, // 0x55 U 101 101 111
    0xAA40, // 0x56 V 101 101 010
    0xAEE0, // 0x57 W 101 111 111
    0xA4A0, // 0x58 X 101 010 101
    0xA440, // 0x59 Y 101 010 010
    0xE4E0, // 0x5A Z 111 010 111
    0xC8C0, // 0x5B [ 110 100 110
    0x8420, // 0x5C \ 100 010 001
    0x6260, // 0x5D ] 011 001 011
    0x4A00, // 0x5E ^ 010 101 000
    0x00E0, // 0x5F _ 000 000 111

    0x8400, // 0x60 ` 100 010 000
    0x04C0, // 0x61 a 000 010 110
    0x8CC0, // 0x62 b 100 110 110
    0x0CC0, // 0x63 c 000 110 110
    0x4CC0, // 0x64 d 010 110 110
    0x08C0, // 0x65 e 000 100 110
    0x2440, // 0x66 f 001 010 010
    0x0CCC, // 0x67 g 000 110 110 110
    0x8CC0, // 0x68 h 100 110 110
    0x0440, // 0x69 i 000 010 010
    0x0448, // 0x6A j 000 010 010 100
    0x8CA0, // 0x6B k 100 110 101
    0x4420, // 0x6C l 010 010 001
    0x0CE0, // 0x6D m 000 110 111
    0x0CA0, // 0x6E n 000 110 101
    0x0CC0, // 0x6F o 000 110 110

    0x0CC8, // 0x70 p 000 110 110 100
    0x0CC4, // 0x71 q 000 110 110 010
    0x0C80, // 0x72 r 000 110 100
    0x0480, // 0x73 s 000 010 100
    0x4C60, // 0x74 t 010 110 011
    0x0AE0, // 0x75 u 000 101 111
    0x0A40, // 0x76 v 000 101 010
    0x0E60, // 0x77 w 000 111 011
    0x0CC0, // 0x78 x 000 110 110
    0x0AE2, // 0x79 y 000 101 111 001
    0x0840, // 0x7A z 000 100 010
    0x6C60, // 0x7B { 011 110 011
    0x4444, // 0x7C | 010 010 010 010
    0xC6C0, // 0x7D } 110 011 110
    0x6C00, // 0x7E ~ 011 110 000
    0xA4A4  // 0x7F   101 010 101 010 // Alternative: Could even have a "full" 4x4 checkerboard
};

// Swap Nibbles and BitReverse
// abcd -> DBCA
// ======================================================================== 
uint16_t SwapNibblesBitReverse( uint16_t x )
{
    const uint8_t nibble[ 16 ] =
    {
        0x0, // 0  0000 -> 0000
        0x8, // 1  0001 -> 1000
        0x4, // 2  0010 -> 0100
        0xC, // 3  0011 -> 1100
        0x2, // 4  0100 -> 0010
        0xA, // 5  0101 -> 1010
        0x6, // 6  0110 -> 0110
        0xE, // 7  0111 -> 1110

        0x1, // 8  1000 -> 0001
        0x9, // 9  1001 -> 1001
        0x5, // A  1010 -> 0101
        0xD, // B  1011 -> 1101
        0x3, // C  1100 -> 0011
        0xB, // D  1101 -> 1011
        0x7, // E  1110 -> 0111
        0xF  // F  1111 -> 1111
    };
    return  (nibble[ (x >> 12) & 0xF ] <<  0)
    |       (nibble[ (x >>  8) & 0xF ] <<  4)
    |       (nibble[ (x >>  4) & 0xF ] <<  8)
    |       (nibble[ (x >>  0) & 0xF ] << 12); 
}

// Expand 1-bit to 8-bit font texture atlas
// ======================================================================== 
void MonochromeFont2Bitmap( const int CellWidth, const int CellHeight, uint16_t *bitmap, Font & font )
{
    font.setCellWidth ( CellWidth  );
    font.setCellHeight( CellHeight );

    font._width  = 16 * CellWidth; // 16 glyphs/row, each cell = 4 pixels wide
    font._height =  8 * CellHeight; // 8 rows, each cell = 4 pixels tall
    font.alloc();

    int first = 0x20; // start at space
    int last  = 0x80; // end at 7-bit ASCII

    for( int iGlyph = first; iGlyph < last; iGlyph++ )
    {
        int      x      = (iGlyph % 16) * CellWidth;  // cellX
        int      y      = (iGlyph / 16) * CellHeight; // cellY
        uint8_t *pDst   = font.getOffset( x, y );
        uint16_t pixels = SwapNibblesBitReverse( bitmap[ iGlyph - 0x20 ] );

        int minWidth  = 0;
        int minHeight = 0;
        if (iGlyph == ' ') // 0x20 SPACE
        {
            minWidth  = CellWidth -1;
            minHeight = CellHeight-1;
        }

        for( int gY = 0; gY < CellHeight; gY++ )
        {
            if( pixels )
                minHeight = gY + 1;

            for( int gX = 0; gX < CellWidth; gX++ )
            {
                *pDst++ = ~((pixels & 1) - 1); // Optimization: *pDst++ = (pixels & 1) ? 0xFF : 0x00;

                if (pixels & 1)
                {
                    if (minWidth <= gX)
                        minWidth  = gX + 1;
                }
                pixels >>= 1;
            }

            pDst -= CellWidth;
            pDst += font._width;
        }

        font.setGlyphsWidth ( iGlyph, minWidth  );
        font.setGlyphsHeight( iGlyph, minHeight );
    }
#if DEBUG
    font.dump(); // DEBUG
#endif
}

// g  Glyph
// s  Source
// d  Destination
//  x Column
//  y Row
//  w Width
//  h Height
// ======================================================================== 
void Glyph_Copy( int gw, int gh, const uint8_t * srcPixels, int sw, int sh, int sx, int sy, uint8_t * dstPixels, int dw, int dh, int dx, int dy )
{
    (void)dh; // Technically not used but kept to be orthogonal / consistent
    (void)sh; // Technically not used but kept to be orthogonal / consistent
    const uint8_t * pSrc = srcPixels + sx + sy*sw;
    /* */ uint8_t * pDst = dstPixels + dx + dy*dw;

    for( int y = 0; y < gh; y++ )
    {
        for( int x = 0; x < gw; x++ )
            *pDst++ = *pSrc++;

        pSrc -= gw;
        pSrc += sw;

        pDst -= gw;
        pDst += dw;
    }
}

// Find index of longest word that will fit on this line
// ======================================================================== 
bool Font::doesWordWrap( int len, const char * text, int i, int cursorX, int rightMarginX, bool forceUppercase )
{
    int  wrap = cursorX;
    char c;

    for( int j = i; j < len; j++ )
    {
        c = text[j];
        if (forceUppercase && (c >= 'a')  && (c <= 'z'))
            c -= 'a' - 'A';

        if (c == ' ')
            return (wrap > rightMarginX);

        wrap += getGlyphsWidth( c ) +  1;
    }
    return false;
}

// ======================================================================== 
void Font::render( Image & texture, int x, int y, const char * text, int length /* = 0*/, bool forceUppercase /*= false*/, int rightMargin /*= 0*/ )
{
    const int LeftMarginX = x;
    const int n = length ? length : strlen( text );
    /* */ uint8_t c;
    const uint8_t *p = (const uint8_t*) text;

    int sx, sy; // SourceCell
    const uint8_t * srcBitmap = _pixels;
    const int       srcWidth  = _width ;
    const int       srcHeight = _height;

    uint8_t * dstBitmap   = texture._pixels;
    const int dstWidth    = texture._width ;
    const int dstHeight   = texture._height;
    const int RightMargin = rightMargin ? rightMargin : dstWidth;

    int gw = 0; // current glyph width  = getGlyphWidths ( c );
    int gh = 0; // current glyph height = getGlyphHeights( c );
    int cw = getCellWidth ();
    int ch = getCellHeight();

    for( int i = 0; i < n; i++ )
    {
        c = *p++;
        if (forceUppercase && (c >= 'a') && (c <= 'z'))
            c -= 'a' - 'A';

        sx = (c & 0xF) * cw; // most modern GPUs support at least 2Kx2K = max glyph width or height = 2048/16 = 128 pixels/glyph cell
        sy = (c >> 4 ) * ch;

        gw = getGlyphsWidth( c );
        gh = getGlyphsHeight( c );

        if (c == ' ')
            if( doesWordWrap( n, text, i+1, x + gw + 1, RightMargin, forceUppercase ) ) // Scan ahead to see if word will fit on line
                c = '\n'; // force hard wrap

        if ((c == '\n') || ((x + gw) >= RightMargin)) // (x+cw)
        {
            x = LeftMarginX;
            y += ch;
            if (c == '\n') 
                 continue;
        }

        if ((y + gh) >= dstHeight) //printf( "Warn: Texture out-of-bounds\n" );
            break;

        Glyph_Copy( gw, gh, srcBitmap, srcWidth, srcHeight, sx, sy, dstBitmap, dstWidth, dstHeight, x, y );

        x += gw + 1;
    }
}

// ======================================================================== 
int main( const int nArg, const char *aArg[] )
{
    Font  Font3x4;
    Image Output( 320, 240 );
    MonochromeFont2Bitmap( 4, 4, aGlyphs3x4, Font3x4 );

    const int  BUFFER_SIZE   = 64 * 1024;
    char text[ BUFFER_SIZE ] = "\
When in the Course of human events it becomes necessary for one people to dissolve the \
political bands which have connected them with another and to assume among the powers of \
the earth, the separate and equal station to which the Laws of Nature and of Nature's God \
entitle them, a decent respect to the opinions of mankind requires that they should declare the \
causes which impel them to the separation. We hold these truths to be self-evident, that all men \
are created equal, that they are endowed by their Creator with certain unalienable Rights, that \
among these are Life, Liberty and the pursuit of Happiness.  That to secure these rights, \
Governments are instituted among Men, deriving their just powers from the consent of the \
governed,   That whenever any Form of Government becomes destructive of these ends, it is the \
Right of the People to alter or to abolish it, and to institute new Government, laying its \
foundation on such principles and organizing its powers in such form, as to them shall seem \
most likely to effect their Safety and Happiness. Prudence, indeed, will dictate that \
Governments long established should not be changed for light and transient causes; and \
accordingly all experience hath shewn, that mankind are more disposed to suffer, while evils \
are sufferable, than to right themselves by abolishing the forms to which they are \
accustomed. But when a long train of abuses and usurpations, pursuing invariably the same \
Object evinces a design to reduce them under absolute Despotism, it is their right, it is their \
duty, to throw off such Government, and to provide new Guards for their future security.  Such \
has been the patient sufferance of these Colonies; and such is now the necessity which \
constrains them to alter their former Systems of Government. The history of the present King \
of Great Britain is a history of repeated injuries and usurpations, all having in direct object \
the establishment of an absolute Tyranny over these States. To prove this, let Facts be \
submitted to a candid world. He has refused his Assent to Laws, the most wholesome and \
necessary for the public good. He has forbidden his Governors to pass Laws of immediate and \
pressing importance, unless suspended in their operation till his Assent should be obtained; \
and when so suspended, he has utterly neglected to attend to them. He has refused to pass other \
Laws for the accommodation of large districts of people, unless those people would relinquish \
the right of Representation in the Legislature, a right inestimable to them and formidable to \
tyrants only. He has called together legislative bodies at places unusual, uncomfortable, and \
distant from the depository of their public Records, for the sole purpose of fatiguing them \
into compliance with his measures. He has dissolved Representative Houses repeatedly, for \
opposing with manly firmness his invasions on the rights of the people. He has refused for a \
long time, after such dissolutions, to cause others to be elected; whereby the Legislative \
powers, incapable of Annihilation, have returned to the People at large for their exercise;";

    bool bUpperCase = false;
    if (nArg > 1) 
    {
        int iArg = 1;
        if ((aArg[1][0] == '-') && (aArg[1][1] == 'u'))
        {
            bUpperCase = true;
            iArg = 2;
        }

        if (nArg > iArg)
        {
            const char *filename = aArg[ iArg ];
            printf( "Reading: %s\n", filename );
            FILE *file = fopen( filename, "rb" );
            if( file )
            {
                fseek( file, 0, SEEK_END );
                long filesize = ftell( file );
                fseek( file, 0, SEEK_SET );

                if (filesize > BUFFER_SIZE)
                    filesize = BUFFER_SIZE;

                fread( text, filesize, 1, file );
                text[ filesize ] = 0;
                fclose( file );
            }
        }
    }

    const char *output_filename = bUpperCase
        ? "output_upper3x4.bmp"
        : "output_lower3x4.bmp";


    Font3x4.render( Output, 2, 2, text, 0, bUpperCase );
    bool saved = BMP_Write( output_filename, 8, Output._width, Output._height, Output.getOffset(0,0), 24 );
    const char *status = saved 
        ?  "Saved: "
        :  "ERROR. Unable to write: ";
    printf( "%s%s\n", status, output_filename );

#if 1 // DEBUG Save Texture Atlas
    BMP_Write( "nanofont3x4.bmp", 8, Font3x4._width, Font3x4._height, Font3x4.getOffset(0,0), 8 );
#endif

    return 0;
}
