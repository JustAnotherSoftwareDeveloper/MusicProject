/*!
 * \author Michael Sila
 * \brief Main Method for My Project
 *
 */
#ifndef HELPDIALOG_H_
#include "HelpDialog.h"
#endif


#ifndef SOUNDCAT_H_
#include "SoundCat.h"
#endif
#ifndef SOUNDINFO_H_
#include "soundInfo.h"
#endif
#include "SoundMix.h" //For some reason include guards don't work on this one. YAAAAAAAY!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifndef SOUNDGEN_H_
#include "SoundGen.h"
#endif
#include <iostream>
#include <stdlib.h>

using namespace std;
int main(int argc, char** argv)
{
	/* For test purposes. If you see these lines in my code I fucked up in the worst possible way
	 * ...but if you'd be awesome enough to delete them that'd be great;
	 */

	if (argc < 2) {
		cout << "Not Enough Arguments" << endl;
	}
	else if (argc==2) {
		HelpDialog* help = new HelpDialog();
		help->helpWrite(argv[1]);
		//don't need to call destructor since program ends right after
		return 0;
	}
	else {
		/* If there is a -h flag anywhere in the arguments, the program will display the appropriate help file
		*/
		HelpDialog* help;
		string command;
		for (int i = 0; i < argc; i++) {
			command=argv[i];
			if ("-h" == command) {
				help = new HelpDialog();
				help->helpWrite(argv[1]);
				return 0;
			}
		}
		command=argv[1];
		string name;
		if (command=="sndinfo") {
			if(argc>2) {name=argv[2];}
			else {name="cin";}
			soundInfo* sndinfo=new soundInfo(name);
			int correct=sndinfo->writeFile();
			delete sndinfo;
			return correct;
		}
		else if (command=="sndcat") {
			SoundCat* sndcat=new SoundCat(argc,argv);
			bool valid;
			valid=sndcat->createMusicFiles();
			if (!valid) {
				cout<<"SoundCat Input Error"<<endl;
				throw;
				return -1;
			}
			valid=sndcat->createOutput();
			if (!valid) {
				cout<<"MusicFile creation error"<<endl;
				throw;
			}
			sndcat->write();
			delete sndcat;
			return 0;
		}
		else if (command=="sndmix") {
			SoundMix* sndmix=new SoundMix(argc,argv);
			bool valid;
			valid=sndmix->createfiles();
			if (!valid) {
				throw "Sound Mix Creation Error";
			}
			valid=sndmix->createOutput();
			if (!valid) {
				throw "Sound Mix Creation Error";
			}
			sndmix->write();
			delete sndmix;
			return 0;
		}
		else if (command=="sndgen") {
			SoundGen* sndgen=new SoundGen(argc,argv);
			bool valid=sndgen->validateInput();
			if (!valid) {
				throw "Sound Gen Creation Error";
			}
			valid=sndgen->genOutput();
			if (!valid) {
				throw "Sound Gen Creation Error";
			}
			sndgen->write();
			delete sndgen;
			return 0;

		}
		else {
			cout<<"No Corresponding Commands Found"<<endl;
		}
	}
	//Simply Returns 0...maybe it should be -1?
	return 0;
}

