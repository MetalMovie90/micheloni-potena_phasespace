// Autori del codice
/*

   Ciro Potena, Hamal Marino, Carlos Rosales, Alessio Micheloni

*/
//

// ROS headers
#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/point_cloud_conversion.h>
#include <pcl_conversions/pcl_conversions.h>

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
void kinectPCD(PhaseSpace* PS, ros::NodeHandle nh_)
{
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration td,inc = boost::posix_time::microseconds(T_STEP_KINECTPCD_MICROSEC);
	
    std::string topic = nh_.resolveName("/camera/depth_registered/points");
    ROS_INFO("waiting for a point_cloud2 on topic %s", topic.c_str());
	
    sensor_msgs::PointCloud2::ConstPtr scene_ptr (new sensor_msgs::PointCloud2);
    
	while(1 /*RUNNING*/) // ToDo: exit the loop when the Acquisition/PSGetData is done
	{
        scene_ptr = ros::topic::waitForMessage<sensor_msgs::PointCloud2>(topic, nh_, ros::Duration(3.0));
		if (!scene_ptr)
		{
			ROS_ERROR("no point_cloud2 has been received");
            return;
		}
		else
		{
			ROS_INFO("point_cloud read!");
		}

		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGBA>);
		pcl::fromROSMsg (*scene_ptr, *cloud_ptr);
		
		// the filename has the timestamp to sync with the rest of the data.
        PS->WritePCD( cloud_ptr, (double)td.seconds() + td.fractional_seconds()/1000000.0 );
		
		t += inc;

		td = t - boost::posix_time::microsec_clock::universal_time();
		if( !(td.fractional_seconds() < 0) )
        {
			usleep(td.fractional_seconds());
        }
        else
        {

        }
        std::cout << "[INFO] Exiting kinectPCD thread..." << std::endl;
	}

	std::cout << "[INFO] Exiting kinectRGB thread..." << std::endl;
    return;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "phasespace_kinect_acquisition_node");
	ros::NodeHandle nh;
    
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
			sleep(time_start);
			Marker->init_PhaseSpace(INIT_FLAGS, MARKER_COUNT,std::string("192.168.1.230"));
			
			// inizializza thread kinect RGB
			// inizializza thread kinect pcd
			
			// fai partire tutti i thread, compreso uno che fa soltanto Marker->GetData();
// 			Marker->GetData(objects.at(p), rip);
// 			Marker->stop_PhaseSpace();
// 			std::thread thrKinectPCD(kinectPCD, std::ref(Marker), std::ref(nh));
			
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
