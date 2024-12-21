#include "Crypto.h"

#include <valarray>

#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.
    GrayscaleImage image=secret_image.reconstruct();
    int h=image.get_height();
    int w=image.get_width();
    int bits=message_length*7;
    if(h*w<bits) {
        throw std::runtime_error("error");
    }

    int starting_pixel=h*w-bits;
    int row=starting_pixel/w;
    int col=starting_pixel%w;
    for (int i=row;i<h;i++) {
        if (LSB_array.size()==bits) {
            return LSB_array;
        }
        for (int j=(i==row ? col :0);j<w;j++) {
            if (LSB_array.size()==bits) {
                return LSB_array;
            }
            int lsb=image.get_pixel(i,j)%2;
            LSB_array.push_back(lsb);


        }
    }
    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    // 2. Calculate the image dimensions.
    // 3. Determine the total bits required based on message length.
    // 4. Ensure the image has enough pixels; if not, throw an error.
    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    // 6. Extract LSBs from the image pixels and return the result.

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    int l=LSB_array.size();

    int counter=0;
    while (counter<l) {
        int sum=0;
        int expo=6;
        for (int i=counter;i<counter+7;i++) {
            sum+=LSB_array[i]*static_cast<int>(pow(2,expo));
            expo--;
        }
        char a=static_cast<char>(sum);
        message+=a;

        counter+=7;
    }

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    // 2. Convert each group of 7 bits into an ASCII character.
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    for(int i=0;i<message.size();i++) {
        int a=static_cast<int>(message[i]);

        std::bitset<7> bits(a);

        for (int j=6;j>=0;j--) {
            LSB_array.push_back(bits[j]);

        }

    }
    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.
    // 3. Return the array of bits.

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {

    // TODO: Your code goes here.
    int h=image.get_height();
    int w=image.get_width();
    int l=LSB_array.size();
    if(h*w<l) {
        throw std::runtime_error("error");
    }
    int starting_index=h*w-l;
    int row=starting_index/w;
    int col=starting_index%w;
    int arr_index=0;
    for (int i=row;i<h;i++) {
        for (int j=(i==row ? col :0);j<w;j++) {
            if(arr_index>=l) {
                break;
            }
            int imValue=image.get_pixel(i,j);
            if(LSB_array[arr_index]==1) {
                if(imValue%2==0) {
                    image.set_pixel(i,j,imValue+1);
                }
            }else {
                if (imValue%2==1) {
                    image.set_pixel(i,j,imValue-1);

                }
            }
            arr_index++;
        }
    }
    SecretImage secret_image(image);
    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    // 2. Find the starting pixel based on the message length knowing that  
    //    the last LSB to embed should end up in the last pixel of the image.
    // 3. Iterate over the image pixels, embedding LSBs from the array.
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.

    return secret_image;
}
