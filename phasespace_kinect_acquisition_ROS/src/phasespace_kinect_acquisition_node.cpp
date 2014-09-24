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

#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>


int time_start, time_stop;

// dichiarazione thread
//*******************************************************************************************
// getData THREAD FUNCTION
//*******************************************************************************************
void PSGetData(PhaseSpace* PS,char* object, int rip)
{
	PS->GetData(object,rip);
	
	std::cout << "[INFO] Exiting PSGetData thread..." << std::endl;
	return;
}

//*******************************************************************************************
// kinectPCD THREAD FUNCTION
//*******************************************************************************************
void kinectPCDimage(PhaseSpace* PS, ros::NodeHandle nh_, char* oggetto, int rip)
{

    std::string b = "/";
    std::string a = "mkdir ";
    std::string c,d,e;
    c.assign(PS->Subject);
    d.assign(oggetto);
    e.assign(PS->Task);
    a = a + c + e + b + d + b;
    system(a.c_str());

	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration td,inc = boost::posix_time::microseconds(T_STEP_KINECTPCD_MICROSEC);
	boost::posix_time::ptime init_t = t;
    std::string topic = nh_.resolveName("/camera/rgb/image_rect_color");
	
    sensor_msgs::Image::ConstPtr scene_ptr (new sensor_msgs::Image);
    
	while(PS->read == 1) // ToDo: exit the loop when the Acquisition/PSGetData is done
	{
		ROS_INFO("waiting for a image on topic %s", topic.c_str());
        scene_ptr = ros::topic::waitForMessage<sensor_msgs::Image>(topic, nh_);
        cv_bridge::CvImagePtr cv_ptr;
        cv_ptr = cv_bridge::toCvCopy(scene_ptr, sensor_msgs::image_encodings::BGR8);


		if (!scene_ptr)
		{
			ROS_ERROR("no image has been received");
            return;
		}
		else
		{
			ROS_INFO("image read!");
		}
		
		// the filename has the timestamp to sync with the rest of the data.
        PS->WriteIMAGE( cv_ptr->image, (double)td.seconds() + td.fractional_seconds()/1000000.0, std::ref(oggetto), rip );
		
		t += inc;

		td = t - boost::posix_time::microsec_clock::universal_time();
		if( !(td.fractional_seconds() < 0) )
        {
			usleep(td.fractional_seconds());
        }
        else
        {

        }
	}

	std::cout << "[INFO] Exiting kinectPCD thread..." << std::endl;
    return;
}


void kinectPCDpointcloud(PhaseSpace* PS, ros::NodeHandle nh_, char* oggetto, int rip)
{

	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration td,inc = boost::posix_time::microseconds(T_STEP_KINECTPCD_MICROSEC);
	boost::posix_time::ptime init_t = t;

	
    std::string topic = nh_.resolveName("/camera/depth_registered/points");
    sensor_msgs::PointCloud2::ConstPtr scene_ptr (new sensor_msgs::PointCloud2);
    
	while(PS->read == 1) // ToDo: exit the loop when the Acquisition/PSGetData is done
	{

		ROS_INFO("waiting for a point cloud on topic %s", topic.c_str());
        scene_ptr = ros::topic::waitForMessage<sensor_msgs::PointCloud2>(topic, nh_);


		if (!scene_ptr)
		{
			ROS_ERROR("no point cloud has been received");
            return;
		}
		else
		{
			ROS_INFO("point cloud read!");
		}
		
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_ptr (new pcl::PointCloud<pcl::PointXYZRGBA>);
        pcl::fromROSMsg(*scene_ptr, *cloud_ptr);

		// the filename has the timestamp to sync with the rest of the data.
        PS->WritePCD( cloud_ptr, (double)td.seconds() + td.fractional_seconds()/1000000.0, std::ref(oggetto), rip );
		
		t += inc;

		td = t - boost::posix_time::microsec_clock::universal_time();
		if( !(td.fractional_seconds() < 0) )
        {
			usleep(td.fractional_seconds());
        }
        else
        {

        }
	}

	std::cout << "[INFO] Exiting kinectPCD thread..." << std::endl;
    return;
}





int main(int argc, char **argv)
{
	ros::init(argc, argv, "phasespace_kinect_acquisition_node");
	ros::NodeHandle nh;
    
    PhaseSpace* Marker;
	int MARKER_COUNT = 72;
	int INIT_FLAGS = 0; 
	char soggetto[20];
	char task[20];
	char oggetto[20];
	std::vector<char*> objects;
	
	/*char str[40][20] = {"mela","martello","banana_spazzola","dado","palla_tennis"
		               ,"lampadina","boccale_birra","coperchio_pentola","bottiglia_vetro","teiera",
		                "sigaretta","smartphone","pc_portatile","tazza_the","gesso"
		               ,"penna","carta_gioco","bottone","bicchiere_plastica","pettine",
		                "forbici","cd-rom","cacciavite","pc_mouse","telecomando"
		               ,"spillatrice","dizionario","cucchiaio","portafoglio","scodella",
		                "cordless","chiave","spazzolino","scotch","uovo"
		               ,"chiave_inglese","padella","accendino","fotocamera","videocassetta"};*/
		               
    char str[18][20] = {"dentifr_spazzol","pettin_capelli","abbott_camicia","chiud_cerniera","mettere_calze"
		               ,"mettere_scarpe","bere","usare_cucchiaio","usare_colt_forc","versare_acqua",
		                "scri_foglio","utilizz_forbici","chiave_serrat","utilizz_smphone","chiodi_martello"
		               ,"pieg_asciug","aprire_lettera","mescolare"};

    

    int n = (sizeof(str)/sizeof(*str));
    std::vector<int> sample (n);

	std::cout << "Inserire il nome del soggetto: " << std::endl;
	std::cin >> soggetto;
	
	std::cout << "Inserire il nome del task da eseguire: " << std::endl;
	std::cin >> task;

    std::string a = "mkdir ";
    std::string c,d; 
    c.assign(soggetto);
    d.assign(task);
    a = a + c + d;

    system(a.c_str());


	std::cout << "Inserire il tempo per il bip iniziale e per quello finale " << std::endl;
	// std::cout << "(Il tempo iniziale deve essere di almeno 3 secondi per permettere una corretta inizializzazione della kinect) " << std::endl;
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
			// create subject/trial specific directory
			std::string b = "/";
		    std::string a = "mkdir ";
		    std::string c,d,e;
		    c.assign(Marker->Subject);
		    d.assign(objects.at(p));
		    e.assign(Marker->Task);
		    a = a + c + e + b + d + b;
		    system(a.c_str());

            Marker->read = 1;
			sample.at(p) = 1;
			std::cout << "E' stato selezionato l'oggetto " << objects.at(p) << ", numero " << k + 1 << " di " << n  << std::endl;
			std::cout << "Premere un tasto per iniziare la prova" << std::endl;
			rip++;
			std::cin.get();
			// std::thread thrKinectPCD(kinectPCDpointcloud, std::ref(Marker), std::ref(nh), std::ref(objects.at(p)),rip);
			sleep(time_start);
			Marker->init_PhaseSpace(INIT_FLAGS, MARKER_COUNT,std::string("192.168.1.230"));		
			std::thread thrGetData(PSGetData, std::ref(Marker), std::ref(objects.at(p)), std::ref(rip));
			thrGetData.join();
			// thrKinectPCD.join();
			
		    Marker->stop_PhaseSpace(); 
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
