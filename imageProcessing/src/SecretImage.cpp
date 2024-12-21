#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {
    int** arr=image.get_data();
    int h=image.get_height();
    int w=image.get_width();
    int c=0;
    upper_triangular=new int[w*(w+1)/2];
    lower_triangular=new int[w*(w-1)/2];
    height=h;
    width=w;
    for (int i=0;i<h;i++) {
        for (int j=i;j<w;j++) {
            upper_triangular[c]=arr[i][j];
            c++;
        }
    }
    c=0;
    for (int i=1;i<h;i++) {
        for (int j=0;j<i;j++) {
            lower_triangular[c]=arr[i][j];
            c++;
        }
    }

    // TODO: Your code goes here.
    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    // 2. Fill both matrices with the pixels from the GrayscaleImage.
}// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {

    upper_triangular=upper;
    lower_triangular=lower;
    width=w;
    height=h;

    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
}



// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete [] upper_triangular;
    delete [] lower_triangular;
    upper_triangular=nullptr;
    lower_triangular=nullptr;
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);
    int upper=0;
    int lower=0;
    for (int i=0;i<height;i++) {
        for (int j=0;j<width;j++) {
            int val=0;
            if(j>=i) {
                val=upper_triangular[upper];
                upper++;
            }
            else {
                val=lower_triangular[lower];
                lower++;
            }

            image.set_pixel(i,j, val);
        }
    }
    // TODO: Your code goes here.

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {
    int upper=0;
    int lower=0;
    for (int i=0;i<image.get_height();i++) {
        for (int j=0;j<image.get_width();j++) {
            int val=image.get_pixel(i,j);
            if(j>=i) {
                upper_triangular[upper]=val;
                upper++;
            }
            else {
                lower_triangular[lower]=val;
                lower++;

            }
        }
    }

    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices
    // based on the GrayscaleImage given as the parameter.
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {
    std::fstream file(filename,std::ios::out);

    file<<width<<" "<<height<<std::endl;
    for (int i=0;i<width*(width+1)/2;i++) {
        int val=upper_triangular[i];
        file<<val;
        if(i<width*(width+1)/2-1) {
            file<<" ";
        }

    }
    file<<std::endl;
    for (int i=0;i<width*(width-1)/2;i++) {
        int val=lower_triangular[i];
        file << val;
        if (i < width * (width - 1) / 2 - 1) {
            file << " ";
        }
    }
    file<<std::endl;
    file.close();
    // TODO: Your code goes here.
    // 1. Write width and height on the first line, separated by a single space.
    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    // as the second line.
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {

    std::fstream file(filename,std::ios::in);



    std::string line;
    std::getline(file,line);
    std::stringstream ss1(line);

    std::string s1;

    ss1>>s1;
    int width= std::stoi(s1);
    ss1>>s1;
    int height= std::stoi(s1);

    int upper =0;
    int *upperArr=new int[width*(width+1)/2];
    int *lowerArr=new int[width*(width-1)/2];
    int lower=0;

    std::getline(file,line);
    std::stringstream ss2(line);
    std::string s2;

    while (ss2>>s2) {
        upperArr[upper]=std::stoi(s2);
        upper++;
    }

    std::getline(file,line);
    std::stringstream ss3(line);
    std::string s3;

    while (ss3>>s3) {
        lowerArr[lower]=std::stoi(s3);
        lower++;
    }

    file.close();


    SecretImage secret_image(width,height,upperArr,lowerArr);


    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    // 2. Calculate the sizes of the upper and lower triangular arrays.
    // 3. Allocate memory for both arrays.
    // 4. Read the upper_triangular array from the second line, space-separated.
    // 5. Read the lower_triangular array from the third line, space-separated.
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.


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
