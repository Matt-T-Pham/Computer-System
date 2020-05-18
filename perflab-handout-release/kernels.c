/*******************************************
 * Solutions for the CS:APP Performance Lab
 ********************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

void assigner(int i, int j, int dim,pixel *dest,pixel *src);
void assigner1(int place, pixel *dest,pixel *src);
static pixel weighted_combo1(int dim, int i, int j, pixel *src,int flag);
static pixel weighted_combo2(int dim, int i, int j, pixel *src);
static pixel weighted_combo3(int dim, int i, int j, pixel *src);
static pixel weighted_combo5(int dim, int i, int j, pixel *src);
/* 
 * Please fill in the following student struct 
 */
student_t student = {
  "Matthew Pham",     /* Full name */
  "u0952138@utah.edu",  /* Email address */
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

      dest[RIDX(dim - j - 1, dim - i - 1, dim)].red = ((int)src[RIDX(i, j, dim)].red +
						       (int)src[RIDX(i, j, dim)].green +
						       (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].green = ((int)src[RIDX(i, j, dim)].red +
							 (int)src[RIDX(i, j, dim)].green +
							 (int)src[RIDX(i, j, dim)].blue) / 3;
      
      dest[RIDX(dim - j - 1, dim - i - 1, dim)].blue = ((int)src[RIDX(i, j, dim)].red +
							(int)src[RIDX(i, j, dim)].green +
							(int)src[RIDX(i, j, dim)].blue) / 3;
    }
}


/* 
 * Version 1
 */
/*
char version_one_description[] = "Version 1 common sub expression";
void version_one(int dim, pixel *src, pixel *dest)
{
  int i, j;
  pixel *pix;
  short current;
  for(i = 0; i < dim; i++)
    for(j = 0; j < dim; j++)
      {
	pix = &dest[RIDX(dim - j - 1, dim - i - 1, dim)];

	current = ((int)src[RIDX(i, j, dim)].red +
		   (int)src[RIDX(i, j, dim)].green +
		   (int)src[RIDX(i, j, dim)].blue) / 3;
		      
	(*pix).red=current;
	(*pix).blue=current;
	(*pix).green=current;    
      } 
}
*/
/* 
 * Version 2
 */
/*
char version_two_description[] = "Version 2 common sub expression moving the dimension out";
void version_two(int dim, pixel *src, pixel *dest)
{
  int i, j;
  pixel *pix;
  short current;
  int dimMinusOne, dimMinusI, dimMinusJ;
  dimMinusOne = dim - 1;

  for(i = 0; i < dim; i++){
    dimMinusI = dimMinusOne - i;
    for(j = 0; j < dim; j++)
      {
	
	dimMinusJ = dimMinusOne - j;
	pix = &dest[RIDX(dimMinusJ, dimMinusI, dim)];


	
	current = ((int)src[RIDX(i, j, dim)].red +
		   (int)src[RIDX(i, j, dim)].green +
		   (int)src[RIDX(i, j, dim)].blue) / 3;
		      
	(*pix).red=current;
	(*pix).blue=current;
	(*pix).green=current;   
	 
      } 
  }
}
*/


