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

Now you are ready to compute the surface normals of the object. Write a program
 s3 that given 3 images of an object, computes the normals to that objects surface as well
 as the albedo at each point. You should use the formulas in the class lecture notes. Be
 careful here! Make sure to take into account the different intensities of the light sources.
 Assume that a pixel (x, y) is visible from all 3 light sources if its brightness in all 3 images
 is greater than a certain threshold. Do not compute for pixels that are not visible in all
 images. The threshold is a parameter that will be supplied as a command line argument.
 So, for each pixel (x, y) with brighness greater than the threshold in all three images,
 compute the normal and albedo. Do not display normals for each pixel of the object.
 Display only for a grid of pixels, i.e. display the normals every N pixels along x and y
 axes. The value of N will be supplied as another command line argument, named step.
 The output normals image is input object image 1 with the normals ”superimposed”
 shown as a ”needle map”. Show the gridpoints in black (use value 0). You may want to
 draw a small circle (of radius 1 pixel) around each gridpoint to make it easily visible.
 From each grid point draw a line (a ”needle”) that is the projection of the normal vector
 at this point onto the image plane (again, use an orthographic projection). To make
 sure that this line is visible, scale the vector 10 times, after you normalize it (if it is
 normalized, the maximum projection will be 1 pixel, which is not very informative).
 Draw the needles in white color (255).
 The output albedo image contains the albedos. Compute the albedos for all pixels
 visible from all 3 light sources, scale them up or down to fit in the range 0...255 and
show them in the output image. Thus each pixel in the output image should be this
 pixels albedo scaled by a constant factor. Note that for pixels that an albedo cannot be
 calculated (see discussion about threshold above), just display black.
*/

