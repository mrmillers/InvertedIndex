#ifndef MY_COMPRESSER
#define MY_COMPRESSER


class Compresser{
public:
	virtual int compress(unsigned int *in, unsigned int*& out, int inSize,int outSize) = 0;
	virtual int uncompress(unsigned int *in, unsigned int*& out, int inSize,int outSize) = 0;
	virtual ~Compresser(){}
};

class Simple9:public Compresser{
public:
	int compress(unsigned int *in, unsigned int*& out, int inSize,int outSize);
	int uncompress(unsigned int *in, unsigned int*& out, int inSize,int outSize);
private:
	static const unsigned int nMask[9];
	static const unsigned int nCode[9];
	static const unsigned int nCunt[9];
	static const unsigned int nLen[9];
};

class NoCompresser :public Compresser{
	int compress(unsigned int *in, unsigned int*& out, int inSize, int outSize);
	int uncompress(unsigned int *in, unsigned int*& out, int inSize, int outSize);
};
#endif