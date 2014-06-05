#include<cello/frame.h>
using std::set;

Frame::Frame() {
	originX=0; originY=0;
	translateX=0.0; translateY=0.0; scaleX=1.0; scaleY=1.0f; rotate=1.0f;
	pixels=new GifByteType[cello::width*cello::height*3]();
	delay=cello::frameSep;
}

Pixel Frame::pixel(int x, int y) {
	int pos=3*y*cello::width+3*x;
	return Pixel(&(pixels[pos]), &(pixels[pos+1]), &(pixels[pos+2]));
}

int Frame::getPaletteSize() {
	set<Pixel, PixelCompare> uniqueColours;
	for(int i=0; i<cello::width;i++) {
		for(int j=0; j<cello::height;j++) {
			uniqueColours.insert(pixel(i,j));
		}
	}
	return uniqueColours.size();
}

GifColorType* Frame::getColourMap() {
	set<Pixel, PixelCompare> uniqueColours;
	for(int i=0; i<cello::width;i++) {
		for(int j=0; j<cello::height;j++) {
			uniqueColours.insert(pixel(i,j));
		}
	}

	GifColorType* colourMap=new GifColorType[uniqueColours.size()];
	int i=0;
	for(set<Pixel, PixelCompare>::iterator it=uniqueColours.begin(); it!=uniqueColours.end(); ++it) {
		colourMap[i].Red=*(it->r);
		colourMap[i].Blue=*(it->g);
		colourMap[i++].Green=*(it->b);
	}
	return colourMap;
}

void Frame::defaultRestrictPalette() {
	int currentSize=getPaletteSize();
	if(currentSize <= cello::maxPaletteSize) { return; }

	for(int i=0; i<cello::width;i++) {
		for(int j=0; j<cello::height;j++) {
			GifByteType r=*(pixel(i,j).r);
			GifByteType g=*(pixel(i,j).g);
			GifByteType b=*(pixel(i,j).b);
			*pixel(i,j).r=r-r%3;
			*pixel(i,j).g=g-g%3;
			*pixel(i,j).b=b-b%3;
		}
	}
}
