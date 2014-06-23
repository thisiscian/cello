#ifndef __cello_frame__
#define __cello_frame__
	#include<cmath>
	#include<set>
	#include<vector>
	#include<map>
	#include<cello/pixel.h>
	#include<cello/gif.h>
	#include<cello/image.h>

	typedef std::vector<int> Code;

	class Frame {
		public:
			float delay;
			byte* pix;
			Pixel* pixels;
			std::vector<Image> images;

			void initialise(int w, int h, int d);
		private:	
			int width, height;
	};
	
#endif
