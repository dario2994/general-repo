#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include "defines.h"
#include "crypt.hpp"
//~ #include "variablesInitializer.hpp"
using namespace std;

char execCommand[1000], inputCommand[1000];
char masterPassword[MAXPASSLEN];

struct entry{
	char place[MAXPLACELEN], user[MAXUSERLEN], pass[MAXPASSLEN];
	void enc() {
		encrypt(place,place);
		encrypt(user,user);
		encrypt(pass,pass);
	}
	void dec() {
		decrypt(place,place);
		decrypt(user,user);
		decrypt(pass,pass);
	}
};

vector <entry*> Entries;

void fgets2(char* destination, int limitSize, FILE* source) {
	fgets(destination, limitSize, source);
	int l=strlen(destination);
	if(l>=1 and destination[l-1]=='\n') destination[l-1]='\0';
}

char home[100], mainFolder[100], MasterEncryption[100], PasswordList[100];
void initPath() {
	strcpy(home,getenv("HOME"));
	strcpy(mainFolder,home);
	strcat(mainFolder,"/.password-keeper");
	strcpy(MasterEncryption,mainFolder);
	strcat(MasterEncryption, "/.masterPasswordHash.txt");
	strcpy(PasswordList,mainFolder);
	strcat(PasswordList, "/.passwordList.txt");
	
	//~ printf("%s %s %s %s", home, mainFolder, MasterEncryption, PasswordList);
}

void save() {
	FILE* out=fopen(PasswordList,"w");
	fprintf(out,"%d\n",(int)Entries.size());
	for(int i=0;i<(int)Entries.size();i++) {
		entry* ee=new entry();
		*ee=*(Entries[i]);
		ee->enc();
		fprintf(out, "%s %s %s\n", ee->place, ee->user, ee->pass );
		//~ delete ee;
	}
	fclose(out);
}

vector <int> searchEntry( char* place ) {
	vector <int> found;
	for( int i=0;i<(int)Entries.size();i++) {
		if( strcmp(Entries[i]->place , place) == 0 ) found.push_back(i);
	}
	return found;
}

void add( entry* val) {
	vector <int> found=searchEntry(val->place);
	int cc=found.size();
	if ( cc == 0 ) {
		Entries.push_back(val);
		printf("Entry has been registered.\n");
	}
	else{
		printf("%s has already some entries:\n", val->place );
		for(int i=0;i<cc;i++){
			printf("\t%d- User: %s ; Pass: %s\n",i+1,Entries[found[i]]->user, Entries[found[i]]->pass);
		}
		printf("Do you want to add a new entry (write new) or to update one of the entry (write its number)? ");
		fgets2(inputCommand,10,stdin);
		if( strcmp(inputCommand,"new") == 0 ) {
			Entries.push_back(val);
			printf("Entry has been registered.\n");
		}
		else {
			int nn=atoi(inputCommand);
			if ( 1<=nn and nn<=cc ) {
				Entries[found[nn]]=val;
				printf("Entry has been updated.\n");
			}
			else{
				printf("You entered an invalid value.\n");
			}
		}
	}
	
}

void remove( char* place ) {
	vector <int> found=searchEntry(place);
	int cc=found.size();
	if( cc==0 ) printf("The place %s isn't in the password list.\n", place);
	else if( cc==1 ) {
		Entries.erase(Entries.begin()+found[0]);
		printf("Entry has been deleted.\n");
	}
	else {
		printf("The place %s is present in more than one entry:\n", place);
		for(int i=0;i<cc;i++){
			printf("\t%d- User: %s ; Pass: %s\n",i+1,Entries[found[i]]->user, Entries[found[i]]->pass);
		}
		printf("Which (* for all) should I delete? ");
		fgets2(inputCommand,10,stdin);
		if( strcmp(inputCommand,"*") == 0 ) {
			for(int i=cc-1; i>=0 ;i--) { //cancello dall'ultimo per non rompere i puntatori
				assert( strcmp( Entries[found[i]]->place , place) == 0 );
				Entries.erase(Entries.begin()+found[i]);
			}
			printf("All entries relative to %s have been deleted.\n",place);
		}
		else {
			int nn=atoi(inputCommand);
			if ( 1<=nn and nn<=cc ) {
				Entries.erase(Entries.begin()+found[nn-1]);
				printf("Entry has been deleted.\n");
			}
			else{
				printf("You entered an invalid value.\n");
			}
		}
	}
}

void retrieve( char* place ) {
	vector <int> found=searchEntry(place);
	int cc=found.size();
	if( cc==0 ) printf("The place %s isn't in the password list.\n", place);
	else {
		for(int i=0;i<cc;i++) {
			printf("\t%d- User: %s ; Pass : %s\n",i+1,Entries[found[i]]->user,Entries[found[i]]->pass);
		}
	}
}

