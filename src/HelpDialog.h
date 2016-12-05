enum FileType {WAVE,CS229,ABC229,UNDEFINED};

#ifndef MUSICFILE_H_
#define MUSICFILE_H_
#include <ostream>
#include <iostream>
#ifndef STRING
#include <string>
#endif
using namespace std;
/*
* \brief Class for Displaying Help files
* This method will display the help files for the various programs
* it will can also simply return the help string, and have another program
* do the I/O. I was debating removing the I/O so that my MusicIO class will do all the
* Writing, but I felt the simple cout statement was concise enough to keep within the help
* class
*
*/
class HelpDialog
{
public:
	HelpDialog();
	HelpDialog(ostream &writeout);
	string helpString(string program);
	void helpWrite(string program);
	virtual ~HelpDialog();
private:
	ostream* writer;
};
#endif
