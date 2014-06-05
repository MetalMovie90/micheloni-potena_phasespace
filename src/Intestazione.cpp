#include <iostream>
#include "Intestazione.h"
#include "owl.h"

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

	//std::cout << "\a" << std::endl;

    std::cout << "Inserire il nome del soggetto: " << std::endl;
	std::cin >> soggetto;
	
    std::cout << "Inserire il nome del task da eseguire: " << std::endl;
	std::cin >> task;

	std::cout << "Inserire il tempo per il bip iniziale e per quello finale " << std::endl;
	std::cin >> time_start;
	std::cin >> time_stop;

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

	for(unsigned int k=0; k<sample.size(); k++)
	{
		objects.push_back(str[k]);
	}


	int* p =new int(0);
	for(unsigned int k=0; k<sample.size(); k++)
	{
	
		PhaseSpace* Marker;
	    Marker = new PhaseSpace();
	    Marker->GetInfo(soggetto, task, time_start, time_stop);

	   srand (time(0));	 
	   *p = rand() % 10 ;

	   while(sample.at(*p) == 1)
	   {
      	   *p = (*p%9)+1;
	   }

	  
	   sample.at(*p) = 1;
	   Marker->GetData(objects.at(*p),k,n);


	   delete Marker;
	}
	delete p;
    owlDone();
    
	std::cout << "Prova terminata " << std::endl;
	std::cout << "Premere un tasto per terminare" << std::endl;
	std::cin.get();
	std::cin.get();

	return 0;
}