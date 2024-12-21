#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <numeric>
#include <math.h>

// Mean Filter
// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    int n = (kernelSize - 1) / 2;
    int h = image.get_height();
    int w = image.get_width();

    GrayscaleImage copy(image);

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int sum = 0;
            int count=0;

            for (int ik = i - n; ik <= i + n; ik++) {
                for (int jk = j - n; jk <= j + n; jk++) {
                    if (ik >= 0 && ik < h && jk >= 0 && jk < w) {
                        sum += image.get_pixel(ik, jk);
                        count++;
                    }
                    else {
                        sum+=0;
                        count++;
                    }
                }
            }

            double mean = static_cast<double>(sum) / count;
            int rounded = static_cast<int>(mean);
            if(rounded>255){rounded=255;}
            if(rounded<0){rounded=0;}

            copy.set_pixel(i, j, (rounded));
        }
    }


    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.set_pixel(i, j, copy.get_pixel(i, j));
        }
    }
}

void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    int n = (kernelSize - 1) / 2;
    int h = image.get_height();
    int w = image.get_width();

    double **kernel=new double *[kernelSize];
    for (int i=0;i<kernelSize;i++) {
        kernel[i]=new double[kernelSize];
    }
    double sum=0;
    for (int i=0;i<kernelSize;i++) {
        for (int j=0;j<kernelSize;j++) {
            int x=i-n;
            int y=j-n;
            kernel[i][j] = (1 / (2 * M_PI * sigma * sigma)) * exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum+=kernel[i][j];
        }
    }
    for (int i=0;i<kernelSize;i++) {
        for (int j=0;j<kernelSize;j++) {
            kernel[i][j]/=sum;
        }
    }

    GrayscaleImage copy(image);
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            double sum = 0.0;

            for (int ik = i - n; ik <= i + n; ik++) {
                for (int jk = j - n; jk <= j + n; jk++) {
                    if (ik >= 0 && ik < h && jk >= 0 && jk < w) {
                        sum += image.get_pixel(ik, jk) * kernel[ik - i + n][jk - j + n];
                    }
                }
            }

            if(sum>255){sum=255;}
            if(sum<0){sum=0;}
            copy.set_pixel(i, j, static_cast<int>(sum));
        }
    }
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            image.set_pixel(i, j, copy.get_pixel(i, j));
        }
    }

    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    for (int i=0;i<kernelSize;i++) {
        delete[] kernel[i];
    }
    delete[] kernel;
}



// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {


    int h = image.get_height();
    int w = image.get_width();

    GrayscaleImage original(image);

    apply_gaussian_smoothing(image, kernelSize, 1);


    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            int original_pixel = original.get_pixel(i, j);
            int blurred_pixel = image.get_pixel(i, j);


            int result = original_pixel + static_cast<int>(std::floor(amount * (original_pixel - blurred_pixel)));



            if (result < 0) {
                result = 0;
            }
            if (result > 255) {
                result= 255;
            }

            image.set_pixel(i, j, result);
        }
    }
}

