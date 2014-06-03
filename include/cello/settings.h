#ifndef __cello_settings__
#define __cello_settings__
	#include<cello/version.h>
	#include<boost/program_options.hpp>
	#include<vector>
	#include<string>
	class Settings {
		std::vector<std::string> inputFiles;
		public:
			int exit;
			Settings(int ac, char* av[]);
		private:
			void addToInputFiles(std::vector<std::string> add);
	};
#endif
