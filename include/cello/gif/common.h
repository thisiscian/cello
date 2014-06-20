#ifndef __cello_gif_common__
#define __cello_gif_common__
	#include<fstream>
	#include<string>
	#include<set>
	#include<cmath>
	typedef unsigned char byte;
	namespace cello {
		extern int width;
		extern int height;
		extern int delay;
		extern bool useStandard;
		extern std::string filename;
		extern int currentFrame;
	
		void setFunctionsToCustom();
		void setFunctionsToStandard();

		void start(std::string filename, int width, int height);

		extern void (*stop)();
		extern void (*writeHeader)(std::string version);
		extern void (*writeApplicationExtension)(byte label, byte size, std::string identifier, std::string auth);
		extern void (*writeGraphicControlExtension)(int disposal, int ui, int transparent, int delay, byte transparentColour);
		extern void (*writeImageDescriptor)(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize);
		extern void (*writeColourTable)(int size, byte* table);
		extern void (*writeLogicalScreenDescriptor)(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio);
		extern void (*writeDataSubBlock)(byte size, byte* data);
		extern void (*writeTableBasedImageData)(byte minLZWSize, byte size, byte* data);
		extern void (*writeBlockTerminator)();
		extern void (*writeTrailer)();
	};

#endif
