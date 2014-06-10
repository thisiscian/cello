#ifndef __cello_pixel__
#define __cello_pixel__
	#include<set>
	#include<cello/settings.h>
	class Pixel {
		public:
			GifByteType *r, *g, *b;
			Pixel(GifByteType *r, GifByteType *g, GifByteType *b);
			void operator=(GifByteType g);
			void operator()(GifByteType g);
			void initialise(GifByteType R, GifByteType G, GifByteType B);
	};

	class PixelCompare {
		public:
			bool operator()(const Pixel& lhs, const Pixel& rhs);
			bool operator()(const unsigned char* lhs, const unsigned char* rhs);
	};
#endif
