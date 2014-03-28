/*Andor example program showing the use of the SDK to
perform a single full image acquisition from
the CCD. The image is saved in file image.bmp*/

#include "qtoma2.h"

#ifdef ANDOR_

#include <stdio.h>

#include "atmcdLXd.h"
// data acquisition modes supported
#define SINGLE 1
#define ACCUMULATE 2
// trigger mode supported
#define INTERNAL 0
#define EXTERNAL 1



static int inited = 0;
static int ccd_width=0,ccd_height=0;
static int em_low=0, em_high=0;
static int em_gain = 1;
static int trig_mode = INTERNAL;

int andor(int n, char* args)
{
  
	static float ExposureTime = 0.1;
	static int SetTemp = 0;
	static int ac_mode = SINGLE;
	static int num_frames = 1;
	static AndorCapabilities caps;
	
	caps.ulSize = sizeof(AndorCapabilities); 
	
	float exp,accum, kinetic;
	
	unsigned long error;
	short temp1,temp2;
	
	char choice;
	float fChoice;
	int width, height,x0,y0;

    	at_32 lNumCameras;
	int status,i;
	at_32 *imageData;

    char txt[CHPERLN];

    //extern DATAWORD *datpt;
    //extern TWOBYTE header[];
    //extern int doffset,have_max;
    //extern char txt[],cmnd[];

    extern Image iBuffer;
	
    for( i=0; args[i] != 0; i++)
        args[i] = toupper(args[i]);
	
    if(*args == 0 && inited != 0) {
		// after initialized, let ANDOR command be used to acquire
        //index = 1;
        strncpy(args,"ACQ",3);
	}
	
    int* specs = iBuffer.getspecs();

	if(!inited){
		error = GetAvailableCameras(&lNumCameras);

		printf("Cameras: %d\n ",lNumCameras);

		if(lNumCameras == 0){
			printf("No camera\n");
            free(specs);
			return -1;
		}

		//Initialize CCD
		error = Initialize("/usr/local/etc/andor");

		if(error!=DRV_SUCCESS){
			//cout << "Initialisation error: " << error << endl;
			printf("Initialization error: %d\n" , error);
            free(specs);
			return(-1);
		}
	
		sleep(2); //sleep to allow initialization to complete

		//Get Detector dimensions
		GetDetector(&ccd_width, &ccd_height);
		printf("Detector is %d x %d pixels\n",ccd_width, ccd_height);

        temp1 = specs[COLS];
        temp2 = specs[ROWS];

        specs[COLS] = ccd_width;
        specs[ROWS] = ccd_height;
        specs[X0] = specs[Y0] = 0;
        specs[DX] = specs[DY] = 1;
        iBuffer.setspecs(specs);
        if(iBuffer.err()) {
            specs[COLS] = temp1;
            specs[ROWS] = temp2;
			inited = 1;
			printf("Problem initializing memory to CCD size.\n");
            iBuffer.setspecs(specs);
            free(specs);
			return -3;
		}
        update_UI();
        //printparams();
		CoolerON();
		SetTemperature(SetTemp);
		SetTriggerMode(trig_mode);
		
		GetEMGainRange(&em_low, &em_high);
		printf("EM gain range is %d - %d\n\n",em_low, em_high);
		
		GetCapabilities(&caps);
		
		if(caps.ulAcqModes & AC_ACQMODE_SINGLE)  
		   printf("Single Scan Acquisition Mode available.\n");
		if(caps.ulAcqModes & AC_ACQMODE_VIDEO )
		    printf("Video (Run Till Abort) Acquisition Mode available.\n"); 
		if(caps.ulAcqModes & AC_ACQMODE_ACCUMULATE )
		    printf("Accumulation Acquisition Mode available.\n");
		if(caps.ulAcqModes & AC_ACQMODE_KINETIC )
		    printf("Kinetic Series Acquisition Mode available.\n");		   
		if(caps.ulAcqModes & AC_ACQMODE_FRAMETRANSFER )
		    printf("Frame Transfer Acquisition Mode available.\n");		   
		if(caps.ulAcqModes & AC_ACQMODE_FASTKINETICS )
		    printf("Fast Kinetics Acquisition Mode available.\n");
		printf("\n");

 		if(caps.ulReadModes & AC_READMODE_FULLIMAGE )
		   printf("Full Image Read Mode available.\n"); 
		if(caps.ulReadModes & AC_READMODE_SUBIMAGE )
		   printf("Sub Image Read Mode available.\n"); 
		if(caps.ulReadModes & AC_READMODE_SINGLETRACK )
		   printf("Single track Read Mode available.\n"); 
		if(caps.ulReadModes & AC_READMODE_FVB )
		   printf("Full Vertical Binning Read Mode available.\n"); 
		if(caps.ulReadModes & AC_READMODE_MULTITRACK )
		   printf("Multi Track Read Mode available.\n"); 
		if(caps.ulReadModes & AC_READMODE_RANDOMTRACK )
		   printf("Random-Track Read Mode available.\n"); 
		printf("\n");
		
		if(trig_mode == EXTERNAL)
			printf("Trigger Mode: External\n");
		else 
			printf("Trigger Mode: Internal\n");
		
		
		
		
		error = SetReadMode(4);				// image mode only for now
		SetAcquisitionMode(ac_mode);		// default is single frame mode
		SetExposureTime(ExposureTime);
		
		inited = 1;
	}

    if(*args == 0){
        free(specs);
        return 0;
    }
	
	/*
	 EXPosure command
	 Specify exposure time in seconds
	 */
	
    if( strncmp(args,"EXP",3) == 0){
        sscanf(args,"%s %f",txt, &ExposureTime);
		if( ExposureTime < 0.0) ExposureTime = .1;
		error = SetExposureTime(ExposureTime);
		if( error == DRV_SUCCESS)
			printf("Exposure set to %g seconds.\n",ExposureTime);
		else
			printf("Error: %d\n",error);
		GetAcquisitionTimings(&exp,&accum, &kinetic); 
		printf("Actual Exposure Time: %g\n",exp);
		ExposureTime = exp;
        free(specs);
		return 0;
		
	/*
	TEMperature command
	Specify set temperature of the CCD in degrees C
	*/
		
    } else if ( strncmp(args,"TEM",3) == 0){
        sscanf(args,"%s %d",txt, &SetTemp);
		error = SetTemperature(SetTemp);
		if( error == DRV_SUCCESS)
			printf("Temperature set to %d degrees C.\n",SetTemp);
		else
			printf("Error: %d\n",error);
        free(specs);
		return error;
		
	/*
	 TRIgger command
	 Specify camera trigger mode
	 TRIG 1 (default) set to internal trigger
	 TRIG 2 -- set to external trigger mode
	 */
		
    } else if ( strncmp(args,"TRI",3) == 0){
        sscanf(args,"%s %d",txt, &i);
		if ( i == 2){
			trig_mode = EXTERNAL;
		} else {
			trig_mode = INTERNAL;
		}
		error = SetTriggerMode(trig_mode);
		if( error == DRV_SUCCESS)
			printf("Trigger mode set.\n",SetTemp);
		else
			printf("Error: %d\n",error);
		if(trig_mode == EXTERNAL)
			printf("Trigger Mode: External\n");
		else 
			printf("Trigger Mode: Internal\n");
        free(specs);
		return error;
	/*
	 MODe command
	 Specify set acquisition mode
	 Mode 1 (default) is single scan
	 Mode 2 is accumulate mode
	*/
		
    } else if ( strncmp(args,"MOD",3) == 0){
        sscanf(args,"%s %d",txt, &ac_mode);

		error = SetAcquisitionMode(ac_mode);
		if( error == DRV_SUCCESS)
			printf("Mode set to %d.\n",ac_mode);
		else
			printf("Error: %d\n",error);
		if (ac_mode == ACCUMULATE) {
			printf("%d Accumulation Frames.\n",num_frames);
			SetNumberAccumulations(num_frames);
			SetAccumulationCycleTime(0.0);		// for now, just accumulate as fast as possible
		}
        free(specs);
		return error;
		
	/*
	 FRAmes command
	 Specify the number of frames to be accumulated in ACCUMULATE mode
	*/
		
    } else if ( strncmp(args,"FRA",3) == 0){
        sscanf(args,"%s %d",txt, &num_frames);
		if(ac_mode == ACCUMULATE){
			error = SetNumberAccumulations(num_frames);
			if( error == DRV_SUCCESS)
				printf("Mode 2 will accumulate %d frames.\n",num_frames);
			else
				printf("Error: %d\n",error);
		} else {
			printf("Current Acquisition Mode: %d.\nAcquisition Mode 2 will accumulate %d frames.\n",
						ac_mode,num_frames);
		}
        free(specs);
		return error;
		
	/*
	GAIn command
	Specify the EM gian for the EMCCD -- typically a number between 1 and 255
	*/
		
    } else if ( strncmp(args,"GAI",3) == 0){
        sscanf(args,"%s %d",txt, &em_gain);
		if(em_gain < em_low || em_gain > em_high){
			printf("Illegal value.\n");
            free(specs);
			return -1;
		}
		error = SetEMCCDGain(em_gain);
		if( error == DRV_SUCCESS)
			printf("EMCCD Gain set to %d\n",em_gain);
		else
			printf("Error: %d\n",error);
        free(specs);
		return error;
		
	/*
	STAtus command
	Print out various camera parameters
	*/	
		
    } else if (strncmp(args,"STA",3) == 0){
		// get CCD info
		printf("Detector is %d x %d pixels\n",ccd_width, ccd_height);
		printf("EM gain range is %d - %d\n",em_low, em_high);
		GetEMCCDGain(&i);
		printf("EMCCD Gain: %d.\n\n",i);
		//
		printf("Exposure set to %g seconds.\n",ExposureTime);
		GetAcquisitionTimings(&exp,&accum, &kinetic); 
		printf("Exposure Time: %g\n\nAccumulate Delay: %g\nKinetic Delay: %g\n\n",exp,accum, kinetic);
		//
		printf("Temperature set to %d degrees C.\n",SetTemp);
		GetTemperature(&i);
		printf("Actual Temperature: %d degrees C.\n\n",i);
		//
		printf("Acquisition Mode: %d.\n",ac_mode);
		printf("Acquisition Mode 2 will accumulate %d frames.\n",num_frames);
		//
		
		if(trig_mode == EXTERNAL)
			printf("Trigger Mode: External\n");
		else 
			printf("Trigger Mode: Internal\n");
        free(specs);
		return 0;
		
	/*
	ACQuire command
	Get an image using the current settings
	*/	
		
    } else if (strncmp(args,"ACQ",3) == 0){
		
		//Set Read Mode to --Image--
		//error = SetReadMode(4);
		//cout << "error " << error << endl;
		
		//Set Acquisition mode to --Single scan--
		//SetAcquisitionMode(1);
		
		//Set initial exposure time
		//SetExposureTime(ExposureTime);
		
		//Initialize Shutter
		//SetShutter(1,0,50,50);
        
        width = specs[COLS];
        height = specs[ROWS];
        x0 = specs[X0]+1;
        y0 = specs[Y0]+1;
        if( width*specs[DX]+specs[X0] <= ccd_width && height*specs[DY]+specs[Y0] <= ccd_height) {
        	//Setup Image dimensions
            error = SetImage(specs[DX],specs[DY],x0,width+x0-1,y0,height+y0-1);
			if(error != DRV_SUCCESS ) {
				printf("Image setup error: %d\n",error);
                free(specs);
				return -1;
			}
		} else {
			printf("Specified image area does not match CCD characteristics.\n");
            free(specs);
			return -1;
		}
		
		
		StartAcquisition();
		
		imageData = malloc(width*height*sizeof(at_32));
		
		//Loop until acquisition finished
		GetStatus(&status);
		while(status==DRV_ACQUIRING) GetStatus(&status);
		
		GetAcquiredData(imageData, width*height);
        int k=0;
        for( i=0;i<height;i++) {
            for(int j=0;j<width;j++){
                iBuffer.setpix(i,j,*(imageData+k++));
            }
		} 
		
		//SaveAsBmp("./image.bmp", "./GREY.PAL", 0, 0);
		
        if(imageData != 0) free(imageData);

        iBuffer.getmaxx();
		
        free(specs);
		return 0;
		
	} else {
		printf("Unknown ANDOR Command\nValid Commands are:\n ACQuire\n");
		printf(" EXPose exposure_time\n");
		printf(" FRAmes num_frames\n");		
		printf(" GAIn gain_val\n");
		printf(" MODe acquire_mode\n");
		printf(" STAtus\n");
		printf(" TEMperature set_temp\n");
		printf(" TRIgger command: 1 = internal, 2 = external\n");
        free(specs);
		return -2;
	}
	
}

