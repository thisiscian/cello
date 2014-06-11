#include<cello/frame.h>
#include<iostream>
using std::set;
using gif::gout;
using std::pair;
using std::map;
using std::string;
using std::min;

Image::Image(unsigned char* p, int w, int h) {
	pixels=p;
	frameWidth=w;
	frameheight=h;
	indexStream = new unsigned char[w*h];
}

void Image::calculateColourTable(int posX, int posY, int width, int height) {
	set<Pixel> uniqueColours;
	int k=0;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			pair<set<Pixel>::iterator, bool> tmp=uniqueColours.insert(Pixel(pixels+3*(i+posY)*frameWidth+3*(j+posX)));
			if(tmp.second) {
				colourTable[k++]=(*(tmp.first))[0];
				colourTable[k++]=(*(tmp.first))[1];
				colourTable[k++]=(*(tmp.first))[2];
			}
		}
	}

	lzwMinCodeSize=ceil(log(uniqueColours.size())/log(2));
	clearCode=pow(2,lzwMinCodeSize);
	eoiCode=clearCode+1;
	for(int i=0; i<height; i++) {
		for(int j=0; j<width; j++) {
			for(int k=0; k<(int) clearCode; k++) {
				if(Pixel(&colourTable[3*k])==Pixel(&pixels[3*(i+posY)*frameWidth+3*(j+posX)])) {
					indexStream[i*width+j]=k;
					break;
				}
			}
		}
	}
}

void Image::lzwCompress(int streamSize) {
	string output="";
	output+=clearCode;
	for(int i=0; i<streamSize; i++) {
		std::cout << (int) indexStream[i];
		if(i%10==9) std::cout << std::endl;
	}
	std::cout << (int) clearCode << ",";
	map<string,char> codeTable;
	int nextCode=0;
	for(char i=0; i<clearCode; i++) {
		codeTable[""+i]=nextCode++;
	}
	codeTable[""+clearCode]=nextCode++;
	codeTable[""+clearCode+1]=nextCode++;
		
	string indexBuffer=""+indexStream[0];

	for(int i=0; i<streamSize-1; i++) {
		char newIndex=indexStream[i+1];
		if( codeTable.count(indexBuffer+newIndex) == 0 ) {
			codeTable[indexBuffer+newIndex]=nextCode++;
			output+=codeTable[indexBuffer];
			std::cout << (int) codeTable[indexBuffer] << ",";
			indexBuffer=""+newIndex;
		} else {
			std::cout << ","; 
			indexBuffer+=newIndex;
		}
	}
	output+=codeTable[indexBuffer];
	std::cout << (int) codeTable[indexBuffer] << ",";
	output+=eoiCode;	
	std::cout << (int) eoiCode << std::endl;

	for(int i=0, j=output.length(), k=min(j,256); i<output.length(); j=output.length()-i, k=min(j,256), i+=k) {
		std::cout << i << "/" << output.length() << std::endl;
		gout << (char) k-1;
		gout << output.substr(i,k);
	}
}

void Image::draw(int posX, int posY, int width, int height) {
	calculateColourTable(posX, posY, width,height);

	char position[4]={posX%256,posX/256,posY%256,posY/256};
	char size[4]={width%256,width/256,height%256,height/256};
	gout << gif::imageBlock;
		gout.write(position, 4);
		gout.write(size,4);
		gout << clearCode;
		gout << gif::backgroundColour;
		gout << gif::pixelAspectRatio;
		gout.write(colourTable,clearCode);
		lzwCompress(width*height);
	gout << gif::endBlock;
}
