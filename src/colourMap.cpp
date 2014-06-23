#include <cello/colourMap.h>
#include <iostream>
#include <cmath>
using std::set;
using std::log;
using std::ceil;
using std::cout;
using std::endl;

ColourMap::ColourMap() {
	initialise();	
}

void ColourMap::add(Byte r, Byte g, Byte b) {
	Byte pixel[3]={r,g,b};
	int prevSize=unique.size();
	unique.insert(Pixel(pixel));
	if(prevSize!=unique.size()) {
		colours[3*_size]=r;
		colours[3*_size+1]=g;
		colours[3*_size+2]=b;
		_size=(_size+1)%256;
	}
};

void ColourMap::add(Byte* p) { 
	int prevSize=unique.size();
	unique.insert(Pixel(p));
	if(prevSize!=unique.size()) {
		colours[3*_size]=p[0];
		colours[3*_size+1]=p[1];
		colours[3*_size+2]=p[2];
		_size=(_size+1)%256;
	}
};

size_t ColourMap::size() {
	if(_size==0) { return 0; }
	return (size_t) std::max(2.0,pow(2.0,ceil(log(_size)/log(2))));
}

int ColourMap::contains(Byte *p) {
	set<Pixel>::iterator px=unique.find(Pixel(p));
	int index=0;
	for(set<Pixel>::iterator it=unique.begin(); it!=unique.end(); ++it) {
		if(px==it) { return index; }
		index++;
	}
	return -1;
	return (unique.count(Pixel(p))!=0); //contains(p[0], p[1], p[2]);
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
	int count=(int) pow(2,(int) ceil(log(size())/log(2)));
	GifColorType standardMap[count];
	for(int i=0; i<size(); i++) {
		standardMap[i].Red=colours[3*i+0];
		standardMap[i].Green=colours[3*i+1];
		standardMap[i].Blue=colours[3*i+2];
	}

	ColorMapObject *map=GifMakeMapObject(count, standardMap);
	cout << "toStandard() size=" << count << ", address=" << map << endl;

	return map;
};

void ColourMap::initialise() {
	_size=0;
	for(int i=0; i<3*256; i++) {
		colours[i]=0;
	}
}