/* 
 * Version 3
 *
char version_three_description[] = "Version 3 attempt to unroll ";
void version_three(int dim, pixel *src, pixel *dest)
{
  int i, j;
  pixel *pix,*pix1,*pix2,*pix3;
  short current, current1 ,current2, current3;
  int dimMinusOne, dimMinusI, dimMinusJ;
  int dimi, dimj,dimi1,dimj1;
  int srcPix, srcPix1, srcPix2, srcPix3;
  dimMinusOne = dim - 1;

  for(i = 0; i < dim; i+=2){
    dimMinusI = dimMinusOne - i;
    dimi = i*dim;
    dimi1 = (i+1)*dim;
    for(j = 0; j < dim; j+=2)
      {
	
	dimMinusJ = dimMinusOne - j;
	dimj = dim * dimMinusJ;
	dimj1 = (dimj-1)*dim;

	pix = &dest[dimMinusI+dimj];
	pix1 = &dest[dimMinusI+dimj1];
	pix2 = &dest[dimMinusI-1+dimj];
	pix3 = &dest[dimMinusI-1+dimj1];
	
	srcPix = dimi+j;
	srcPix1 = dimi+j+1;
	srcPix2 = dimi1+j;
	srcPix3 = dimi1+j+1;


	current = ((int)src[srcPix].red +
		   (int)src[srcPix].green +
		   (int)src[srcPix].blue) / 3;
	current1 = ((int)src[srcPix1].red +
		    (int)src[srcPix1].green +
		    (int)src[srcPix1].blue) / 3;
	current2 = ((int)src[srcPix2].red +
		    (int)src[srcPix2].green +
		    (int)src[srcPix2].blue) / 3;
	current3 = ((int)src[srcPix3].red +
		    (int)src[srcPix3].green +
		    (int)src[srcPix3].blue) / 3;
		      
	(*pix).red=current;
	(*pix).blue=current;
	(*pix).green=current;

	(*pix1).red=current1;
	(*pix1).blue=current1;
	(*pix1).green=current1;

	(*pix2).red=current2;
	(*pix2).blue=current2;
	(*pix2).green=current2;

    	(*pix3).red=current3;
	(*pix3).blue=current3;
	(*pix3).green=current3;
      } 
  }
  for(; i < dim; i ++)
    {
      dimMinusI= dimMinusOne - i;
      dimi = (i + 1) *dim;
      for(j = 0; j < dim-1; j+=2)
	{
	  dimMinusJ = dimMinusOne - j;
	  dimj = dim * dimMinusJ;
	  dimj1 = (dimMinusJ-1)*dim;

	  srcPix = dimi + j;
	  srcPix1 = dimi+j+1;

	  pix = &dest[dimj+dimMinusI];
	  pix1 = &dest[dimj1 + dimMinusI];

	  current = ((int)src[srcPix].red +
		     (int)src[srcPix].green +
		     (int)src[srcPix].blue) / 3;
	  current1 = ((int)src[srcPix1].red +
		      (int)src[srcPix1].green +
		      (int)src[srcPix1].blue) / 3;

	  (*pix).red=current;
	  (*pix).blue=current;
	  (*pix).green=current;

	  (*pix1).red=current1;
	  (*pix1).blue=current1;
	  (*pix1).green=current1;

	  
	}
      for(; j <dim;j++){
	dimMinusJ = dimMinusOne - j;
	srcPix = dimi + j;

	pix = &dest[RIDX(dimMinusJ,dimMinusI,dim)];

	
	current = ((int)src[srcPix].red +
		   (int)src[srcPix].green +
		   (int)src[srcPix].blue) / 3;

	(*pix).red=current;
	(*pix).blue=current;
	(*pix).green=current;


      }
    }

}

*/

/* 
 * Version 4
 */
/*
char version_four_description[] = "Version 4:loop unroll 2 with helper method";
void version_four(int dim, pixel *src, pixel *dest)
{
  int i, j;


  int dimMinusOne, dimMinusI, dimMinusJ;
  int dimj,dimi1,dimj1;
  pixel *pix,*pix1,*pix2,*pix3;

  dimMinusOne = dim - 1;

  for(i = 0; i < dim; i+=2){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1) * dim;
    for(j = 0; j < dim; j+=2)
      {
	
	dimMinusJ = dimMinusOne - j;

	dimj = dimMinusJ * dim;
	dimj1 = (dimMinusJ-1)*dim;

	pix = &dest[dimMinusI+dimj];
	pix1 = &dest[dimMinusI+dimj1];

	pix2 = &dest[dimMinusI-1+dimj];
	pix3 = &dest[dimMinusI-1+dimj1];

	assigner( i,  j,  dim,pix,src);
	assigner( i,  j+1,  dim,pix1,src);

	assigner( i+1,  j,  dim,pix2,src);
	assigner( i+1,  j+1,  dim,pix3,src);
      } 

  }

  for(; i < dim ; i ++){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1)*dim;

    for(j = 0; j< dim-1; j+=2){
      dimMinusJ = dimMinusOne - j;
      dimj = dimMinusJ * dim;
      dimj1 = (dimj-1)*dim;

      pix = &dest[dimMinusI+dimj];
      pix1 = &dest[dimMinusI+dimj1];

      assigner( i+1,  j,  dim,pix,src);
      assigner( i+1,  j+1,  dim,pix1,src);

    }
    for(; j <dim; j++){
      pix = &dest[RIDX(dimMinusJ,dimMinusI,dim)];
      assigner( i+1,  j,  dim,pix,src);
    }
  }
}
*/

