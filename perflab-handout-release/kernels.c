/*******************************************
* Solutions for the CS:APP Performance Lab
********************************************/
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
* Please fill in the following student struct
*/
student_t student = {
"Sean Moon", /* Full name */
"u1401172@utah.edu", /* Email address */
};
/***************
* COMPLEX KERNEL
***************/
/******************************************************
* Your different versions of the complex kernel go here
******************************************************/
/*
* naive_complex - The naive baseline version of complex
*/
char naive_complex_descr[] = "naive_complex: Naive baseline implementation";
void naive_complex(int dim, pixel *src, pixel *dest)
{
int i, j;
for(i = 0; i < dim; i++)
for(j = 0; j < dim; j++)
{
dim)].red +
dim)].red +
dim)].red +
dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j,
(int)src[RIDX(i, j, dim)].green +
(int)src[RIDX(i, j, dim)].blue) / 3;
dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j,
(int)src[RIDX(i, j, dim)].green +
(int)src[RIDX(i, j, dim)].blue) / 3;
dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j,
(int)src[RIDX(i, j, dim)].green +
(int)src[RIDX(i, j, dim)].blue) / 3;
}
}
char complex_v5_descr[] = "complex_v5: v5 baseline implementation";
void complex_v5(int dim, pixel *src, pixel *dest)
{
int bs = 16;
for (int ii = 0; ii < dim; ii += bs)
{
int i_end = (ii + bs < dim) ? (ii + bs) : dim;
for (int jj = 0; jj < dim; jj += bs)
{
int j_end = (jj + bs < dim) ? (jj + bs) : dim;
// Process a BSxBS tile
for (int i = ii; i < i_end; i++)
{
// Start of this source row segment in the tile
pixel *srow = &src[RIDX(i, jj, dim)];
// Start of the destination column for this source row
pixel *dcol = &dest[RIDX(dim - 1 - jj, dim - 1 - i, dim)];
for (int j = jj; j < j_end; j++)
{
pixel p = *srow++; // Advance across the source row segment
int gray = (p.red + p.green + p.blue) / 3;
dcol->red = dcol->green = dcol->blue = (unsigned short)gray;
dcol -= dim; // Move up one row in the same destination column
}
}
}
}
}
/*
* complex - Your current working version of complex
* IMPORTANT: This is the version you will be graded on
*/
char complex_descr[] = "complex: Current working version";
void complex(int dim, pixel *src, pixel *dest)
{
complex_v5(dim, src, dest);
}
/*********************************************************************
* register_complex_functions - Register all of your different versions
* of the complex kernel with the driver by calling the
* add_complex_function() for each test function. When you run the
* driver program, it will test and report the performance of each
* registered test function.
*********************************************************************/
void register_complex_functions() {
add_complex_function(&complex, complex_descr);
add_complex_function(&naive_complex, naive_complex_descr);
}
/***************
* MOTION KERNEL
**************/
/***************************************************************
* Various helper functions for the motion kernel
* You may modify these or add new ones any way you like.
**************************************************************/
/*
*/
* weighted_combo - Returns new pixel value at (i,j)
static pixel weighted_combo(int dim, int i, int j, pixel *src)
{
int ii, jj;
pixel current_pixel;
int red, green, blue;
red = green = blue = 0;
int num_neighbors = 0;
for(ii=0; ii < 3; ii++)
for(jj=0; jj < 3; jj++)
if ((i + ii < dim) && (j + jj < dim))
{
num_neighbors++;
red += (int) src[RIDX(i+ii,j+jj,dim)].red;
green += (int) src[RIDX(i+ii,j+jj,dim)].green;
blue += (int) src[RIDX(i+ii,j+jj,dim)].blue;
}
current_pixel.red = (unsigned short) (red / num_neighbors);
current_pixel.green = (unsigned short) (green / num_neighbors);
current_pixel.blue = (unsigned short) (blue / num_neighbors);
return current_pixel;
}
/******************************************************
* Your different versions of the motion kernel go here
******************************************************/
/*
* naive_motion - The naive baseline version of motion
*/
char naive_motion_descr[] = "naive_motion: Naive baseline implementation";
void naive_motion(int dim, pixel *src, pixel *dst)
{
int i, j;
for (i = 0; i < dim; i++)
for (j = 0; j < dim; j++)
dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}
char motion_v4_descr[] = "motion_v4: v4 baseline implementation";
void motion_v4(int dim, pixel *src, pixel *dst)
{
for (int i = 0; i < dim - 2; i++)
{
// Initialize three column sums at j = 0
int CR1 = (int)src[RIDX(i, 0, dim)].red + (int)src[RIDX(i+1, 0, dim)].red
+ (int)src[RIDX(i+2, 0, dim)].red;
int CG1 = (int)src[RIDX(i, 0, dim)].green + (int)src[RIDX(i+1, 0,
dim)].green + (int)src[RIDX(i+2, 0, dim)].green;
int CB1 = (int)src[RIDX(i, 0, dim)].blue + (int)src[RIDX(i+1, 0, dim)].blue
+ (int)src[RIDX(i+2, 0, dim)].blue;
int CR2 = (int)src[RIDX(i, 1, dim)].red + (int)src[RIDX(i+1, 1, dim)].red
+ (int)src[RIDX(i+2, 1, dim)].red;
int CG2 = (int)src[RIDX(i, 1, dim)].green + (int)src[RIDX(i+1, 1,
dim)].green + (int)src[RIDX(i+2, 1, dim)].green;
int CB2 = (int)src[RIDX(i, 1, dim)].blue + (int)src[RIDX(i+1, 1, dim)].blue
+ (int)src[RIDX(i+2, 1, dim)].blue;
int CR3 = (int)src[RIDX(i, 2, dim)].red + (int)src[RIDX(i+1, 2, dim)].red
+ (int)src[RIDX(i+2, 2, dim)].red;
int CG3 = (int)src[RIDX(i, 2, dim)].green + (int)src[RIDX(i+1, 2,
dim)].green + (int)src[RIDX(i+2, 2, dim)].green;
int CB3 = (int)src[RIDX(i, 2, dim)].blue + (int)src[RIDX(i+1, 2, dim)].blue
+ (int)src[RIDX(i+2, 2, dim)].blue;
// Use fast sliding-window for j = 0 to dim-5
for (int j = 0; j < dim - 4; j++)
{
int sumR = CR1 + CR2 + CR3;
int sumG = CG1 + CG2 + CG3;
int sumB = CB1 + CB2 + CB3;
pixel out;
out.red = (unsigned short)(sumR / 9);
out.green = (unsigned short)(sumG / 9);
out.blue = (unsigned short)(sumB / 9);
dst[RIDX(i, j, dim)] = out;
// Slide window one step right: drop left col, shift, add new right col
CR1 = CR2; CG1 = CG2; CB1 = CB2;
CR2 = CR3; CG2 = CG3; CB2 = CB3;
int jp = j + 3;
CR3 = (int)src[RIDX(i, jp, dim)].red + (int)src[RIDX(i+1, jp, dim)].red
+ (int)src[RIDX(i+2, jp, dim)].red;
CG3 = (int)src[RIDX(i, jp, dim)].green + (int)src[RIDX(i+1, jp,
dim)].green + (int)src[RIDX(i+2, jp, dim)].green;
CB3 = (int)src[RIDX(i, jp, dim)].blue + (int)src[RIDX(i+1, jp,
dim)].blue + (int)src[RIDX(i+2, jp, dim)].blue;
}
// Last 4 columns in this row
dst[RIDX(i, dim - 4, dim)] = weighted_combo(dim, i, dim - 4, src);
dst[RIDX(i, dim - 3, dim)] = weighted_combo(dim, i, dim - 3, src);
dst[RIDX(i, dim - 2, dim)] = weighted_combo(dim, i, dim - 2, src);
dst[RIDX(i, dim - 1, dim)] = weighted_combo(dim, i, dim - 1, src);
}
// Bottom two rows: helper for all columns
for (int i = dim - 2; i < dim; i++)
{
for (int j = 0; j < dim; j++)
{
}
dst[RIDX(i, j, dim)] = weighted_combo(dim, i, j, src);
}
}
/*
* motion - Your current working version of motion.
* IMPORTANT: This is the version you will be graded on
*/
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst)
{
motion_v4(dim, src, dst);
}
/*********************************************************************
* register_motion_functions - Register all of your different versions
* of the motion kernel with the driver by calling the
* add_motion_function() for each test function. When you run the
* driver program, it will test and report the performance of each
* registered test function.
*********************************************************************/
void register_motion_functions() {
add_motion_function(&motion, motion_descr);
add_motion_function(&naive_motion, naive_motion_descr);
}