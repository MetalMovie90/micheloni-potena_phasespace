// Autori del codice
/*

   Ciro Potena, Alessio Micheloni

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
int MARKER_COUNT;
int INIT_FLAGS;
int No_error;


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
    INIT_FLAGS = 1;
    MARKER_COUNT = 72;
    No_error = 0;
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

int GetData(char* oggetto, int p, int n){

	strcat(filename, oggetto);
	
	int rip = 0;
	char ripc[10];

	while(repeat >= 1)
	{
		char *filename_end = new char[40];
		strcpy(filename_end, filename);
		repeat = 0;
		rip++;
        sprintf(ripc,"%d",rip);
		strcat(filename_end, ripc);
		strcat(filename_end, extension);

		Marker = fopen( filename_end ,"w");
	
		std::cout << "E' stato selezionato l'oggetto " << oggetto << ", numero " << p + 1 << " di " << n  << std::endl;
		std::cout << "Premere un tast per iniziare la prova" << std::endl;
	
		std::cin.get();
		std::cin.get();

    	int tracker;

    	if(owlInit("192.168.1.230", INIT_FLAGS) < 0)
    	{
        	std::cout << "Errore con il server" << std::endl;
        	return 0;
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
			return 0;
		}

    	// set default frequency
		owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

		// start streaming
		owlSetInteger(OWL_STREAMING, OWL_ENABLE);


        int n;
        int num = 0;
        int err;

		const float begin_time = clock();
		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_start )
		{	
		}
		std::cout << "\a" << std::endl;


		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_stop )
		{
	
            n = owlGetMarkers(markers, 72);
            if((err = owlGetError()) != OWL_NO_ERROR)
    		{
				std::cout << "Errore Owl"<<std::endl;
				return 0;
			}

            if(n>0)
            {
        	num=0;
				for(int i = 0; i < n; i++)
				{
					if(markers[i].cond > 0)
            		{
                		fprintf(Marker, "%d %f %f %f %f %d\n", i, markers[i].cond, markers[i].x, markers[i].y, markers[i].z, markers[i].id);
                		num++;

					}

				}
        		fprintf(Marker, "%f %d 0 0 0\n\n", float( clock() - begin_time )/CLOCKS_PER_SEC, num);
	    	}

    	}
	    std::cout << "\a" << std::endl;
	    fclose(Marker);
	    delete filename_end;
        No_error=1;
	    std::cout << "Vuoi ripetere la prova? (1 si, 0 no) " << std::endl;
	    std::cin >> repeat;

	    return 1;
	}
};


}; // class end
