#ifndef __cello_gif_colourmap__
#define __cello_gif_colourmap__
	#include<cstddef>
	#include<gif_lib.h>
	#include<set>
	#include<cello/pixel.h>

	typedef unsigned char Byte;

	class ColourMap {
		public:
			ColourMap();

			void add(Byte r, Byte g, Byte b);
			void add(Byte *p);
			
			int contains(Byte *p);
			int contains(Byte r, Byte g, Byte b);

			ColorMapObject *toStandard();
			size_t size();
			Byte colours[3*256];
		private:
			size_t _size;
			void initialise();
			std::set<Pixel> unique;
	};

#endif
