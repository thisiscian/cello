#ifndef __cello_pixel__
#define __cello_pixel__

	class Pixel {
		public:
		Pixel(unsigned char* P);
		Pixel(char* P);
		unsigned char operator[](int x);
		const unsigned char operator[](int x) const;
		bool operator<(const Pixel& P) const;
		bool operator==(const Pixel& P) const;
		private:
		unsigned char* p;
		const int sum() const;
	};
#endif
