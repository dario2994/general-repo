#ifndef passwordkeeper_defines
#define passwordkeeper_defines

#define SEPARATOR			" #@?sep?@# "

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

string questionForm(string question) {
	cout << question << " ";
	string answer;
	getline(cin,answer);
	return answer;
}

bool decisionForm(string question){
	while ( 1 ) {
		string answer=questionForm(question+" (yes or no)");
		if( answer == "yes" ) return true;
		else if( answer == "no") return false;
		else {
			cout << "You have to answer yes or no.\n";
		}
	} 
}

vector <string> splitString(string S, string P) {
	vector <string> res;
	int pp=0;
	while( 1 ) {
		int px=S.find(P,pp);
		if( px==-1 ){
			res.push_back( S.substr(pp,S.size()-pp) );
			break;
		}
		res.push_back( S.substr(pp,px-pp) );
		pp=px+P.size();
	}
	return res;
}

string home, mainFolder, privateKeyHash, PasswordList;
void initPath() {
	home=getenv("HOME");
	mainFolder=home+"/.password-keeper";
	privateKeyHash=mainFolder+"/.privateKeyHash.txt";
	PasswordList=mainFolder+"/.passwordList.txt";
}

#endif