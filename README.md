micheloni-potena_phasespace
===========================
(last edited by Alessio Micheloni 16/10/2014)


Of course, the first thing to do is to open the terminal.
To run the code, first check if roscore is activated. If not, run:
$ roscore
in the command line. Maybe you should need "rosclean" before.
Then, the MAIN COMMAND is:
$ rosrun phasespace_kinect_acquisition_ROS phasespace_kinect_acquisition_node
which starts the code and the acquisition.
If you edit any file, then use:
$ catkin_make
in the /catkin_ws folder to build them. Then run the code.


Summarizing, here are all the commands:
- $ catkin_make (to build edited files, in catkin_ws folder)
- $ rosclean purge (before roscore, answer yes when needed)
- $ roscore
- $ rosrun phasespace_kinect_acquisition_ROS phasespace_kinect_acquisition_node


TIPS AND TRICKS:
We don't know why, but if you try to use Ctrl+C to stop the code
in the terminal this doesn't work. The best (and by now only) way
to stop the process is TO CLOSE the terminal!