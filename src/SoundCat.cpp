#ifndef CS229IO_H_
#include "CS229IO.h"
#endif

#include "SoundCat.h"

/*
 * \brief Constructor. Will read file names and output file
 * \param argc : passed from main
 * \param argv: passed from main
 */
using namespace std;
SoundCat::SoundCat(int argc, char** argv) {
	string arg;
	wave=false;
	outputFile=NULL; //to be created later
	files=NULL; //to be created later
	filename="cout";
	file_names=new vector<string>();
	for (int i=2; i<argc; i++) {
		arg=argv[i]; //Get the current parameter
		if (arg=="-o") {
			if (i!=argc-1) { //Because you know some clown is gonna think it's clever to test this by putting -o as the last argument
				i++;
				filename=argv[i];
			}
		}
		else {
			file_names->push_back(argv[i]);
		}
	}

}

SoundCat::~SoundCat() {
	delete file_names;
	for(int i=0; i<files->size(); i++) {
		delete files->at(i);
	}
	delete files;
	delete outputFile;
}
/*!
 * \brief creates output file from file list
 * \return false if anything fails
 */
bool SoundCat::createMusicFiles() {
	CS229IO* reader;
	MusicFile* tocat;
	bool valid;
	files=new std::vector<MusicFile*>();
	for(int i=0; i<file_names->size(); i++) {
		reader=new CS229IO(file_names->at(i));
		valid=reader->readHeader();
		if (!valid) {
			return false; //Header info incorrect
		}
		valid=reader->readData();
		if (!valid) {
			return false;
		}
		tocat=reader->create_MusicFile();
		delete reader; //So does this delete the variable name too? If so : f**ck
		files->push_back(tocat);
		tocat=NULL; //If I delete tocat it'll be gone from the vector too. this just removes the reference
	}
	/*
	 * NOTE: This will not check for duplicate files. So You can cat the same file over and over and over again. It will also overwrite
	 * the same file
	 */
	return true;
}
/*!
 *  \brief This creates the output file that'll be returned at the end of all this
 */
bool SoundCat::createOutput() {
	CS229File* tocat;
	long headerStuff[3]; // 0: SampleRate , 1: Channels 2: Bdepth,
	long fileStuff[3]; //for the loop
	if (files->size()<1) { return false;} //Something would have to be wrong in my coding
	headerStuff[0]=((CS229File*)files->at(0))->sampleRate();
	headerStuff[1]=((CS229File*)files->at(0))->numChannels();
	headerStuff[2]=((CS229File*)files->at(0))->bitDepth();
	for(int i=1; i<files->size();i++) {
		fileStuff[0]=((CS229File*)files->at(i))->sampleRate();
		fileStuff[1]=((CS229File*)files->at(i))->numChannels();
		fileStuff[2]=((CS229File*)files->at(i))->bitDepth();

		if (fileStuff[0]!=headerStuff[0]) {
			return false; //Different Sample Rate
		}
		if (fileStuff[1]>headerStuff[1]) {
			headerStuff[1]=fileStuff[1]; //Make sure highest number of channels is passed initially
		}
		if (fileStuff[2]>headerStuff[2]) {
			headerStuff[2]=fileStuff[2]; //Same for bitdepth
		}
	}
	outputFile=new CS229File(headerStuff[1],headerStuff[0],headerStuff[2],filename); //creates empty object
	/*
	 * Adds the data from each of the MusicFile objects passed
	 */
	for(int i=0; i<files->size(); i++) {
		tocat=(CS229File*)files->at(i);
		vector<vector<int>* >* toadd=(tocat->returnData());
		outputFile->addData(toadd,tocat->numSamples(),tocat->numChannels());
	}
	return true;
}
/*
 * \brief Just calls the internal MusicFile* method
 */
string SoundCat::toString() {
	return outputFile->toString();
}
/*
 * \brief this is what I'm actually going to use in the
 */
void SoundCat::write() {
	outputFile->writefile();
}
