#include<cello/animation.h>
using std::cout;
using std::cerr;
using std::endl;

int writeAnimation() {
	int err=0;
	Frame frame;
	GifFileType *output = EGifOpenFileName(cello::fileName.c_str(), cello::checkFileExists, &err);
	if(err) {
		cerr << "Error: libgif failed with message \"" << GifErrorString(err) << "\"... quitting" << endl;
		return err;
	}

	EGifPutScreenDesc(output, cello::width, cello::height, 8,0, NULL);

	// initiate infiniteLoop
	if(cello::infiniteLoop) {
		char loopCode[16]={0x4E,0x45,0x54,0x53,0x43,0x41,0x50,0x45,0x32,0x2E,0x30,0x03,0x01,0x00,0x00,0x00};
		EGifPutExtension(output, APPLICATION_EXT_FUNC_CODE,16,loopCode);
	}

	// write frames to file
	for(cello::currentFrame=0; cello::currentFrame<cello::period; cello::currentFrame++) {

		char frameControl[4]={0x04,0x00,0x00,0xff};
		frameControl[1]=(int)(frame.delay*100) % 256;
		frameControl[2]=(int)(frame.delay*100) / 256;
		EGifPutExtension(output, GRAPHICS_EXT_FUNC_CODE, 4, frameControl);

		frame.draw();
		frame.defaultRestrictPalette();
		cout << (int) frame.pixels[102] << endl;
		const GifColorType* colourMap=frame.getColourMap();
		ColorMapObject* palette=GifMakeMapObject(cello::maxPaletteSize, colourMap);
		EGifPutImageDesc(output,0,0,cello::width,cello::height,false,palette);
		
		for(int i=0; i<cello::height; i++) {
		//for(int i=cello::height-1; i>=0; i--) {
			EGifPutLine(output, &(frame.pixels[i*cello::height]), cello::width);
		}
	}

	// close infiniteLoop
	if(cello::infiniteLoop) {
		char loopControl[3]={1,0,0};
	//	EGifPutExtension(output, APPLICATION_EXT_FUNC_CODE,3, loopControl);
	}
	EGifCloseFile(output);
	cout << "Saved gif to " << cello::fileName << endl;
}
