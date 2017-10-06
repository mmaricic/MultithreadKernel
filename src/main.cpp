
#include "kernel.h"
#include "userthr.h"

int userMain(int argc, char* argv[]);

int retVal;

int main(int argc, char* argv[]){

	Kernel::inic();
  
	UserThread * userThr = new UserThread(argc, argv);
	userThr->start();
	delete userThr;
	
	Kernel::restore();
  
	return retVal;
}

