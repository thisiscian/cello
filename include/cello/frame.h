#ifndef __cello_frame__
#define __cello_frame__
	#include<vector>
	#include<set>
	#include<gif_lib.h>
	typedef unsigned char PixCol;
	class Frame;

	namespace cello {
		void size(int squareWidth);
		void size(int width, int height);
		extern float frameSep;
		extern int width;
		extern int height;
		extern int maxPaletteSize;
		extern bool checkFileExists;
		extern GifColorType defaultPalette[];
		void restrictPalette(Frame &f);
		GifColorType* getPalette(Frame &f);
	};

	class Pixel {
		public:
			PixCol *r, *g, *b;
			Pixel(PixCol *r, PixCol *g, PixCol *b);
			void operator=(PixCol g);
			void operator()(PixCol g);
			void initialise(PixCol R, PixCol G, PixCol B);
	};

	class PixelCompare {
		public: bool operator()(const Pixel& lhs, const Pixel& rhs);
	};

	class Frame {
		public:
			Frame();
			Pixel pixels(int x, int y);
			PixCol *pixelArray;
			int getPaletteSize();
		private:
			int width, height;
			int originX, originY;
			float translateX, translateY, scaleX, scaleY, rotate;
	};
#endif
