/*
    Uber 4x4 Texture Atlas by Michaelangel007 aka MysticReddit
    License: Creative Commons Zero v1.0
        https://creativecommons.org/publicdomain/zero/1.0/   

    Draw all possible 4x4 monochrome bitmap permutations into an
    uber texture atlas. There 65,536 different glyphs.
    We organize them into a 256x256 sprite sheet for convenience.
*/

// Includes
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>

// Macros
#ifndef     BORDER
    #define BORDER 1
#else
    #define BORDER 0
#endif

#if BORDER
    #define AXISX  9 // indent left edge = 2*CharacterCellWidth
    #define AXISY 10 
    // 8 pixels on left  edge '##' for all 256 Y values
    // 8 pixels on top   edge '00 01 02 03 .. FF'
    // TODO: 8 pixels on botom edge '00 01 02 03 .. FF'
#else
    #define AXISX 0
    #define AXISY 0
#endif

const uint16_t aGlyphs3x4[] =
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

    0xCC20, // 0x40 @ 110 110 001 
    0x4EA0, // 0x41 A 010 111 101 
    0xCEE0, // 0x42 B 110 111 111
    0x6860, // 0x43 C 011 100 011
    0xCAC0, // 0x44 D 110 101 110
    0xECE0, // 0x45 E 111 110 111
    0xEC80, // 0x46 F 111 110 100
    0xCAE0, // 0x47 G 110 101 111
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


// Consts
    // Enumerate all 65536 permutations of 4x4 monochrome bitmaps.
    // For convenience we can construct this as a master texture atlas
    // with 256 x 256 glyphs
    const int    CELL = (4+BORDER);
    const int    DIMX = 256*(4+BORDER)+AXISX; // left y-axis ##
    const int    DIMY = 256*(4+BORDER)+AXISY; // top x-axis 
    const size_t AREA = DIMX*DIMY;

    bool gbLabelAxis = AXISX | AXISY;

// Implementation _____________________________________________________________

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

// ======================================================================== 
void DrawGlyph4x4( uint8_t *texels, uint16_t x, uint16_t y, uint16_t glyph, uint8_t brightness = 0xFF )
{
    // Given nibbles PQRT they will be drawn in the wrong order: T'R'Q'P'
    // and with the bits reversed.
    // e.g.
    //   0x1234
    //
    // Is drawn as:
    //
    //     0010
    //     1100
    //     0100
    //     1000
    //
    // But we want it to be drawn as:
    //     0001
    //     0010
    //     0011
    //     0100
    //
    // Therefore we need to not only shuffle the nibbles to draw them top-to-bottom
    // but we also need to reverse the nibbles
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

    uint16_t bits = 0
        | (aReverseNibble[(glyph >> 12) & 0xF] <<  0)
        | (aReverseNibble[(glyph >>  8) & 0xF] <<  4)
        | (aReverseNibble[(glyph >>  4) & 0xF] <<  8)
        | (aReverseNibble[(glyph >>  0) & 0xF] << 12);

//printf( "--- %04X ---\n", glyph ); // DEBUG

    // Linearize [y+v][x+u]
    uint8_t *p = texels + (y*DIMX) + x;

    // Draw 4x4 into master texture atlas
    for( int v = 0; v < 4; v++ )
    {
        for( int u = 0; u < 4; u++ )
        {
            if( bits & 1 )
                p[u] = brightness;
            else
                p[u] = 0x00;
//putchar( ".X"[bits&1] ); // DEBUG

            bits >>= 1; 
        } 
//putchar( '\n' ); // DEBUG

        p += DIMX;
    }
}


// ======================================================================== 
void RAW_SaveGreyscale8bit( const char *filename, const uint8_t *texels, const int width, const int height )
{
    FILE *pFile = fopen( filename, "w+b" );
    if( pFile )
    {
        const size_t area = width * height;
        fwrite( texels, area, 1, pFile );
        fclose( pFile );
    }
}

