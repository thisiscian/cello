#ifndef __cello_image__
#define __cello_image__
	#include<cello/pixel.h>
	#include<cello/gif.h>
	#include<set>
	#include<map>
	#include<cmath>
	#include<string>
	class Image {
		public:
			Image(unsigned char* pixels, int width, int height);
			void draw(int posX, int posY, int width, int height);
		private:
			int frameWidth;
			int frameheight;
			int posX, posY;
			unsigned char* pixels;
			unsigned char* indexStream;
			char colourTable[256*3];
			unsigned char lzwMinCodeSize;
			unsigned char clearCode;
			unsigned char eoiCode;
			void calculateColourTable(int posX, int posY, int width, int height);
			void lzwCompress(int size);
	};
#endif
