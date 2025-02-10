#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>

constexpr double PI = 3.14159265358979323846;
// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    int kernelIndex = (kernelSize - 1)/2;
    GrayscaleImage copyImage(image);
    for (int i = 0; i < copyImage.get_height(); ++i) {
        for (int j = 0; j < copyImage.get_width(); ++j) {
            int total = 0;
            for (int k = 0; k < kernelSize; ++k) {
                // if the kernel row is out of the image, then just skip that row (it corresponds to 0 value)
                if (i - kernelIndex + k < 0 || i - kernelIndex + k > copyImage.get_height() - 1) {
                    continue;
                }
                for (int l = 0; l < kernelSize; ++l) {
                    // if the kernel column is out of the image, skip this column (it corresponds to 0 value)
                    if (j - kernelIndex + l < 0 || j - kernelIndex + l > copyImage.get_width() - 1) {
                        continue;
                    }
                    total += copyImage.get_data()[i - kernelIndex + k][j - kernelIndex + l];
                }
            }
            // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
            // 3. Update each pixel with the computed mean.
            image.set_pixel(i, j, total / (kernelSize*kernelSize));
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    GrayscaleImage copyImage(image);
    int rows = copyImage.get_height();
    int cols = copyImage.get_width();

    int kernelIndex = (kernelSize - 1)/2; // this represents the distance from the center to the edges
    double base = 1.0 / (2.0 * sigma * sigma * PI);
    double** matrix = new double*[kernelSize];
    double total = 0;

    for (int i = 0; i < kernelSize; ++i) {
        matrix[i] = new double[kernelSize];
    }
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            double exponent = - ((i-kernelIndex) * (i-kernelIndex) + (j-kernelIndex) * (j-kernelIndex)) / (2*sigma*sigma); //-(x^2 + y^2) / (2 * sigma * sigma)
            double value = base * std::exp(exponent);
            total += value;
            matrix[i][j] = value;
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            matrix[i][j] = matrix[i][j] / total;
        }
    }
    // 3. For each pixel, compute the weighted sum using the kernel.
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            double sum = 0;
            for (int k = 0; k < kernelSize; ++k) {
                // if the kernel row is out of the image, then just skip that row (it corresponds to 0 value)
                if (i - kernelIndex + k < 0 || i - kernelIndex + k > rows - 1) {
                    continue;
                }
                // if the kernel column is out of the image, skip this column (it corresponds to 0 value)
                for (int l = 0; l < kernelSize; ++l) {
                    if (j - kernelIndex + l < 0 || j - kernelIndex + l > cols - 1) {
                        continue;
                    }
                    sum += matrix[k][l] * copyImage.get_pixel(i - kernelIndex + k, j - kernelIndex + l);
                }
            }
            int newValue = (int) sum;
            // 4. Update the pixel values with the smoothed results.
            image.set_pixel(i, j, newValue);
        }
    }
    for (int i = 0; i < kernelSize; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    GrayscaleImage copyImage(image);
    apply_gaussian_smoothing(copyImage, kernelSize, 1.0);

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    int rows = image.get_height();
    int cols = image.get_width();
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int originalPixel = image.get_pixel(i, j);
            int blurredPixel = copyImage.get_pixel(i, j);
            int newPixel = originalPixel + amount * (originalPixel - blurredPixel);

            // 3. Clip values to ensure they are within a valid range [0-255].
            if (newPixel < 0) {
                newPixel = 0;
            }else if (newPixel > 255) {
                newPixel = 255;
            }
            image.set_pixel(i, j, newPixel);
        }
    }
}