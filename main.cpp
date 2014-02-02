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
	DWORD fileSize;	// includes its own sub-header in this size
	DWORD type;		// sometimes 0xFFFFFFFF but subtype still set.
					// mp3 0x53, tex 0x1C, step chart 0x31
	DWORD subtype;	// not always same as above
}UNITY_DATA_TOC;	

typedef struct{
	DWORD filenameSize;
	char *filename;		// null padded to nearest DWORD	
	DWORD unk1[4];		// never change? 0x02000000,0x0D000000x,0x00010000,0x01000000
	DWORD fileSize;

}UNITY_MP3_HEADER;

typedef struct{
	DWORD filenameSize;
	char filename;	// null padded to nearest DWORD


}UNITY_IMG_HEADER;

#define UNITY_MP3 0x53
#define CYTUS_NOTE 0x31
void SUP_FileExists(char *fn);

void doIt(char *fn){
	
	BYTE *buf = NULL;
	UNITY_DATA_HEADER unityHdr;
	UNITY_DATA_TOC *unityToc;
	UNITY_MP3_HEADER mp3Hdr;
	FILE *out = NULL;
	FILE *in = fopen(fn, "rb");

	fread(&unityHdr, sizeof(UNITY_DATA_HEADER), 1, in);
	unityToc = (UNITY_DATA_TOC*)calloc(1,sizeof(UNITY_DATA_TOC)*unityHdr.numFiles);
	for(int i =0;i<unityHdr.numFiles;i++){
		fread(&unityToc[i], sizeof(UNITY_DATA_TOC), 1, in);
	}
	fseek(in, 0x1000, SEEK_SET);
	for(int i =0;i<unityHdr.numFiles;i++){
		if(unityToc[i].type==UNITY_MP3){
			fread(&mp3Hdr.filenameSize, 4, 1, in);
			mp3Hdr.filename = (char*)calloc(1,mp3Hdr.filenameSize+5);
			fread(mp3Hdr.filename, mp3Hdr.filenameSize, 1, in);
			strcat(mp3Hdr.filename, ".mp3");
			SUP_FileExists(mp3Hdr.filename);
			if(ftell(in)%4!=0)
				fseek(in, 4-(ftell(in)%4), SEEK_CUR);
			fread(mp3Hdr.unk1, 4*4, 1, in);
			fread(&mp3Hdr.fileSize, 4, 1, in);
			buf = (BYTE*)malloc(mp3Hdr.fileSize);
			out = fopen(mp3Hdr.filename, "wb");
			fread(buf, mp3Hdr.fileSize, 1, in);
			fwrite(buf, mp3Hdr.fileSize, 1, out);			
			fclose(out);
			fclose(in);
			free(buf);
			free(mp3Hdr.filename);
		}
		if(unityToc[i].type==CYTUS_NOTE){
			fread(&mp3Hdr.filenameSize, 4, 1, in);
			mp3Hdr.filename = (char*)calloc(1,mp3Hdr.filenameSize+5);
			fread(mp3Hdr.filename, mp3Hdr.filenameSize, 1, in);
			strcat(mp3Hdr.filename, ".not");
			SUP_FileExists(mp3Hdr.filename);
			if(ftell(in)%4!=0)
				fseek(in, 4-(ftell(in)%4), SEEK_CUR);
			//fread(mp3Hdr.unk1, 4*4, 1, in);
			fread(&mp3Hdr.fileSize, 4, 1, in);
			buf = (BYTE*)malloc(mp3Hdr.fileSize);
			out = fopen(mp3Hdr.filename, "wb");
			fread(buf, mp3Hdr.fileSize, 1, in);
			fwrite(buf, mp3Hdr.fileSize, 1, out);			
			fclose(out);
			fclose(in);
			free(buf);
			free(mp3Hdr.filename);
		}
	}

	free(unityToc);
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
	doIt(argv[1]);
	//doIt("025a67ca0b4cbd544a7f12bcae8a5365.dat");

	return;
}