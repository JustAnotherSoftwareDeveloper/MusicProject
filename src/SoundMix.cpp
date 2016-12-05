
#include "SoundMix.h"
#include "CS229IO.h"
#include <math.h>
#include <iostream>
/*
 * Default Constructor
 * \param argc: passed from main
 * \param argv: passed from main
 */
SoundMix::SoundMix(int argc,char** argv) {
	if (argc%2!=0) {
		throw "Command Argument Error"; //every
	}
	std::string value;
	int multiplier;
	filename="cout";
	multiply=new std::vector<int>();
	filenames=new std::vector<std::string>();
	for(int i=2; i<argc-1; i+=2) { //Because the multi and filename comes in pairs
		value=argv[i];
		if(value=="-o") {
			filename=argv[i+1];
		}
		else {
			//Multiplier
			multiplier=atoi(argv[i]); //Better Conversion
			if((multiplier<-10)||(multiplier>10)) { throw "Incorrect Mult Value"; }
			multiply->push_back(multiplier);
			//Filename
			value=argv[i+1];
			filenames->push_back(value);
		}
	}
	output=NULL;
	min=0;
	max=0;
	inputs=new std::vector<MusicFile*>();
	data=new std::vector<std::vector<int>* >();
}

SoundMix::~SoundMix() {
	delete this->output; //The problem with so many warnings is that I don't know which ones I can ignore.
	for(int i=0; i<inputs->size();i++) {
		delete inputs->at(i);
	}
	delete inputs;
	delete multiply;
	for(int i=0; i<data->size();i++) {
		delete data->at(i);
	}
	delete data;
}
/*
 * \brief create the file objects used in the mixer
 */
bool SoundMix::createfiles() {
	/*
	 * This is a copy of SoundCat's copy. If I was doing this again, I make an abstract class over the two because
	 * there is a lot of similar functionality
	 */
	CS229IO* reader;
	MusicFile* tomix;
	bool valid;
	for(int i=0; i<filenames->size(); i++) {
		reader=new CS229IO(filenames->at(i));
		valid=reader->readHeader();
		if (!valid) {
			return false; //Header info incorrect
		}
		valid=reader->readData();
		if (!valid) {
			return false;
		}
		tomix=reader->create_MusicFile();
		delete reader; //So does this delete the variable name too? If so : f**ck
		inputs->push_back((CS229File*)tomix);
		tomix=NULL; //If I delete tocat it'll be gone from the vector too. this just removes the reference
	}
	/*
	 * NOTE: This will not check for duplicate files. So You can cat the same file over and over and over again. It will also overwrite
	 * the same file
	 */
	return true;
}
/*!
 * \brief : creates the output file used in
 */
bool SoundMix::createOutput() {
	long headerStuff[4]; //SampleRate,BitDepth,Channels ,SampleNum
	long inputStuff[4];
	//Primers
	headerStuff[0]=((CS229File*)inputs->at(0))->sampleRate();
	headerStuff[1]=((CS229File*)inputs->at(0))->bitDepth();
	headerStuff[2]=((CS229File*)inputs->at(0))->numChannels();
	headerStuff[3]=((CS229File*)inputs->at(0))->numSamples();
	//Loop to get header data
	for(int i=1; i<inputs->size(); i++) {
		//Read From Inputs
		inputStuff[0]=((CS229File*)inputs->at(i))->sampleRate();
		inputStuff[1]=((CS229File*)inputs->at(i))->bitDepth();
		inputStuff[2]=((CS229File*)inputs->at(i))->numChannels();
		inputStuff[3]=((CS229File*)inputs->at(i))->numSamples();
		//Check against data in primer
		if (inputStuff[0]!=headerStuff[0]) { return false;}
		if (inputStuff[1]>headerStuff[1]) {
			headerStuff[1]=inputStuff[1];
		}
		if(headerStuff[2]<inputStuff[2]) {
			headerStuff[2]=inputStuff[2];
		}
		if (headerStuff[3]<inputStuff[3]) {
			headerStuff[3]=inputStuff[3];
		}
	}
	output=new CS229File(headerStuff[2],headerStuff[0],headerStuff[1],filename);
	CS229File* tomix;
	std::vector<std::vector<int>* >* toadd; //will be from the musicfile objects
	//Adds the Appropriate amount of channels
	for(int i=0; i<output->numChannels(); i++) {
		data->push_back(new std::vector<int>());
		//Putting 0s in the appropriate places because why the hell not
		for(int j=0; j<headerStuff[3];j++) {
			data->at(i)->push_back(0);
		}
	}
	//We're creating a max and min value to cap things at
	max=(pow(2,output->bitDepth())/2)-1;
	min=max*-1;
	int tomultiply;
	std::vector<int>* channel;
	for(int i=0; i<inputs->size(); i++) { //We go through every file. This is gonna be a complexity mess
		tomix=(CS229File*)inputs->at(i); //get MusicFile
		toadd=tomix->returnData();
		tomultiply=multiply->at(i); //Get Multiplier
		for(int j=0;j<toadd->size();j++) {
			channel=tomix->returnData()->at(j); //Get Channel
			for(int k=0;k<channel->size();k++) {
				this->addValue(j,k,channel->at(k)); //actually add the value
			}
		}
	}
	output->addData(data,headerStuff[3],headerStuff[2]);
	return true; //This could be void. I don't know how to error check

}
/*
 * \brief method to add a single value to the data array
 */
void SoundMix::addValue(int i,int j,int value) {
	std::vector<int>* channel=data->at(i);
	int newval=channel->at(j)+value;
	if (newval>max) {
		newval=max;
	}
	else if (value<min) {
		newval=min;
	}
	channel->operator [](j)=newval; //Sets new Value
}

void SoundMix::write() {
	output->writefile();
}
