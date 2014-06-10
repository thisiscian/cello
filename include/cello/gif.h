#ifndef __cello_gif__
#define __cello_gif__
	#include <fstream>
	#include <iomanip>
	#include <string>
	#include <set>

	namespace gif {
		int width;
		int height;
		extern char backgroundColour;
		extern int colourResolution;
		extern char applicationBlock[2];
		extern char graphicBlock[2];
		extern char imageBlock; 
		extern char noTransparency;
		extern char transparentColour;
		extern char endBlock;
		extern int delay;
		extern std::fstream gout;
		void init(std::string filename, int width, int height);
		void drawFrame(unsigned char* frame);	
		void writeTableAndFrame(unsigned char* frame);
	}	

	class Pixel {
		Pixel(unsigned char* p);
		unsigned char operator[](size_t x);
		const unsigned char operator[](size_t x) const;
	};

#endif
