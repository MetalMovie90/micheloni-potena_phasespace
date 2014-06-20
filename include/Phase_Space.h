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

// PCL headers
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>


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
int read;


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
    read = 0;
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

void WritePCD(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud, double timeStamp, char* oggetto )
{
	pcl::PCDWriter writer;
	char *filename_tot = new char[40];
	strcpy(filename_tot, filename);
	strcat(filename_tot, oggetto);
    writer.writeBinaryCompressed(filename_tot + std::string("_") + boost::lexical_cast<std::string>(timeStamp) + std::string(".pcd"), *cloud);
}

void GetData(char* oggetto, int rip){

	
	char *filename_end = new char[40];
	char ripc[10];

        strcpy(filename_end, filename);
	    strcat(filename_end, oggetto);
		repeat = 0;
        sprintf(ripc,"%d",rip);
		strcat(filename_end, ripc);
		strcat(filename_end, extension);

		Marker = fopen( filename_end ,"w");
	
        int n_marker;
        int num = 0;
        int err;

        while(  owlGetMarkers(markers, 72) == 0  );

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
				for(int i = 0; i < n_marker; i++)
				{
					if(markers[i].cond > 0)
					{
						fprintf(Marker, "%d %f %f %f %f \n",i,markers[i].cond,markers[i].x, markers[i].y, markers[i].z);
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


};

void init_PhaseSpace(int INIT_FLAGS, int MARKER_COUNT,std::string IP_string){

	int tracker;

	if(owlInit(IP_string.c_str(), INIT_FLAGS) < 0)
	{
		std::cout << "Errore con il server" << std::endl;
		return;
	}

	// create tracker 0
	tracker = 0;
	owlTrackeri(tracker, OWL_CREATE, OWL_POINT_TRACKER);

	// set markers
	for(int i = 0; i < MARKER_COUNT; i++)
		owlMarkeri(MARKER(tracker, i), OWL_SET_LED, i);

	// activate tracker
	owlTracker(tracker, OWL_ENABLE);

	// flush requests and check for errors
	if(!owlGetStatus())
	{
		std:std::cout << "Flush Error" << std::endl;
		return;
	}

    // set default frequency
	owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

	this->start_streaming_PhaseSpace();
};

// start data streaming
void start_streaming_PhaseSpace()
{
 
    owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

	// start streaming
	owlSetInteger(OWL_STREAMING, OWL_ENABLE);
};

// stop data streaming
void stop_streaming_PhaseSpace()
{
 
    owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

	// stop streaming
	owlSetInteger(OWL_STREAMING, OWL_DISABLE);
};

// stop phaseSpace acquisition
void stop_PhaseSpace()
{
	owlDone();
}


}; // class end
