#include "userthr.h"

int userMain(int argc, char* argv[]);

extern int retVal;

UserThread::UserThread(int argc, char** argv){
	this->argc = argc;
	this->argv = argv;
}

void UserThread::run() {

	retVal = userMain(argc, argv);

}

Thread* UserThread::clone() const{

	return new UserThread(argc, argv);

}


UserThread::~UserThread() {

	waitToComplete();

}

