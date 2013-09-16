#include "note.h"
#include <Windows.h>
#include <stdio.h>

#define STRINGLEN 64
#define PAGEHEIGHT 720
#define PAGEWIDTH  1280
static const char TAG_VERSION[8] = "VERSION";
static const char TAG_NOTE[5] = "NOTE";
static const char TAG_LINK[5] = "LINK";
static const char TAG_BPM[4] = "BPM";
static const char TAG_PAGE_SHIFT[11] = "PAGE_SHIFT";
static const char TAG_PAGE_SIZE[10] = "PAGE_SIZE";

BYTE version;
float BPM;
float PAGE_SHIFT;
float PAGE_SIZE;
float songLen;		// miliseconds
DWORD numPages;

typedef struct{
	DWORD num;
	float vert;
	float horiz;
	float holdDuration;
	float x;
	float worldY;
	float pageY;
}CYTUS_NOTE;

void NOTE_Open(char *fn){
	char string[STRINGLEN];
	FILE *in = fopen(fn, "rb");
	while(!feof(in)){
		fgets(string, 64, in);
		char *pch;
		pch = strtok(string, " ");
		while(pch!= NULL){

			pch=strtok(NULL, " ");
		}
	}
	//numPages = songLen-PAGE_SHIFT/PAGE_SIZE;
	//totalHeight = PAGE_SHIFT/PAGE_SIZE*PAGEHEIGHT;
	//x=PAGEWIDTH*horiz;
	//pageY=vert\songLen*PAGEHEIGHT; wrong
	//pageNum=vert/PAGE_SIZE; wrong
}

