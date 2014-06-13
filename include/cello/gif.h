#ifndef __cello_gif__
#define __cello_gif__
	#include <fstream>
	#include <set>
	#include <string>
	#include <cmath>

	typedef unsigned char byte;
	namespace gif {
		extern std::fstream gout;
		void writeTrailer();
		void writeApplicationExtension(byte label, byte size, std::string identifier, std::string auth);
		void writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour);
		void writeTableBasedImageData(byte minLZWSize, byte size, byte* data);
		void writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize);
		void writeColourTable(int size, byte* table);
		void writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio);
		void writeHeader(std::string version);
		void writeBlockTerminator();
		void writeDataSubBlock(byte size, byte* data);
	}	

#endif
