#define BOOT_SECTOR_SIZE 512
#define PARTITION_COUNT 4

#define BLOCK_SIZE	1024

#define NAME_LEN 14

typedef struct Partition {

	unsigned char bootInd;

	unsigned char head;

	unsigned char sector;

	unsigned char cyl;

	unsigned char sysInd;

	unsigned char endHead;

	unsigned char endSector;

	unsigned char endCyl;

	unsigned int startSect;

	unsigned int sectCount;

} Partition;

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

typedef struct IndexNode {

	unsigned short mode;

	unsigned short uid;

	unsigned long size;

	unsigned long time;

	unsigned char gid;

	unsigned char numLinks;

	unsigned short zone[9];

} IndexNode;

typedef struct DirEntry {

	unsigned short inode;

	char name[NAME_LEN];

} DirEntry;

