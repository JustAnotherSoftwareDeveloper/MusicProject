
#ifndef SOUNDMIX_H_
#define SOUNDMIX_H_
#include <string>
#ifndef VECTOR
#include <vector>
#endif
class MusicFile; //Worked for SoundCat
class SoundMix {
public:
	SoundMix(int argc, char** argv);
	bool createfiles();
	bool createOutput();
	//TODO: Create a multiply method in MusicFile() and CS229File();
	virtual ~SoundMix();
	void write();
private:
	std::string filename;
	MusicFile* output;
	std::vector<std::string>* filenames;
	std::vector<int>* multiply;
	std::vector<MusicFile*>* inputs;
	void addValue(int i,int j,int value);
	int min,max;
	std::vector<std::vector<int>*>* data;

};


#endif /* SOUNDMIX_H_ */
