#ifndef __cello_frame__
#define __cello_frame__
	#include<cmath>
	#include<set>
	#include<vector>
	#include<map>
	#include<cello/image.h>
	#include<cello/pixel.h>
	#include<cello/gif.h>

	typedef std::vector<int> Code;

	class Frame {
		public:
			float delay;
			byte* pix;
			Pixel* pixels;

			Frame();
			void draw();
			void write();
		private:	
			int width, height;
			void drawImage(byte* p, int posX, int posY, int w, int h, int fw, int fh);
			std::vector<byte> compressIndexStream(Code indexStream, int minCodeSize);
	};
	
#endif
