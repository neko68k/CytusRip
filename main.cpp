#include <Windows.h>
#include <stdio.h>

// need to look more into imports section 0x44 in 5af0a116b391d7648a7706755254209c
// all hash codes are 0x20 long

typedef struct{
	DWORD unk1;
	DWORD filesize;
	DWORD unk2;			// version? always 9?
	DWORD ofs;			// offset to TOC data
	DWORD unk3;			// null pad?
	char verString[14];	// newline terminated version string with null padding	
	DWORD null1;
	DWORD null2;
	DWORD numFiles;
}UNITY_DATA_HEADER;

typedef struct{
	
	DWORD fileNum;	
	DWORD unk5;			
	DWORD fileSize;	// includes its own sub-header in this size
	DWORD type;		// sometimes 0xFFFFFFFF but subtype still set.
					// mp3 0x53, tex 0x1C, step chart 0x31
	DWORD subtype;	// not always same as above
}UNITY_DATA_TOC;	

typedef struct{
	DWORD filenameSize;
	char *filename;		// null padded to nearest DWORD
	BYTE unk2;			// null pad?
	DWORD unk1[5];		// never change? 0x02000000,0x0D000000x,0x00010000,0x01000000

}UNITY_MP3_HEADER;

typedef struct{
	DWORD filenameSize;
	char filename;	// null padded to nearest DWORD


}UNITY_IMG_HEADER;

void SUP_CopyString(BYTE *inPtr, char *outStr)
{
	char temp[1];
	memcpy(temp, inPtr, 1);
	char *cpyStr = outStr;
	while(strcmp(temp, "\0") != 0)
	{
		memcpy(cpyStr, temp, 1);
		cpyStr++;
		inPtr++;
		memcpy(temp, inPtr, 1);
	}
	memcpy(cpyStr, temp, 1);
}

void SUP_FileExists(char *fn){
	static int count = 0;
	count++;
	FILE *test = fopen(fn, "r");
	char fixed[MAX_PATH];
	char *ext = NULL;
	memset(fixed, 0, MAX_PATH);
	if(test==NULL)
		return;

	fclose(test);
	ext = strrchr(fn, '.');
	memset(ext, 0, 1);
	
	sprintf(fixed, "%s_%i.mp3",fn,count);
	SUP_FileExists(fixed);
	memset(fn, 0, MAX_PATH);
	strcpy(fn, fixed);
	count=0;
	return;
}

void main(int argc, char *argv[]){
	FILE *in = NULL;
	FILE *out = NULL;
	BYTE *buf = NULL;
	BYTE tbuf[MAX_PATH];
	DWORD namelen = 0;
	char magic[3];
	int filesize = 0;
	int base = 0;
	char title[MAX_PATH];
	char c;
	char outfn[MAX_PATH];

	memset(title, 0, MAX_PATH);
	memset(tbuf, 0, MAX_PATH);
	in = fopen(argv[1], "rb");
	
	fseek(in, 0x1000, SEEK_SET);
	if(ftell(in)!=0x1000)
		return;

	fread(&namelen, 4, 1, in);
	
	fread(&title, namelen, 1, in);
	strcpy(outfn, title);
	strcat(outfn, ".mp3");
	SUP_FileExists(outfn);


	fseek(in, strlen(title)+0x15, SEEK_CUR);
	base = ftell(in);
	fseek(in, 0, SEEK_END);
	filesize = ftell(in)-base;
	fseek(in, base, SEEK_SET);
	buf = (BYTE*)calloc(filesize, 1);

	fread(buf, 1, filesize, in);

	out = fopen(outfn, "wb");
	fwrite(buf, 1, filesize, out);
	fclose(out);
	fclose(in);
	free(buf);

	
	return;
}