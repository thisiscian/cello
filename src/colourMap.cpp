#include <cello/colourMap.h>
using std::set;

ColourMap::ColourMap() {
	initialise();	
}

void ColourMap::add(Byte r, Byte g, Byte b) { 
	colours[3*size()]=r;
	colours[3*size()+1]=g;
	colours[3*size()+2]=b;
	_size=(_size+1)%256;
};

void ColourMap::add(Byte* p) { add(p[0], p[1], p[2]); };

size_t ColourMap::size() {
	return _size;
}

int ColourMap::contains(Byte *p) {
	return contains(p[0], p[1], p[2]);
}

int ColourMap::contains(Byte r, Byte g, Byte b) {
	for(int i=0; i<size(); i++) {
		Byte R=colours[3*i+0];
		Byte G=colours[3*i+1];
		Byte B=colours[3*i+2];
		if(r==R && g==G && b==B) { return i; }
	}
	return -1;
}

ColorMapObject *ColourMap::toStandard() {
	if(size()==0) { return NULL; }
	GifColorType standardMap[size()];
	for(int i=0; i<size(); i++) {
		standardMap[i].Red=colours[3*i+0];
		standardMap[i].Green=colours[3*i+1];
		standardMap[i].Blue=colours[3*i+2];
	}
	return GifMakeMapObject(size(), standardMap);
};

void ColourMap::initialise() {
	_size=0;
	for(int i=0; i<3*256; i++) {
		colours[i]=0;
	}
}
