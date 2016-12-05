
#include "MusicIO.h"
#include <exception>
#include <ctype.h>
MusicIO::MusicIO() {

}

MusicIO::~MusicIO() {
}
/*
 * \brief Constructor with specified Filename
 */
MusicIO::MusicIO(string f) {

}
/*!
 * \brief Replicates the Java split method
 */
vector<string>* MusicIO::split(string info,string tolken) {
	vector<string>* matches=new vector<string>();
	char* cInfo=(char*) info.c_str(); //Convert to Cstring for using strtok
	char* singleMatch=strtok(cInfo,tolken.c_str());
	while (singleMatch!=NULL) {
		matches->push_back(singleMatch);
		singleMatch=strtok(NULL,tolken.c_str());
	};
	return matches;
}
/*!
 * \brief converts a string to entirely lowercase because for some
 * goddamned reason the headers aren't in the same case all the time
 */
string MusicIO::tolower(char cstring[],int len) {
	string returnval="";
	char letter;
	for(int i=0; i<len;i++) {
		letter=cstring[i];
		if(isalpha(letter)) {
			returnval+=std::tolower(letter);
		}
		else {
			returnval+=letter;
		}
	}
	return returnval;
}
