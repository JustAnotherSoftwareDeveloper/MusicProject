

#ifndef CS229IO_H_
#define CS229IO_H_
#ifndef MUSICIO_H_
#include "MusicIO.h"
#endif
#include <map>
#include <exception>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifndef CS229FILE_H_
#include "CS229File.h"
#endif
class CS229IO: public MusicIO {
public:
	CS229IO(string f);
	CS229IO();
	virtual ~CS229IO();
	virtual bool readHeader();
	virtual bool readData();
	virtual MusicFile* create_MusicFile();
	virtual bool validate_header();
	virtual bool validate_data();
private:
	map<string,int>* headerNames;
	ifstream* musicStream;
	string filename;
	string* headerInfo;
	vector<int>* dataInfo;
	bool tofile;
};


#endif /* CS229IO_H_ */
