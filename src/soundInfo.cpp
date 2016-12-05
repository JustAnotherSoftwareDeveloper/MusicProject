#ifndef C229IO_H_
#include "CS229IO.h"
#endif
#include "soundInfo.h"
/*!
 * \brief Default constructor. Never Called
 */
soundInfo::soundInfo() {
	filename=new string("in.cs229");
	output=&cout;

}
/*
 * Default Destructor
 */
soundInfo::~soundInfo() {
	if (output!=&cout) { //I feel like deleting cout will result in a very bad time
		delete output;
	}
	delete filename;
}
/*
 * \brief Constructor that will actually be used. passes the filename
 */
soundInfo::soundInfo(string f) {
	filename=new string(f);
	output=&cout;
}
/*
 * \brief Constructor I might implement if I have time. can also write to a file
 */
soundInfo::soundInfo(string f,string fileout) {
	filename=new string(f);
	output=new ofstream(fileout.c_str());
}
/*
 * \brief this will actually perform the functionality
 * \return 0 if successful, -1 outherwise
 */
int soundInfo::writeFile() {
	/*
	 * reading data
	 */
	CS229IO* toread;
	string wrong="cin";
	if(*filename!=wrong) {
		toread=new CS229IO(*filename);
	}
	else {
		toread=new CS229IO();
	}
	bool valid=toread->readHeader();
	if (!valid) {
		cout<<"Header Info on "<<*filename<<" not valid"<<std::endl;
		throw;
		return -1;
	}
	valid=toread->readData();
	if (!valid) {
		cout<<"Data Info on "<<*filename<<" not valid"<<endl;
		return -1;
	}
	MusicFile* musicinfo=toread->create_MusicFile(); //realllly hope I have my slicing knowledge down correctly
	delete toread;
	string wr=musicinfo->toString();
	(*output)<<wr; //All that effort for a string, Made my sndcat class easier though.
	delete musicinfo;
	return 0;
}

