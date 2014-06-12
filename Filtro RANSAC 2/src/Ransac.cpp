// Autori del codice
/*

   Ciro Potena, Hamal Marino, Carlos Rosales, Alessio Micheloni

*/
//



#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>


using namespace std;
using namespace cv;

Mat GetData(FILE* data)
{

    Mat out;
    int i=0;
    float c1,c2,c3,c4,c5;
    float m1,m2,m3,M5;
    vector<float> time;
    M5 = 0;

    while(fscanf(data, "%f %f %f %f %f \n", &c1,&c2,&c3,&c4,&c5)==5)
    {       

        if(c1==5 && c3!= 0 && c4 !=0 && c5!=0)
        {

            m1 = c3; m2 = c4; m3 = c5; M5=1;

        }

        if(c3==0 && c4== 0 && c5==0)
        { 
            time.push_back(c1);
            cout << time[i] << endl;
            i++;

            if(M5==1)
            {
                M5=0;
                out.push_back( Point3f( m1,m2,m3 ) );
                out.push_back( Point3f( c1,0,0 ) );

            }
     
        }

    }

    return out;

}

int main()
{
    FILE *Marker;   
    Marker = fopen( "alessio_ransac_telefonino1.dat" ,"r");
    

 
    Mat Marker5 = GetData(Marker);



cout << Marker5 << endl;
	return 0;
}
