/*
Ramisha Chowdhury
CSCI 493
Homework 4: In this programming assignment you are asked to develop a vision system that recovers the
 orientation and reflectance of an objects surface. For this purpose you will use photometric
 stereo. You will be given 3 images of an object taken using three different light sources. Your
 task is to compute the surface normals and albedo for this object. For this purpose, however,
 you will need to know the directions and intensities of the 3 light sources. Thus, in the first
 part of the assignment, you will compute the light sources directions and intensities from 4
 images of a sphere and use this information in the second part to recover the orientation and
 reflectance. The 7 greyscale images, sphere0.pgm ... sphere3.pgm and object1.pgm ...
 object3.pgm, can be found on Piazza. Your program will be tested also with additional test
 images we have taken.

First you need to find the location of the sphere and its radius. For this purpose
you will use the image sphere0, which is taken using many light sources (so that the
 entire front hemisphere is visible).
*/

#include "image.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char* argv[]){
    //argc has to be 4, otherwise it output this message
    if (argc != 4) {
        printf("Usage: %s {input gray–level sphere image} {input threhsold value} {output parameters file}\n", argv[0]);
        return 0;  
    }

    const string input_file(argv[1]); //Input gray-level sphere image
    int threshold = stoi(argv[2]); //Threshold value, I used 115
    const string output_file(argv[3]); //Output parameter files

    Image an_image;

    if (!ReadImage(input_file, &an_image)) {
        cout <<"Can't open file " << input_file << endl;
        return 0;
    }


//Check every pixel and compare it with threshld value. Set the pixel according to it
    for(int i = 0; i < an_image.num_rows(); ++i){
        for(int j = 0; j < an_image.num_columns(); ++j){
            const int pixel_value = an_image.GetPixel(i , j);
            if(pixel_value < threshold){
                an_image.SetPixel(i, j, 0);
            }
            else {
                an_image.SetPixel(i, j, 255);
            }
        }
    }


//Variables for calculating the center and radius
    int x_count = 0;
    int y_count = 0;
    int pixel_count = 0;
    int x_leftmost = an_image.num_columns();
    int x_rightmost = 0;
    int y_uppermost = an_image.num_rows();
    int y_lowermost = 0;

    //Calculate centroid and find extreme points for radius
    for (int i = 0; i < an_image.num_rows(); ++i) {
        for (int j = 0; j < an_image.num_columns(); ++j) {
            if (an_image.GetPixel(i, j) == 255) {
                x_count = x_count + j;
                y_count = y_count + i;
                pixel_count++;

                if (j < x_leftmost) x_leftmost = j;
                if (j > x_rightmost) x_rightmost = j;
                if (i < y_uppermost) y_uppermost = i;
                if (i > y_lowermost) y_lowermost = i;
            }
        }
    }

    //Calculate center and radius
    int center_x = x_count / pixel_count;
    int center_y = y_count / pixel_count;
    //Average  differences between the leftmost and the rightmost and the uppermost and the lowermost, then devide by 2
    int radius = ((x_rightmost - x_leftmost) / 2 + (y_lowermost - y_uppermost) / 2) / 2;

    // Write the result to the output file
    ofstream output(output_file);
    if (!output.is_open()) {
        cout << "Can't write to file " << output_file << endl;
        return 0;
    }
    output << center_x << " " << center_y << " " << radius << endl;
    output.close();

    return 0;
}
