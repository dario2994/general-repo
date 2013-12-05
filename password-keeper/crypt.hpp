#ifndef passwordkeeper_crypt
#define passwordkeeper_crypt

#include <string.h>
#include "defines.hpp"
using namespace std;

//MAI A CAPI

extern char privateKey[MAXPRIVATEKEYLEN];

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
	int l=strlen(privateKey);
	for(int i=0;i<(int)strlen(text);i++){
		//~ if( text[i]!=' ' )S[i]=text[i];
		//~ else S[i]='&';
		int c=text[i]^privateKey[i%l];
		S[2*i]='0'+char(c/26);
		S[2*i+1]='a'+char(c%26);
	}
	S[2*strlen(text)]='\0';
	strcpy(dest,S);
}

void decrypt ( char* text , char* dest ) { //prende sempre SOLO lettere o numeri
	char S[MAXTEXTLEN];
	int l=strlen(privateKey);
	for(int i=0;i<(int)strlen(text);i++){
		//~ if( text[i]!='&' )S[i]=text[i];
		//~ else S[i]=' ';
		int c=int(text[2*i]-'0')*26+int(text[2*i+1]-'a');
		S[i]=char(c)^privateKey[i%l];
	}
	S[strlen(text)/2]='\0';
	strcpy(dest,S);
}

#endif