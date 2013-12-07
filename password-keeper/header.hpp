#ifndef passwordkeeper_header
#define passwordkeeper_header

#include <string>
#include <vector>
using namespace std;

#define SEPARATOR	" #@?sep?@# "
#define SPACESEP	"?^sp^?"

struct entry{
	string place,user,pass;
	void enc(string);
	void dec(string);
};


extern string homePath, mainFolderPath, privateKeyHashPath, passwordListPath;
extern vector <entry*> Entries;

//~ string removeSpaces ( string );
//~ string addSpaces ( string );
string hashPassword( string );
//~ string encrypt ( string , string );
//~ string decrypt ( string , string );
//~ string sha256( string );

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
void setPrivateKey( string );
void importPasswords();
void exportPasswords();
void destroy ();
void changePrivateKey();
void init ();
void readAllPass();
bool login ();

#endif
