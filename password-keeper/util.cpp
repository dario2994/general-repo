#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

#include "header.hpp"
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
		//~ cout << res.back() << endl;
		pp=px+P.size();
	}
	return res;
}

string homePath, mainFolderPath, privateKeyHashPath, passwordListPath;
void initPath() {
	homePath=getenv("HOME");
	mainFolderPath=homePath+"/.password-keeper";
	privateKeyHashPath=mainFolderPath+"/.privateKeyHash.txt";
	passwordListPath=mainFolderPath+"/.passwordList.txt";
}