#endif

#ifdef SERIAL_PORT
// use unix terminal control routies
#include <termios.h> /* POSIX terminal control definitions */

int serial_fd; /* File descriptor for the port */
struct termios serial_options;
int serial_inited = 0;

int open_serial_port(char* dev_name){
	  serial_fd = open(dev_name, O_RDWR | O_NOCTTY | O_NDELAY);
      if (serial_fd == -1) {
		return -1;
      }else{
		fcntl(serial_fd, F_SETFL, 0);	
		fcntl(serial_fd, F_SETFL, FNDELAY);		// return at once from a read, even if there are no characters ready
	  }

     //Get the current options for the port...
    tcgetattr(serial_fd, &serial_options);

    // Set the baud 
    cfsetispeed(&serial_options, B9600);
    cfsetospeed(&serial_options, B9600);
    //cfsetispeed(&serial_options, B19200);
    //cfsetospeed(&serial_options, B19200);

    // Enable the receiver and set local mode...

    serial_options.c_cflag |= (CLOCAL | CREAD);

	serial_options.c_cflag &= ~CSIZE; /* Mask the character size bits */
    serial_options.c_cflag |= CS8;    /* Select 8 data bits */
	serial_options.c_cflag &= ~PARENB;
	serial_options.c_cflag &= ~CSTOPB;
	serial_options.c_cflag &= ~CSIZE;
	serial_options.c_cflag |= CS8;
    //options.c_cflag &= ~CNEW_RTSCTS;
	
    // Set the new options for the port...
    tcsetattr(serial_fd, TCSANOW, &serial_options);
	return 0;
}