void destroy () { //Deletes the folder if present
	strcpy(execCommand,"rm -fr ");
	strcat(execCommand,mainFolder);
	system(execCommand);
	
}

void init () {
	
	destroy();
	
	printf("This is the first time you execute password-keeper.\n\n");
	
	//TODO AGGIUNGERE ANCHE CONTROLLI SULLA PASSWORD
	printf("Insert a password to encrypt your data: ");
	fgets2(masterPassword,MASTERPASSLEN,stdin);
	
	//Creates the new folder
	strcpy(execCommand, "mkdir ");
	strcat(execCommand,mainFolder);
	system(execCommand);
	
	FILE* passOut= fopen(MasterEncryption, "w");
	assert(passOut!=NULL);
	char hashed[MAXHASHLEN];
	hashPassword( masterPassword , hashed );
	fprintf( passOut, "%s\n", hashed );
	fclose(passOut);
	
	FILE* listOut=fopen(PasswordList,"w");
	fprintf(listOut,"0\n");
	fclose(listOut);
	
	printf( "Password-keeper initialized\n" );
}

void readAllPass( ) {
	FILE* in=fopen(PasswordList,"r");
	int entryNumber;
	assert( 1 == fscanf(in, "%d\n", &entryNumber));
	for(int i=0;i<entryNumber;i++) {
		entry* newOne=new entry();
		
		assert( 3 == fscanf(in, "%s %s %s\n", newOne->place, newOne->user, newOne->pass ) );//separati da spazio
		//~ printf("%s\n",newOne->place);
		newOne->dec();
		Entries.push_back(newOne);
	}
	
	fclose(in);
}

int login () {
	FILE* in =fopen(MasterEncryption,"r");
	char realHash[MAXHASHLEN];
	fgets2(realHash,MAXHASHLEN,in); //NON HA SPAZI
	fclose(in);
	
	//~ printf("%s %s\n", realHash, encryptPassword( masterPassword ));
	char hashed[MAXHASHLEN];
	hashPassword( masterPassword, hashed );
	if ( strcmp( hashed , realHash ) != 0 ) {
		printf("Password is incorrect, try again.\n");
		return 1;
	}
	
	printf("Login successful.\n");
	return 0;
}

int main(){
	initPath();
	
	if( access(mainFolder , F_OK) != 0 ) init();
	else {
		int triesCount=0;
		do {
			if( triesCount == 3 ) {
				printf("Too many tries, the file has just been destroyed.\n");
				return 0;
			}
			triesCount++;
			printf( "Password: " );
			//TODO: this should take the password in **** mode.
			fgets2(masterPassword, MASTERPASSLEN, stdin);
			printf("\n");
		} while( login( ) == 1 );
	}
	
	//~ return;
	
	readAllPass();
	
	while(1) {
		printf("> ");
		
		fgets2(inputCommand, 1000, stdin);
		
		if( strcmp( inputCommand , "add" ) == 0 ) {
			entry* newOne=new entry();
			printf("Place: ");
			fgets2(newOne->place,MAXPLACELEN,stdin);
			printf("User: ");
			fgets2(newOne->user,MAXPLACELEN,stdin);
			printf("Password: ");
			fgets2(newOne->pass,MAXPASSLEN,stdin);
			
			add( newOne );
			
		}
		
		else if( strcmp( inputCommand , "remove" ) == 0 ) {
			char place[MAXPLACELEN];
			printf("Place: ");
			fgets2(place,MAXPLACELEN,stdin);
			remove( place );
		}
		
		else if( strcmp( inputCommand , "get" ) == 0 ) {
			char place[MAXPLACELEN];
			printf("Place: ");
			fgets2(place,MAXPLACELEN,stdin);
			retrieve( place );
		}
		
		else if( strcmp( inputCommand , "quit" ) == 0 ) {
			save();
			return 0;
		}
		
		else if( strcmp( inputCommand , "destroy" ) == 0 ) {
			while(1) {
				printf("Are you sure you want to delete all your saved password and configuration? (yes or no) ");
				fgets2(inputCommand, 1000, stdin);
				if( strcmp( inputCommand , "yes" ) == 0 ) {
					destroy();
					printf("Everything has been deleted.");
					return 0;
				} 
				else if( strcmp( inputCommand , "no" ) == 0 ) break;
				else {
					printf("You have to answer yes or no\n");
				}
			}
		}
		
		else if( strcmp( inputCommand , "help" ) == 0 ) {
			printf("This are all the possible commands:\n");
			printf("\tadd\t: add an entry to the password list\n");
			printf("\tremove\t: add an entry to the password list\n");
			printf("\tget\t: get an entry from the password list\n");
			printf("\tquit\t: quit the program\n");
			printf("\tdestroy\t: destroy the password list and any configuration\n");
			printf("\thelp\t: gives help about the usage of password-keeper\n");
		}
		
		else {
			printf("Command doesn't exist, for help about the usage type help.\n");
		}
	}
}
