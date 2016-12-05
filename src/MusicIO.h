
#ifndef MUSICIO_H_
#define MUSICIO_H_

#ifndef MUSICFILE_H_
#include "MusicFile.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
/*!
 * \brief class to define IO api. Really just the read part.
 */
using namespace std;
class MusicIO {
public:
	MusicIO(string f);
	MusicIO();
	virtual ~MusicIO();
	virtual bool readHeader()=0;
	virtual bool readData()=0;
	virtual MusicFile* create_MusicFile()=0;
	virtual bool validate_header()=0;
	virtual bool validate_data()=0;
	vector<string>* split(string info,string tolken);
	std::string tolower(char[],int);
protected:








};

#endif /* MUSICIO_H_ */