int serclo(int n, char* args)
{
	if(serial_inited == 0) {
		beep();
		printf("Serial port not initialized.\n");
		return -1;
	} else {
		close(serial_fd);
		serial_inited = 0;
		return 0;
	}
		
		
}
/*
Old serial command that only sends command
int serial(int n, int index)
{
	int length;
	extern char cmnd[];
	
	if(serial_inited == 0) {
        if(open_serial_port(args) != 0){
			beep();
            printf("Can't open serial port %s.\n",args);
			return -1;
		} else {
            printf("Opened serial port %s.\n",args);
			serial_inited = 1;
			return 0;
		}
	}
    length = strlen(args);
	if( length == 0 ) {
		beep();
		printf("Nothing sent\n");
		return -1;
	}
	cmnd[index+length] = 13;	// add a <CR> (=13)
	length++;
    write(serial_fd, args, length);
    //printf("sent %d characters -%s-\n",length,args);
	return 0;
}
*/
int serial(int n, char* args)
{
	int length,i;
    //extern char cmnd[];
	char buffer[ESTRING_LENGTH];
	extern Variable user_variables[];
	
	if(serial_inited == 0) {
        if(open_serial_port(args) != 0){
			beep();
            printf("Can't open serial port %s.\n",args);
			return -1;
		} else {
            printf("Opened serial port %s.\n",args);
			serial_inited = 1;
			return 0;
		}
	}
    length = strlen(args);
	if( length == 0 ) {
		beep();
		printf("Nothing sent\n");
		return -1;
	}
    args[length] = 13;	// add a <CR> (=13)
	length++;
    write(serial_fd, args, length);
	//echo any reply
	length = read(serial_fd, buffer, ESTRING_LENGTH-1);
	for(i=0;i<length;i++)buffer[i] &= 127;
	while (length > 0){
		buffer[length] = 0;
		printf("%d characters returned",length);
		pprintf("%s\n",buffer);
		strcpy(&user_variables[0].estring[0],buffer);
		user_variables[0].is_float = -1;
		length = read(serial_fd, buffer, ESTRING_LENGTH-1);

	}
    //printf("sent %d characters -%s-\n",length,args);
	return 0;
}



#endif