/* 
 * Version 5
 */
/*
char version_five_description[] = "Version 5:loop unroll 2 with helper method: cleaned";
void version_five(int dim, pixel *src, pixel *dest)
{

  int i, j;

  int dimMinusOne, dimMinusI, dimMinusJ;
  int dimj,dimi1,dimj1;

  dimMinusOne = dim - 1;

  for(i = 0; i < dim-1; i+=2){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1) * dim;
    for(j = 0; j < dim-1; j+=2)
      {
	
	dimMinusJ = dimMinusOne - j;

	dimj = dimMinusJ * dim;
	dimj1 = (dimMinusJ-1)*dim;

	assigner(i,  j,  dim, &dest[dimMinusI+dimj],src);
	assigner(i,  j+1,  dim, &dest[dimMinusI+dimj1],src);

	assigner(i+1,  j,  dim,&dest[dimMinusI-1+dimj],src);
	assigner(i+1,  j+1,  dim,&dest[dimMinusI-1+dimj1],src);
      } 

  }

  for(; i < dim ; i ++){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1)*dim;

    for(j = 0; j< dim-1; j+=2){
      dimMinusJ = dimMinusOne - j;
      dimj = dimMinusJ * dim;
      dimj1 = (dimj-1)*dim;

      assigner( i+1,  j,  dim, &dest[dimMinusI+dimj],src);
      assigner( i+1,  j+1,  dim,&dest[dimMinusI+dimj1],src);

    }
    for(; j < dim; j++){
      assigner(i+1,  j,  dim, &dest[RIDX(dimMinusJ,dimMinusI,dim)] ,src);
    }
  }
}
*/


/* 
 * Version 6
 */
