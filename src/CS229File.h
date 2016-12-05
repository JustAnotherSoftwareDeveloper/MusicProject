

#ifndef CS229FILE_H_
#define CS229FILE_H_

#ifndef MUSICFILE_H_
#include "MusicFile.h"
#endif

class CS229File: public MusicFile {
public:
	CS229File();
	CS229File(int c,long sr, int n,int d,std::string nm);
	CS229File(int c,long sr,int d,std::string nm);
	CS229File(int c,long sr,int num,int d,std::string nm,std::vector<int> data);
	virtual std::string toString();
	virtual std::vector<std::vector<int>* >* returnData();
	virtual void addData(std::vector<std::vector<int>*>* data,int nS,int nC);
	virtual void writefile();
	virtual ~CS229File();
	virtual int numChannels(); //Number of Channels
	virtual long sampleRate(); //
	virtual long numSamples();
	virtual double soundLength();
	virtual int bitDepth();
	virtual FileType returnType();
private:
	std::vector< std::vector<int>* >* channelsInfo; //An Array of Channels and their info
	std::string fileName; //Name of File
	int nChannels;
	long sRate;
	int nSamples;
	int bdepth;
	FileType type;
};


#endif /* CS229FILE_H_ */
