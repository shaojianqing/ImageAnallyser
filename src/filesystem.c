#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "filesystem.h"

int length;

byte *buffer;

SuperBlock superBlock;

SuperBlockDisk *superBlockDisk;

int main(int argc, char **argv) {
	
	
}

void loadFileSystem(char *imageName) {
	FILE *file = fopen(filename, "rw");
	if (file!=NULL) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		buffer = (byte *)malloc(length);
		rewind(file);
		fread(buffer, 1, length, file);
		fclose(file);
		buffer[length] = 0;

		initSuperBlock();
	}
}

void initSuperBlock() {
	superBlockDisk = (SuperBlockDisk *)(buffer+BLOCK_SIZE);
	superBlock.ninodes = superBlockDisk->ninodes;
	superBlock.nzones = superBlockDisk->nzones;
	superBlock.imapBlocks = superBlockDisk->imapBlocks;
	superBlock.zmapBlocks = superBlockDisk->zmapBlocks;
	superBlock.firstDatazone = superBlockDisk->firstDatazone;
	superBlock.logZoneSize = superBlockDisk->logZoneSize;
	superBlock.maxSize = superBlockDisk->maxSize;
	superBlock.magic = superBlockDisk->magic;
}
