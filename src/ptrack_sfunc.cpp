
// ptrack_sfunc.cpp
// simple point tracking program 

#define S_FUNCTION_NAME  ptrack_sfunc
#define S_FUNCTION_LEVEL 2

// change these to match your configuration
#define MARKER_COUNT 72
#define SERVER_NAME "192.168.1.230"
#define INIT_FLAGS 0
    
#include <stdio.h>
#include "owl.h"
#include "simstruc.h"

void owl_print_error(SimStruct *S, int n);

static void mdlInitializeSizes(SimStruct *S)
{
	ssSetNumSFcnParams(S, 0);
	ssSetNumContStates(S, 0);
    ssSetNumDiscStates(S, 0);

	ssSetNumInputPorts(S, 0);
	ssSetNumOutputPorts(S, 4);
    
    ssSetOutputPortMatrixDimensions(S, 0, 72, 1);
    ssSetOutputPortMatrixDimensions(S, 1, 72, 1);
    ssSetOutputPortMatrixDimensions(S, 2, 72, 1);
    ssSetOutputPortMatrixDimensions(S, 3, 1, 1);
    
    ssSetNumSampleTimes(S, 1);
    ssSetNumPWork(S, 2);
}

static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 0.1);
    ssSetOffsetTime(S, 0, 0.0);
}

#define MDL_START
#if defined(MDL_START) 
static void mdlStart(SimStruct *S)
{
    FILE *out_file;
    out_file = fopen("Dati.dat", "w");
    
    real_T *time = ssGetOutputPortRealSignal(S, 3);
    time[0] = 0;
    
	ssGetPWork(S)[0] = (void *) out_file;
    ssGetPWork(S)[1] = (void *) new OWLMarker[72];
    
    int tracker;

	if(owlInit(SERVER_NAME, INIT_FLAGS) < 0){
        ssSetErrorStatus(S, "Server Connection Error\n");
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
		owl_print_error(S, owlGetError());
		return;
	}

	// set default frequency
	owlSetFloat(OWL_FREQUENCY, OWL_MAX_FREQUENCY);

	// start streaming
	owlSetInteger(OWL_STREAMING, OWL_ENABLE);
}
#endif

static void mdlOutputs(SimStruct *S, int_T tid)
{
    FILE *out_file = (FILE *) ssGetPWork(S)[0]; 
    OWLMarker *markers = (OWLMarker *) ssGetPWork(S)[1];
    
    real_T *xout = ssGetOutputPortRealSignal(S, 0);
    real_T *yout = ssGetOutputPortRealSignal(S, 1);
    real_T *zout = ssGetOutputPortRealSignal(S, 2);
    real_T *time = ssGetOutputPortRealSignal(S, 3); 
    
    int err;
    int num = 0;
    
    // get some markers
    int n = owlGetMarkers(markers, 72);
    
    // check for error
    if((err = owlGetError()) != OWL_NO_ERROR)
    {
		owl_print_error(S, err);
		return;
	}
    
	if(n > 0)
    {
        time[0] = time[0]+0.0021;
		for(int i = 0; i < n; i++)
		{
			if(markers[i].cond > 0)
            {
                fprintf(out_file, "%d %f %f %f %f\n", i, markers[i].cond, markers[i].x, markers[i].y, markers[i].z);
                xout[i] = markers[i].x;
                yout[i] = markers[i].y;
                zout[i] = markers[i].z;
                num++;
			}
		}
		printf("Time: %f seconds.\n", time[0]);
        printf("%d marker(s) found.\n\n", num);
        fprintf(out_file, "%f %d 0 0 0\n\n", time[0], num);
	}
}

void owl_print_error(SimStruct *S, int n)
{
  if(n < 0){ ssSetErrorStatus(S, "Error: n < 0\n"); }
  else if(n == OWL_NO_ERROR) { ssSetErrorStatus(S, "No Error\n"); }
  else if(n == OWL_INVALID_VALUE) { ssSetErrorStatus(S, "Error: Invalid Value\n"); }
  else if(n == OWL_INVALID_ENUM) { ssSetErrorStatus(S, "Error: Invalid Enum\n"); }
  else if(n == OWL_INVALID_OPERATION) { ssSetErrorStatus(S, "Error: Invalid Operation\n"); }
  else { ssSetErrorStatus(S, "Unknown Error\n"); }
}

static void mdlTerminate(SimStruct *S)
{
	// cleanup
    FILE *out_file = (FILE *) ssGetPWork(S)[0];
    OWLMarker *markers = (OWLMarker *) ssGetPWork(S)[1];
    delete markers;
    
    fclose(out_file);
	owlDone();
}

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
