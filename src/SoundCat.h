
#ifndef SOUNDCAT_H_
#define SOUNDCAT_H_

#ifndef MUSICFILE_H_
#include "MusicFile.h"
#endif
#ifndef VECTOR
#include <vector>
#endif
class MusicFile; //Class Header? Fuck it, fixed the compile error
class SoundCat {
public:
	SoundCat(int argc, char** argv);
	virtual ~SoundCat();
	bool createMusicFiles();
	bool createOutput();
	string toString();
	void write();
private:
	string filename;
	vector<MusicFile*>* files;
	vector<string>* file_names;
	MusicFile* outputFile;
	bool wave;
};


#endif /* SOUNDCAT_H_ */
