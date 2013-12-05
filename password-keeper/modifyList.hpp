#ifndef passwordkeeper_modifyList
#define passwordkeeper_modifyList

#include <string.h>
#include "defines.hpp"
#include <stdio.h>
#include <stdlib.h>
#include "crypt.hpp"
#include <vector>
#include <assert.h>
using namespace std;

struct entry{
	char place[MAXPLACELEN], user[MAXUSERLEN], pass[MAXPASSLEN];
	void enc() {
		//~ encrypt(place,place);
		//~ encrypt(user,user);
		encrypt(pass,pass);
	}
	void dec() {
		//~ decrypt(place,place);
		//~ decrypt(user,user);
		decrypt(pass,pass);
	}
};

extern vector <entry*> Entries;
extern char inputCommand[MAXINPUTLEN];

vector <int> searchEntry( char* place ) {
	vector <int> found;
	for( int i=0;i<(int)Entries.size();i++) {
		if( strcmp(Entries[i]->place , place) == 0 ) found.push_back(i);
	}
	return found;
}

//Search in entries for the exact same pair (place,user) of val. Return true if present, false otherwise.
bool searchCollisions( entry* val ) {
	for(int i=0;i<(int)Entries.size();i++) {
		if( strcmp(Entries[i]->place,val->place)==0 and strcmp(Entries[i]->user,val->user)==0 ){
			return true;
		}
	}
	return false;
}

void add( ) {
	entry* val=new entry();
	printf("Place: ");
	fgets2(val->place,MAXPLACELEN,stdin);
	printf("User: ");
	fgets2(val->user,MAXPLACELEN,stdin);
	printf("Password: ");
	fgets2(val->pass,MAXPASSLEN,stdin);
	
	vector <int> found=searchEntry(val->place);
	int cc=found.size();
	if ( cc == 0 ) { //If there isn't another entry with the same place
		Entries.push_back(val);
		printf("Entry has been registered.\n");
	}
	else{ //Else gives the opportunity to update an existing entry or to add a new
		printf("%s has already some entries:\n", val->place );
		for(int i=0;i<cc;i++){ //list of entries with the same place
			printf("\t%d- User: %s ; Pass: %s\n",i+1,Entries[found[i]]->user, Entries[found[i]]->pass);
		}
		printf("Do you want to add a new entry (write new) or to update one of the entry (write its number)? ");
		fgets2(inputCommand,MAXINPUTLEN,stdin);
		if( strcmp(inputCommand,"new") == 0) { //Add a new if it doesn't collide
			if( !searchCollisions(val) ) {
				Entries.push_back(val);
				printf("Entry has been registered.\n");
			}
			else {
				printf("The pair (place, user) = (%s, %s) is already present.\n", val->place, val->user);
			}
		}
		else { //Update if doesn't collide
			int nn=atoi(inputCommand);
			if ( 1<=nn and nn<=cc ) {
				if( strcmp( Entries[found[nn-1]]->user,val->user )==0 or !searchCollisions(val) ) {
					Entries[found[nn-1]]=val;
					printf("Entry has been updated.\n");
				}
				else {
					printf("The pair (place, user) = (%s, %s) is already present.\n", val->place, val->user);
				}
			}
			else{ //If the value given by the user is wrong
				printf("You entered an invalid value.\n");
			}
		}
	}
	
}

void remove() {
	char place[MAXPLACELEN];
	printf("Place: ");
	fgets2(place,MAXPLACELEN,stdin);
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
		fgets2(inputCommand,MAXINPUTLEN,stdin);
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

void get( ) {
	char place[MAXPLACELEN];
	printf("Place: ");
	fgets2(place,MAXPLACELEN,stdin);
	
	vector <int> found=searchEntry(place);
	int cc=found.size();
	if( cc==0 ) printf("The place %s isn't in the password list.\n", place);
	else {
		for(int i=0;i<cc;i++) {
			printf("\t%d- User: %s ; Pass : %s\n",i+1,Entries[found[i]]->user,Entries[found[i]]->pass);
		}
	}
}

void getAll( ) {
	for(int i=0;i<(int)Entries.size();i++) {
		printf("Place: %s ; User: %s ; Pass : %s\n",
			Entries[i]->place, Entries[i]->user, Entries[i]->pass);
	}
}

#endif