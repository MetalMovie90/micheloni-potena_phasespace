// Autori del codice
/*

   Ciro Potena, Hamal Marino, Carlos Rosales, Alessio Micheloni

*/
//


#include <iostream>
#include <time.h>
#include <vector>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <fcntl.h> 
#include "owl.h"
#include <boost/date_time/posix_time/posix_time.hpp>   


class PhaseSpace{

public:

int T_start, T_stop;
char  Subject[20];
char Task[20];
char filename[30];
char Object[20];
char extension[5];
FILE* Marker;
int repeat;
OWLMarker *markers;


PhaseSpace(){

	T_start = 0;
	T_stop = 0;
	Subject[20];
    Task[20];
	Object[20];
	filename[30];
	Marker = NULL;
    strcpy(extension, ".dat");
	repeat = 1;
    markers = new OWLMarker[72];
};

~PhaseSpace(){

	delete markers;
};

void GetInfo(char* name, char* task, int t1, int t2)
{

strcpy(Subject, name);
strcpy(Task, task);
T_start = t1;
T_stop = t2;

strcpy(filename,Subject);
strcat(filename,"_");
strcat(filename,Task);
strcat(filename,"_");


};

void GetData(char* oggetto, int p, int n){

	strcat(filename, oggetto);

	int rip = 0;
	char ripc[10];

	//while(repeat >= 1)
	//{

		char *filename_end = new char[40];
		strcpy(filename_end, filename);
		repeat = 0;
		rip++;
        sprintf(ripc,"%d",rip);
		strcat(filename_end, ripc);
		strcat(filename_end, extension);

		Marker = fopen( filename_end ,"w");
	


        int n_marker;
        int num = 0;
        int err;

        sleep(T_start);

		boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
		boost::posix_time::ptime init_t = t;
		boost::posix_time::time_duration td,inc = boost::posix_time::microseconds(1000000/OWL_MAX_FREQUENCY);
		std::cout << "\a" << std::endl;


		while(t < init_t + boost::posix_time::seconds(T_stop-T_start) )
		{
            t+=inc;
		    //std::cout << float( clock() - begin_time )/CLOCKS_PER_SEC << " begin_time: " << begin_time << " T_stop: " << T_stop << " clock: " << clock() << std::endl;
            n_marker = owlGetMarkers(markers, 72);
            if((err = owlGetError()) != OWL_NO_ERROR)
    		{
				std::cout << "Errore Owl"<<std::endl;
				return;
			}

            if(n_marker>0)
            {
        num=0;
		//fprintf(Marker," Sono nella prova %f \n", float( clock() - begin_time )/CLOCKS_PER_SEC);
				for(int i = 0; i < n_marker; i++)
				{
					if(markers[i].cond > 0)
            		{
                		fprintf(Marker, "%d %f %f %f %f \n", i, markers[i].cond, markers[i].x, markers[i].y, markers[i].z);
                		num++;

					}

				}
				td = t-init_t;
        		fprintf(Marker, "%f %d 0 0 0\n\n", (double)td.seconds() + td.fractional_seconds()/1000000.0, num);
	    	}
	    	td = t - boost::posix_time::microsec_clock::universal_time();
	    	if(!(td.fractional_seconds()<0))
	    	{
	    		usleep(td.fractional_seconds());
	    	}
	    	
    	}
	   std::cout << "\a" << std::endl;
	   fclose(Marker);
	   delete filename_end;


	//}
};


}; // class end
