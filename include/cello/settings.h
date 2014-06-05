#ifndef __cello_settings__
#define __cello_settings__
	#include<vector>
	#include<string>
	#include<boost/program_options.hpp>
	#include<cello/version.h>
	#include<gif_lib.h>

	class Frame;

	namespace cello {
		void size(int squareWidth);
		void size(int width, int height);
		extern float frameSep;
		extern int width;
		extern int height;
		extern int maxPaletteSize;
		extern bool checkFileExists;
		extern bool infiniteLoop;
		extern int period;
		extern int currentFrame;
		extern GifColorType defaultColourMap[];
		extern std::string fileName;
		void defaultRestrictPalette();
	};

	class Settings {
		std::vector<std::string> inputFiles;
		public:
			int exit;
			Settings(int ac, char* av[]);
		private:
			void addToInputFiles(std::vector<std::string> add);
	};
#endif
