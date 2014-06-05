#include "Intestazione.h"

// Costruttore
PhaseSpace::PhaseSpace(){

	T_start = 0;
	T_stop = 0;
	Subject[20];
    Task[20];
	Object[20];
	filename[30];
	Marker = NULL;
    strcpy(extension, ".dat");
	repeat = 1;
}

// Distruttore
PhaseSpace::~PhaseSpace(){
}

void PhaseSpace::GetInfo(char* name, char* task, int t1, int t2){

strcpy(Subject, name);
strcpy(Task, task);
T_start = t1;
T_stop = t2;

strcpy(filename,Subject);
strcat(filename,"_");
strcat(filename,Task);
strcat(filename,"_");


}

void PhaseSpace::GetData(char* oggetto, int p, int n){

	strcat(filename, oggetto);
	
	int rip = 0;
	char ripc[10];

	while(repeat >= 1){
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

		const float begin_time = clock();
		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_start )
		{
	
		fprintf(Marker," Sono prima dell'inizio della prova %f \n", float( clock() - begin_time )/CLOCKS_PER_SEC);
	
		}
		//_beep(600, 500);
		while(float( clock() - begin_time )/CLOCKS_PER_SEC<T_stop ){
	
		fprintf(Marker," Sono nella prova %f \n", float( clock() - begin_time )/CLOCKS_PER_SEC);
	
    	}
	   //_beep(600, 500);
	   fclose(Marker);
	   delete filename_end;
	   std::cout << "Vuoi ripetere la prova? (1 si, 0 no) " << std::endl;
	   std::cin >> repeat;
	}


}
