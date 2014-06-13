#ifndef __cello_gif__
#define __cello_gif__
	#include <fstream>
	#include <set>
	#include <string>
	#include <cmath>

	namespace gif {
		extern std::fstream gout;
		void writeTrailer();
		void writeApplicationExtension(char label, char size, std::string identifier, std::string auth);
		void writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, char transparentColour);
		void writeTableBasedImageData(char minLZWSize, char size, char* data);
		void writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize);
		void writeColourTable(int size, char* table);
		void writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, char backgroundColour, char pixelAspectRatio);
		void writeHeader(std::string version);
		void writeBlockTerminator();
		void writeDataSubBlock(char size, char* data);
	}	

#endif
