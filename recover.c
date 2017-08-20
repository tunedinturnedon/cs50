#include <stdio.h>

int main (int argc, char *argv[])
{  
    FILE *outptr = NULL;
    int filecount = 0;
    if(argc != 2)
        {
        return 2;
        printf("please use as follows ./recover nameofimagetorecover ");
        }
    //introduce and open raw image
     
    FILE *inptr = fopen(argv[1],"r");
    if(inptr == NULL)
        {
            return 3;
            printf("this file cannot be read");
        }
    //create a buffer
    unsigned char buffer[512] = {0};
    char filename [8];
    //read 512 bytes into file, into the buffer
     while(fread(buffer, 512, 1, inptr) > 0)
    {  
    //check the first three bytes of the buffer for a JPEG header
     if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
     {
        if(outptr != NULL)
            { 
                //if its not, close it,
                fclose(outptr);
            }
            // name and open the new outfile, 
        sprintf(filename, "%03i.jpg", filecount);
        outptr = fopen(filename, "w+");
        if (outptr == NULL)
            {
                printf("Error! Could create the file!\n");
                return 1;
            }
        filecount++;
        fwrite(buffer, 512, 1, outptr);
        } 
     }
    if(outptr != NULL)
            { 
    fwrite(buffer, 512), 1, outptr);
}

    //when EOF is reached, close infile to prevent a memory leak.
    printf("returning 0");
    fclose(inptr);
    return 0;
}
