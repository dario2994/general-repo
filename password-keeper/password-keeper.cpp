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

char execCommand[MAXEXECLEN], inputCommand[MAXINPUTLEN];
char privateKey[MAXPRIVATEKEYLEN];

vector <entry*> Entries;

char home[MAXPATHLEN], mainFolder[MAXPATHLEN], privateKeyHash[MAXPATHLEN], PasswordList[MAXPATHLEN];
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

void savePrivateKeyHash() {
	FILE* passOut= fopen(privateKeyHash, "w");
	assert(passOut!=NULL);
	char hashed[MAXHASHLEN];
	hashPassword( privateKey , hashed );
	fprintf( passOut, "%s\n", hashed );
	fclose(passOut);
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

bool decisionForm(const char* question){
	while ( 1 ) {
		printf("%s (yes or no) ", question);
		fgets2(inputCommand,MAXINPUTLEN,stdin);
		if(strcmp(inputCommand,"yes")==0) return true;
		else if(strcmp(inputCommand,"no")==0) return false;
		else {
			printf("You have to answer yes or no.\n");
		}
	} 
}

void importPasswords() {
	bool sepBool=decisionForm("Is the separator between place,user,password used (otherwise space will be used)?");
	char separator[20];
	if(sepBool) strcpy(separator,SEPARATOR);
	else strcpy(separator," ");
	int sepLen=strlen(separator);
	
	printf("Write the absolute file path from which import: ");
	fgets2(inputCommand,MAXINPUTLEN,stdin);
	FILE* source=fopen(inputCommand,"r");
	
	int nn, collNum=0, errLine=0;
	fscanf(source,"%d\n",&nn);
	for( int i=0;i<nn;i++){
		entry* newOne=new entry();
		fgets2(inputCommand,MAXINPUTLEN,source);
		bool goodLine=true;
		char* pointer1=strstr(inputCommand,separator);
		if( pointer1 == NULL ) goodLine=false;
		else{
			*pointer1='\0';
			strcpy(newOne->place,inputCommand);
			pointer1+=sepLen;
			char* pointer2=strstr(pointer1,separator);
			if( pointer2 == NULL) goodLine=false;
			else {
				*pointer2='\0';
				strcpy(newOne->user,pointer1);
				pointer2+=sepLen;
				char* pointer3=strstr(pointer2,separator);
				if(pointer3 != NULL) goodLine=false;
				else {
					strcpy(newOne->pass,pointer2);
				}
			}
		}
		
		if( goodLine ) {
			if( !searchCollisions(newOne) ) Entries.push_back(newOne);
			else collNum++;
		}
		else errLine++;
	}
	
	save();
	
	printf("\nAll passwords have been imported (%d collisions).\n", collNum);
}

void exportPasswords() {
	bool sepBool=decisionForm("Do you want a separator between place,user,password (suggested if some spaces are used)?");
	char separator[20];
	if(sepBool) strcpy(separator,SEPARATOR);
	else strcpy(separator," ");
	bool fileBool=decisionForm("Do you want to export in a file?");
	
	FILE* dest;
	
	if(fileBool) {
		printf("Write the absolute file path: ");
		fgets2(inputCommand,MAXINPUTLEN,stdin);
		dest=fopen(inputCommand,"w");
	}
	else dest=stdout;
	
	printf("\n");
	printf("%d\n",(int)Entries.size());
	for( int i=0;i<(int)Entries.size();i++){
		entry* x=Entries[i];
		fprintf(dest,"%s%s%s%s%s\n",x->place,separator,x->user,separator,x->pass);
	}
	if(fileBool) printf("\nAll passwords have been saved in %s.\n", inputCommand);
	else printf("\nAll passwords have been printed.\n");
}

void destroy () { //Deletes the folder if present
	strcpy(execCommand,"rm -fr ");
	strcat(execCommand,mainFolder);
	system(execCommand);	
}

void changePrivateKey() {
	fgets2(privateKey,MAXPRIVATEKEYLEN,stdin);
	savePrivateKeyHash();
	save();
}

void init () {
	
	destroy();
	
	printf("This is the first time you execute password-keeper.\n\n");
	
	//TODO AGGIUNGERE ANCHE CONTROLLI SULLA PASSWORD
	printf("Insert a private key to encrypt your data: ");
	fgets2(privateKey,MAXPRIVATEKEYLEN,stdin);
	
	//Creates the new folder
	strcpy(execCommand, "mkdir ");
	strcat(execCommand,mainFolder);
	system(execCommand);
	
	savePrivateKeyHash();
	
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
	
	readAllPass();
	
	while(1) {
		printf("> ");
		
		fgets2(inputCommand, 1000, stdin);
		
		if( strcmp( inputCommand , "add" ) == 0 ) add();
		
		else if( strcmp( inputCommand , "remove" ) == 0 ) remove();
		
		else if( strcmp( inputCommand , "get" ) == 0 ) get();
		
		else if( strcmp( inputCommand , "getAll" ) == 0 ) getAll();
		
		else if( strcmp( inputCommand , "quit" ) == 0 ) {
			save();
			return 0;
		}
		
		else if( strcmp( inputCommand , "import" ) ==0 ) importPasswords(); 
		
		else if( strcmp( inputCommand , "export" ) ==0 ) exportPasswords();
		
		else if( strcmp( inputCommand , "changeKey" ) == 0 ) changePrivateKey();
		
		else if( strcmp( inputCommand , "destroy" ) == 0 ) {
			bool desBool=decisionForm("Are you sure you want to delete all your saved password and configuration?");
			if( desBool ) {
				destroy();
				printf("Everything has been deleted.");
				return 0;
			}
		}
		
		else if( strcmp( inputCommand , "help" ) == 0 ) {
			printf("This are all the possible commands:\n");
			printf("\tadd\t: add an entry to the password list\n");
			printf("\tremove\t: remove an entry from the password list\n");
			printf("\tget\t: get an entry from the password list\n");
			printf("\tgetAll\t: get all entries from the password list\n");
			printf("\tquit\t: quit the program\n");
			printf("\texport\t: export the list of passwords\n");
			printf("\timport\t: import the list of passwords\n");
			printf("\tchangeKey\t: change the private key used for the encryption\n");
			printf("\tdestroy\t: destroy the password list and any configuration\n");
			printf("\thelp\t: gives help about the usage of password-keeper\n");
		}
		
		else {
			printf("Command doesn't exist, for help about the usage type help.\n");
		}
	}
}
