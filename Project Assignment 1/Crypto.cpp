#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int width = image.get_width();
    int height = image.get_height();
    int allPixels = width * height;

    // 3. Determine the total bits required based on message length.
    int neededBits = message_length * 7;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (allPixels < neededBits) {
        std::cerr << "Error in Crypto::extract_LSBits() : There are not enough pixels for this message!";
    }

    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    int startingRow = (allPixels - neededBits) / width;
    int startingColumn = ((allPixels - neededBits) % width);

    // 6. Extract LSBs from the image pixels and return the result.
    for (int row = startingRow; row < height; row++) {
        if (row != startingRow) {
            startingColumn = 0;
        }
        for (int column = startingColumn; column < width; column++) {
            if (image.get_pixel(row, column) % 2 == 1) { // if the pixel is an odd number, then in binary, LSB is 1
                LSB_array.push_back(1);
            } else { // the pixel is an even number so, in binary, LSB is 0
                LSB_array.push_back(0);
            }
        }
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        std::cerr << "Error in Crypto::decrypt_message() : LSB_array size is not acceptable.";
        exit(1);
    }

    // 2. Convert each group of 7 bits into an ASCII character.
    std::string binaryToString ;
    int numberOfChars = LSB_array.size() / 7;
    for (int i = 0; i < numberOfChars; i++) {
        for (int j = 0; j < 7; j++) {
            binaryToString += std::to_string(LSB_array[i * 7 + j]);
        }
        int ascii_value = std::stoi(binaryToString, nullptr, 2);
        char character = static_cast<char>(ascii_value);

        // 3. Collect the characters to form the decrypted message.
        message += character;
        binaryToString.clear();
    }

    // 4. Return the resulting message.
    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    for (char character : message) {
        int ascii_value = static_cast<int>(character);
        for (int i = 6; i >= 0; i--) {
            LSB_array.push_back((ascii_value >> i) & 1);
        }
    }
    // 3. Return the array of bits.
    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {
    // TODO: Your code goes here.

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    int width = image.get_width();
    int height = image.get_height();
    int allPixels = width * height;
    int neededPixels = LSB_array.size();
    if (neededPixels > allPixels) {
        std::cerr << "Error in Crypto::embed_LSBits() : There are not enough pixels for this message!";
        exit(1);
    }

    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int startingRow = (allPixels - neededPixels) / width;
    int startingColumn = ((allPixels - neededPixels) % width);
    int LSB_index = 0;

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int row = startingRow; row < height; row++) {
        if (row != startingRow) {
            startingColumn = 0;
        }
        for (int column = startingColumn; column < width; column++) {
            if (LSB_array[LSB_index] == 1) {
                image.set_pixel(row, column, image.get_pixel(row, column) | 1);
                LSB_index += 1;
            } else {
                image.set_pixel(row, column, image.get_pixel(row, column) & ~1);
                LSB_index += 1;
            }
        }
    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage
    //    with the embedded message.
    SecretImage secret_image(image);
    return secret_image;
}
