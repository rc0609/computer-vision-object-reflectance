 This programming assignment is asked to develop a vision system that recovers the
 orientation and reflectance of an objects surface. For this purpose I am using photometric
 stereo. I am given 3 images of an object taken using three different light sources. My
 task is to compute the surface normals and albedo for this object. For this purpose, however,
 I need to know the directions and intensities of the 3 light sources. Thus, in the first
 part of the assignment, I will compute the light sources directions and intensities from 4
 images of a sphere and use this information in the second part to recover the orientation and
 reflectance. The 7 greyscale images, sphere0.pgm ... sphere3.pgm and object1.pgm ...
 object3.pgm. 
There aee few assumptions you can make about the capture settings:

• The surface of all objects (including the sphere) is Lambertian. This means there is only
 a diffuse peak in the reflectance map (no specular component).
 • For the images, assume orthographic projections.
 • Image files with the same indices are taken using the same light source. For example,
 sphere1.pgm and object1.pgm are taken using light source number 1 only. The image
 sphere0.pgm is taken using ambient illumination.
 • The objects maintain the same position, orientation and scale through the different im
ages– the only difference is the light source. For example, the sphere in sphere0.pgm
 ... sphere3.pgm has the same coordinates and the same radius.
 • The light sources are not in singular configuration, i.e. the S-matrix that you will compute
 should not be singular.
 • You may NOT assume that the light sources are of equal intensities. This means that
 you need to recover not only the directions of the light sources but also their intensities.

#Program 1 

 First find the location of the sphere and its radius. For this purpose
 I will use the image sphere0, which is taken using many light sources (so that the
 entire front hemisphere is visible).
 The program, s1 locates the sphere in an image and computes its center and
 radius. The program parameters are as follows:
 
 s1 {input gray–level sphere image} {input threhsold value} {output parameters file}
 
 Assuming an orthographic projection, the sphere projects into a circle on the image
 plane. It needs a thresholding for the greyscale image to obtain a binary one. Make sure to
 choose a good threshold, so that the circle in the resulting image looks clean. Then the program
 can find the location of the circle by computing its centroid. As for the radius, 
 average the differences between the leftmost and the rightmost and the uppermost and
 the lowermost points of the binary circle to obtain the diameter. Then divide by two to
 obtain the radius. The resulting parameters file is a text file consisting of a single line
 containing the
 x-coordinate of the center, the y-coordinate of the center, and the radius of the circle,
 separated by a space.

#Program 2

 Now we need to compute the directions and intensities of the light sources. For
 this purpose I am using the images sphere1.pgm ... sphere3.pgm. We need to
 derive a formula to compute the normal vector to the spheres surface at a given point,
 knowing the points coordinates (in the image coordinate frame), and the coordinates of
 the center and the radius of the spheres projection onto the image plane (again, assume
 an orthonormal projection). This formula should give you the resulting normal vector in
 a 3-D coordinate system, originating at the spheres center, having its x-axis and y-axis
 parallel respectively to the x-axis and the y-axis of the image, and z-axis chosen such
 as to form an orthonormal right-hand coordinate system. 
 The program s2 that uses this formula, along with the parameters computed in
 Program 1, to find the normal to the brightest surface spot on the sphere in each of
 the 3 images. Assume that this is the direction of the corresponding light source. 
 Finally, for the intensity of the light source, use the magnitude
 (brightness) of the brightest pixel found in the corresponding image. Scale the direction
 vector so that its length equals this value.
 Here are the program parameters:
 s2 {input parameters filename} {input sphere image 1 filename} {input sphere image 2
 filename} {input sphere image 3 filename} {output directions filename}

 The input parameters file is the one computed in Program 1. The image files are the 3
 images of the sphere (sphere 1 through 3, do not hardcode their names in the program!).
 The resulting directions file is a plain text file that consists of 3 lines. Line i contains
 the x-, y-, and z-components (separated by a space character) of the vector computed
 for light source i.

#Program 3

 Now we are ready to compute the surface normals of the object. The program
 s3 will, given 3 images of an object, computes the normals to that objects surface as well
 as the albedo at each point. Making sure to take into account the different intensities of the light sources.
 Assume that a pixel (x, y) is visible from all 3 light sources if its brightness in all 3 images
 is greater than a certain threshold. Do not compute for pixels that are not visible in all
 images. The threshold is a parameter that will be supplied as a command line argument.
 So, for each pixel (x, y) with brighness greater than the threshold in all three images,
 compute the normal and albedo. Do not display normals for each pixel of the object.
 Display only for a grid of pixels, i.e. display the normals every N pixels along x and y
 axes. The value of N will be supplied as another command line argument, named step.
 The programs parameters are as follows:
 s3 {input directions filename} {input object image 1 filename} {input object image 2
 filename} {input object image 3 filename} {input step parameter (integer greater than0)} 
 {input threshold parameter (integer greater than 0)} {output normals image
 filename} {output albedo image filename}

 Here, the input directions file is the file generated by s2. The 3 image files are the files
 of the object taken with light sources 1, 2, and 3, in this order. The step and threshold
 parameters were described above.
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