/*
char version_six_description[] = "Version 6:loop unroll 4 with helper method: cleaned";
void version_six(int dim, pixel *src, pixel *dest)
{
  int i, j;

  int dimMinusOne, dimMinusI; 
  int dimMinusJ = 0;
  int dimj,dimj1,dimj2,dimj3;

 

  dimMinusOne = dim - 1;

  for(i = 0; i < dim-3; i+=4){
    dimMinusI = dimMinusOne - i;


    for(j = 0; j < dim-3; j+=4)
      {
        
	dimMinusJ = dimMinusOne - j;

	dimj = dimMinusJ * dim;
	dimj1 = (dimMinusJ-1)*dim;
	dimj2 = (dimMinusJ-2)*dim;
	dimj3 = (dimMinusJ-3)*dim;


	assigner(i,  j,  dim, &dest[dimMinusI+dimj],src);
	assigner(i,  j+1,  dim, &dest[dimMinusI+dimj1],src);
	assigner(i,  j+2,  dim, &dest[dimMinusI+dimj2],src);
	assigner(i,  j+3,  dim, &dest[dimMinusI+dimj3],src);

	assigner(i+1,  j,  dim, &dest[dimMinusI-1+dimj],src);
	assigner(i+1,  j+1,  dim, &dest[dimMinusI-1+dimj1],src);
	assigner(i+1,  j+2,  dim, &dest[dimMinusI-1+dimj2],src);
	assigner(i+1,  j+3,  dim, &dest[dimMinusI-1+dimj3],src);

	assigner(i+2,  j,  dim, &dest[dimMinusI-2+dimj],src);
	assigner(i+2,  j+1,  dim, &dest[dimMinusI-2+dimj1],src);
	assigner(i+2,  j+2,  dim, &dest[dimMinusI-2+dimj2],src);
	assigner(i+2,  j+3,  dim, &dest[dimMinusI-2+dimj3],src);

	assigner(i+3,  j,  dim, &dest[dimMinusI-3+dimj],src);
	assigner(i+3,  j+1,  dim, &dest[dimMinusI-3+dimj1],src);
	assigner(i+3,  j+2,  dim, &dest[dimMinusI-3+dimj2],src);
	assigner(i+3,  j+3,  dim, &dest[dimMinusI-3+dimj3],src);

      } 

  }

  for(; i < dim ; i ++){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1)*dim;

    for(j = 0; j< dim-1; j+=4){
      dimj = dimMinusJ * dim;
      dimj1 = (dimMinusJ-1)*dim;
      dimj2 = (dimMinusJ-2)*dim;
      dimj3 = (dimMinusJ-3)*dim;

      assigner(i+1,  j,  dim, &dest[dimMinusI-1+dimj],src);
      assigner(i+1,  j+1,  dim, &dest[dimMinusI-1+dimj1],src);
      assigner(i+1,  j+2,  dim, &dest[dimMinusI-1+dimj2],src);
      assigner(i+1,  j+3,  dim, &dest[dimMinusI-1+dimj3],src);
    }
    for(; j < dim; j++){
      assigner(i+1,  j,  dim, &dest[RIDX(dimMinusJ,dimMinusI,dim)] ,src);
    }
  }
}
*/

/* 
 * Version 7
 */
