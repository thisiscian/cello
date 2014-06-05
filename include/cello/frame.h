#ifndef __cello_frame__
#define __cello_frame__
	#include<cello/pixel.h>
	class Frame {
		public:
			GifByteType *pixels;
			float delay;

			Frame();
			void draw();
			void defaultRestrictPalette();
			void restrictPalette();
			Pixel pixel(int x, int y);
			int getPaletteSize();
			GifColorType* getColourMap();
		private:
			int originX, originY;
			float translateX, translateY, scaleX, scaleY, rotate;
	};

#endif
