#ifndef __cello_frame__
#define __cello_frame__
	#include<vector>
	#include<gif_lib.h>
	typedef unsigned char PixCol;

	namespace cello {
		void size(int squareWidth);
		void size(int width, int height);
		float frameSep;
		float width, height;
	};

	class Pixel {
		public:
			PixCol *r, *g, *b, *a;
			Pixel(PixCol *r, PixCol *g, PixCol *b, PixCol *a);
			void operator=(PixCol g);
			void operator()(PixCol g);
			void operator()(PixCol g, PixCol alpha);
			void initialise(PixCol R, PixCol G, PixCol B, PixCol A);
	};

	class Frame {
		public:
			Frame();
			Pixel pixels(int x, int y);
		private:
			int width, height;
			int originX, originY;
			float translateX, translateY, scaleX, scaleY, rotate;
			GifByteType *r,*g,*b,*a;
	};
#endif
