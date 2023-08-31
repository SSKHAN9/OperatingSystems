
#include "BMPHandler.h"

void readBMPHeader(FILE* file, struct BMP_Header* header) {
    fread(&header->signature, sizeof(char) * 2, 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->reserved1, sizeof(short), 1, file);
    fread(&header->reserved2, sizeof(short), 1, file);
    fread(&header->offset_pixel_array, sizeof(int), 1, file);
}

void writeBMPHeader(FILE* file, struct BMP_Header* header) {
    fwrite(&header->signature, sizeof(char) * 2, 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->offset_pixel_array, sizeof(int), 1, file);
}

void readDIBHeader(FILE* file, struct DIB_Header* header) {
    fread(&header->DIBHeaderSize, sizeof(int), 1, file);
    fread(&header->width, sizeof(int), 1, file);
    fread(&header->height, sizeof(int), 1, file);
    fread(&header->Planes, sizeof(short), 1, file);
    fread(&header->BPP, sizeof(short), 1, file);
    fread(&header->Compression, sizeof(int), 1, file);
    fread(&header->size, sizeof(int), 1, file);
    fread(&header->rez_horizontal, sizeof(int), 1, file);
    fread(&header->rez_vertical, sizeof(int), 1, file);
    fread(&header->color_count, sizeof(int), 1, file);
    fread(&header->important_color, sizeof(int), 1, file);
}

void writeDIBHeader(FILE* file, struct DIB_Header* header) {
    fwrite(&header->DIBHeaderSize, sizeof(int), 1, file);
    fwrite(&header->width, sizeof(int), 1, file);
    fwrite(&header->height, sizeof(int), 1, file);
    fwrite(&header->Planes, sizeof(short), 1, file);
    fwrite(&header->BPP, sizeof(short), 1, file);
    fwrite(&header->Compression, sizeof(int), 1, file);
    fwrite(&header->size, sizeof(int), 1, file);
    fwrite(&header->rez_horizontal, sizeof(int), 1, file);
    fwrite(&header->rez_vertical, sizeof(int), 1, file);
    fwrite(&header->color_count, sizeof(int), 1, file);
    fwrite(&header->important_color, sizeof(int), 1, file);
}

void makeBMPHeader(struct BMP_Header* header, int width, int height) {
    header->signature[0] = 'B';
    header->signature[1] = 'M';
    header->reserved1 = 0;
    header->reserved2 = 0;
    header->offset_pixel_array = sizeof(struct BMP_Header) + sizeof(struct DIB_Header) - 2;
    header->size = header->offset_pixel_array + width * height * 24 / 8;
}

void makeDIBHeader(struct DIB_Header* header, int width, int height) {
    header->DIBHeaderSize = sizeof(struct DIB_Header);
    header->height = height;
    header->width = width;
    header->size = width * height * 24 / 8;
    header->Planes = 1;
    header->BPP = 24;
    header->color_count = 0;
    header->Compression = 0;
    header->important_color = 0;
    header->rez_horizontal = 3780;
    header->rez_vertical = 3780;
}

void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){

            fread(&pArr[i][j].red, sizeof(char), 1, file);
            fread(&pArr[i][j].blue, sizeof(char), 1, file);
            fread(&pArr[i][j].green, sizeof(char), 1, file);
        }
    }
}

void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height) {
    int padding = 0;
    int rowSize = 3 * width;
    while ((rowSize + padding) % 4 != 0) {
        padding++;
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            fwrite(&pArr[i][j].red, sizeof(char), 1, file);
            fwrite(&pArr[i][j].blue, sizeof(char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(char), 1, file);
        }
    }

        for (int j = 0; j < padding; j++) {
            fwrite("\0", sizeof(unsigned char), 1, file);
        }
    }