char version_seven_description[] =  "Version 7: pull out multiplication of RIDX";
void version_seven(int dim, pixel *src, pixel *dest)
{
  int i, j;

  int dimMinusOne, dimMinusI; 
  int dimMinusJ = 0;
  int dimj,dimj1,dimj2,dimj3;
  int dimi,dimi1,dimi2,dimi3;

  dimMinusOne = dim - 1;

  for(i = 0; i < dim-3; i+=4){
    dimMinusI = dimMinusOne - i;

    dimi = i * dim;
    dimi1 = (i+1) * dim;
    dimi2 = (i+2) * dim;
    dimi3 = (i+3) * dim;

    for(j = 0; j < dim-3; j+=4)
      {
	
	dimMinusJ = dimMinusOne - j;

	dimj = dimMinusJ * dim;
	dimj1 = (dimMinusJ-1)*dim;
	dimj2 = (dimMinusJ-2)*dim;
	dimj3 = (dimMinusJ-3)*dim;


	assigner1(dimi+j,   &dest[dimMinusI+dimj],src);
	assigner1(dimi+j+1, &dest[dimMinusI+dimj1],src);
	assigner1(dimi+j+2, &dest[dimMinusI+dimj2],src);
	assigner1(dimi+j+3, &dest[dimMinusI+dimj3],src);

	assigner1(dimi1+j,   &dest[dimMinusI-1+dimj],src);
	assigner1(dimi1+j+1, &dest[dimMinusI-1+dimj1],src);
	assigner1(dimi1+j+2, &dest[dimMinusI-1+dimj2],src);
	assigner1(dimi1+j+3, &dest[dimMinusI-1+dimj3],src);

	assigner1(dimi2 +j,  &dest[dimMinusI-2+dimj],src);
	assigner1(dimi2+j+1, &dest[dimMinusI-2+dimj1],src);
	assigner1(dimi2+j+2, &dest[dimMinusI-2+dimj2],src);
	assigner1(dimi2+j+3, &dest[dimMinusI-2+dimj3],src);

	assigner1(dimi3+j,   &dest[dimMinusI-3+dimj],src);
	assigner1(dimi3+j+1, &dest[dimMinusI-3+dimj1],src);
	assigner1(dimi3+j+2, &dest[dimMinusI-3+dimj2],src);
	assigner1(dimi3+j+3, &dest[dimMinusI-3+dimj3],src);

      } 

  }

  for(; i < dim ; i ++){
    dimMinusI = dimMinusOne - i;
    dimi1 = (i+1)*dim;

    for(j = 0; j< dim-1; j+=4){
      dimj = dimMinusJ * dim;
      dimj1 = (dimMinusJ-1)*dim;
      dimj2 = (dimMinusJ-2)*dim;
      dimj3 = (dimMinusJ-3)*dim;

      assigner1(dimi1+j,   &dest[dimMinusI-1+dimj],src);
      assigner1(dimi1+j+1, &dest[dimMinusI-1+dimj1],src);
      assigner1(dimi1+j+2, &dest[dimMinusI-1+dimj2],src);
      assigner1(dimi1+j+3, &dest[dimMinusI-1+dimj3],src);
    }
    for(; j < dim; j++){
      assigner1(dimi1+j, &dest[RIDX(dimMinusJ,dimMinusI,dim)] ,src);
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
   version_seven(dim, src, dest);
}

/*********************************************************************
 * register_complex_functions - Register all of your different versions
 *     of the complex kernel with the driver by calling the
 *     add_complex_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_complex_functions() {
  add_complex_function(&complex, complex_descr);
  add_complex_function(&naive_complex, naive_complex_descr);
  // add_complex_function(&version_one, version_one_description);
  // add_complex_function(&version_two, version_two_description);
  // add_complex_function(&version_three, version_three_description);
  // add_complex_function(&version_four, version_four_description);
  // add_complex_function(&version_five,version_five_description);
  //  add_complex_function(&version_six,version_six_description);
  add_complex_function(&version_seven,version_seven_description);
 
}


/*
 * Helper to calculate and assign pixel
 *
 */
void assigner(int i, int j, int dim, pixel *dest,pixel *src)
{

  short current;

  current = ((int)src[RIDX(i, j, dim)].red +
	     (int)src[RIDX(i, j, dim)].green +
	     (int)src[RIDX(i, j, dim)].blue) / 3;
		      
  (*dest).red=current;
  (*dest).blue=current;
  (*dest).green=current;
}
void assigner1(int place, pixel *dest,pixel *src)
{

  short current;

  current = ((int)src[place].red +
	     (int)src[place].green +
	     (int)src[place].blue) / 3;
		      
  (*dest).red=current;
  (*dest).blue=current;
  (*dest).green=current;
}


/***************
 * MOTION KERNEL
 **************/

/***************************************************************
 * Various helper functions for the motion kernel
 * You may modify these or add new ones any way you like.
 **************************************************************/


/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
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



/* 
 * weighted_combo - Returns new pixel value at (i,j) 
 */
static pixel weighted_combo1(int dim, int i, int j, pixel *src,int flag) 
{
  if(!flag){
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
  else{
    int ii, jj;
    pixel current_pixel;

    int red, green, blue;
    red = green = blue = 0;

    for(ii = 0; ii < 3; ii++)
      for(jj = 0; jj < 3; jj++){
	red += (int) src[RIDX(ii+i,jj+j,dim)].red;
	green += (int) src[RIDX(ii+i,jj+j,dim)].green;
	blue += (int) src[RIDX(ii+i,jj+j,dim)].blue;
      }
    current_pixel.red = (unsigned short) (red / 9);
    current_pixel.green = (unsigned short) (green /9);
    current_pixel.blue = (unsigned short) (blue / 9);
  
    return current_pixel;
  }
}

static pixel weighted_combo2(int dim, int i, int j, pixel *src) 
{
   pixel current_pixel;

    int red, green, blue;
    red = green = blue = 0;

    int num_neighbors = 0;
    
    /*
      loop isn't necessary for only 3 iterations if statments are faster
    */
    if ((i  < dim) && (j  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i,j,dim)].red;
      green += (int) src[RIDX(i,j,dim)].green;
      blue += (int) src[RIDX(i,j,dim)].blue;
    } 
    if ((i  < dim) && (j+1  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i,j+1,dim)].red;
      green += (int) src[RIDX(i,j+1,dim)].green;
      blue += (int) src[RIDX(i,j+1,dim)].blue;
    }  
    if ((i  < dim) && (j+2 < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i,j+2,dim)].red;
      green += (int) src[RIDX(i,j+2,dim)].green;
      blue += (int) src[RIDX(i,j+2,dim)].blue;
    }
    if ((i+1  < dim) && (j  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+1,j,dim)].red;
      green += (int) src[RIDX(i+1,j,dim)].green;
      blue += (int) src[RIDX(i+1,j,dim)].blue;
    }
    if ((i+1  < dim) && (j+1  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+1,j+1,dim)].red;
      green += (int) src[RIDX(i+1,j+1,dim)].green;
      blue += (int) src[RIDX(i+1,j+1,dim)].blue;
    }
    if ((i+1  < dim) && (j+2  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+1,j+2,dim)].red;
      green += (int) src[RIDX(i+1,j+2,dim)].green;
      blue += (int) src[RIDX(i+1,j+2,dim)].blue;
    }
    if ((i+2  < dim) && (j  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+2,j,dim)].red;
      green += (int) src[RIDX(i+2,j,dim)].green;
      blue += (int) src[RIDX(i+2,j,dim)].blue;
    }
    if ((i+2  < dim) && (j+1  < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+2,j+1,dim)].red;
      green += (int) src[RIDX(i+2,j+1,dim)].green;
      blue += (int) src[RIDX(i+2,j+1,dim)].blue;
    }
    if ((i+2  < dim) && (j+2 < dim)){
      num_neighbors++;
      red += (int) src[RIDX(i+2,j+2,dim)].red;
      green += (int) src[RIDX(i+2,j+2,dim)].green;
      blue += (int) src[RIDX(i+2,j+2,dim)].blue;
    }

    current_pixel.red = (unsigned short) (red / num_neighbors);
    current_pixel.green = (unsigned short) (green / num_neighbors);
    current_pixel.blue = (unsigned short) (blue / num_neighbors);
  
    return current_pixel;

}


