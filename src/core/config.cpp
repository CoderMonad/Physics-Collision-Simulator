/*****************************************************************************/
#include "config.h"
#include "../version.h"
/*****************************************************************************/
/////////////////////////////////////////////////
/// @file config.cpp
/// @brief This holds all the functions related to the config file, its loading, making,
///        and holding the values pulled from the config.
/////////////////////////////////////////////////
/*****************************************************************************/
clsConfig::clsConfig() {
  /////////////////////////////////////////////////
  /// @brief The default constructor, will set the config values as follows
  ///        * values.blnLogging = false;
  ///        * values.blnDragMode = false;
  ///        * values.blnDrawPathOnScreen = false;
  ///        * values.uintScreenWidth = 640;
  ///        * values.uintScreenHeight = 480;
  ///        * values.uchrCollisionMethod = CollideInelastic;
  ///        * values.uintMaxNumPastPoints = 5;
  ///        * values.uintPastDelay = 25;
  ///        It will also set the OS based on values that are defined
  ///
  /////////////////////////////////////////////////

  //Set default values
  values.blnLogging = false;
  values.blnDragMode = false;
  values.blnDrawPathOnScreen = false;
  values.uintScreenWidth = 640;
  values.uintScreenHeight = 480;
  values.uintMaxNumPastPoints = 5;
  values.uintPastDelay = 25;
  values.uchrCollisionMethod = CollideInelastic;
#if defined(_AIX)
  values.OperatingSystem = "AIX";
#elif defined(__unix__)
  values.OperatingSystem = "Unix";
#elif defined(__hpux)
  values.OperatingSystem = "HP-UX";
#elif defined(__linux__)
  values.OperatingSystem = "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
  //Apple Sucks
  values.OperatingSystem = "OSX";
  //I could add additional stuff to see if iOS or OSX but I don't care.
#elif defined(__sun) && defined(__SVR4)
  values.OperatingSystem = "Solaris";
#elif defined(__CYGWIN__) && !defined(_WIN32)
  values.OperatingSystem = "Cygwin POSIX";
#elif defined(_WIN64)
  values.OperatingSystem = "Windows 64 bit";
#elif defined(_WIN32)
  values.OperatingSystem = "Windows 32 bit";
#else
  values.OperatingSystem = "UNDEFINED OS!";
#endif // defined OS
}
/*****************************************************************************/
bool clsConfig::exists(void) {
  /////////////////////////////////////////////////
  /// @brief Checks if config file already exists
  /// @return TRUE / FALSE
  /////////////////////////////////////////////////

	//Returns true or false if config file exists
	FILE* pTempFile = fopen(FileName_, "r");
  bool result = (pTempFile != NULL);
  if (pTempFile) { fclose(pTempFile); }
  return result;
}
/*****************************************************************************/
void clsConfig::make(void) {
  /////////////////////////////////////////////////
  /// @brief Will make a new config file, based on the default values in the constructor
  /// @return void
  /////////////////////////////////////////////////

	//Makes the config file
	configFile_ = fopen(FileName_,"w");
	printf("Config File will now be created!\n");

	fprintf(configFile_,"Config File for the Physics-Simulation.exe\n");
	fprintf(configFile_,"%s\n",DEFINED_VER_FULLVERSION_STRING);
	//Write the config file with the defaults above
	//this way I only have to change the defaults in one place
  fprintf(configFile_,"Screen Width: %u\n",values.uintScreenWidth);
  fprintf(configFile_,"Screen Height: %u\n",values.uintScreenHeight);
  fprintf(configFile_,"Log Ball's path: %u\n", (values.blnLogging ? 1 : 0) ) ;
  fprintf(configFile_,"Enable Drag Mode: %u\n",
                        (values.blnDragMode ? 1 : 0) );
  fprintf(configFile_,"Draw Ball path on screen: %u\n",
                        (values.blnDrawPathOnScreen ? 1 : 0) );
  fprintf(configFile_,"Collision Method: %u\n", values.uchrCollisionMethod);
  fprintf(configFile_,"Number of points to track: %u\n",
          values.uintMaxNumPastPoints);
  fprintf(configFile_,"Delay between recording points: %u\n",
          values.uintPastDelay);
	fclose(configFile_);
}
/*****************************************************************************/
void clsConfig::load(void) {
  /////////////////////////////////////////////////
  /// @brief Will load the values of the config file and put them where they belong
  /// @return void
  /////////////////////////////////////////////////

	//Loads all of the config values
  // TODO: Config parsing is brittle — it counts fixed numbers of words with %*s to skip tokens.
  //       Any change to the key strings breaks loading silently. Consider using a
  //       key=value format (e.g. "ScreenWidth=640") and parsing by key name instead.
	char chrTempString[50];
	int intTempBool, intValuesScanned;

  //Get Screen Width
  fgets(chrTempString,50,configFile_);
  intValuesScanned = sscanf(chrTempString,"%*s %*s %u",&values.uintScreenWidth);
  if (intValuesScanned < 1) { printf("Error"); values.uintScreenWidth = 680; }
  if (global::blnDebugMode)
    { printf("Screen Width \t %u\n", values.uintScreenWidth); }

  //Get Screen Height
  fgets(chrTempString,50,configFile_);
  intValuesScanned = sscanf(chrTempString,"%*s %*s %u",&values.uintScreenHeight);
  if (intValuesScanned < 1) { printf("Error"); values.uintScreenHeight = 480; }
  if (global::blnDebugMode)
    { printf("Screen Height \t %u\n", values.uintScreenHeight); }

  //Get if logging
	fgets(chrTempString,50,configFile_);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) { printf("ERROR!"); intTempBool = 0; }
	if(global::blnDebugMode) { printf("Log Path \t %d\n",intTempBool); }
	values.blnLogging = (intTempBool == 1);

  fgets(chrTempString,50,configFile_);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %d",&intTempBool);
	if (intValuesScanned < 1) { printf("ERROR!"); intTempBool = 0; }
	if(global::blnDebugMode) { printf("Enable Drag \t %d\n",intTempBool); }
	values.blnDragMode = (intTempBool == 1);

  fgets(chrTempString,50,configFile_);
	intValuesScanned = sscanf(chrTempString, "%*s %*s %*s %*s %*s %d",
                           &intTempBool);
	if (intValuesScanned < 1) { printf("ERROR!"); intTempBool = 0; }
	if(global::blnDebugMode) { printf("Enable Screen Path \t %d\n",intTempBool); }
	values.blnDrawPathOnScreen = (intTempBool == 1);

  fgets(chrTempString,50,configFile_);
  intValuesScanned = sscanf(chrTempString, "%*s %*s %d",
                            &values.uchrCollisionMethod);
  if (intValuesScanned < 1) {
    printf("ERROR!");
    values.uchrCollisionMethod = CollideInelastic;
  }
  if (values.uchrCollisionMethod > CollideNone) {
    printf("ERROR!");
    values.uchrCollisionMethod = CollideNone;
  }
  if(global::blnDebugMode)
    { printf("Collision Method: \t %d\n",values.uchrCollisionMethod); }

  fgets(chrTempString,50,configFile_);
  intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %*s %*s %u",
                            &values.uintMaxNumPastPoints);
  if (intValuesScanned < 1) { printf("Error"); values.uintMaxNumPastPoints = 5; }
  if (global::blnDebugMode)
    { printf("Max num of past points: \t %u\n", values.uintMaxNumPastPoints); }

  fgets(chrTempString,50,configFile_);
  intValuesScanned = sscanf(chrTempString,"%*s %*s %*s %*s %u",
                            &values.uintPastDelay);
  if (intValuesScanned < 1) { printf("Error"); values.uintPastDelay = 25; }
  if (global::blnDebugMode)
    { printf("Delay on points: \t %u\n", values.uintPastDelay); }

	fclose(configFile_);
	printf("\n\n");
}
/*****************************************************************************/
char clsConfig::verisonCheck(const char *ConfigVerison) {
    /////////////////////////////////////////////////
    /// @brief Will check the version number written into the config vs the Program version number.
    ///        Version number is done like [MAJOR].[MINOR].[PATH]-[SOFTWARESTATUS] with these numbers defined in version.h
    ///        Based on which (if any) of these number are different between the two it will do the following:
    ///        * Software Statuses are different = Make New Config File
    ///        * Major Number different = Make New Config File
    ///        * Minor, or Patch Numbers different = Use Old Config File
    ///        * No Number different = Use Old Config File
    ///
    /// @param ConfigVersion = The Version number in the config file
    /// @return USECONFIG / NEWCONFIG
    ///
    /////////////////////////////////////////////////
	uint C_MajorNum, C_MinorNum, C_PatchNum;
  char C_SoftwareStatus, P_SoftwareStatus;
	sscanf(ConfigVerison,"%u.%u.%u-%c",&C_MajorNum, &C_MinorNum,
                        &C_PatchNum, &C_SoftwareStatus);

	if (global::blnDebugMode) {
    printf("Config: v %u %u %u %c \n",
             C_MajorNum, C_MinorNum, C_PatchNum, C_SoftwareStatus);
	}

	sscanf(DEFINED_VER_STATUS,"%c",&P_SoftwareStatus);
	if (P_SoftwareStatus != C_SoftwareStatus) { return 'N'; }
	else if (DEFINED_VER_MAJOR != C_MajorNum) { return 'N'; }
	else if (DEFINED_VER_MINOR != C_MinorNum) { return 'U'; }
	else { return 'U'; }
}
/*****************************************************************************/
void clsConfig::Check(void) {
    /////////////////////////////////////////////////
    /// @brief This is the basic method used to call on the config.
    ///        It first checks if there is already a config file (if not makes a new one)
    ///        if there is an existing config file it will then check its version number and load its values
    ///        or make a new one depending on the return of clsConfig::versionCheck
    /////////////////////////////////////////////////

	char chrTempString[50], chrConfigVerison;

	if ( !exists() ) {
		printf("Config file was not found; creating now one\n");
		make();
	} else {
		configFile_ = fopen(FileName_,"r");
		printf("Config file found; loading values\n");
		fgets(chrTempString,50,configFile_);
		fgets(chrTempString,50,configFile_);
		//printf("%s\n",chrTempString);
		chrConfigVerison = verisonCheck(chrTempString);

		if (chrConfigVerison == 'N') {
			printf("Current config file out of date. Making new one.\n");
			fclose(configFile_);
			make();
		} else { load(); }
	} //end if exists
}
/*****************************************************************************/
