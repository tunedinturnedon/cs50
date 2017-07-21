/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./copy infile outfile\n");
        return 1;
    }
    //get n from user
    printf("what number to resize by?\n");
    int n = get_int();
    
    // remember filenames
    char *infile = argv[1];
    char *outfile = argv[2];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
   // save old width and height
  
    int oldheight = bi.biHeight;
    int oldwidth = bi.biWidth;
   // change outfiles's HEIGHT, WIDTH, padding and FILE and IMAGE SIZE
    bi.biWidth = bi.biWidth * n;
  bi.biHeight = bi.biHeight * n;
       // determine new  and old padding for scanlines
    int newpadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
     int padding =  (4 - (oldwidth * sizeof(RGBTRIPLE)) % 4) % 4;
     // change file and image size
    bi.biSizeImage = (sizeof(RGBTRIPLE) * bi.biWidth + padding) *abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
//n times

        // for each row       
    for (int i = 0, biHeight = abs(oldheight); i < biHeight; i++)
        {
            for(int t = 0; t < n - 1; t++)
              {
        // iterate over pixels in scanline
      
                for (int j = 0; j < oldwidth; j++)
                 {
                    // temporary storage
                    RGBTRIPLE triple;
                    // read RGB triple from infile
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);   
         
                    // write RGB triple to outfile n times
         
                     for(int y = 0; y < n; y++)
                        {
                            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                        }
                 }            
                        
                 // skip over padding, if any
                fseek(inptr, padding, SEEK_CUR);
                // then add outfile padding 
                for (int k = 0; k < newpadding; k++)
                    {
                    fputc(0x00, outptr);
        }
              }
     long offset = oldwidth * sizeof(RGBTRIPLE) + padding;
    fseek(inptr, -offset, SEEK_CUR);   
        }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
              
}
