#include <string>
//~ #include <iostream>
#include "header.hpp"
using namespace std;

string removeSpaces ( string text ) {
	string res;
	for(int i=0;i<(int)text.size();i++) {
		if( text[i]!=' ' ) res+=text[i];
		else {
			res+=SPACESEP;
		}
	}
	return res;
}

string addSpaces ( string text ) {
	vector <string> pieces=splitString( text , SPACESEP );
	string res=pieces[0];
	for(int i=1;i<(int)pieces.size();i++) res+=" "+pieces[i];
	return res; 
}

void entry::enc( string key ){
	place=removeSpaces( place );
	user=removeSpaces( user );
	pass=encrypt( pass, key );
}

void entry::dec( string key ){
	place=addSpaces( place );
	user=addSpaces( user );
	pass=decrypt( pass , key );
}

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

string encrypt ( string text , string key ) { //spara fuori sempre SOLO lettere o numeri
	int l=key.size();
	string res;
	res.resize(2*text.size());
	
	for(int i=0;i<(int)text.size();i++){
		int c=text[i]^key[i%l];
		res[2*i]='0'+char(c/26);
		res[2*i+1]='a'+char(c%26);
	}
	return res;
}

string decrypt ( string text , string key ) { //prende sempre SOLO lettere o numeri
	string res;
	res.resize(text.size()/2);
	
	int l=key.size();
	for(int i=0;i<(int)text.size()/2;i++){
		int c=int(text[2*i]-'0')*26+int(text[2*i+1]-'a');
		res[i]=char(c)^key[i%l];
	}
	return res;
}

string md5( string S ) {
	vector <unsigned int> bits;
	for(int i=0;i<
}



