//TODO: Implementare l'import da file
//TODO: Implementare l'export su file
//TODO: Implementare i backup criptati.

#include <cassert>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include "defines.hpp"
#include "crypt.hpp"
#include "modifyList.hpp"
using namespace std;

char execCommand[1000], inputCommand[1000];
char privateKey[MAXPRIVATEKEYLEN];

vector <entry*> Entries;

char home[100], mainFolder[100], privateKeyHash[100], PasswordList[100];
void initPath() {
	strcpy(home,getenv("HOME"));
	strcpy(mainFolder,home);
	strcat(mainFolder,"/.password-keeper");
	strcpy(privateKeyHash,mainFolder);
	strcat(privateKeyHash, "/.privateKeyHash.txt");
	strcpy(PasswordList,mainFolder);
	strcat(PasswordList, "/.passwordList.txt");
	
	//~ printf("%s %s %s %s", home, mainFolder, privateKeyHash, PasswordList);
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
	fgets2(privateKey,MAXPRIVATEKEYLEN,stdin);
	
	//Creates the new folder
	strcpy(execCommand, "mkdir ");
	strcat(execCommand,mainFolder);
	system(execCommand);
	
	FILE* passOut= fopen(privateKeyHash, "w");
	assert(passOut!=NULL);
	char hashed[MAXHASHLEN];
	hashPassword( privateKey , hashed );
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
	FILE* in =fopen(privateKeyHash,"r");
	char realHash[MAXHASHLEN];
	fgets2(realHash,MAXHASHLEN,in); //NON HA SPAZI
	fclose(in);
	
	//~ printf("%s %s\n", realHash, encryptPassword( privateKey ));
	char hashed[MAXHASHLEN];
	hashPassword( privateKey, hashed );
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
			fgets2(privateKey, MAXPRIVATEKEYLEN, stdin);
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
		
		else if( strcmp( inputCommand , "getAll" ) == 0 ) {
			retrieveAll();
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
			printf("\tremove\t: remove an entry from the password list\n");
			printf("\tget\t: get an entry from the password list\n");
			printf("\tgetAll\t: get all entries from the password list\n");
			printf("\tquit\t: quit the program\n");
			printf("\tdestroy\t: destroy the password list and any configuration\n");
			printf("\thelp\t: gives help about the usage of password-keeper\n");
		}
		
		else {
			printf("Command doesn't exist, for help about the usage type help.\n");
		}
	}
}
