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

			int add(Byte *p);
			
			int contains(Byte *p);

			ColorMapObject *toStandard();
			size_t size();
			Byte colours[3*256];
		private:
			size_t _size;
			void initialise();
			std::set<Pixel> unique;
	};

#endif
