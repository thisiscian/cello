#include <cello/settings.h>
using namespace std;
using namespace boost::program_options;

void Settings::addToInputFiles(vector<string> add) {
	inputFiles.insert(inputFiles.end(), add.begin(), add.end());
}

Settings::Settings(int ac, char* av[]) {
	// temporary variables
	variables_map vm;
	string geometry;
	options_description desc("Allowed options");
	positional_options_description p;	

	// default to not exit
	exit=0;

	desc.add_options()
		("help,h", "produce help message")
		("input,i", "files to be read")
	;
//		("width,w", value<int>(&width)->default_value(300), "set width of display, default: 800") // an example option
	p.add("input-file", -1);
	store(command_line_parser(ac,av).options(desc).positional(p).run(), vm);
	notify(vm);
	if(vm.count("help")) {
		cout << TITLE << " v" << VERSION_MAJOR << "." << VERSION_MINOR << endl;
		cout << desc << flush;
		exit=1;
	}
	if(vm.count("input-file")) {
		vector<string> tmp=vm["input"].as< vector<string> >();
		addToInputFiles(vm["input-file"].as< vector<string> >());
	}
	if(vm.count("input")) {
		addToInputFiles(vm["input"].as< vector<string> >());
	}
}