/**
*					PKZip Stored File Unarch In C
*					Works on MacOSX / Windows x86,x64.
*	Author		:	aaaddress1@gmail.com 
*	Date		:	2015/10/27
*	Refferences	:	http://www.fileformat.info/format/zip/corion.htm
**/
#include <iostream>
#include <stdio.h>
#include <string>

bool outputFile(char *fileName,char fileData[], int size)
{
	if (FILE *filePtr = fopen(fileName, "w")) 
	{
		fwrite(fileData, size, 1, filePtr);
		fclose(filePtr);
		return true;
	}
	printf("Create File \"%s\" Fail.\n" , fileName);
	return false;
}
int main(int argc, char ** argv)
{
	if (!(--argc))
	{
		printf("Please input something.\n");
		return 0;
	}
	if (FILE *filePtr = fopen(argv[1] , "r"))
	{
		printf("Read File OK.\n");
		fseek(filePtr, 0, SEEK_END);
		int fileSize = ftell(filePtr);
		fseek(filePtr, 0, SEEK_SET);
		
		char *data = (char*)malloc(sizeof(char) * fileSize);
		fread(data, fileSize, sizeof(data), filePtr);

		unsigned int HeaderOffset = 0;
		while (*(unsigned int*)(data+HeaderOffset) == 0x04034B50) //ID == PK\x03\x04
		{
			printf("Entry Header Offset :%d OK.\n", HeaderOffset);
			if (!*(short*)(data+HeaderOffset+8)) 
			{
				printf(" - Current PkZip File is No compression(Stored).\n");
				int fileCount = *(int*)(data+HeaderOffset+0x12);//File Len(org)
				short fileNameLen = *(short*)(data+HeaderOffset+0x1A);
				char * fileNameBuff = (char*)malloc(sizeof(char) * fileNameLen);
				memcpy(fileNameBuff, (data+HeaderOffset+0x1E) ,fileNameLen);
				printf(" - Current File Name: %s\n", fileNameBuff);

				char * dataStartPtr = data + HeaderOffset + 0x1E + fileNameLen;
				outputFile(fileNameBuff,dataStartPtr,fileCount);
				HeaderOffset +=  0x1E + fileCount + fileNameLen;
				printf("\n");			
			}
			else printf(" - Current PkZip File is compressed!(Method:%d)\n",*(data+8)); 
		}
		fclose(filePtr);
	}
	else printf("Read File Fail.\n");
	return 0;
}