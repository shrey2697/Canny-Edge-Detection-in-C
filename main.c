


#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <math.h> //for sqrt function
#define PI 3.14159265


//Sobel Mask Creation

void createSobel1(double Sobel1[][3])
{

    Sobel1[0][0]=-0.25;
    Sobel1[0][1]=0;
    Sobel1[0][2]=0.25;
    Sobel1[1][0]=-0.5;
    Sobel1[1][1]=0;
    Sobel1[1][2]=0.5;
    Sobel1[2][0]=-0.25;
    Sobel1[2][1]=0;
    Sobel1[2][2]=0.25;
}
void createSobel2(double Sobel2[][3])
{

    Sobel2[0][0]=-0.5;
    Sobel2[0][1]=-0.25;
    Sobel2[0][2]=-0.25;
    Sobel2[1][0]=0;
    Sobel2[1][1]=0;
    Sobel2[1][2]=0;
    Sobel2[2][0]=0.5;
    Sobel2[2][1]=0.25;
    Sobel2[2][2]=0.25;
}
int main( int argc, char** argv )
{
    //KERNEL CREATION

    double Sobel1[3][3];
    double Sobel2[3][3];

    createSobel1(Sobel1);
    createSobel2(Sobel2);
    //KERNEL CREATION ENDS
    int i,j,k,l;
    double val = 180/PI, angle;
   IplImage* img = cvLoadImage("lenagaussiansmooth1.JPG", CV_LOAD_IMAGE_GRAYSCALE); //VERTICAL GRAD
    IplImage* img1 = cvLoadImage("lenagaussiansmooth1.JPG", CV_LOAD_IMAGE_GRAYSCALE); //HORIZONTAL GRAD
     IplImage* img2 = cvLoadImage("lenagaussiansmooth1.JPG", CV_LOAD_IMAGE_GRAYSCALE); //GRAD MAGNITUDE
    IplImage* img3 = cvLoadImage("lenagaussiansmooth1.JPG", CV_LOAD_IMAGE_GRAYSCALE); //GRAD DIRECTION

     if (!img) //check whether the image is loaded or not
     {
          printf("Image cannot be loaded");

          return -1;
     }
     if (!img1) //check whether the image is loaded or not
     {
          printf("Image cannot be loaded");

          return -1;
     }
    cvNamedWindow("Before",CV_WINDOW_AUTOSIZE);
     cvShowImage("Before",img);  //Display the original image that has to be blurred
     cvWaitKey(0);  //Wait for image for infinite time

  for(i=1; i<(img->height); i++)   //Go through each row element
     {
         for( j=1; j<(img->width); j++)   //Go through each column element
         {

            //Convolving SOBEL VERTICAL Matrix with the pixel and with its 8-connected pixels
             CV_IMAGE_ELEM(img,uchar,i,j) = (CV_IMAGE_ELEM(img,uchar,i,j)*Sobel1[1][1])+(CV_IMAGE_ELEM(img,uchar,i,j-1)*Sobel1[1][0])+
                                            (CV_IMAGE_ELEM(img,uchar,i,j+1)*Sobel1[1][2])+ (CV_IMAGE_ELEM(img,uchar,i-1,j)*Sobel1[0][1])+
                                            (CV_IMAGE_ELEM(img,uchar,i+1,j)*Sobel1[2][1])+ (CV_IMAGE_ELEM(img,uchar,i-1,j-1)*Sobel1[0][0])+
                                            (CV_IMAGE_ELEM(img,uchar,i-1,j+1)*Sobel1[0][2])+ (CV_IMAGE_ELEM(img,uchar,i+1,j-1)*Sobel1[2][0])+
                                            (CV_IMAGE_ELEM(img,uchar,i+1,j+1)*Sobel1[2][2]);

         }
     }
   for(k=1; k<(img1->height); k++)   //Go through each row element
     {
         for( l=1; l<(img1->width); l++)   //Go through each column element
         {

            //Convolving SOBEL HORIZONTAL Matrix with the pixel and with its 8-connected pixels
             CV_IMAGE_ELEM(img1,uchar,k,l) = (CV_IMAGE_ELEM(img1,uchar,k,l)*Sobel2[1][1])+(CV_IMAGE_ELEM(img1,uchar,k,l-1)*Sobel2[1][0])+
                                            (CV_IMAGE_ELEM(img1,uchar,k,l+1)*Sobel2[1][2])+ (CV_IMAGE_ELEM(img1,uchar,k-1,l)*Sobel2[0][1])+
                                            (CV_IMAGE_ELEM(img1,uchar,k+1,l)*Sobel2[2][1])+ (CV_IMAGE_ELEM(img1,uchar,k-1,l-1)*Sobel2[0][0])+
                                            (CV_IMAGE_ELEM(img1,uchar,k-1,l+1)*Sobel2[0][2])+ (CV_IMAGE_ELEM(img1,uchar,k+1,l-1)*Sobel2[2][0])+
                                            (CV_IMAGE_ELEM(img1,uchar,k+1,l+1)*Sobel2[2][2]);

         }
     }

    int o,p;
     for(o=1; o<(img2->height); o++)   //Go through each row element
     {
         for( p=1; p<(img2->width); p++)   //Go through each column element
         {
                //create the resultant of the two gradients
             CV_IMAGE_ELEM(img2,uchar,o,p) =   sqrt((CV_IMAGE_ELEM(img,uchar,o,p)* CV_IMAGE_ELEM(img,uchar,o,p))
                                                   +  (CV_IMAGE_ELEM(img1,uchar,o,p)* CV_IMAGE_ELEM(img1,uchar,o,p)));
        }
     }


// Normalize the angles to the four axes and diagonal
double res, max=0;
double arr[500][500];
for(i=1;i<458;i++)
{
    for(j=1;j<432;j++)
    {
        if(CV_IMAGE_ELEM(img1,uchar,i,j)!=0)
        {
            res = atan(CV_IMAGE_ELEM(img,uchar,i,j) / CV_IMAGE_ELEM(img1,uchar,i,j)) ;
            arr[i][j] = res * val;
        }
         if(0<=arr[i][j] && arr[i][j]<45 )
            arr[i][j] = 0;
        else
            if(45<=arr[i][j] && arr[i][j]<90)
            arr[i][j] = 45;
        else
            if(90<=arr[i][j] && arr[i][j]<135)
            arr[i][j] = 90;
        else
            if(135<=arr[i][j] && arr[i][j]<180)
            arr[i][j] = 135;


    }
}

// non maximum suppression
 int r,c,maximum;
    for( r = 1; r < img3->height; r++){
        for(c = 1; c < img3 -> width; c++){
            maximum = CV_IMAGE_ELEM(img2,uchar,r,c);

            if(arr[r][c] == arr[r-1][c]){
                if(CV_IMAGE_ELEM(img2,uchar,r-1,c) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r-1,c);
            }
            if(arr[r][c] == arr[r+1][c]){
                if(CV_IMAGE_ELEM(img2,uchar,r+1,c) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r+1,c);
            }
            if(arr[r][c] == arr[r][c+1]){
                if(CV_IMAGE_ELEM(img2,uchar,r,c+1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r,c+1);
            }
            if(arr[r][c] == arr[r][c-1]){
                if(CV_IMAGE_ELEM(img2,uchar,r,c-1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r,c-1);
            }
            if(arr[r][c] == arr[r+1][c+1]){
                if(CV_IMAGE_ELEM(img2,uchar,r+1,c+1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r+1,c+1);
            }
            if(arr[r][c] == CV_IMAGE_ELEM(img3,uchar,r+1,c-1)){
                if(CV_IMAGE_ELEM(img2,uchar,r+1,c-1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r+1,c-1);
            }
            if(arr[r][c] == CV_IMAGE_ELEM(img3,uchar,r-1,c+1)){
                if(CV_IMAGE_ELEM(img2,uchar,r-1,c+1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r-1,c+1);
            }
            if(arr[r][c] == CV_IMAGE_ELEM(img3,uchar,r-1,c-1)){
                if(CV_IMAGE_ELEM(img2,uchar,r-1,c-1) > CV_IMAGE_ELEM(img2,uchar,r,c))
                    maximum = CV_IMAGE_ELEM(img2,uchar,r-1,c-1);
            }
            if(maximum != CV_IMAGE_ELEM(img2,uchar,r,c))
                CV_IMAGE_ELEM(img2,uchar,r,c) = 0;
        }
    }

// double thresholding
double pixVal;
for(i=1; i<img2->height; i++)
{
    for(j=1; j<img2->width; j++)
    {
       if( 240 > CV_IMAGE_ELEM(img2, uchar, i,j) || 255 < CV_IMAGE_ELEM(img2, uchar, i,j)  )
        CV_IMAGE_ELEM(img2, uchar, i,j) = 0;
        pixVal = CV_IMAGE_ELEM(img2, uchar, i,j);


    }
}

//hysteresis
for(i=1; i<img2->height; i++)
{
    for(j=1; j<img2->width; j++)
    {
        if(CV_IMAGE_ELEM(img2,uchar,i,j+1) || CV_IMAGE_ELEM(img2,uchar,i,j-1) ||
           CV_IMAGE_ELEM(img2,uchar,i+1,j) || CV_IMAGE_ELEM(img2,uchar,i-1,j) ||
           CV_IMAGE_ELEM(img2,uchar,i+1,j+1) || CV_IMAGE_ELEM(img2,uchar,i+1,j-1) ||
           CV_IMAGE_ELEM(img2,uchar,i-1,j+1) || CV_IMAGE_ELEM(img2,uchar,i-1,j-1))

            CV_IMAGE_ELEM(img2,uchar,i,j) = 0;
    }
}


cvNamedWindow("After", CV_WINDOW_AUTOSIZE);
cvShowImage("After", img2);
cvWaitKey(0);




}

