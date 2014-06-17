#ifndef __cello_gif_custom__
#define __cello_gif_custom__
	#include<cello/gif/common.h>
	namespace cello {
		namespace custom {
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
			void start(std::string filename, int width, int height);
			void stop();
		}	
	}

#endif
