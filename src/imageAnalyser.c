#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "type.h"
#include "imageAnalyser.h"

int length;

byte *buffer;

SuperBlockDisk *superBlock;

IndexNode *rootNodeData;

DirEntry *startDirEntry;

void prepareDataBuffer(char *filename);

bool match(char *name, int length, DirEntry *entry);

void printDirectoryInfo();

void resolvePartition();

void resolveSuperBlock();

void resolveDatazone();

void printFileData();

void printDataFormat(IndexNode* fileNode);

int getPathLength(char *filepath);

void setupBitmap(int index, byte *buffer);

void clearBitmap(int index, byte *buffer);

IndexNode* getFileNode(char *filepath, IndexNode *parent);

int main(int argc, char **argv) {

	if (argc==2 && argv!=NULL) {
		char *filename = argv[1];
		prepareDataBuffer(filename);
		printFileData();
	} else {
		printf("The Input Parameter is not Valid!!\n");
	}
}

void prepareDataBuffer(char *filename) {
	printf("The Image File is:%s\n", filename);
	FILE *file;
	if ((file = fopen(filename, "rw"))!=NULL) {
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		buffer = (byte *)malloc(length);
		rewind(file);
		fread(buffer, 1, length, file);
		fclose(file);
		buffer[length] = 0;
		printf("The Image Lenth is:%d\n", length);

		superBlock = (SuperBlockDisk *)(buffer+BLOCK_SIZE);
		rootNodeData = (IndexNode *)(buffer + superBlock->imapBlocks*BLOCK_SIZE + 
											   superBlock->zmapBlocks*BLOCK_SIZE + 
											   BLOCK_SIZE*2);

		startDirEntry = (DirEntry *)(buffer + superBlock->firstDatazone*BLOCK_SIZE);
	}
}

void printDirectoryInfo(DirEntry *entry, int space) {
	while(entry->inode>0) {

		int i=space;
		while(i>0) {
			printf("	");
			i--;		
		}
		IndexNode *inode = rootNodeData + (entry->inode - 1);
		printf("%s		{%x}\n", entry->name, inode->mode);

		if (inode->mode==0x41ED) {
			DirEntry *nextEntry = (DirEntry *)(buffer + (inode->zone[0])*BLOCK_SIZE);

			if (strcmp(entry->name, ".")!=0 && strcmp(entry->name, "..")!=0) {
				printDirectoryInfo(nextEntry, space+1);		
			}
		}		
		entry++;
	}
}

void printFileData() {
	
	char *pathname = "/bin/mkfs";
	
	if (*pathname == '/') {
		pathname++;
		IndexNode* fileNode = getFileNode(pathname, rootNodeData);
		if (fileNode!=NULL) {
			printf("The File Size is:%lu \n\n", fileNode->size);
			printDataFormat(fileNode);			
		} else {
			printf("Can not find the file:%s \n", pathname);	
		}
	} else {
		printf("The Input pathname is invalid^_^");
	}
}

void printDataFormat(IndexNode* fileNode) {
	if (fileNode!=NULL) {
		int i=0, h=0;
		while(fileNode->zone[i]!=0 && h<fileNode->size) {
			int k = 0;
			byte *dataBuffer = buffer + (fileNode->zone[i])*BLOCK_SIZE;
			for (k=0;k<BLOCK_SIZE && h<fileNode->size;++k) {
				if (k%16==0) {
					printf("\n %4X: ", k+i*BLOCK_SIZE);				
				}	
				byte num = dataBuffer[k];
				unsigned int a = num;
				printf("%02X ", a);
				h++;	
			}
			i++;
		}
	}	
}

IndexNode* getFileNode(char *filepath, IndexNode *parent) {
	if (parent!=NULL) {
		IndexNode *childNode = NULL;
		int length = getPathLength(filepath);
		byte *dataBuffer = (buffer + (parent->zone[0])*BLOCK_SIZE);
	
		printf("length:%d \n", length);
	
		if (length>0) {
			DirEntry *entry = (DirEntry *)dataBuffer;
			while(entry->inode!=0) {
				if (match(filepath, length, entry)) {
					childNode = rootNodeData + (entry->inode - 1);
					break;		
				} else {
					entry++;
				}
			}
			char c = *(filepath+length);
			if (c!='\0') {
				filepath+=length+1;		
			} else {
				filepath+=length;			
			}
			
			if (*filepath=='\0') {
				return childNode;		
			} else {
				return getFileNode(filepath, childNode);	
			}
		}
	}
	return NULL;
}

int getPathLength(char *filepath) {
	char *pathname = filepath;
	char c = *pathname;
	int length = 0;

	while(c!='/' && c!='\0') {
		pathname++;
		length++;

		c = *pathname;		
	}

	if (length==22) {
		printf("The 22 filepath:%s-----\n", filepath);		
	}

	return length;
}

bool match(char *name, int length, DirEntry *entry) {
	int i=0;
	while (i<length) {
		char c = entry->name[i];
		char e = name[i];
		if (c == e) {
			i++;		
		} else {
			return false;		
		}
	}
	if ( entry->name[length]=='\0') {
		return true;	
	} else {
		return false;	
	}
}

void resolveSuperBlock() {
	int count = 0, i = 0;
	
	printf("The Data Zone is:\n");
	for (i=0;i<length-1;++i) {
		byte num1 = buffer[i];
		byte num2 = buffer[i+1];
		
		if (num1==0x7F && num2==0x13) {
			printf("\n %4X: \n", i);
			printf("0x137F\n\n");
			break;
		}
	}

	SuperBlockDisk *superDisk = (SuperBlockDisk *)(buffer+0x400);
	printf("The SuperBlock		%4X\n", (unsigned int)(0x400));
	printf("	ninodes:		%d\n", superDisk->ninodes);
	printf("	nzones:			%d\n", superDisk->nzones);
	printf("	imapBlocks:		%d\n", superDisk->imapBlocks);
	printf("	zmapBlocks:		%d\n", superDisk->zmapBlocks);
	printf("	firstDatazone:	%d\n", superDisk->firstDatazone);
	printf("	logZoneSize:	%d\n", superDisk->logZoneSize);
	printf("	maxSize:		%ld\n", superDisk->maxSize);
	printf("	magic:			%2X\n", superDisk->magic);
	printf("\n\n");
}

void resolveDatazone() {
	int i=0;
	for (i=0;i<1024;++i) {
		if (i%16==0) {
			printf("\n %4X: ", i);				
		}	
		byte num = buffer[i+19*1024];
		unsigned int a = num;
		printf("%02X ", a);		
	}
	printf("\n\n");
}

