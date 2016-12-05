/*!
 * \brief The different types of files that can be parsed
 */

#ifndef MUSICFILE_H_
#define MUSICFILE_H_
#ifndef IOSTREAM
#include <iostream>
#endif
#ifndef STRING
#include <string>
#endif
#ifndef VECTOR
#include <vector>
#endif
#ifndef HELPDIALOG_H_
#include "HelpDialog.h"
#endif
/*!
 * \author Michael Sila
 * \brief Class for Generic Music File. virtual methods that will be expanded on in sub-classes
 *
 * I'm making all methods virtual so if I want to change things down the line, I can
 */

class MusicFile {
public:
	MusicFile();
	virtual int numChannels()=0; //Number of Channels
	virtual long sampleRate()=0; //
	virtual long numSamples()=0;
	virtual double soundLength()=0;
	virtual int bitDepth()=0;
	virtual ~MusicFile();
	virtual FileType returnType()=0;
	virtual std::string toString()=0;
	virtual void writefile()=0;
	virtual void addData(std::vector<std::vector<int>*>* data,int nS,int nC)=0;


};


#endif /* MUSICFILE_H_ */

