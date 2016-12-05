/*
 * CS229IO.cpp
 *
 *  Created on: Dec 2, 2015
 *      Author: michael
 */
#include <stdexcept>
#include "CS229IO.h"
#include <algorithm>
#include <sstream>
/*!
 * \brief Creates a new object from a file
 */
CS229IO::CS229IO(string f) {
	string check=f.substr(f.length()-6);
	musicStream=new ifstream();
	if (check!=".cs229") {
		std::cout<<"File Type Error"<<endl;
		throw;
	}
	tofile=true;
	musicStream->open(f.c_str());
	headerInfo=new string[5]; //Array of File Info
	filename=f;
	headerInfo[0]=f;
	headerInfo[4]="UNDEFINED"; //Will remain undefined unless found in file
	dataInfo=new vector<int>();
	headerNames=new map<string,int>();
	//Create A map for file reading
	(*headerNames)["samplerate"]=1;
	(*headerNames)["channels"]=2;
	(*headerNames)["bitres"]=3;
	(*headerNames)["samples"]=4;

}
/*!
 * \brief from reading from std::cin
 */
CS229IO::CS229IO() {
	tofile=false;
	musicStream=NULL;
	headerInfo=new string[5]; //Array of File Info
	filename="cout";
	headerInfo[0]=filename;
	headerInfo[4]="UNDEFINED"; //Will remain undefined unless found in file
	dataInfo=new vector<int>();
	headerNames=new map<string,int>();
	//Create A map for file reading
	(*headerNames)["samplerate"]=1;
	(*headerNames)["channels"]=2;
	(*headerNames)["bitres"]=3;
	(*headerNames)["samples"]=4;
}

CS229IO::~CS229IO() {
	delete headerNames;
	musicStream->close();
	//delete headerInfo;
	delete dataInfo;
	//Should be taken care of in Deconstructor of MusicIO
}
/*
 * \brief reads the
 * \return true if read correctly, false otherwise
 */
bool CS229IO::readHeader() {
	string fileline;
	if(tofile==true) {
		std::getline(*musicStream,fileline);
			if (fileline!="CS229"&&(fileline!="Cs229")&&(fileline!="cs229")) { //header incorrect
				throw "File Type Error: 1st Line";
			}
			while (getline(*musicStream,fileline)) {
				//We have to make sure there isn't just a space for the first char
				int first=0;
				while((first<fileline.length())&&((fileline[first]=='\t')||(fileline[first]==' '))) {
					first++;
				}
				if(first==fileline.length()&&fileline!="") {
					cout<<"Line Read Error"<<endl;
					throw;
				}
				if (fileline=="") {
					//Empty Line
				}
				else if (fileline[first]=='#') {
					//Comment
				}
				else if ((fileline=="STARTDATA")||(fileline=="startdata")||(fileline=="StartData")) { //data start
					return this->validate_header(); //Validates header and returns info
				}
				else {
					vector<string>* elements=this->split(fileline,"\t "); //Splits the string by ' ' char

					if (elements->size()<=1) { //Incorrect Number of Elements
						return false;
					}
					int index;
					//Now we gotta lowercase the values
					string loweredHeader=this->tolower((char*)elements->at(0).c_str(),elements->at(0).length());
					try {
						index=headerNames->at(loweredHeader); //Gets Correct Index
					}
					catch(std::out_of_range &e) {
						return false; //Incorrect Info in File
					}
					headerInfo[index]=elements->at(1); //Loads data into header array for evaluation
					delete elements;
				} //If there is more than one of any header data, the most recent one will be accepted as the 'true' header

			}
				return false; //No STARTDATA
	}
	else { //yes I know this is lazy
		std::getline(std::cin,fileline);
			if (fileline!="CS229"&&(fileline!="Cs229")&&(fileline!="cs229")) { //header incorrect
				throw "File Type Error: 1st Line";
			}
			while (getline(*musicStream,fileline)) {
				//We have to make sure there isn't just a space for the first char
				int first=0;
				while((first<fileline.length())&&((fileline[first]=='\t')||(fileline[first]==' '))) {
					first++;
				}
				if(first==fileline.length()&&fileline!="") {
					cout<<"Line Read Error"<<endl;
					throw;
				}
				if (fileline=="") {
					//Empty Line
				}
				else if (fileline[first]=='#') {
					//Comment
				}
				else if ((fileline=="STARTDATA")||(fileline=="startdata")||(fileline=="StartData")) { //data start
					return this->validate_header(); //Validates header and returns info
				}
				else {
					vector<string>* elements=this->split(fileline,"\t "); //Splits the string by ' ' char

					if (elements->size()<=1) { //Incorrect Number of Elements
						return false;
					}
					int index;
					//Now we gotta lowercase the values
					string loweredHeader=this->tolower((char*)elements->at(0).c_str(),elements->at(0).length());
					try {
						index=headerNames->at(loweredHeader); //Gets Correct Index
					}
					catch(std::out_of_range &e) {
						return false; //Incorrect Info in File
					}
					headerInfo[index]=elements->at(1); //Loads data into header array for evaluation
					delete elements;
				} //If there is more than one of any header data, the most recent one will be accepted as the 'true' header

			}
				return false; //No STARTDATA
	}

}
/*!
 * \brief validates header file
 * \return true if header file is 'accurate' false otherwise
 */
