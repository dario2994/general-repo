#ifndef passwordkeeper_defines
#define passwordkeeper_defines

#define MAXPRIVATEKEYLEN	100
#define MAXHASHLEN			20
#define MAXPLACELEN			100
#define MAXUSERLEN			50
#define MAXPASSLEN			100
#define MAXTEXTLEN			1000

#include <stdio.h>
#include <string.h>

void fgets2(char* destination, int limitSize, FILE* source) {
	fgets(destination, limitSize, source);
	int l=strlen(destination);
	if( l>=1 and destination[l-1]=='\n' ) destination[l-1]='\0';
}

#endif