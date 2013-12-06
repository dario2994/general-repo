//TODO: Implementare i backup criptati.
//TODO: Togliere gli spazi a place e user

#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unistd.h>
#include "defines.hpp"
#include "crypt.hpp"
#include "modifyList.hpp"
using namespace std;

string privateKey;

vector <entry*> Entries;

extern string home, mainFolder, privateKeyHash, PasswordList;

void savePrivateKeyHash() {
	ofstream passOut(privateKeyHash.c_str());
	assert( passOut!=NULL );
	passOut << hashPassword( privateKey );
	passOut.close();
}

void save() {
	ofstream out(PasswordList.c_str());
	out << Entries.size() << "\n";
	for(int i=0;i<(int)Entries.size();i++) {
		entry* ee=new entry();
		*ee=*(Entries[i]);
		ee->enc();
		out << ee->place << " " << ee->user << " " << ee->pass << "\n";
	}
	out.close();
}

void importPasswords() {
	bool sepBool=decisionForm("Is the separator between place,user,password used (otherwise space will be used)?");
	string separator;
	if(sepBool) separator=SEPARATOR;
	else separator=" ";

	string sourcePath;
	while( 1 ) {
		sourcePath=questionForm("Write the absolute file path from which to import: ");
		if( access( sourcePath.c_str() , R_OK ) == 0 ) break;
		cout << "The path entered doesn't correspond to a readable file.\n";
	}
	
	ifstream source( sourcePath.c_str() );
	
	int nn, collNum=0, errLine=0;
	
	source >> nn;
	string stupidNewLine;//To avoid errors with getline
	getline(source,stupidNewLine);
	
	for( int i=0;i<nn;i++){
		entry* newOne=new entry();
		string line;
		getline(source,line);
		vector <string> pieces=splitString(line,separator);
		if( pieces.size() != 3 ) errLine++;
		else {
			newOne->place=pieces[0];
			newOne->user=pieces[1];
			newOne->pass=pieces[2];
			if( !searchCollisions(newOne) ) Entries.push_back(newOne);
			else collNum++;
		}
	}
	
	save();
	cout << "\nAll passwords have been imported (" << collNum << " collisions and " << errLine << " errors)\n";
}

void exportPasswords() {
	bool sepBool=decisionForm("Do you want a separator between place,user,password (suggested if some spaces are used)?");
	string separator;
	if(sepBool) separator=SEPARATOR;
	else separator=" ";
	bool fileBool=decisionForm("Do you want to export in a file?");
	
	if(fileBool) {
		ofstream dest( questionForm("Write the absolute path:").c_str() );
		dest << Entries.size() << "\n";
		for( int i=0;i<(int)Entries.size();i++){
			entry* x=Entries[i];
			dest << x->place << separator << x->user << separator << x->pass << "\n";
		}
		dest.close();
		cout << "\nAll passwords have been saved in the specified file.\n";
	}
	
	else {
		cout << "\n";
		cout << Entries.size() << "\n";
		for( int i=0;i<(int)Entries.size();i++){
			entry* x=Entries[i];
			cout << x->place << separator << x->user << separator << x->pass << "\n";
		}
		cout << "\nAll passwords have been printed.\n";
	}
}

void destroy () { //Deletes the folder if present
	system( ("rm -fr "+mainFolder).c_str() );
}

void changePrivateKey() {
	privateKey = questionForm("Insert the private key (used for encryption):");
	savePrivateKeyHash();
	save();
}

void init () {
	
	destroy();
	
	cout << "This is the first time you execute password-keeper.\n\n";
	
	system(("mkdir "+mainFolder).c_str());
	
	changePrivateKey();
	
	ofstream listOut(PasswordList.c_str());
	listOut << "0\n";
	listOut.close();
	
	cout <<  "Password-keeper initialized\n" ;
}

void readAllPass( ) {
	ifstream in(PasswordList.c_str());
	int entryNumber;
	in >> entryNumber;
	for(int i=0;i<entryNumber;i++) {
		entry* newOne=new entry();
		in >> newOne->place >> newOne->user >> newOne->pass;
		newOne->dec();
		Entries.push_back(newOne);
	}
	in.close();
}

bool login () {
	privateKey=questionForm("Password:");
	
	ifstream in(privateKeyHash.c_str());
	string correctHash;
	in >> correctHash;
	in.close();
	string hashedKey = hashPassword( privateKey );
	if ( hashedKey != correctHash ) {
		cout << "Password is incorrect, try again.\n";
		return 0;
	}
	
	cout << "Login successful.\n";
	return 1;
}

int main(){
	
	initPath();
	
	if( access(mainFolder.c_str() , F_OK) != 0 ) init();
	else {
		while( !login() );
	}
	
	readAllPass();
	
	while(1) {
		cout << "> ";

		string command;
		getline(cin,command);
		
		if( command == "add" ) add();
		
		else if( command == "remove" ) remove();
		
		else if( command == "get" ) retrieve();
		
		else if( command == "getAll" ) retrieveAll();
		
		else if( command == "quit" ) {
			save();
			return 0;
		}
		
		else if( command == "import" ) importPasswords(); 
		
		else if( command == "export" ) exportPasswords();
		
		else if( command == "changeKey" ) changePrivateKey();
		
		else if( command == "destroy" ) {
			bool desBool=decisionForm("Are you sure you want to delete all your saved password and configuration?");
			if( desBool ) {
				destroy();
				cout << "Everything has been deleted.";
				return 0;
			}
		}
		
		else if( command == "help" ) {
			cout << "This are all the possible commands:\n";
			cout << "\tadd\t: add an entry to the password list\n";
			cout << "\tremove\t: remove an entry from the password list\n";
			cout << "\tget\t: get an entry from the password list\n";
			cout << "\tgetAll\t: get all entries from the password list\n";
			cout << "\tquit\t: quit the program\n";
			cout << "\texport\t: export the list of passwords\n";
			cout << "\timport\t: import the list of passwords\n";
			cout << "\tchangeKey\t: change the private key used for the encryption\n";
			cout << "\tdestroy\t: destroy the password list and any configuration\n";
			cout << "\thelp\t: gives help about the usage of password-keeper\n";
		}
		
		else {
			cout << "Command doesn't exist, for help about the usage type help.\n";
		}
	}
}
