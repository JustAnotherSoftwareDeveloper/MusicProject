
#include "HelpDialog.h"

/*!
   \brief Default constructer. Sets cout as the output stream
*/
HelpDialog::HelpDialog() {
	writer = &std::cout;
}
/*!
	\brief Allows an alternative writer to be set
*/
HelpDialog::HelpDialog(ostream &writeout) {
	writer = &writeout;
}
/*!
	\brief Returns the appropriate help string based off the program
	\param program: name of program
*/
string HelpDialog::helpString(string program) {
	string message;
	//Should be able to use switch statement, but compiler keeps detecting my string as a char*
	if (program == "sndinfo") {
		message = "This program reads all the sound files passed as arguments, and for each one, displays the following:\n\t-The File Name \n\t-The File Type \n\t-The sample rate \n\t-The bit depth \n\t-The number of channels \n\t-The number of samples \n\t-The length of sound (in seconds) \n To use, please type '... sndinfo FILENAME'. Passing the argument '-h' at any point returns this message \n";
	}
	else if (program == "sndcat") {
		message = "This program reads all sound files passed as arguments, and writes a single sound file that is the concatenation of inputs. Note the files must be of the same type and have the same bit depth. The program also supports the following switches:\n\t-h : displays this message \n\t-o FILE: specifies the output file. If empty, the program writes to the standard output \n\t-w outputs a .wav file format \n";
	}
	else if (program == "sndmix") {
		message = "This program reads all sound files passed as arguments, and 'mixes' them into a single sound file. This program is invoked as 'sndmix [switches] MULTI_I FILE_I...MULTI_N FILE_N. The scaled data is summed to produce the output file. The following switches are supported:\n\t-h : displays this message \n\t-o FILE: specifies the output file name. otherwise, writes to std::out \n";
	}
	else if (program == "sndgen") {
		message = "This program produces a sound of a specified frequency and waveform, using a simple ADSR envelop. The following switches are supported:\n\t-h: displays this message \n\t--bits N: uses a bit depth of n( default of 8) \n\t-sr N uses a sample rate of n \n\t-f R: use a frequency of r Hz\n\t-t R total duration of r seconds\n\t-v P peak volume (between 0 and 1)\n\t-a attack time R: attack time of r seconds\n\t-d R: decay time in R seconds \n\t-s P: sustain volume (from 0 to 1) \n\t-r R: release time in R seconds\n\t--sine : generate a sine wave \n\t--triangle : generate a triangle wave \n\t--sawtooth : generate a sawtooth wave \n\t--pulse : generate a pulse wave \n\t-pf Fraction of the time the pulse wave is up \n\t-o : The file to write the output to. Uses standard output be default \n ";
	}
	else if (program == "sndcvt") {
		message = "TODO \n";
	}
	else if (program == "sndfx") {
		message = "TODO \n";
	}
	else if (program == "sndplay") {
		message = "TODO \n";
	}
	else {
		message = "Program not recognized \n";
	}
	return message;
}

/*
	\brief prints out the correct string based of the program
	\param program: the name of the program
*/
void HelpDialog::helpWrite(string program)
{
	string s = HelpDialog::helpString(program);
	*writer << s;
}

HelpDialog::~HelpDialog()
{
	if (writer != &std::cout) {
		delete writer;
	}
}



