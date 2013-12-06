#ifndef passwordkeeper_modifyList
#define passwordkeeper_modifyList


#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
//~ #include "defines.hpp"
//~ #include "crypt.hpp"
using namespace std;


void entry::enc(){
	pass=encrypt(pass);
}

void entry::dec(){
	pass=decrypt(pass);
}

??extern vector <entry*> Entries; 

vector <int> searchEntry( string place ) {
	vector <int> found;
	for( int i=0;i<(int)Entries.size();i++) if( Entries[i]->place == place ) found.push_back(i);
	return found;
}

//Search in entries for the exact same pair (place,user) of val. Return true if present, false otherwise.
bool searchCollisions( entry* val ) {
	for(int i=0;i<(int)Entries.size();i++) {
		if( Entries[i]->place==val->place and Entries[i]->user==val->user ){
			return true;
		}
	}
	return false;
}

void add( ) {
	entry* val=new entry();
	cout << "Place: ";
	getline(cin,val->place);
	cout << "User: ";
	getline(cin,val->user);
	cout << "Password: ";
	getline(cin,val->pass);
	
	vector <int> found=searchEntry(val->place);
	int cc=found.size();
	if ( cc == 0 ) { //If there isn't another entry with the same place
		Entries.push_back(val);
		cout << "Entry has been registered.\n";
	}
	else{ //Else gives the opportunity to update an existing entry or to add a new
		cout << val->place << " has already some entries:\n";
		for(int i=0;i<cc;i++){ //list of entries with the same place
			entry* x=Entries[found[i]];
			cout << "\t" << i+1 << "- User: " << x->user << " ; Pass: " << x->pass << "\n";
		}
		string answer=questionForm("Do you want to add a new entry (write new) or \
to update one of the entry (write its number)?");
		if( answer == "new" ) { //Add a new if it doesn't collide
			if( !searchCollisions(val) ) {
				Entries.push_back(val);
				cout << "Entry has been registered.\n";
			}
			else {
				cout << "The pair (place, user) = (" << val->place << ", " << val->user << ") is already present.\n";
			}
		}
		else { //Update if doesn't collide
			int nn=atoi(answer.c_str());
			if ( 1<=nn and nn<=cc ) {
				entry* x=Entries[found[nn-1]];
				if( x->user == val->user or !searchCollisions(val) ) {
					Entries[found[nn-1]]=val;
					cout << "Entry has been updated.\n";
				}
				else {
					cout << "The pair (place, user) = (" << val->place << ", " << val->user << ") is already present.\n";
				}
			}
			else{ //If the value given by the user is wrong
				cout << "You entered an invalid value.\n";
			}
		}
	}
	
}

void remove() {
	cout << "Place: ";
	string place;
	getline(cin,place);
	
	vector <int> found=searchEntry(place);
	int cc=found.size();
	if( cc==0 ) cout << "The place " << place << " isn't in the password list.\n";
	else if( cc==1 ) {
		Entries.erase(Entries.begin()+found[0]);
		cout << "Entry has been deleted.\n";
	}
	else {
		cout << "The place " << place << " is present in more than one entry:\n";
		for(int i=0;i<cc;i++){
			entry* x=Entries[found[i]];
			cout << "\t" << i+1 << "- User: " << x->user << " ; Pass: " << x->pass << "\n";
		}
		
		string answer=questionForm("Which (* for all) should I delete?");
		
		if( answer == "*" ) {
			for(int i=cc-1; i>=0 ;i--) { //cancello dall'ultimo per non rompere i puntatori
				assert( Entries[found[i]]->place == place );
				Entries.erase(Entries.begin()+found[i]);
			}
			cout << "All entries relative to " << place << " have been deleted.\n";
		}
		else {
			int nn=atoi(answer.c_str());
			if ( 1<=nn and nn<=cc ) {
				Entries.erase(Entries.begin()+found[nn-1]);
				cout << "Entry has been deleted.\n";
			}
			else{
				cout << "You entered an invalid value.\n";
			}
		}
	}
}

void retrieve( ) {
	cout << "Place: ";
	string place;
	getline(cin,place);
	
	vector <int> found=searchEntry(place);
	int cc=found.size();
	if( cc==0 ) cout << "The place " << place << " isn't in the password list.\n";
	else {
		for(int i=0;i<cc;i++) {
			entry* x=Entries[found[i]];
			cout << "\t" << i+1 << "- User: " << x->user << " ; Pass : " << x->pass << "\n";
		}
	}
}

void retrieveAll( ) {
	for(int i=0;i<(int)Entries.size();i++) {
		entry* x=Entries[i];
		cout << "Place: " << x->place << " ; User: " << x->user << " ; Pass : " << x->pass << "\n";
	}
}

#endif