static pixel weighted_combo3(int dim, int i, int j, pixel *src) 
{
  //int ii, jj;
    pixel current_pixel;

    int red, green, blue;
    red = green = blue = 0;
    /*
        loop isn't necessary for only 3 iterations if statments are faster 
    */

    red += (int) src[RIDX(i,j,dim)].red;
    green += (int) src[RIDX(i,j,dim)].green;
    blue += (int) src[RIDX(i,j,dim)].blue;

    red += (int) src[RIDX(i,j+1,dim)].red;
    green += (int) src[RIDX(i,j+1,dim)].green;
    blue += (int) src[RIDX(i,j+1,dim)].blue;

    red += (int) src[RIDX(i,j+2,dim)].red;
    green += (int) src[RIDX(i,j+2,dim)].green;
    blue += (int) src[RIDX(i,j+2,dim)].blue;

    red += (int) src[RIDX(i+1,j,dim)].red;
    green += (int) src[RIDX(i+1,j,dim)].green;
    blue += (int) src[RIDX(i+1,j,dim)].blue;

    red += (int) src[RIDX(i+1,j+1,dim)].red;
    green += (int) src[RIDX(i+1,j+1,dim)].green;
    blue += (int) src[RIDX(i+1,j+1,dim)].blue;

    red += (int) src[RIDX(i+1,j+2,dim)].red;
    green += (int) src[RIDX(i+1,j+2,dim)].green;
    blue += (int) src[RIDX(i+1,j+2,dim)].blue;

    red += (int) src[RIDX(i+2,j,dim)].red;
    green += (int) src[RIDX(i+2,j,dim)].green;
    blue += (int) src[RIDX(i+2,j,dim)].blue;

    red += (int) src[RIDX(i+2,j+1,dim)].red;
    green += (int) src[RIDX(i+2,j+1,dim)].green;
    blue += (int) src[RIDX(i+2,j+1,dim)].blue;

    red += (int) src[RIDX(i+2,j+2,dim)].red;
    green += (int) src[RIDX(i+2,j+2,dim)].green;
    blue += (int) src[RIDX(i+2,j+2,dim)].blue;

    current_pixel.red = (unsigned short) (red / 9);
    current_pixel.green = (unsigned short) (green /9);
    current_pixel.blue = (unsigned short) (blue / 9);
  
    return current_pixel;

}


