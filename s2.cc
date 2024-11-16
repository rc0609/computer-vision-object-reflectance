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

Write a program s2 that uses this formula, along with the parameters computed in
 Program 1, to find the normal to the brightest surface spot on the sphere in each of
 the 3 images. Assume that this is the direction of the corresponding light source (why is
 it safe to assume this?). Finally, for the intensity of the light source, use the magnitude
 (brightness) of the brightest pixel found in the corresponding image. Scale the direction
 vector so that its length equals this value
*/

#include "image.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

int main(int argc, char* argv[]){
    //argc has to be 6, otherwise it output this message
    if (argc != 6) {
        printf("Usage: %s {input parameters filename} {input sphere image 1 filename} {input sphere image 2 filename} {input sphere image 3 filename} {output directions filename} \n", argv[0]);
        return 0;  
    }

    const string input_parameter(argv[1]); //Input paremeter filename
    const string input_sphere1(argv[2]); //Input sphere image 1 filename
    const string input_sphere2(argv[3]); //Input sphere image 2 filename
    const string input_sphere3(argv[4]); //Input sphere image 3 filename
    const string output_file(argv[5]); //Output direction filename

    //Read in x_center, y_center, radius from input parameter file
    int center_x, center_y, radius;
    ifstream parameter_file(input_parameter);
    if(!parameter_file.is_open()){
        cout << "Can't open parameter file " << input_parameter << endl;
        return 0;
    }

    parameter_file >> center_x >> center_y >> radius;
    parameter_file.close();

    //Vector to store input sphere image filenames
    vector<string> image_filename = {input_sphere1, input_sphere2, input_sphere3};
    ofstream output(output_file);
    if(!output.is_open()){
        cout << "Can't open output file " << output_file << endl;
        return 0;
    }

    //Go through each sphere images
    for(const auto& image_file : image_filename){
        Image sphere_image;
        if(!ReadImage(image_file, &sphere_image)){
            cout << "Can't open image file " << image_file << endl;
            return 0;
        }

        //Find brightest point in each images
        int max_x = 0;
        int max_y = 0;
        int max_intensity = 0;

        for(int i = 0; i < sphere_image.num_rows(); ++i){
            for(int j = 0; j < sphere_image.num_columns(); ++j){
                int pixel_value = sphere_image.GetPixel(i, j);

                if(pixel_value > max_intensity){
                    max_intensity = pixel_value; //Set the current pixel value as max intensity
                    max_x = j; //Set current column value as max x
                    max_y = i; //Set current row value as max y
                }
            }
        }

        //Calculate z coordinate of brightest point
        //z^2 = r^2 - (x - x0)^2 - (y - y0)^2
        double dz_square = static_cast<double> (radius*radius) - ((max_x - center_x) * (max_x - center_x)) - 
                                                ((max_y - center_y) * (max_y - center_y));

        if(dz_square < 0){
            cout << "brightest point is out of bound" << endl;
            return 0;
        }

        //z = +sqrt(r^2 - (x - x0)^2 - (y - y0)^2)
        double dz = sqrt(dz_square);

        //Calculate normal vector from center to brightest point
        double N1 = max_x - center_x;
        double N2 = max_y - center_y;
        double N3 = dz;

        //Normalize normal vector and scale based on max intensity
        double norm_normal_vector = sqrt(N1*N1 + N2*N2 + N3*N3);
        double norm_scale = max_intensity / norm_normal_vector;

        N1 = N1*norm_scale;
        N2 = N2*norm_scale;
        N3 = N3*norm_scale;

        //Write out direction in output direction file
        output << N1 << " " << N2 << " " << N3 << endl;

    }

    output.close();
    cout << "Output was saved to " << output_file << endl;
    return 0;

}


