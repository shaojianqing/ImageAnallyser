
#define BOOT_SECTOR_SIZE 512
#define PARTITION_COUNT 4

#define BLOCK_SIZE	1024

#define NAME_LEN 14

typedef struct SuperBlockDisk {

	unsigned short ninodes;

	unsigned short nzones;

	unsigned short imapBlocks;

	unsigned short zmapBlocks;

	unsigned short firstDatazone;

	unsigned short logZoneSize;

	unsigned long maxSize;

	unsigned short magic;

} SuperBlockDisk;

typedef struct CacheBlock {
	
	byte data[1024];	

} CacheBlock;

typedef struct SuperBlock {

	unsigned short ninodes;

	unsigned short nzones;

	unsigned short imapBlocks;

	unsigned short zmapBlocks;

	unsigned short firstDatazone;

	unsigned short logZoneSize;

	unsigned long maxSize;

	unsigned short magic;

	CacheBlock *nodeMap[9];
	
	CacheBlock *zoneMap[9];	
	
} SuperBlock;

void loadFileSystem(char *imageName);
