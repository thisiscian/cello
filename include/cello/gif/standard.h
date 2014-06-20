#ifndef __cello_gif_standard__
#define __cello_gif_standard__
	#include<cello/gif/common.h>
	#include<gif_lib.h>
	#include<vector>

	typedef unsigned char byte;
	namespace cello {
		namespace standard {
			extern std::fstream gout;
			extern GifFileType* filetype;
			extern ColorMapObject* globalColourMap;	
			extern ExtensionBlock animationControl;
			extern ExtensionBlock graphicControl;
			extern GifImageDesc currentImageDesc;
			extern std::vector<SavedImage> savedImages;
			extern int imageCount;

			void start();
			void stop();

			int writeFunction(GifFileType *gf, const GifByteType *gb, int count);
			void writeGraphicControlExtension(int disposal, int ui, int transparent, int delay, byte transparentColour);
			void writeImageDescriptor(int left, int top, int width, int height, int table, int interlace, int sort, int tableSize);
			void writeColourTable(int size, byte* table);
			void writeTableBasedImageData(byte minLZWSize, byte size, byte* data);

			void writeApplicationExtension(byte label, byte size, std::string identifier, std::string auth);
			void writeLogicalScreenDescriptor(int width, int height, int table, int resolution, int sort, int tableSize, byte backgroundColour, byte pixelAspectRatio);
			void writeHeader(std::string version);
			void writeBlockTerminator();
			void writeDataSubBlock(byte size, byte* data);
		};
	};

#endif
