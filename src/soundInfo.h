
#ifndef SOUNDINFO_H_
#define SOUNDINFO_H_

#include <string>
#include <iostream>
#include <fstream>
/*
 * \brief the
 */
using namespace std;
class soundInfo {
public:
	soundInfo(string f);
	virtual ~soundInfo();
	soundInfo();
	soundInfo(string f,string fileout); //This won't be called unless I have extra time
	int writeFile();
private:
	string* filename;
	ostream* output;
};

#endif /* SOUNDINFO_H_ */
