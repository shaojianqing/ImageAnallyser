#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "type.h"
#include "bitmap.h"

byte setupList[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 
				    0b00010000, 0b00100000, 0b01000000, 0b10000000};

byte clearList[] = {0b11111110, 0b11111101, 0b11111011, 0b11110111, 
				    0b11101111, 0b11011111, 0b10111111, 0b01111111};

void renderDataGrid(byte *buffer, int size) {
	
	int i=0, j=0;
	char string[8];
	
	for (i<0;i<size/8;++i) {
		for (j=0;j<8;++j) {
			byte num = *(buffer+i*8+j);
			printf("%02X ", num);		
		}
		printf("\n");
	}
}

void setupBitmap(int index, byte *buffer) {
	if (buffer!=NULL) {
		int pos = index/BYTE_SIZE;
		int bit = index%BYTE_SIZE;
		byte data = *(buffer+pos);
		data = data | setupList[bit];
		*(buffer+pos) = data;
	}
}

void clearBitmap(int index, byte *buffer) {
	if (buffer!=NULL) {
		int pos = index/BYTE_SIZE;
		int bit = index%BYTE_SIZE;
		byte data = *(buffer+pos);
		data = data & clearList[bit];
		*(buffer+pos) = data;
	}
}
