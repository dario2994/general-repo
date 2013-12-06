#ifndef passwordkeeper_header
#define passwordkeeper_header

#include <string>
#include <vector>
using namespace std;

#define SEPARATOR	" #@?sep?@# "

struct entry{
	string place,user,pass;
	void enc();
	void dec();
};


extern string home, mainFolder, privateKeyHash, PasswordList;
extern string privateKey;
extern vector <entry*> Entries;

string hashPassword( string );
string encrypt ( string );
string decrypt ( string );

string questionForm( string );
bool decisionForm( string );
vector <string> splitString( string , string );
void initPath();

vector <int> searchEntry( string );
bool searchCollisions( entry* );
void add();
void remove();
void retrieve();
void retrieveAll();

void savePrivateKeyHash();
void save();
void importPasswords();
void exportPasswords();
void destroy ();
void changePrivateKey();
void init ();
void readAllPass();
bool login ();

#endif