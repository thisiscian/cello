#ifndef __cello_frame__
#define __cello_frame__
	#include<cmath>
	#include<set>
	#include<cello/cello.h>
	#include<cello/pixel.h>
	#include<cello/gif.h>
	class Frame {
		public:
			float delay;
			Frame();
			void draw();	
			unsigned char* pix;
			Pixel* pixels;
		private:	
			int width, height;
	};
	
	void drawImage(char* p, int posX, int posY, int w, int h, int fw, int fh);
#endif
