#include <iostream>
#include <fstream>
#include <cstring>
#include <ctype.h>

char* INPUT_FILENAME;



int main(int argc, char **argv) {

    INPUT_FILENAME = argv[1];

    std::ifstream file, outputFile;
    size_t size = 0;
    int grayscale = 0;

    unsigned int imgWidth, imgHeight, maxRGB;

    std::cout << "Attempting to open " << INPUT_FILENAME << std::endl;

    file.open( INPUT_FILENAME, std::ios::in | std::ios::binary | std::ios::ate ); /* Open file for reading in binary mode*/
    char* data = 0;

    /* Output number of bytes */
    file.seekg( 0, std::ios::end );
    size = file.tellg();
    //std::cout << "File size: " << size << std::endl;
    file.seekg( 0, std::ios::beg );                                               /* Go to the beginning */

    /* Read the header */
    std::string p;

    std::getline(file, p);                                                        /* Get the first line */
    
    /* Output file typr */
    std::cout << "PPM type: ";
    if(p == "P3" )
    {
        std::cout << "ASCII PPM file" << std::endl;
    }
    else if(p == "P6")
    {
        std::cout << "Binary PPM file - not supported" << std::endl;
        
        return -1;
    }
    else
    {
        std::cout << "Unable to use specified file" << std::endl;

        return -1;
    }

    /* Output width, height and max color*/
    while(D_FLAG <= 2)                                                            /* Read until it get all 3 values of the header */
    {

        file >> p;

        if(p[0] != '#')                                                           /* Skip comments */
        {
            if(isdigit(p[0]) && D_FLAG == 0)                                      /* First number is the width */
            {
                imgWidth = stoi(p);
                D_FLAG++;                                                         /* Increase the data flag */
            }
            else if(isdigit(p[0]) && D_FLAG == 1)                                 /* Second number is the height */
            {
                imgHeight = stoi(p);
                D_FLAG++;
            }else if(isdigit(p[0]) && D_FLAG == 2)                                /* Third number is the max color value */
            {
                maxRGB = stoi(p);
                D_FLAG++;
            }
        }
        else
            std::getline(file, p);
    
    }

    std::cout << "Width: " << imgWidth << std::endl;
    std::cout << "Height: " << imgHeight << std::endl;
    std::cout << "Max Color Value: " << maxRGB << std::endl;

    while(!file.eof())
    {

        grayscale = 0;

        for (int i = 1; i <= 3; i++)                                              /* Store the RGB values of each pixel from the file */
        {
            file >> p;
            grayscale += stoi(p);
        }

        W_FLAG++;

        grayscale /= 3;                                                           /* Get the average value of the pixel */

        std::cout << " " << grayscaleToChar(grayscale);                           /* Prints the ASCII character */

        if(W_FLAG >= imgWidth)                                                    /* Go down everytime it reach the desired width */
        {
            std::cout << std::endl;

            W_FLAG = 0;
        }
    }

}
