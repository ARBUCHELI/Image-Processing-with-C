// BMP-related data types based on Microsoft's own

#include <stdint.h>

/**
 * Common Data Types
 *
 * The data types in this section are essentially aliases for C/C++
 * primitive data types.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/cc230309.aspx.
 * See http://en.wikipedia.org/wiki/Stdint.h for more on stdint.h.
 */
typedef uint8_t  BYTE; //typdef is used to create an additional name (alias) for existing data types like uint8_t, uint32_t, int32_t, uint16_t //uint8_t (1 byte)
typedef uint32_t DWORD; //uint32_t is 32 bits width (4 bytes)
typedef int32_t  LONG; //int32_t is 32 bits width (4 bytes)
typedef uint16_t WORD; //uint16_t is 16 bits width (2 bytes)

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */
typedef struct //In total this struct occupies 14 bytes
{
    WORD   bfType; //2 bytes
    DWORD  bfSize; //4 bytes
    WORD   bfReserved1; //2 bytes
    WORD   bfReserved2; //2 bytes
    DWORD  bfOffBits; //4 bytes
} __attribute__((__packed__)) //This attribute specifies that each member of the structure is placed to minimize the memory required.
BITMAPFILEHEADER;

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */
typedef struct //In total this sruct occupies 40 bytes
{
    DWORD  biSize; //4 bytes
    LONG   biWidth; //4 bytes
    LONG   biHeight; //4 bytes
    WORD   biPlanes; //2 bytes
    WORD   biBitCount; //2 bytes
    DWORD  biCompression; //4 bytes
    DWORD  biSizeImage; //4 bytes
    LONG   biXPelsPerMeter; //4 bytes
    LONG   biYPelsPerMeter; //4 bytes
    DWORD  biClrUsed; //4 bytes
    DWORD  biClrImportant; //4 bytes
} __attribute__((__packed__)) //This attribute specifies that each member of the structure is placed to minimize the memory required.
BITMAPINFOHEADER;

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;
