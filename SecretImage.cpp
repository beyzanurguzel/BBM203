#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    width = image.get_width();
    height = image.get_height();
    int upperArraySize = (width * width + width) / 2;
    int lowerArraySize = (width * width - width) / 2;
    upper_triangular = new int[upperArraySize];
    lower_triangular = new int[lowerArraySize];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    int startIndexU = 0;
    int startIndexL = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                upper_triangular[startIndexU]= image.get_data()[i][j];
                startIndexU += 1;
            }else {
                lower_triangular[startIndexL]= image.get_data()[i][j];
                startIndexL += 1;
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}

// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete(upper_triangular);
    delete(lower_triangular);
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.
    int upperIndex = 0;
    int lowerIndex = 0;
    for (int i = 0; i < height ; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) { // if column index is greater than row index or equal to the col index, it means the pixel is in the upper matrix
                image.set_pixel(i, j, upper_triangular[upperIndex]);
                upperIndex += 1;
            } else {  // else the pixel is in the lower matrix
                image.set_pixel(i, j, lower_triangular[lowerIndex]);
                lowerIndex += 1;
            }
        }
    }
    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    //delete(upper_triangular);
    //delete(lower_triangular);
    int width = image.get_width();
    int height = image.get_height();

    int upperArraySize = (width * width + width) / 2;
    int lowerArraySize = (width * width - width) / 2;
    upper_triangular = new int[upperArraySize];
    lower_triangular = new int[lowerArraySize];

    int startIndex = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = i; j < width; ++j) {
            upper_triangular[startIndex]= image.get_data()[i][j];
            startIndex += 1;
        }
    }
    startIndex = 0;
    for(int i = 1; i < height; ++i) {
        for (int j = 0; j < i + 1; ++j) {
            lower_triangular[startIndex] = image.get_data()[i][j];
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a single space.
    std::ofstream outFile(filename, std::ios::out);
    outFile << width << " " << height << std::endl;

    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    int upperSize = (width * width + width) / 2;
    int lowerSize = (width * width - width) / 2;for ( int i = 0; i < upperSize; ++i) {
        outFile << upper_triangular[i];
        if (i != upperSize - 1) {
            outFile << " ";
        }
    }
    outFile << std::endl;

    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    for (int i = 0; i < lowerSize; ++i) {
        outFile << lower_triangular[i];
        if (i != lowerSize - 1) {
            outFile << " ";
        }
    }
    outFile << std::endl;

    // close the file
    outFile.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream inFile(filename);
    int width, height;
    std::string line;
    if (std::getline(inFile, line)) {
        std::istringstream iss(line);
        iss >> width >> height;
    }

    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int upperSize = (width * width + width) / 2;
    int lowerSize = (width * width - width) / 2;

    // 3. Allocate memory for both arrays.
    int* upper = new int[upperSize];
    int* lower = new int[lowerSize];

    // 4. Read the upper_triangular array from the second line, space-separated.
    std::string secondLine;
    if (std::getline(inFile, line)) {
        std::istringstream iss(line);
        for (int i = 0; i < upperSize; ++i) {
            iss >> upper[i];
        }
    }

    // 5. Read the lower_triangular array from the third line, space-separated.
    std::string thirdLine;
    if (std::getline(inFile, line)) {
        std::istringstream iss(line);

        for (int i = 0; i < lowerSize; ++i) {
            iss >> lower[i];
        }
    }

    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.
    inFile.close();
    SecretImage secret_image(width, height, upper, lower);
    //delete[] upper;
    //delete[] lower;
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
