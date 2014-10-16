micheloni-potena_phasespace
===========================


To run the code, first check if roscore is activated. If not, run:

$ roscore

in the command line. Maybe you should need "rosclean" before.



Then, the MAIN COMMAND is:

$ rosrun phasespace_kinect_acquisition_ROS phasespace_kinect_acquisition_node

which starts the code and the acquisition.



If you edit any file, then use:

$ catkin_make

in the /catkin_ws folder to build them. Then run the code.



We don't know why, but if you try to use Ctrl+C to stop the code 
in the terminal this doesn't work. The best (and by now only) way 
to stop the process is TO CLOSE the terminal!



(last edited by Alessio Micheloni 16/10/2014)