#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Image.h"
#include "BMPHandler.h"


int main(int argc, char *argv[]) {
    char* inputFile = "/Users/saadkhan/Downloads/bmp/ttt.bmp";
    char* outputFile = "/Users/saadkhan/Downloads/bmp/ttt_copy.bmp";
    int shiftR = 56, shiftG = 0, shiftB = 0;
    float scale = 0.5;
    bool grayscale = true;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            // output file name
            i++;
            outputFile = argv[i];
        } else if (!strcmp(argv[i], "-r")) {
            // red color shift
            i++;
            shiftR = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-g")) {
            // green color shift
            i++;
            shiftG = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-b")) {
            // blue color shift
            i++;
            shiftB = atoi(argv[i]);
        } else if (!strcmp(argv[i], "-w")) {
            // grayscale
            grayscale = true;
        } else if (!strcmp(argv[i], "-s")) {
            // scale
            i++;
            scale = atof(argv[i]);
        } else {
            // input file name
            inputFile = argv[i];
        }
    }

    // Validate input file
    if (!inputFile) {
        printf("Error: Input file not specified\n");
        return 1;
    }
    FILE *input = fopen(inputFile, "rb");
    if (!input) {
        printf("Error: Input file not found\n");
        return 1;
    }

    // Set output file name
    if (!outputFile) {
        outputFile = "output.bmp";
    }

    // Read BMP header
    struct BMP_Header* bmp_header;
    bmp_header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    readBMPHeader(input, bmp_header);

    // Read DIB header
    struct DIB_Header* dib_header;
    dib_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));
    readDIBHeader(input, dib_header);

    // Read pixels
    int width = dib_header->width;
    int height = dib_header->height;
    struct Pixel** pArr;
    pArr = (struct Pixel**)malloc(sizeof(struct Pixel*) * dib_header->height);
    for (int i = 0; i < dib_header->height; i++) {
        pArr[i] = (struct Pixel*)malloc(sizeof(struct Pixel) * dib_header->width);
    }

    readPixelsBMP(input, pArr, width, height);
    fclose(input);

    Image* img = image_create(pArr, dib_header->width, dib_header->height);

    // Apply filters
    if (grayscale) {
        image_apply_bw(img);
    }
    image_apply_colorshift(img, shiftB, shiftR, shiftG);
    image_apply_resize(img, scale);

    dib_header->width = img->width;
    dib_header->height = img->height;

    //write file
    FILE* output = fopen(outputFile, "wb");
    writeBMPHeader(output, bmp_header);
    writeDIBHeader(output, dib_header);
    writePixelsBMP(output, img->pArr, dib_header->width, dib_header->height);

    fclose(output);

    for (int i = 0; i < dib_header->height; i++) {
        free(pArr[i]);
    }

    free(pArr);
    free(dib_header);
    free(bmp_header);

    return 0;
}
