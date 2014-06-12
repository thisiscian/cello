#ifndef __cello_required__
#define __cello_required__
	#include<string>
	#include<cello/gif.h>
	namespace cello {
		extern int width;
		extern int height;
		extern int delay;
		void start(std::string filename, int width, int height);
		void stop();
	};

#endif
