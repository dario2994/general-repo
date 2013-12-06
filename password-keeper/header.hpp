#include <string>
#include <vector>
using namespace std;

string home, mainFolder, privateKeyHash, PasswordList;
vector <entry*> Entries;
struct entry{
	string place,user,pass;
	void enc();
	void dec();
};

string hashPassword( string );
string encrypt ( string );
string decrypt ( string )

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
