#ifndef passwordkeeper_crypt
#define passwordkeeper_crypt

//~ #include "defines.hpp"
#include <string>
using namespace std;

//MAI A CAPI

extern string privateKey;

string hashPassword ( string pass ) { //spara fuori sempre SOLO lettere o numeri
	int l=pass.size();
	unsigned long long int hashResult=0, p=1;
	for(int i=0;i<l;i++) hashResult+=(unsigned long long int)(pass[i])*p, p*=29;
	
	hashResult+=1234;
	hashResult*=hashResult+15;
	
	unsigned long long int hash2=hashResult%54321;
	
	while ( hashResult > 0 ) hash2 += hashResult%10ull, hashResult/=10ull;
	
	string res;
	int j=0;
	while( hash2 > 0 ) res+='a'+char(hash2%26ull), j++, hash2/=26ull;
	return res;
}

string encrypt ( string text ) { //spara fuori sempre SOLO lettere o numeri
	int l=privateKey.size();
	string res;
	res.resize(2*text.size());
	
	for(int i=0;i<(int)text.size();i++){
		int c=text[i]^privateKey[i%l];
		res[2*i]='0'+char(c/26);
		res[2*i+1]='a'+char(c%26);
	}
	return res;
}

string decrypt ( string text ) { //prende sempre SOLO lettere o numeri
	string res;
	res.resize(text.size()/2);
	
	int l=privateKey.size();
	for(int i=0;i<(int)text.size();i++){
		int c=int(text[2*i]-'0')*26+int(text[2*i+1]-'a');
		res[i]=char(c)^privateKey[i%l];
	}
	return res;
}

#endif
