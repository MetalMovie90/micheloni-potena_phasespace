// Autori del codice
/*

   Ciro Potena, Hamal Marino, Carlos Rosales, Alessio Micheloni

*/
//



#include "Phase_Space.h"
#include "owl.h"

void init_PhaseSpace(int INIT_FLAGS, int MARKER_COUNT){

int tracker;

    if(owlInit("192.168.1.230", INIT_FLAGS) < 0)
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

	// start streaming
	owlSetInteger(OWL_STREAMING, OWL_ENABLE);



};



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


	int* p =new int(0);
	for(unsigned int k=0; k<sample.size(); k++)
	{
	
	    int rip = 0;
		PhaseSpace* Marker;
	    Marker = new PhaseSpace();
	    Marker->GetInfo(soggetto, task, time_start, time_stop);

	   srand (time(0));	 
	   *p = rand() % 10 ;

	   while(sample.at(*p) == 1)
	   {
      	   *p = (*p%9)+1;
	   }
       char repeat = 'y';
	   while(repeat=='y'){
		   sample.at(*p) = 1;
		   std::cout << "E' stato selezionato l'oggetto " << objects.at(*p) << ", numero " << k + 1 << " di " << n  << std::endl;
		   std::cout << "Premere un tasto per iniziare la prova" << std::endl;
	       rip++;
		   std::cin.get();
		   init_PhaseSpace(INIT_FLAGS, MARKER_COUNT);
		   Marker->GetData(objects.at(*p), rip);
		   owlDone();
		   std::cout << "Vuoi ripetere la prova? (y si) " << std::endl;
		   std::cin >> repeat;
		   std::cin.ignore(INT_MAX,'\n');

	       
       }

	   delete Marker;
	  
	}
	delete p;
    
    
	std::cout << "Prova terminata " << std::endl;
	std::cout << "Premere un tasto per terminare" << std::endl;
	std::cin.get();
	std::cin.get();

	return 0;
}
