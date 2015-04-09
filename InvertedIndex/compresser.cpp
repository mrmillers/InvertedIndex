#include "compresser.h"
#include <stdlib.h>
#include <stdio.h>


const unsigned int Simple9::nMask[9]={0xFFFFFFFE,0xFFFFFFFC,0xFFFFFFF8,0xFFFFFFF0,0xFFFFFFE0,0xFFFFFF80,0xFFFFFE00,0xFFFFC000,0xF0000000};
//                                        1        2            3            4        5           7           9        14         28
const unsigned int Simple9::nCode[9]={0x00000000,0x10000000,0x20000000,0x30000000,0x40000000,0x50000000,0x60000000,0x70000000,0x80000000};
const unsigned int Simple9::nCunt[9]={28,14,9,7,5,4,3,2,1};
const unsigned int Simple9::nLen[9] ={1,2,3,4,5,7,9,14,28};
int Simple9::compress(unsigned int *in, unsigned int*& out, const int inSize,int outSize){
	int pos = 0,outPos = 0;
	bool flag;
	while (pos<inSize){
		for (int i=0;i<9;i++){
			flag = true;
			if (pos+nCunt[i]>inSize)
				continue;
			for (int j=0;j<nCunt[i];j++){
				if (in[pos + j] & nMask[i]){
					flag = false;
					break;
				}
			}
			if (flag){
				if (outPos == outSize){
					outSize *= 2;
					out = (unsigned int *)realloc(out,outSize * sizeof(unsigned int));
				}
					
				out[outPos] = 0;
				for (int j=0;j<nCunt[i];j++){
					out[outPos] = (out[outPos]<<nLen[i]) | in[pos + j] ;
					//*outPos = *outPos| in[pos + j];
				}
				out[outPos] = out[outPos] | nCode[i];
				pos += nCunt[i];
				outPos++;
				break;
			}

		}
	}
	
	//flag = false;
	//unsigned int * test = (unsigned int*)malloc(sizeof(int)*outSize);
	//int testS = uncompress(out,test,outPos,outSize);
	//for (int i = 0; i < inSize; i++){
	//	if (in[i] != test[i]) {
	//		flag = true;
	//		//compress(in, out, inSize, outSize);
	//		fprintf(stderr,"Compress ERROR\n");
	//		getchar();
	//		break;
	//	}
	//}
	return outPos;
}

int Simple9::uncompress(unsigned int *in, unsigned int*& out, int inSize,int outSize){
	int pos = 0,outPos = 0,code;
	unsigned int tmp;
	
	while (pos<inSize){
		code = in[pos]>>28;
		//if (code > 8) { fprintf(stderr, "code %d\n", code, in[pos]); }
		tmp = in[pos];
		if (outPos+nCunt[code] > outSize){
			outSize = outPos * 2 + nCunt[code];
			out = (unsigned int *)realloc(out,sizeof(unsigned int) * outSize);
			//fprintf(stderr,"%d %d\n",outSize,out);
		}
		for (int i=0;i<nCunt[code];i++){
			out[outPos + nCunt[code] - i - 1] = tmp & ((1<<nLen[code])-1);
			tmp = tmp >> nLen[code];
		}
		pos++;
		outPos += nCunt[code];
		//fprintf(stderr, "%d ", outPos);
	}
	return outPos;
}

int NoCompresser::compress(unsigned int *in, unsigned int*& out, int inSize, int outSize){
	out = (unsigned int *)realloc(out, sizeof(unsigned int) * inSize);
	for (int i = 0; i < inSize; i++){
		out[i] = in[i];
	}
	return inSize;
}
int NoCompresser::uncompress(unsigned int *in, unsigned int*& out, int inSize, int outSize){
	out = (unsigned int *)realloc(out, sizeof(unsigned int) * inSize);
	for (int i = 0; i < inSize; i++){
		out[i] = in[i];
	}
	return inSize;
}