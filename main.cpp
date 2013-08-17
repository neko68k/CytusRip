#include <Windows.h>
#include <stdio.h>

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
	char magic[3];
	int filesize = 0;
	int base = 0;
	char title[MAX_PATH];
	char c;
	char outfn[MAX_PATH];

	memset(title, 0, MAX_PATH);
	memset(tbuf, 0, MAX_PATH);
	in = fopen(argv[1], "rb");
	//in = fopen("a8ee1ea5054ab67499f383cba54c090c", "rb");
	
	fseek(in, 0x1004, SEEK_SET);
	if(ftell(in)!=0x1004)
		return;
	
	fread(tbuf, 1, 24, in);
	fseek(in, 0x1004, SEEK_SET);
	SUP_CopyString(tbuf, title);
	
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