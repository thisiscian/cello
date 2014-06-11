#ifndef __cello_frame__
#define __cello_frame__
	#include<cello/image.h>
	#include<cmath>
	#include<cello/gif.h>
	class Frame {
		public:
			float delay;
			Frame();
			void draw();	
			unsigned char* pixels;
		private:	
			int width, height;
	};
#endif