#include "image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char *argv[]){
    //argc has to be 6, otherwise it output this message
    if (argc != 9) {
        printf("Usage: %s  {input directions filename} {input object image 1 filename} {input object image 2 filename} {input object image 3 filename} {input step parameter (integer greater than 0)} {input threshold parameter (integer greater than 0)} {output normals image filename} {output albedo image filename \n", argv[0]);
        return 0;  
    }

    const string input_direction(argv[1]); //Input directions filename
    const string input_object1(argv[2]); //Input object image 1 filename
    const string input_object2(argv[3]); //Input object image 2 filename
    const string input_object3(argv[4]); //Input object image 3 filename
    int step = stoi(argv[5]); //Input step value greater than 0
    int threshold = stoi(argv[6]); //Input threshold value greater than 0
    const string output_normal_image(argv[7]); //Output normal image filename
    const string output_albedo_image(argv[8]); //Output albedo image filename

    //Read in input direction
    vector<vector<double>> direction;
    ifstream direction_file(input_direction);
    if(!direction_file.is_open()){
        cout << "Can't open direction file " << input_direction << endl;
        return 0;
    }

    for(int i = 0; i < 3; ++i){
        double N1, N2, N3;
        direction_file >> N1 >> N2 >> N3;
        direction.push_back({N1,N2,N3});
    }

    direction_file.close();

    //Read in object images
    vector<Image> object_image(3);
    vector<string> object_filename = {input_object1, input_object2, input_object3};
    for(int i = 0; i < 3; ++i){
        if(!ReadImage(object_filename[i], &object_image[i])){
            cout << "Can't open object file " << object_filename[i] << endl;
            return 0;
        }
    }

    //For output images
    Image normal_image = object_image[0];
    Image albedo_image;
    albedo_image.AllocateSpaceAndSetSize(object_image[0].num_rows(), object_image[0].num_columns());
    albedo_image.SetNumberGrayLevels(255);

    double max_albedo = 0;
    vector<double> albedo_vec; //Store albedo values

    //Vector for light direction S
    vector<vector<double>> matrixS(3, vector<double>(3)); //3X3 matrix
    for(int k = 0; k < 3; ++k) {
            //Feed input direction from s2 to new vector
            matrixS[k][0] = direction[k][0];
            matrixS[k][1] = direction[k][1];
            matrixS[k][2] = direction[k][2];
    }

    //Determinant
    double det = matrixS[0][0] * (matrixS[1][1] * matrixS[2][2] - matrixS[1][2] * matrixS[2][1]) - 
                 matrixS[0][1] * (matrixS[1][0] * matrixS[2][2] - matrixS[1][2] * matrixS[2][0]) +
                 matrixS[0][2] * (matrixS[1][0] * matrixS[2][1] - matrixS[1][1] * matrixS[2][0]);
    if (det == 0) return 0; //Skip if it is singular matrix
    
    vector<vector<double>> invMatrixS(3, vector<double>(3)); //Inverted matrixS
    invMatrixS[0][0] = (matrixS[1][1] * matrixS[2][2] - matrixS[1][2] * matrixS[2][1]) / det;
    invMatrixS[0][1] = (matrixS[0][2] * matrixS[2][1] - matrixS[0][1] * matrixS[2][2]) / det;
    invMatrixS[0][2] = (matrixS[0][1] * matrixS[1][2] - matrixS[0][2] * matrixS[1][1]) / det;
    invMatrixS[1][0] = (matrixS[1][2] * matrixS[2][0] - matrixS[1][0] * matrixS[2][2]) / det;
    invMatrixS[1][1] = (matrixS[0][0] * matrixS[2][2] - matrixS[0][2] * matrixS[2][0]) / det;
    invMatrixS[1][2] = (matrixS[0][2] * matrixS[1][0] - matrixS[0][0] * matrixS[1][2]) / det;
    invMatrixS[2][0] = (matrixS[1][0] * matrixS[2][1] - matrixS[1][1] * matrixS[2][0]) / det;
    invMatrixS[2][1] = (matrixS[0][1] * matrixS[2][0] - matrixS[0][0] * matrixS[2][1]) / det;
    invMatrixS[2][2] = (matrixS[0][0] * matrixS[1][1] - matrixS[0][1] * matrixS[1][0]) / det;

    //Calculate normal and albedo
    for(int i = 0; i < object_image[0].num_rows(); ++i){
        for(int j = 0; j < object_image[0].num_columns(); ++j){

            int I1 = object_image[0].GetPixel(i,j);
            int I2 = object_image[1].GetPixel(i,j);
            int I3 = object_image[2].GetPixel(i,j);

            //Threshold the visible pixels
            if(I1 < threshold || I2 < threshold || I3 < threshold){
                albedo_image.SetPixel(i,j,0); //Set pixel below threshold black
                albedo_vec.push_back(0);
                continue;
            }

            //Vector storing intensity
            vector<double> I = {static_cast<double>(I1), static_cast<double>(I2), static_cast<double> (I3)};

            //Using the formula on lecture note, calculate normal
            vector<double> N(3, 0); //Vector for normal

            //Calculate invMatrixS * I
            for(int k = 0; k < 3; ++k){
                for(int h = 0; h < 3; ++h){
                    N[k] = N[k] + invMatrixS[k][h] * I[h];
                }
            }

            //Calculate albedo
            double albedo = sqrt((N[0]*N[0]) + (N[1]*N[1]) + (N[2]*N[2])); // ||N||
            if(albedo > max_albedo) max_albedo = albedo; 

            albedo_vec.push_back(albedo);

            //Normalize N 
            for(int k = 0; k < 3; ++k){
                N[k] = N[k] / albedo;
            }

            //Draw the needle
            if(i % step == 0 && j % step == 0){
                int centerX = i; //Normal line start x
                int centerY = j; //Normal line start y
                int line_length = 10; 

                DrawLine(centerX, centerY, centerX + static_cast<int>(line_length * N[1]),
                         centerY + static_cast<int>(line_length * N[0]), 255, &normal_image);

                //Draw small dot at starting of line for visibility
                for (int dx = -1; dx <= 1; ++dx) {  // Iterate over a small neighborhood around the center
                    for (int dy = -1; dy <= 1; ++dy) {
                        if (centerX + dx >= 0 && centerY + dy >= 0)  
                            normal_image.SetPixel(centerX + dx, centerY + dy, 255);  // Set pixel to white for visibility
                    }
                }
            }

            albedo_image.SetPixel(i,j,albedo);

        }
    }

    //Normalize albedo and scale it to [0,255]
    for(size_t i = 0; i < albedo_vec.size(); ++i){
        albedo_vec[i] = (albedo_vec[i] / max_albedo) * 255;
    }

    //Normalized albedo image
    int index = 0;
    for(int i = 0; i < object_image[0].num_rows(); ++i){
        for(int j = 0; j < object_image[0].num_columns(); ++j){
            albedo_image.SetPixel(i, j, static_cast<int>(albedo_vec[index]));
            ++index;
        }
    }

    //Save the results
    WriteImage(output_normal_image, normal_image);
    WriteImage(output_albedo_image, albedo_image);
    
    cout << "Maximum albedo value: " << max_albedo << endl;
    return 0;

}