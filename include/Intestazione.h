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

        int n;
        int num = 0;
        int err;

		const float begin_time = clock();
		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_start )
		{
	
		//fprintf(Marker," Sono prima dell'inizio della prova %f \n", float( clock() - begin_time )/CLOCKS_PER_SEC);
	
		}
		std::cout << "\a" << std::endl;


		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_stop )
		{
	
            n = owlGetMarkers(markers, 72);
            std::cout << n << std::endl;
            if((err = owlGetError()) != OWL_NO_ERROR)
    		{
				std::cout << "Errore Owl"<<std::endl;
				return;
			}

            if(n>0)
            {
        num=0;
		//fprintf(Marker," Sono nella prova %f \n", float( clock() - begin_time )/CLOCKS_PER_SEC);
				for(int i = 0; i < n; i++)
				{
					if(markers[i].cond > 0)
            		{
                		fprintf(Marker, "%d %f %f %f %f \n", i, markers[i].cond, markers[i].x, markers[i].y, markers[i].z);
                		num++;

					}

				}
        		fprintf(Marker, "%f %d 0 0 0\n\n", float( clock() - begin_time )/CLOCKS_PER_SEC, num);
	    	}

    	}
	   std::cout << "\a" << std::endl;
	   fclose(Marker);
	   delete filename_end;

	   std::cout << "Vuoi ripetere la prova? (1 si, 0 no) " << std::endl;
	   std::cin >> repeat;
	}
};


}; // class end