bool CS229IO::validate_header() {
	int samplerate=atoi(headerInfo[1].c_str()); //converts string to int
	if (samplerate<1) { return false;} //So if the string actually converts to an int you're SOL
	int channels=atoi(headerInfo[2].c_str());
	if ((channels<1)||(channels>127)) { return false;} // same warning as samplerate
	int bitres=atoi(headerInfo[3].c_str());
	switch(bitres) {
		case 4:
		case 8:
		case 16:
		case 32:
			//Do nothing
			break;
		default:
			return false;
			break; //Not really needed
	}
	if (headerInfo[4]!="UNDEFINED") { //So an actual
		int samplenum=atoi(headerInfo[4].c_str());
		if (samplenum<1) { return false;}
	}
	return true;
}
/*!
 * \brief Reads data into IO object
 * \returns data
 */
bool CS229IO::readData() {
	if (tofile) {
		if (musicStream->eof()) { return false;}
			int linesread=0;
			int numdata=INT_MAX; //
			int channels=atoi(headerInfo[2].c_str());
			if (headerInfo[4]!="UNDEFINED") {
				numdata=atoi(headerInfo[4].c_str()); //I'm sure there's an actual way to
			}
			string line;
			vector<string>* sample_elements;
			while ((getline(*musicStream,line))&&(linesread<=numdata)) {

				if (line=="") {
					//You know the whitespace made more sense in the headers
				}
				else if (line[0]=='#') {
					//do nothing
				}
				else {
					sample_elements=this->split(line," \t");
					if (sample_elements->size()!=channels) { //If there is too little or not enough data, an error will be returned
						return false;
					}
					else {
						for(int i=0; i<channels;i++) {
							int toadd=atoi(sample_elements->at(i).c_str());
							dataInfo->push_back(toadd); //Channel values will be stored in single vector in IO object
						}
				}
					linesread++;
				}
			} //So if there are extra lines they just arent read.
			if (numdata==INT_MAX) {
				numdata=linesread;
			}
			std::stringstream tostr;
			tostr<<numdata;
			line=tostr.str();
			headerInfo[4]=line; //this was the easiest way i could find to do to_string
			return this->validate_data();
	}
	else {
			int linesread=0;
			int numdata=INT_MAX; //
			int channels=atoi(headerInfo[2].c_str());
			if (headerInfo[4]!="UNDEFINED") {
				numdata=atoi(headerInfo[4].c_str()); //I'm sure there's an actual way to
			}
			string line;
			vector<string>* sample_elements;
			while ((getline(cin,line))&&(linesread<=numdata)) {

				if (line=="") {
					//You know the whitespace made more sense in the headers
				}
				else if (line[0]=='#') {
					//do nothing
				}
				else {
					sample_elements=this->split(line," \t");
					if (sample_elements->size()!=channels) { //If there is too little or not enough data, an error will be returned
						return false;
					}
					else {
						for(int i=0; i<channels;i++) {
							int toadd=atoi(sample_elements->at(i).c_str());
							dataInfo->push_back(toadd); //Channel values will be stored in single vector in IO object
						}
				}
					linesread++;
				}
			} //So if there are extra lines they just arent read.
			if (numdata==INT_MAX) {
				numdata=linesread;
			}
			std::stringstream tostr;
			tostr<<numdata;
			line=tostr.str();
			headerInfo[4]=line; //this was the easiest way i could find to do to_string
			return this->validate_data();
	}

}
/*!
 * \brief Validates data
 */
bool CS229IO::validate_data() {
	int bres=atoi(headerInfo[3].c_str());
	int max=pow(2,bres)/2 -1; //Max Value
	int min=-1*max; //Min Value
	int val;
	//Now I'm just gonna check the values in the vector against the min and max values
	//I am using a for loop instead of an iterator because f**ck iterators in c++
	for(int i=0; i<dataInfo->size(); i++) {
		val=dataInfo->at(i);
		if (val<min||val>max) { return false; }
	}
	return true;
}
/*!
 * \brief creats CS229 File from
 */
MusicFile* CS229IO::create_MusicFile() {

	string name=headerInfo[0];
	int sRate=atoi(headerInfo[1].c_str());
	int nChannels=atoi(headerInfo[2].c_str());
	int bitres=atoi(headerInfo[3].c_str());
	int nSamples=atoi(headerInfo[4].c_str());
	return new CS229File(nChannels,sRate,nSamples,bitres,name,*dataInfo);

}
