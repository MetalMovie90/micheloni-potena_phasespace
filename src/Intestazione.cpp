// Autori del codice
/*

   Ciro Potena, Alessio Micheloni

*/
//



#include "Intestazione.h"
#include "owl.h"

int main()
{
    
	int rip = 10;
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

    
	for(unsigned int k=0; k<sample.size(); k++)
	{
		objects.push_back(str[k]);
	}


	int* p = new int(0);
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
	   while ( Marker->GetData(objects.at(*p),k,rip)!=1 )
	   {
	   		delete Marker;
	   		PhaseSpace* Marker;
	   		Marker = new PhaseSpace();
	        Marker->GetInfo(soggetto, task, time_start, time_stop);
	   }


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
