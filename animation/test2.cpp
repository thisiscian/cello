#include <cello.hpp>
using namespace std;
using gif::gout;

int main(int argc, char* argv[]) {
	gif::init("hello.gif",100,100);
	gout.close();
	return 0;
}
