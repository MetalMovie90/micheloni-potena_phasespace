// Autori del codice
/*

   Ciro Potena, Hamal Marino, Carlos Rosales, Alessio Micheloni

*/
//

#define T_STEP_KINECTRGB_MICROSEC 1000000.0/15.0
#define T_STEP_KINECTPCD_MICROSEC 1000000.0/15.0

#include "Phase_Space.h"
#include "owl.h"
#include <thread>


// dichiarazione thread
//*******************************************************************************************
// getData THREAD FUNCTION
//*******************************************************************************************
void PSGetData(PhaseSpace* PS,char* object, int rip)
{
	PS->GetData(object,rip);
	
	PS->stop_PhaseSpace();
	std::cout << "[INFO] Exiting PSGetData thread..." << std::endl;
	return;
}

//*******************************************************************************************
// kinectRGB THREAD FUNCTION
//*******************************************************************************************
void* kinectRGB(void* noarg)
{
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration td,inc = boost::posix_time::microseconds(T_STEP_KINECTRGB_MICROSEC);

	while(1 /*RUNNING*/)
	{
		//istruzioni
		
		
		t += inc;

		td = t - boost::posix_time::microsec_clock::universal_time();
		if( !(td.fractional_seconds() < 0) )
        {
			usleep(td.fractional_seconds());
			//cout << "Wait interval (ATI): " << td << endl;
        }
        else
        {
        	//cout << "Wait interval (NEG-ATI): " << td << endl;
        }
	}

	std::cout << "[INFO] Exiting kinectRGB thread..." << std::endl;
	return NULL;
}


int main()
{
    
	int MARKER_COUNT = 72;
	int INIT_FLAGS = 0; 
	int n = 10;
	char soggetto[20];
	char task[20];
	char oggetto[20];
	std::vector<char*> objects;
	std::vector<int> sample (10);
	int time_start, time_stop;	
	char str[10][20] = {"penna","dizionario","cancellino","occhiali","telefonino"
		               ,"asciuga capelli","bicchiere","tazza da the","asciugamano","coltello"};


	std::cout << "Inserire il nome del soggetto: " << std::endl;
	std::cin >> soggetto;
	
	std::cout << "Inserire il nome del task da eseguire: " << std::endl;
	std::cin >> task;

	std::cout << "Inserire il tempo per il bip iniziale e per quello finale " << std::endl;
	std::cin >> time_start;
	std::cin >> time_stop;
	std::cin.ignore(INT_MAX,'\n');

    
	for(unsigned int k=0; k<sample.size(); k++)
	{
		objects.push_back(str[k]);
	}


	int p = 0;
	for(unsigned int k=0; k<sample.size(); k++)
	{
	
		int rip = 0;
		PhaseSpace* Marker;
		Marker = new PhaseSpace();
		Marker->GetInfo(soggetto, task, time_start, time_stop);

		srand (time(0));
		p = rand() % objects.size();

		while(sample.at(p) == 1)
		{
      	   p = (p+1) % objects.size();
		}
	    char repeat = 'y';
		while(repeat=='y')
		{
			sample.at(p) = 1;
			std::cout << "E' stato selezionato l'oggetto " << objects.at(p) << ", numero " << k + 1 << " di " << n  << std::endl;
			std::cout << "Premere un tasto per iniziare la prova" << std::endl;
			rip++;
			std::cin.get();
			Marker->init_PhaseSpace(INIT_FLAGS, MARKER_COUNT,std::string("192.168.1.230"));
			
			// inizializza thread kinect RGB
			// inizializza thread kinect pcd
			
			// fai partire tutti i thread, compreso uno che fa soltanto Marker->GetData();
// 			Marker->GetData(objects.at(p), rip);
// 			Marker->stop_PhaseSpace();
			std::thread thrGetData(PSGetData, std::ref(Marker), std::ref(objects.at(p)), std::ref(rip));
			thrGetData.join();
			std::cout << "Vuoi ripetere la prova? (y si) " << std::endl;
			std::cin >> repeat;
			std::cin.ignore(INT_MAX,'\n');

		}

	   delete Marker;
	  
	}

	std::cout << "Prova terminata " << std::endl;
	std::cout << "Premere un tasto per terminare" << std::endl;
	std::cin.get();
	std::cin.get();

	return 0;
}
