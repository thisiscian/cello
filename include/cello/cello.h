#ifndef __cello_cello__
#define __cello_cello__
	#include <fstream>
	#include <iostream>
	#include <string>
	#include <set>
	#include <cmath>
	#include <vector>
	#include <cello/colourMap.h>
	#include <cello/image.h>
	#include <cello/frame.h>
	typedef unsigned char Byte;
	int standardWrite(GifFileType * ft, const GifByteType * data, int count);

	class Cello {
		public:
			Cello();

			std::fstream gout;
			Frame currentFrame;
			int frameDelay;
			int mode;
				#define STANDARD 0
				#define CUSTOM 1
				#define BOTH 2
			int frameCount;
			ColourMap globalColourMap;
			
			void start(std::string filename, size_t width, size_t height);

			int getWidth();
			int getHeight();
			std::string getFilename();
		
			void store();
			void save();

		private:
			size_t width, height;
			std::string filename;
			std::vector<Image> images;
			std::vector<Frame> frames;

			void initialise();
			void customSave();
			void standardSave();
		
			void writeDataSubBlock(Byte size, Byte* data);
			void writeBlockTerminator();
			void writeHeader(std::string version);
			void writeLogicalScreenDescriptor(int width, int height, int resolution, int sort, int tableSize, Byte backgroundColour, Byte pixelAspectRatio);
			void writeColourTable(int size, Byte* table);
			void writeImageDescriptor(int left, int top, int width, int height, int interlace, int sort, int tableSize);
			void writeTableBasedImageData(Byte minLZWSize, Byte size, Byte* data);
			void writeApplicationExtension(Byte label, Byte size, std::string identifier, std::string auth);
			void writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, Byte transparentColour);
			void writeTrailer();
	};

#endif
