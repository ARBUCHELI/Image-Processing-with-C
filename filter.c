#include <getopt.h> //Library used to parse command-line options of the Unix/POSIX style. POSIX is a family of standards for maintaining compatibility between
//operating systems.
#include <stdio.h> //Standard input output.
#include <stdlib.h> //Standard library.

#include "helpers.h" //For includying the functions declared in helpers.h.

int main(int argc, char *argv[]) //To accept command-line arguments.
{

    // Define allowable filters
    char *filters = "bgrs"; //Creates a variable called filters that is a pointer to a value of type char (the filter).  Tells the program that the allowable
    //command-line arguments to run the program are: b, g, r, s (blur, grayscale, reflection, sepia).

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?') //Check if the filter is a valid option -b, -g, -r, -s
    //The getopt() function returns different values:
    //If the option takes a value, that value is pointer to the external variable optarg.
    //‘-1’ if there are no more options to process.
    //‘?’ when there is an unrecognized option and it stores into external variable optopt.
    //If an option requires a value (such as -f in our example) and no value is given, getopt normally returns ?.
    //By placing a colon as the first character of the options string, getopt returns: instead of ? when no value is given.
    {
        fprintf(stderr, "Invalid filter.\n"); //the function fprintf sends formatted output to a stream.  In C Programming language, there are different file
        //descriptors which are also known as standard output. There are 3 standards I/O devices that are stdin for standard input, stdout for standard output,
        //stderr for error message output.
        return 1;
    }

    // Ensure only one filter
    //The getopt() function returns different values:
    //If the option takes a value, that value is pointer to the external variable optarg.
    //‘-1’ if there are no more options to process.
    //‘?’ when there is an unrecognized option and it stores into external variable optopt.
    //If an option requires a value (such as -f in our example) and no value is given, getopt normally returns ?.
    //By placing a colon as the first character of the options string, getopt returns: instead of ? when no value is given.

    //In this case we only need one option and for that reason getopt(argc, argv, filters) should be == -1 but if the value is != -1 it means error because there
    //is more than one option entered.
    if (getopt(argc, argv, filters) != -1)
    {
        fprintf(stderr, "Only one filter allowed.\n"); //the function fprintf sends formatted output to a stream.  In C Programming language, there are different
        //file descriptors which are also known as standard output. There are 3 standards I/O devices that are stdin for standard input, stdout for standard
        //output, stderr for error message output.
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2) //Ensures that the filter option is entered after the name of the app: Ex: ./filter -g. The optind variable is the index value of the
    //next argument that should be handled by the getopt() function.  Remember taht argc is the number of elements in the array of arguments.  argc is = 4.  All
    //this means that 4 = optind + 2 and solving for optind, optind has to be = 2.  So optind is the index of the element after 1 that at the same time is the of
    //the flag.
    {
        fprintf(stderr, "Usage: filter [flag] infile outfile\n"); //the function fprintf sends formatted output to a stream.  In C Programming language, there
        //are different file descriptors which are also known as standard output. There are 3 standards I/O devices that are stdin for standard input, stdout for
        //standard output, stderr for error message output.
        return 3;
    }

    // Remember filenames
    //This means that these variables are located in the argv array of command-line arguments
    //Creates a variable called infile that is a pointer to a value of type char that stores the input of the user. optind means that the next argument is the
    //element with the index 2, because before this, the argument with the index = 1 was handled.
    //Creates a variable called outfile that is a pointer to a value of type char that stores the input of the user. optind means that the next argument is the
    //element with the index 3, because because is optind + 1
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Open input file
    FILE *inptr = fopen(infile, "r");
    //Creates a variable called inptr that is a pointer to a value of type FILE.  fopen will open the input file (infile) and it will return a pointer to
    //infile in such way that then we can read information from.
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile); // If there is not file to read, the program will return an error and a message of error.
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    //Creates a variable called outptr that is a pointer to a value of type FILE.  fopen will open the file (outfile)
    //and it will return a pointer to outfile in such way that then we can write information to.
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);  //If there is not file to write to, the program will return an error and a message of error.
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    //Here we create a variable of the type BITMAPFILEHEADER (bf) and we store in the location in memory of that variable, the BITMAPFILEHEADER that we read from
    //the infile (1 unit of info of the size of the BITMAPFILEHEADER read from inptr that is the infile pointer)
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    //Here we create a variable of the type BITMAPINFOHEADER (bi) and we store in the location in memory of that variable, the BITMAPINFOHEADER that we read from
    //the infile (1 unit of info of the size of the BITMAPINFOHEADER read from inptr that is the infile pointer)
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    //Check for typical features of a BMP 4.0 file in the bf and bi variables of type BITMAPFILEHEADER and BITMAPINFOHEADER, created before
    //https://en.m.wikipedia.org/wiki/BMP_file_format
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        //If the file is not a 24.bit uncompressed BMP 4.0 close de pointers and through an error message.
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 6;
    }

    int height = abs(bi.biHeight); //variable of type int to save the height of the image
    int width = bi.biWidth;        //variable of type int to save the width of the image

    // Allocate memory for image
    //calloc is used to allocate memory dynamically and return a pointer to it. The difference with malloc is that malloc doesn't initialize the allocated
    //memory.  If we try to access the content of memory block (before initializing) then we'll get sgementation fault error.
    //calloc() allocates the memory and also initializes the allocated memory block to 0. If we try to access the content of these block then we'll get 0.
    //This is allocating memory for the entire image, row by row.  height blocks with the size width*sizeof(RGBTRIPLE)
    //In other words calloc is allocating memory for all the pixels in the image.

    //image is a pointer to an array of width RGBTRIPLE'S. In other words this is the pixel array.
    //calloc allocates an array  of height of these things. So you can do image[1] to get the second row and image[1][0] to get the first pixel of the second row.
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        //If there is no memory allocated to store the image, close the pointers and through an error message.
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }



    //BETTER EXPLANATION:
    //To speed up mathematical operations with bitmaps, these scanlines must be divisible by 4 (4, 8, 12... 40... etc). Why? Because it is way faster
    //to multiply a number with 4 than say 3. It has to do with bit-shifting.
    //
    //You can ask yourself why to use such tricky technique like using ScanLine property seemingly is when you can simply use Pixels to access your
    //bitmap's pixels. The answer is a big performance difference noticable when you perform pixel modifications even on a relatively small pixel area.
    //The Pixels property internally uses Windows API functions - GetPixel and SetPixel, for getting and setting device context color values. The performance
    //lack at Pixels technique is that you usually need to get pixel color values before you modify them, what internally means the call of both mentioned Windows
    //API functions. The ScanLine property is winning this race because provides a direct access to the memory where the bitmap pixel data are stored. And direct
    //memory access is just faster than two Windows API function calls.
    //
    //But, it doesn't mean that Pixels property is totally bad and that you should avoid to use it in all cases. When you are going to modify just few pixels
    //(not a big areas) occasionally e.g., then Pixels might be sufficient for you. But don't use it when you are going to manipulate with a pixel area.

    //A main dish of this post, the ScanLine property, is a read only indexed property that returns pointer to the first byte of the array of raw data bytes that
    //belongs to a specified row in a bitmap. In other words we request the access to the array of raw data bytes for a given row and what we receive is a pointer
    //to the first byte of that array. The index parameter of this property specifies the 0 based index of a row for which we want to get these data.

    //The following image illustrates our imaginary bitmap and the pointers we get by the ScanLine property using different row indexes:


    //BEST EXPLANATION

    //The BMP color table has four bytes in each color table entry. The bytes
    //are for the blue, green, and red color values. The fourth byte is padding and
    //is always zero. For a 256 gray shade image, the color table is 4x256 bytes
    //long. The blue, green, and red values equal one another.
    //The final part of the BMP file is the image data. The data is stored row
    //by row with padding on the end of each row. The padding ensures the image
    //rows are multiples of four. The four, just like in the color table, makes it
    //easier to read blocks and keep track of addresses.

    //Determine padding for scanlines
    //creates a variable called padding of type int used to create space or padding
    //(width * sizeof(RGBTRIPLE)) finds how many bytes each scanline is
    //(width * sizeof(RGBTRIPLE))% 4 finds the difference between the closest multiple of 4 and how many bytes each scanline is
    //(4 - (width * sizeof(RGBTRIPLE))% 4) finds how much padding is needed unless none is needed (in which case 4 will be returned)
    //(4 - (width * sizeof(RGBTRIPLE)) % 4) % 4 Makes the padding needed 0 instead of 4 if padding is not needed

    //In other words this ensure that the fourth byte is always zero.


    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array.  Remember that image[i] represents a row.  In this case fread is going to read width elements of size of RGBTRIPLE from the
        //inputs file, and store them in the first row of the array of pixels image. image[i]


        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        //fseek sets the file position of the stream to the given offset.  fseek(FILE *stream, long int offset, int whence)

        //Parameters
        //stream − This is the pointer to a FILE object that identifies the stream. (In our case is the pointer that we already defined)

        //offset − This is the number of bytes to offset from whence. (In our case is the padding that we already defined)

        //whence − This is the position from where offset is added. It is specified by one of the following constants −

        //Sr.No.	Constant & Description
        //1
        //SEEK_SET

        //Beginning of file

        //2
        //SEEK_CUR

        //Current position of the file pointer

        //3
        //SEEK_END

        //End of file

        //Return Value
        //This function returns zero if successful, or else it returns a non-zero value.

        //The loop "for" do this for each row

        fseek(inptr, padding, SEEK_CUR);
    }

    // Filter image
    //Pass the filter (-b, -g, -r, or -s) and calls the function which corresponds to each filter
    switch (filter)
    {
        // Blur
        case 'b':
            blur(height, width, image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, image);
            break;

        // Reflection
        case 'r':
            reflect(height, width, image);
            break;

        // Sepia
        case 's':
            sepia(height, width, image);
            break;
    }

    // Write outfile's BITMAPFILEHEADER
    //Copies the BITMAPFILEHEADER stored in the bf variable in the output file
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    //Copies the BITMAPINFOHEADER stored in the bi variable in the output file
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr); //fputc() is used to write a single character at a time to a given file. It writes the given character at the position denoted
            //by the file pointer and then advances the file pointer. This function returns the character that is written in case of successful write operation
            //else in case of error EOF is returned.  Syntax:
            //int fputc(int char, FILE *pointer)
            //char:  character to be written.
            //This is passed as its int promotion.
            //pointer: pointer to a FILE object that identifies the
            //stream where the character is to be written.
        }
    }

    // Free memory for image
    free(image);

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);

    return 0;
}
