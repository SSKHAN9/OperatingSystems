/**
* (basic description of the program or class)
*
* Completion time: (estimation of hours spent on this program)
*
* @author (your name), (anyone else, e.g., Acuna, whose code you used)
* @version (a version number or a date)
*/

////////////////////////////////////////////////////////////////////////////////
//INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <getopt.h>
#include <time.h>

//UNCOMMENT BELOW LINE IF USING SER334 LIBRARY/OBJECT FOR BMP SUPPORT
#include "BmpProcessor.h"
#include "PixelProcessor.h"

////////////////////////////////////////////////////////////////////////////////
//MACRO DEFINITIONS

//problem assumptions
#define BMP_HEADER_SIZE 14
#define BMP_DIB_HEADER_SIZE 40
#define MAXIMUM_IMAGE_SIZE 4096


////////////////////////////////////////////////////////////////////////////////
//DATA STRUCTURES
int option, f_flag, width, height, scaler;
char *file_in, *file_out;
pthread_t tid[4];
pthread_attr_t attr;
struct Pixel **in_pArr, **out_pArr;

struct Circle {
    int x;
    int y;
    int rad;
};

struct Circle* circles;

////////////////////////////////////////////////////////////////////////////////
//MAIN PROGRAM CODE

void* blurPixels(void* param);
void* swissCheese(void* param);
struct Pixel** createPixelArray();
void destroyPixelArray(struct Pixel** pArr);

void main(int argc, char* argv[]) {
    while ((option = getopt(argc, argv, "i:o:f:")) != -1) {
        switch (option){
            case 'i' :
                file_in = optarg;
                break;
            case 'o' :
                file_out = optarg;
                break;
            case 'f' :
                if (*optarg == 'b') f_flag = 0;
                else f_flag = 1;
                break;
        }
    }

    FILE* in = fopen(file_in, "rb");
    FILE* out = fopen(file_out, "wb");

    struct BMP_Header* bmp_header = (struct BMP_Header*)malloc(sizeof(struct BMP_Header));
    struct DIB_Header* dib_header = (struct DIB_Header*)malloc(sizeof(struct DIB_Header));

    readBMPHeader(in, bmp_header);
    readDIBHeader(in, dib_header);

    width = dib_header->width;
    height = dib_header->height;

    in_pArr = createPixelArray(width, height);

    readPixelsBMP(in, in_pArr, width, height);
    writeBMPHeader(out, bmp_header);
    writeDIBHeader(out, dib_header);

    pthread_attr_init(&attr);

    if(f_flag == 0) { //blur
        out_pArr = createPixelArray(width, height);
        for (int i = 0; i < 4; i++) {
            pthread_create(&tid[i], &attr, blurPixels, i);
        }
        for (int i = 0; i < 4; i++) {
            pthread_join(tid[i], NULL);
        }
        writePixelsBMP(out, out_pArr, width, height);
        destroyPixelArray(out_pArr);
    } else { // cheese
        srand(time(0));
        if (width >= height) scaler = width / 10;
        else scaler = height / 10;
        circles = (struct Circle*)malloc(scaler * sizeof(struct Circle));
        for (int i = 0; i < scaler; i++) {
            circles[i].x = rand() % width;
            circles[i].y = rand() % height;
            circles[i].rad = (rand() % (scaler)) + (scaler / 2);
        }
        for (int i = 0; i < 4; i++) {
            pthread_create(&tid[i], &attr, swissCheese,  i);
        }
        for (int i = 0; i < 4; i++) {
            pthread_join(tid[i], NULL);
        }
        free(circles);
        circles = NULL;
        writePixelsBMP(out, in_pArr, width, height);
    }
    destroyPixelArray(in_pArr);
    free(bmp_header);
    free(dib_header);
    bmp_header = NULL;
    dib_header = NULL;
}

void* blurPixels(void* param) {
    int redTotal , greenTotal, blueTotal, num_pix;
    int section = (int)param;
    int size = width / 4;
    for (int i = 0; i < height; i++) {
        for (int j = section * size; j < (section + 1) * size; j++) {
            redTotal = in_pArr[i][j].red;
            greenTotal = in_pArr[i][j].green;
            blueTotal = in_pArr[i][j].blue;
            num_pix = 1;
            if (i != 0 && j != 0) {
                redTotal += in_pArr[i-1][j-1].red;
                greenTotal += in_pArr[i-1][j-1].green;
                blueTotal += in_pArr[i-1][j-1].blue;
                num_pix++;
            }
            if (i != 0) {
                redTotal += in_pArr[i-1][ j ].red;
                greenTotal += in_pArr[i-1][ j ].green;
                blueTotal += in_pArr[i-1][ j ].blue;
                num_pix++;
            }
            if (i != 0 && j != width - 1) {
                redTotal += in_pArr[i-1][j+1].red;
                greenTotal += in_pArr[i-1][j+1].green;
                blueTotal += in_pArr[i-1][j+1].blue;
                num_pix++;
            }
            if (j != 0) {
                redTotal += in_pArr[ i ][j-1].red;
                greenTotal += in_pArr[ i ][j-1].green;
                blueTotal += in_pArr[ i ][j-1].blue;
                num_pix++;
            }
            if (j != width - 1) {
                redTotal += in_pArr[ i ][j+1].red;
                greenTotal += in_pArr[ i ][j+1].green;
                blueTotal += in_pArr[ i ][j+1].blue;
                num_pix++;
            }
            if (i != height - 1 && j != 0) {
                redTotal += in_pArr[i+1][j-1].red;
                greenTotal += in_pArr[i+1][j-1].green;
                blueTotal += in_pArr[i+1][j-1].blue;
                num_pix++;
            }
            if (i != height - 1) {
                redTotal += in_pArr[i+1][ j ].red;
                greenTotal += in_pArr[i+1][ j ].green;
                blueTotal += in_pArr[i+1][ j ].blue;
                num_pix++;
            }
            if (i != height - 1 && j != width - 1) {
                redTotal += in_pArr[i+1][j+1].red;
                greenTotal += in_pArr[i+1][j+1].green;
                blueTotal += in_pArr[i+1][j+1].blue;
                num_pix++;
            }
            out_pArr[i][j].red = redTotal / num_pix;
            out_pArr[i][j].green = greenTotal / num_pix;
            out_pArr[i][j].blue = blueTotal / num_pix;
        }
    }


}

void* swissCheese(void* param){
    int section = (int)param;
    int size = width / 4;
    int distance;
    for (int i = 0; i < height; i++) {
        for (int j = section * size; j < (section + 1) * size; j++){
            for (int k = 0; k < scaler; k++){
                distance = sqrt((j - circles[k].x) * (j - circles[k].x) +
                                (i - circles[k].y) * (i - circles[k].y));

                if (distance < circles[k].rad) {
                    in_pArr[i][j].red = 0;
                    in_pArr[i][j].green = 0;
                    in_pArr[i][j].blue = 0;
                } else {
                    in_pArr[i][j].blue = 0;
                }
            }
        }
    }
}

struct Pixel** createPixelArray() {
    struct Pixel** pArr = (struct Pixel**)malloc(height * sizeof(struct Pixel**));
    for (int i = 0; i < height; i++) {
        pArr[i] = (struct Pixel*)malloc(width * sizeof(struct Pixel));
    }
    return pArr;
}

void destroyPixelArray(struct Pixel** pArr) {
    for (int i = 0; i < height; i++) {
        free(pArr[i]);
        pArr[i] = NULL;
    }
    free(pArr);
    pArr = NULL;
}