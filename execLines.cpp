#include "symbolParsing.h"

using namespace std;

vector<vector<string>> operators{
	{">>", "<<"},
	{"*", "/", "%"},
	{"++", "--"},
	{"+", "-"},
	{"!"},
	{"&", "|", "^"},
	{"&&", "||"},
	{"==", "!="},
	{"=", "+=", "-=", "*=", "/=", "%="}
};

vector<string> keywords{
	"return",
	"end",
	"break",
	"continue"
};

int locateBreak(simpleLine inputLine){
	for (int i = operators.size() - 1; i >= 0; i--){
		int parentheticDepth = 0;
		for (int j = inputLine.size() - 1; j >= 0; j--){
			if (inputLine[j].value == ")"){
				parentheticDepth++;
			}
			else if (inputLine[j].value == "("){
				parentheticDepth--;
			}
			else if (parentheticDepth == 0)
			{
				for (string item : operators[i]){
					if (inputLine[j].value == item){
						return j;
					}
				}
			}
		}
	}
	return -1;
}

bool isKeyword(symbol inputValue){
	for (string item : keywords){
		if (item == inputValue.value){
			return true;
		}
	}
	return false;
}

//execution line
executionLine buildExecutionLine(simpleLine inputLine){
	if (inputLine.size() == 0){
		throw slipParsingError("zero length line building");
	}
	if (inputLine.size() == 1){
		if (isKeyword(inputLine[0])){
			return executionLine{
				inputLine[0], {}, false};
		}
		else
		{
			return executionLine{
				inputLine[0], {}, true};
		}
	}
	int n = locateBreak(inputLine);
	if (n == -1){
		if (inputLine[0].value == "("){
			vector<symbol> temp;
			for (int i = 1; i < inputLine.size() - 1; i++){
				temp.push_back(inputLine[i]);
			}
			return buildExecutionLine(simpleLine(temp));
		}
		else if (inputLine[0].value == "return"){
			vector<symbol> temp;
			for (int i = 1; i < inputLine.size(); i++){
				temp.push_back(inputLine[i]);
			}
			return {inputLine[0], {buildExecutionLine(
				simpleLine(temp))}, false};
		}
		else if (isKeyword(inputLine[0])){
			throw slipSyntaxError("invalid use of keyword",
				to_string(inputLine), inputLine[0].lineNumber,
				inputLine[0].charNumber);
		}
		else{
			vector<simpleLine> thing;
			int parentheticDepth = 0;
			symbol name = inputLine[0];
			vector<symbol> temp;
			for (int i = 2; i < inputLine.size() - 1; i++){
				if (inputLine[i].value == "("){
					parentheticDepth++;
				}
				else if (inputLine[i].value == ")"){
					parentheticDepth--;
				}
				if (parentheticDepth == 0 && 
						inputLine[i].value == ","){
					if (temp.size() != 0){
						thing.push_back(simpleLine(temp));
					}
					temp = vector<symbol>();
				}
				else{
					temp.push_back(inputLine[i]);
				}
			}
			if (temp.size() != 0){
				thing.push_back(simpleLine(temp));
			}
			vector<executionLine> args;
			for (auto item : thing){
				args.push_back(buildExecutionLine(item));
			}
			return {name, args, false};
		}
	}
	else{
		vector<symbol> leftList;
		vector<symbol> rightList;
		for (int i = 0; i < inputLine.size(); i++){
			if (i < n){
				leftList.push_back(inputLine[i]);
			}
			else if (i > n){
				rightList.push_back(inputLine[i]);
			}
		}
		if (leftList.size() > 0){
			if (rightList.size() > 0){
				return {
					inputLine[n],
					{buildExecutionLine(leftList),
						buildExecutionLine(rightList)},
					false
				};
			}
			else{
				return {
					inputLine[n],
					{buildExecutionLine(leftList)},
					false
				};
			}
		}
		else{
			if (rightList.size() > 0){
				return {
					inputLine[n],
					{buildExecutionLine(rightList)},
					false
				};
			}
			else{
				throw slipParsingError("lonely operator");
			}
		}
	}
}
string to_string(executionLine execLine){
	string output = execLine.name.value;
	if (execLine.isVariable){
		return output;
	}
	else{
		output += "(";
		if (execLine.arguments.size() == 0){
			output += ")";
			return output;
		}
		else{
			output += to_string(execLine.arguments[0]);
			for (int i = 1; i < execLine.arguments.size(); i++){
				output += ", " + to_string(execLine.arguments[i]);
			}
			output += ")";
			return output;
		}
	}
}
