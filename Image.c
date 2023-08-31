
#include "Image.h"

Image* image_create(struct Pixel** pArr, int width, int height){
    struct Image *img;
    img = malloc(sizeof(Image));
    img->pArr = pArr;
    img->width = width;
    img->height = height;
    return img;
}

void image_apply_bw(Image* img){

    for(int i = 0; i < img->height; i++){

        for(int j = 0; j < img->width; j++){

            float grayscale = (0.299 * img->pArr[i][j].red) +
                              (0.587 * img->pArr[i][j].green) +
                              (0.114 * img->pArr[i][j].blue);
            img->pArr[i][j].red = grayscale;
            img->pArr[i][j].green = grayscale;
            img->pArr[i][j].blue = grayscale;
        }
    }
}

/* Destroys an image. Does not deallocate internal pixel array.
*
 * @param  img: the image to destroy.
*/
void image_destroy(Image** img){
    free(img);
}

struct Pixel** image_get_pixels(Image* img){
    return img->pArr;
}

int image_get_width(Image* img){
    return img->width;
}

int image_get_height(Image* img){
    return img->height;
}

void image_apply_colorshift(struct Image* img, int rShift, int gShift, int bShift){

    for(int i = 0; i < img->height; i++){
        for(int j = 0; j < img->width; j++){
            if (img->pArr[i][j].red + rShift > 255){
                img->pArr[i][j].red = 255;
            } else if (img->pArr[i][j].red + rShift < 0){
                img->pArr[i][j].red = 0;
            } else {
                img->pArr[i][j].red += rShift;
            }
            if (img->pArr[i][j].green + gShift > 255){
                img->pArr[i][j].green = 255;
            } else if (img->pArr[i][j].green + gShift < 0){
                img->pArr[i][j].green = 0;
            } else {
                img->pArr[i][j].green += gShift;
            }
            if (img->pArr[i][j].blue + bShift > 255) {
                img->pArr[i][j].blue = 255;
            } else if (img->pArr[i][j].blue + bShift < 0) {
                img->pArr[i][j].blue = 0;
            } else {
                img->pArr[i][j].blue += bShift;
            }
        }
    }
}

void image_apply_resize(Image* img, float factor) {
    int newWidth = img->width * factor;
    int newHeight = img->height * factor;

    // verify the factor is greater than 0
    if (factor <= 0) {
        printf("Please try enter a factor greater than 0.\n");
        return;
    }

    // when factor is smaller than 1, overwriting original array
    if (factor <= 1 && factor > 0) {
        for (int i = 0; i < newHeight; i++) {
            for (int j = 0; j < newWidth; j++) {
                int heightFactor = i / factor;
                int widthFactor = j / factor;
                img->pArr[i][j].blue =  img->pArr[heightFactor][widthFactor].blue;
                img->pArr[i][j].green = img->pArr[heightFactor][widthFactor].green;
                img->pArr[i][j].red = img->pArr[heightFactor][widthFactor].red;
            }
        }
    }

    // if factor is greater than 1
    if (factor > 1) {
        // allocate more
        struct Pixel** newPixels = (struct Pixel**)malloc(sizeof(struct Pixel*) * newHeight);
        for (int p = 0; p < newHeight; p++) {
            newPixels[p] = (struct Pixel*)malloc(sizeof(struct Pixel) * newWidth);
        }
        // store pixel info into new array
        for (int i = 0; i < newHeight; i++) {
            for (int j = 0; j < newWidth; j++) {
                int heightFactor = i / factor;
                int widthFactor = j / factor;
                newPixels[i][j].blue =  img->pArr[heightFactor][widthFactor].blue;
                newPixels[i][j].green = img->pArr[heightFactor][widthFactor].green;
                newPixels[i][j].red = img->pArr[heightFactor][widthFactor].red;
            }
        }
        // update array pointer
        img->pArr = newPixels;
    }

    // update width and height
    img->width = newWidth;
    img->height = newHeight;
}
