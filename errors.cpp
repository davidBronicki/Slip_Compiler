#include "symbolParsing.h"

#include <iostream>

using namespace std;

#define print(input) cout << input << endl

//////bad parse implementation///////////////

string slipCompileError::standardBuild(){
	string output = ":\n";
	string line;
	if (lineNumber != -1){
		line = "line " + to_string(lineNumber) + " ";
	}
	string character;
	if (characterNumber != -1){
		character = "character " + to_string(characterNumber)
			+ ": ";
	}
	if (lineSnippet != "" || line != "" || character != ""){
		output += line;
		output += character;
		if (lineSnippet != ""){
			output += "\"";
			output += lineSnippet;
			output += "\"";
		}
		output += "\n";
	}
	output += description;
	output += "\n";
	return output;
}

slipCompileError::slipCompileError(string inDescription,
	string inLineSnippet,
	int inLineNumber,
	int inCharacterNumber)
{
	description = inDescription;
	lineSnippet = inLineSnippet;
	lineNumber = inLineNumber;
	characterNumber = inCharacterNumber;

	whatString = "\nSlip Compiler Error";
	whatString += standardBuild();
}
const char* slipCompileError::what() const throw(){
	return whatString.c_str();
}


slipParsingError::slipParsingError(string inDescription,
	string inLineSnippet,
	int inLineNumber,
	int inCharacterNumber) : slipCompileError(
		inDescription, inLineSnippet, inLineNumber, inCharacterNumber)
{
	whatString = "\nSlip Parsing Error";
	whatString += standardBuild();
}

slipParsingError::slipParsingError(string description,
	int throwLine,
	int throwCharacter) : 
		slipParsingError(description, "", throwLine, throwCharacter){}

slipParsingError::slipParsingError(string description,
	int throwLine) : 
		slipParsingError(description, "", throwLine, -1){}

slipParsingError::slipParsingError(string description) : 
		slipParsingError(description, "", -1, -1){}


slipSyntaxError::slipSyntaxError(string inDescription,
	string inLineSnippet,
	int inLineNumber,
	int inCharacterNumber) : slipCompileError(
		inDescription, inLineSnippet, inLineNumber, inCharacterNumber)
{
	whatString = "\nSlip Syntax Error";
	whatString += standardBuild();
}

slipSyntaxError::slipSyntaxError(string description,
	int throwLine,
	int throwCharacter) : 
		slipSyntaxError(description, "", throwLine, throwCharacter){}

slipSyntaxError::slipSyntaxError(string description,
	int throwLine) : 
		slipSyntaxError(description, "", throwLine, -1){}

slipSyntaxError::slipSyntaxError(string description) : 
		slipSyntaxError(description, "", -1, -1){}