static pixel weighted_combo5(int dim, int i, int j, pixel *src) 
{
  //int ii, jj;
    pixel current_pixel;

    int red, green, blue;
    red = green = blue = 0;
    /*
        loop isn't necessary for only 3 iterations if statments are faster 
    */
    int dimi = i * dim;
    int dimi1 = (i+1) * dim;
    int dimi2 = (i+2) * dim;
    
    int current;
    current = dimi + j;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi + j + 1;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi + j + 2;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi1 + j;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;


    current = dimi1 + j + 1;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi1 + j + 2;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;


    current = dimi2 + j;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi2 + j + 1;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current = dimi2 + j + 2;
    red += (int) src[current].red;
    green += (int) src[current].green;
    blue += (int) src[current].blue;

    current_pixel.red = (unsigned short) (red / 9);
    current_pixel.green = (unsigned short) (green /9);
    current_pixel.blue = (unsigned short) (blue / 9);
  
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

/*
 * Version 1:Move the '< 3' clause outside to this loop 
 */
char version_one_motion_description[] = "Version One: move < 3 clause outside and take out the loop";
void version_one_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      if(i > dim-3 || j > dim -3)
	dst[RIDX(i, j, dim)] = weighted_combo1(dim, i, j, src,0);
      else
	dst[RIDX(i, j, dim)] = weighted_combo1(dim, i, j, src,1);	
}

/*
 * Version 2:seperated weighted combo get and get rid of the loop.
 */
char version_two_motion_description[] = "Version two: Seperate weighted combos";
void version_two_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      if(i > dim-3 || j > dim -3)
	dst[RIDX(i, j, dim)] = weighted_combo2(dim, i, j, src);
      else
	dst[RIDX(i, j, dim)] = weighted_combo3(dim, i, j, src);	
}


/*
 * Version 3:seperate RIDX in weighted combo
 */
char version_three_motion_description[] = "Version three: seperate RIDX";
void version_three_motion(int dim, pixel *src, pixel *dst) 
{
  int i, j;
    
  for (i = 0; i < dim; i++)
    for (j = 0; j < dim; j++)
      if(i > dim-3 || j > dim -3)
	dst[RIDX(i, j, dim)] = weighted_combo2(dim, i, j, src);
      else
	dst[RIDX(i, j, dim)] = weighted_combo5(dim, i, j, src);	
}


/*
 * motion - Your current working version of motion. 
 * IMPORTANT: This is the version you will be graded on
 */
char motion_descr[] = "motion: Current working version";
void motion(int dim, pixel *src, pixel *dst) 
{
  version_three_motion(dim, src, dst);
}

/********************************************************************* 
 * register_motion_functions - Register all of your different versions
 *     of the motion kernel with the driver by calling the
 *     add_motion_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_motion_functions() {
  add_motion_function(&motion, motion_descr);
  add_motion_function(&naive_motion, naive_motion_descr);
  //add_motion_function(&version_one_motion,version_one_motion_description);
  // add_motion_function(&version_two_motion,version_two_motion_description);
  add_motion_function(&version_three_motion,version_three_motion_description);
}
