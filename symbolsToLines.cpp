#include "symbolParsing.h"
#include <iostream>
#include <cstring>

using namespace std;

#define print(input) cout << input << endl

vector<string> symbols{
	"+","-","*","/","%","=",
	"&","^",".",",",";","&&",
	"||","!=","!","(",")","{",
	"}","[","]","+=","-=","*=",
	"/=","%=","++","--","<",">",
	">>","<<",">=","<=","?",":",
	"//", ";"
};

vector<char> separationSymbols{
	' ', '\t', '\n'
};

////////simpleLine implementation///////////////////

simpleLine::simpleLine(vector<symbol> inputSymbols){
	symbols = inputSymbols;
}
string simpleLine::toString(){
	if (symbols.size() == 0){
		return "";
	}
	else{
		string output = symbols[0].value;
		for (int i = 1; i < symbols.size(); i++){
			output += " ";
			output += symbols[i].value;
		}
		return output;
	}
}
int simpleLine::lineNumber(){
	return symbols[0].lineNumber;
}
symbol simpleLine::operator[](int index){
	return symbols[index];
}
int simpleLine::size(){
	return symbols.size();
}
string to_string(simpleLine line){
	if (line.size() == 0){
		return "";
	}
	else if (line.size() == 1){
		return line[0].value;
	}
	else{
		string output = line[0].value;
		for (int i = 1; i < line.size(); i++){
			output += " " + line[i].value;
		}
		return output;
	}
}



///////parsing symbols and putting into lines

bool couldBeSymbol(char c){
	for (string item : symbols){
		if (c == item[0]) return true;
	}
	return false;
}

bool isSeparator(char c){
	for (char item : separationSymbols){
		if (c == item) return true;
	}
	return false;
}

int singleJump(string& globalString, int& currentIndex){
	vector<string> contestantPool;
	for (string item : symbols){
		if (globalString[currentIndex] == item[0]){
			contestantPool.push_back(item);
		}
	}
	if (contestantPool.size() == 0){//not a symbol. look for separator.
		while (currentIndex < globalString.length()){
			if (isSeparator(globalString[currentIndex])){
				return currentIndex;
			}
			else if (couldBeSymbol(globalString[currentIndex])){
				currentIndex--;
				return currentIndex + 1;
			}
			currentIndex++;
		}
		return currentIndex;
	}
	else{//is symbol. figure out which one
		int initialIndex = currentIndex;
		string bestContender;
		while (contestantPool.size() > 0){
			currentIndex++;
			if (currentIndex == globalString.length()){
				return currentIndex;
			}
			for (int i = contestantPool.size() - 1; i >= 0; i--){
				try{
					if (contestantPool[i][currentIndex - initialIndex] !=
							globalString[currentIndex]){
						contestantPool.erase(contestantPool.begin() + i);
					}
				}
				catch(exception e){
					bestContender = contestantPool[i];
					contestantPool.erase(contestantPool.begin() + i);
				}
			}
		}
		if (isSeparator(globalString[currentIndex])){
			return currentIndex;
		}
		else{
			currentIndex--;
			return currentIndex + 1;
		}
	}
}

vector<simpleLine> interpretToLines(string& slipString){
	vector<simpleLine> output;
	vector<symbol> symbols;
	char mode = 'n';
	int characterNumber;
	int lineNumber;
	for (int i = 0; i < slipString.length(); i++){
		switch(mode){
			case 'n':{//normal mode
				if (slipString[i] == '"'){
					//start of string
					mode = 's';
					symbols.push_back({
						lineNumber, characterNumber, "\""});
				}
				else{
					//search for comment
					if (slipString[i] == '/'){
						if (i != slipString.length() - 1){
							if (slipString[i + 1] == '/'){
								mode = 'c';
								break;
							}
						}
					}
					//standard symbol recognition
					int j = i;
					int k = singleJump(slipString, i);
					symbols.push_back({
						lineNumber, characterNumber,
						slipString.substr(j, k - j)});
					//change char number and line number
					if (slipString[i] == '\n'){
						lineNumber++;
						characterNumber = 0;
					}
					else{
						characterNumber += i - j;
					}
					if (symbols.back().value == ""){
						symbols.pop_back();
					}
				}
				break;
			}
			case 's':{//string mode
				if (slipString[i] == '"'){
					mode = 'n';//string ends
				}
				else{//string continues
					symbols.back().value += slipString[i];
				}
				if (slipString[i] == '\n'){
					lineNumber++;
					characterNumber = 0;
				}
				else{
					characterNumber++;
				}
				break;
			}
			case 'c':{//comment mode
				if (slipString[i] == '\n'){//comment ends
					mode = 'n';
					lineNumber++;
					characterNumber = 0;
				}
				break;
				//do nothing if still in comment
			}
		}
	}
	vector<symbol> temp;
	mode = 'n';
	for (symbol s : symbols){
		switch (mode){
			case 'n':{//normal
				if (s.value == ";"){
					if (temp.size() != 0){
						output.push_back(simpleLine(temp));
					}
					temp = vector<symbol>();
				}
				else if (s.value == "def"){
					temp.push_back(s);
					mode = 'd';
				}
				else if (s.value != ""){
					temp.push_back(s);
				}
				break;
			}
			case 'd':{//is a definition line
				temp.push_back(s);
				if (s.value == ")"){
					mode = 'n';
					output.push_back(simpleLine(temp));
					temp = vector<symbol>();
				}
				break;
			}
		}
	}
	if (temp.size() != 0){
		output.push_back(simpleLine(temp));
	}
	return output;
}
