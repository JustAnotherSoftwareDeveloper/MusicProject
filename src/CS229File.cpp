
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "CS229File.h"


CS229File::CS229File() : MusicFile()
{
	this->type=CS229;
	channelsInfo=new std::vector<std::vector<int>*>();
}

CS229File::~CS229File() {
	for(int i=0; i<channelsInfo->size(); i++) {
		delete channelsInfo->at(i);
	}
	delete channelsInfo;
}
/*
 * \brief creates a new object with blank data
 * \param c: number of channels
 * \param sr: sample rate
 * \param n: number of samples
 * \param d, bitdepth
 * \param nm: name of file
 *
 */
CS229File::CS229File(int c,long sr,int n,int d,std::string nm) {
	this->nChannels=c;
	this->sRate=sr;
	this->nSamples=n;
	this->bdepth=d;
	this->fileName=nm;
	this->type=CS229;
	this->channelsInfo= new std::vector<std::vector<int>*>();
	for (int i=0; i<c; i++) {
		channelsInfo->push_back(new std::vector<int>()); //Populates channel list
	}
}
/*
 * \brief data included
 * \param data: data used in sample info
 */
CS229File::CS229File(int c,long sr,int num,int d,std::string nm,std::vector<int> data) {
	this->nChannels=c;
	this->sRate=sr;
	this->nSamples=num;
	this->bdepth=d;
	this->fileName=nm;
	this->type=CS229;
	this->channelsInfo= new std::vector<std::vector<int>*>();
	for (int i=0; i<c; i++) {
		channelsInfo->push_back(new std::vector<int>()); //Populates channel list
	}
	for(int i=0; i<data.size();i++) {
		channelsInfo->at(i%c)->push_back(data[i]);
	}
}
/*
 * \brief Sets nsamples to 0
 */
CS229File::CS229File(int c,long sr,int d,std::string nm) {
	this->nChannels=c;
		this->sRate=sr;
		this->nSamples=0; //Would this automatically go to 0? Suppose this doesn't hurt
		this->bdepth=d;
		this->fileName=nm;
		this->type=CS229;
		this->channelsInfo= new std::vector<std::vector<int>*>();
		for (int i=0; i<c; i++) {
			channelsInfo->push_back(new std::vector<int>()); //Populates channel list
		}
}
/*
 * \brief this follows sndinfo's specs.
 */
std::string CS229File::toString() {
	std::stringstream returnval;
	returnval<<"Filename "<<this->fileName<<std::endl;
	returnval<<"Type: .CS229"<<std::endl;
	returnval<<"Sample Rate: "<<sRate<<std::endl;
	returnval<<"Bit Depth: "<<bdepth<<std::endl;
	returnval<<"Number of Samples: "<<nSamples<<std::endl;
	double len=this->soundLength();
	returnval<<"Sound Length (seconds): "<<len<<std::endl;
	return returnval.str();
}
/*
 * \brief returns the sound data
 */
std::vector<std::vector<int>* >* CS229File::returnData() {
	return channelsInfo;
}
/*!
 * \brief adds data to array
 * \param nS: number of samples in data
 * \param nC: number of channels in data
 */
void CS229File::addData(std::vector<std::vector<int>*>* data,int nS,int nC) {
	if (nC> nChannels) {
		std::cout<<"Channel Size Error"<<std::endl;
		throw;
	}
	int sample;
	//Will go through each sample and add it to the CS229 object
	for(int i=0; i<nS; i++) {
		for(int j=0;j<nChannels; j++) {
			if (j>=nC) {
				channelsInfo->at(j)->push_back(0); //if there are less channels in the data, it'll just put 0 then
			}
			else {
				sample=data->at(j)->at(i);
				channelsInfo->at(j)->push_back(sample);
			}
		}
		nSamples++;
	}

}
/*!
 *  \brief writes this to a file
 *
 */
void CS229File::writefile() {
	std::ofstream w(fileName.c_str());
	bool tofile=false;
	std::streambuf*  buffer;
	if(fileName!="cout") {
		buffer=w.rdbuf();
		tofile=true;
	}
	else {
		buffer=std::cout.rdbuf();
	}
	std::ostream writer(buffer);
	writer<<"CS229"<<std::endl;
	writer<<"Samples "<<this->nSamples<<std::endl;
	writer<<"SampleRate "<<this->sRate<<std::endl;
	writer<<"BitRes "<<this->bdepth<<std::endl;
	writer<<std::endl;
	int sample;
	writer<<"STARTDATA"<<std::endl;
	for(int i=0; i<this->nSamples; i++) {
		for(int j=0; j<this->nChannels; j++) {
			try {
				sample=channelsInfo->at(j)->at(i);
			}
			catch(std::out_of_range &e) {
				std::cout<<j<<","<<i<<std::endl;
				throw;
			}
			writer<<sample<<"\t";
		}
		writer<<std::endl;
	}
	if (tofile) {
		w.close();
	}
}
/*!
 * \brief returns number of channels
 */
int CS229File::numChannels() {
	return nChannels;
}
/*!
 * \brief returns SampleRate
 */
long CS229File::sampleRate() {
	return sRate;
}
/*!
 * \brief returns number of samples
 */
long CS229File::numSamples() {
	return nSamples;
}
/*!
 * \brief returns bit depth
 */
int CS229File::bitDepth() {
	return bdepth;
}
/*
 *  \brief returns the file type
 */
FileType CS229File::returnType() {
	return type;
}

/*!
 * \brief Calculates file length
 */
double CS229File::soundLength() {
	return nSamples/ (double) sRate;
}
