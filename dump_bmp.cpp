// Includes
    #include <stdio.h>
    #include <stdint.h>
    #include <string.h>

// Consts
    const int BITMAPFILEHEADER_SIZE = 14; // 14 = 0x0E = BITMAPFILEHEADER size
    const int BITMAPINFOHEADER_SIZE = 40; // 40 = 0x28 = BITMAPINFOHEADER size

// Macros
	inline
	uint16_t get16( const uint8_t *data, int offset )
	{
		uint16_t *p = (uint16_t*) &data[ offset ];
		return *p;
	}

	inline
	uint32_t get32( const uint8_t *data, int offset )
	{
		uint32_t *p = (uint32_t*) &data[ offset ];
		return *p;
	}

// Types
    struct WindowsBitmapFileHeader
    {
        uint16_t magic;
        uint32_t bfSize;
        uint32_t reserved;
        uint32_t bfOffbits;

        WindowsBitmapFileHeader()
        {
            memset( this, 0, sizeof( *this ) );
        }

        WindowsBitmapFileHeader( const uint8_t *data )
        {
            get( data );
        }

        void get( const uint8_t *data )
        {
//for( int i = 1; i < BITMAP_HEADER_SIZE/2; i += 2 )
//    printf( "%02X: [%2d]: %02X %02X - %02X %02X\n", i*2, i, aHeader[i] & 0xFF, (aHeader[i] >> 8) & 0xFF, aHeader[i+1] & 0xFF, (aHeader[i+1] >> 8) & 0xFF );
            magic     = get16( data,  0 );
            bfSize    = get32( data,  2 );
            reserved  = get32( data,  6 );
            bfOffbits = get32( data, 10 );
        }

        void dump()
        {
            printf( "Magic    : %c%c     \n", magic & 0xFF, (magic >> 8) & 0xFF );
            printf( "File Size: %08X (%d)\n", bfSize   , bfSize    );
            printf( "reserved : %08X     \n", reserved             );
            printf( "Offset   : %08X (%d)\n", bfOffbits, bfOffbits );
			printf( "-------------------\n"        );
        }
    };

    struct WindowsBitmapInfoHeader
    {
        uint32_t biSize;
        uint32_t biWidth;
        uint32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;

        WindowsBitmapInfoHeader()
        {
            memset( this, 0, sizeof( *this ) );
        }

        WindowsBitmapInfoHeader( const uint8_t *data )
        {
            get( data );
        }

        void get( const uint8_t *data )
        {
            biSize          = get32( data, 0 );
            biWidth         = get32( data, 4 );
            biHeight        = get32( data, 8 );
            biPlanes        = get16( data, 12 );
            biBitCount      = get16( data, 14 );
            biCompression   = get32( data, 16 );
            biSizeImage     = get32( data, 20 );
			biXPelsPerMeter = get32( data, 24 );
			biYPelsPerMeter = get32( data, 28 );
			biClrUsed       = get32( data, 32 );
			biClrImportant  = get32( data, 36 );
        }

        void dump()
        {
#define	_ " :     "
			printf( "HeadSize : %08X %s\n"  , biSize, biSize == BITMAPINFOHEADER_SIZE ? "Standard == 40" : "Non-Standard" );
			printf( "Width    : %08X (%d)\n", biWidth, biWidth );
			printf( "Height   : %08X (%d)\n", biHeight, biHeight );
			printf( "Planes  "_"%04X (%d)\n", biPlanes, biPlanes );
			printf(	"BitCount"_"%04X (%d)\n", biBitCount, biBitCount );
			printf( "Compress : %08X %s\n"  , biCompression, biCompression == 0 ? "None" :	"Unknown" );
			printf( "TexelSize: %08X (%d)\n", biSizeImage, biSizeImage );
			printf( "X px/m   : %08X (%d)\n", biXPelsPerMeter, biXPelsPerMeter );
			printf( "Y px/m   : %08X (%d)\n", biYPelsPerMeter, biYPelsPerMeter );
			printf( "Palette  : %08X (%d)\n", biClrUsed, biClrUsed );
			printf( "important: %08X (%d)\n", biClrImportant, biClrImportant );
        }
    };

int main( int nArg, char *aArg[] )
{
    if( nArg > 1 )
    {
        FILE *file = fopen( aArg[1], "rb" );
        if( file )
        {
			uint8_t head[ BITMAPFILEHEADER_SIZE ];
			uint8_t info[ BITMAPINFOHEADER_SIZE ];

            fread( head, BITMAPFILEHEADER_SIZE, 1, file );
            fread( info, BITMAPINFOHEADER_SIZE, 1, file );

            WindowsBitmapFileHeader header( head );
            WindowsBitmapInfoHeader infoer( info );

            header.dump();
            infoer.dump();

            fclose( file );
        }
    }
}