// ======================================================================== 
void LabelAxis( uint8_t* texels )
{
    const uint16_t aNumberGlyphs[ 16 ] = 
    {
// 3x3
#if 0
         0x4A40 // 0x30 0 010 101 010
        ,0x8440 // 0x31 1 100 010 010
        ,0xC460 // 0x32 2 110 010 011
        ,0xE6E0 // 0x33 3 111 011 111
        ,0xAE20 // 0x34 4 101 111 001
        ,0x64C0 // 0x35 5 011 010 110
        ,0x8EE0 // 0x36 6 100 111 111
        ,0xE220 // 0x37 7 111 001 001
        ,0x6EC0 // 0x38 8 011 111 110
        ,0xEE20 // 0x39 9 111 111 001
        ,0x4EA0 // 0x41 A 010 111 101
        ,0xCEE0 // 0x42 B 110 111 111
        ,0x6860 // 0x43 C 011 100 011
        ,0xCAC0 // 0x44 D 110 101 110
        ,0xECE0 // 0x45 E 111 110 111
        ,0xEC80 // 0x46 F 111 110 100

        // NOTE: There is an alternate with all the pixels
        // shifted over left 1 pixel; just double all the nibbles
        // There also variations in the 3x3
        //     0x3530 // 0 y=## x=##
        //    ,0x6270 // 1 y=## x=##
        //    ,0x6230 // 2 y=## x=##
        //    ,0x7370 // 3 y=## x=##
        //    ,0x5710 // 4 y=## x=##
        //    ,0x3270 // 5 y=## x=##
        //    ,0x4770 // 6 y=## x=##
        //    ,0x7110 // 7 y=## x=##
        //    ,0x3770 // 8 y=## x=##
        //    ,0x7710 // 9 y=## x=##
        //    ,0x2750 // A y=## x=##
        //    ,0x6770 // B y=## x=##
        //    ,0x7470 // C y=## x=##
        //    ,0x6560 // D y=## x=##
        //    ,0x7670 // E y=## x=##
        //    ,0x7640 // F y=## x=##
#else
// 3x4 - use a slightly bigger font for better readability
         0x2552 // 0 gy=25 gx=52
        ,0x2222 // 1 gy=22 gx=22
        ,0x6347 // 2 gy=63 gx=47
        ,0x7317 // 3 gy=73 gx=17
        ,0x5571 // 4 gy=55 gx=71
        ,0x7437 // 5 gy=74 gx=37
        ,0x4757 // 6 gy=47 gx=57
        ,0x7122 // 7 gy=71 gx=22
        ,0x3757 // 8 gy=37 gx=57
        ,0x7571 // 9 gy=75 gx=71
        ,0x2575 // A gy=25 gx=75
        ,0x6757 // B gy=67 gx=57
        ,0x7447 // C gy=74 gx=47
        ,0x6556 // D gy=65 gx=56
        ,0x7647 // E gy=76 gx=47
        ,0x7464 // F gy=74 gx=64
#endif
    };

    for( int byte = 0; byte < 0x100; byte++ )
    {
        int digit1 = aNumberGlyphs[ (byte >> 4) & 0xF ];
        int digit0 = aNumberGlyphs[ (byte >> 0) & 0xF ];

        int offset = byte*CELL;

    // Label top X-axis
        DrawGlyph4x4( texels, AXISX + offset, 0, digit1 );
        DrawGlyph4x4( texels, AXISX + offset, 5, digit0 );

    // Label left Y-axis
        DrawGlyph4x4( texels, 0, AXISY + offset, digit1 );
        DrawGlyph4x4( texels, 4, AXISY + offset, digit0 );
    }
}

// ======================================================================== 
void HighlightAlphabet( uint8_t* texels )
{
    for( int byte = 0x20; byte < 0x80; byte++ )
    {
        uint16_t bits = aGlyphs3x4[ byte - 0x20 ];
        int      x  = (bits >> 0) & 0xFF;
        int      y  = (bits >> 8) & 0xFF;
        int      sx = CELL*x + AXISX;
        int      sy = CELL*y + AXISY;
        DrawGlyph4x4( texels, sx, sy, bits, 0xFF );
    }
}

// ======================================================================== 
int main()
{
    uint8_t  aBitmap[ DIMY ][ DIMX ];
    uint8_t *pBitmap = &aBitmap[0][0];
    memset(  pBitmap, 0x22*BORDER, AREA ); // save with faint grid lines if BORDER

//int x = 0; int y = 0; int bits = 0x5A5A; // DEBUG
//DrawGlyph4x4( (uint8_t*)aBitmap, x, y, bits ); // DEBUG

    // Row
    for( int y = 0; y < 0x100; y++ )
    {
        // Column
        for( int x = 0; x < 0x100; x++ )
        {
            int bits = (y << 8) | x;

            // (y*CELL*DIMX) + (x*CELL) + (AXISY*DIMX) + AXISX
            int sx = CELL*x + AXISX;
            int sy = CELL*y + AXISY;
            DrawGlyph4x4( (uint8_t*)aBitmap, sx, sy, bits, 0xB0 );
        }
    }

    if( gbLabelAxis )
    {
        LabelAxis( pBitmap );
    }

    if( 0 )
        HighlightAlphabet( pBitmap );

    // Save Raw
    char filename[ 256 ];
    sprintf( filename, "%dx%d_8bit_textureatlas.raw.data", DIMX, DIMY );

    const char *filenameRAW = filename;
    RAW_SaveGreyscale8bit( filenameRAW, (uint8_t*)aBitmap, DIMX, DIMY );
    printf( "Saved RAW: %s\n", filenameRAW );

    // TODO: Convert 8-bit to 24-bit
    // TODO: Colorize
    // TODO: Now save 8-bit as 24-bit BMP

    return 0;
}

