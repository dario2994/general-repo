#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include "crypt.hpp"
using namespace std;

#define MasterEncryption	".masterPasswordRSA.txt"
#define passwordList		".passwordList.txt"

class entry{
	string user, pass;
};

map <string, entry> ListOfEntries;

void add( string place , entry val) {
	
	if ( ListOfEntries.find( place ) != ListOfEntries.end() ) {
		cout <<  place << " has already a (user, password) entry.\n";
		return;
	}
	
	ListOfEntries[place] = val;
	
}

void remove( string place ) {
	ListOfEntries.erase( place );
}

entry retrieve( string place ) {
	return ListOfEntries[place]; 
}

void init ( string masterPassword ) {
	FILE* passOut= fopen(MasterEncryption, "r");
	fprintf( passOut, "%s\n", encryptPassword( masterPassword ) );
	passOut.close();
	ofstream listOut(passwordList);
	listOut << 0 << "\n";
	listOut.close();
	
	printf( "Password-keeper initialized\n" );
}

int login ( string masterPassword ) {
	ifstream in(MasterEncryption);
	string realHash;
	in >> realHash;
	in.close();
	if ( encryptPassword( masterPassword ) != realHash ) {
		printf("Password is incorrect, try again.\n");
		return 1;
	}
	
	printf("Login successful.\n");
	return 0;
}

void readAllPass( masterPassword ) {
	ifstream in(passwordList);
	int entryNumber;
	cin >> entryNumber;
	for(int i=0;i<entryNumber;i++) {
		assert( 3== fscanf(in, "%s %s %s\n", &place, &user, &password ) );
	}
}

int main(){
	string masterPassword;
	int triesCount=0;
	do {
		triesCount++;
		printf( "Password: " );
		//TODO: this should take the password in **** mode.
		scanf ( "%s", masterPassword ); //senza spazi? boh
		printf("\n");
	} while( login( masterPassword ) == 1 and triesCount < 3 );
	
	if( triesCount == 3 ) {
		printf("Too many tries, the file is just been destroyed.\n");
		return 0;
	}
	
	
	readAllPass( masterPassword );
	
}
