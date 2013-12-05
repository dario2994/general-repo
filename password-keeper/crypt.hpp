#include <string.h>
#include "defines.h"
using namespace std;

//MAI A CAPI

void hashPassword ( char* pass , char* dest) { //spara fuori sempre SOLO lettere o numeri
	int l=strlen(pass);
	unsigned long long int hashResult=0, p=1;
	for(int i=0;i<l;i++) hashResult+=(unsigned long long int)(pass[i])*p, p*=29;
	
	hashResult+=1234;
	hashResult*=hashResult+15;
	
	unsigned long long int hash2=hashResult%54321;
	
	while ( hashResult > 0 ) hash2 += hashResult%10ull, hashResult/=10ull;
	
	
	int j=0;
	while( hash2 > 0 ) dest[j]='a'+char(hash2%26ull), j++, hash2/=26ull;
	dest[j]='\0';
}

void encrypt ( char* text , char* dest) { //spara fuori sempre SOLO lettere o numeri
	char S[MAXTEXTLEN];
	for(int i=0;i<(int)strlen(text);i++){
		if( text[i]!=' ' )S[i]=text[i];
		else S[i]='&';
	}
	S[strlen(text)]='\0';
	strcpy(dest,S);
}

void decrypt ( char* text , char* dest ) { //prende sempre SOLO lettere o numeri
	char S[MAXTEXTLEN];
	for(int i=0;i<(int)strlen(text);i++){
		if( text[i]!='&' )S[i]=text[i];
		else S[i]=' ';
	}
	S[strlen(text)]='\0';
	strcpy(dest,S);
}
