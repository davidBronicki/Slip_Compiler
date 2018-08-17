#include "symbolParsing.h"

using namespace std;


//function definition
functionDefinition buildFunction(
	simpleLine inputLine,
	int execNumber){
	if (inputLine.size() < 4){
		throw slipSyntaxError("bad function definition");
	}
	if (inputLine[2].value != "(" ||
			inputLine[inputLine.size() - 1].value != ")"){
		throw slipSyntaxError("bad function definition",
			inputLine[0].lineNumber,
			inputLine[0].charNumber);
	}
	if (inputLine.size() == 4){
		return {inputLine[1], execNumber, {}};
	}
	else{
		if (inputLine.size() % 2 != 1){
			throw slipSyntaxError("bad function definition",
				inputLine[0].lineNumber,
				inputLine[0].charNumber);
		}
		vector<string> temp;
		for (int i = 3; i < inputLine.size(); i += 2){
			temp.push_back(inputLine[i].value);
			if (temp.back() == "," ||
					temp.back() == "(" ||
					temp.back() == ")"){
				throw slipSyntaxError("bad function definition",
					inputLine[0].lineNumber,
					inputLine[0].charNumber);
			}
		}
		return {inputLine[1], execNumber, temp};
	}
}
string to_string(functionDefinition functDef){
	string output = functDef.name.value;
	output += "(";
	if (functDef.arguments.size() == 0){
		output += ")";
		return output;
	}
	else{
		output += functDef.arguments[0];
		for (int i = 1; i < functDef.arguments.size(); i++){
			output += ", ";
			output += functDef.arguments[i];
		}
		output += ")";
		return output;
	}
}

//typed function

string to_string(typedFunction funct)
{
	string output = funct.returnType;
	output += " " + funct.definition.name.value;
	output += "(";
	if (funct.definition.arguments.size() == 0){
		output += ")";
		return output;
	}
	else
	{
		output += funct.definition.arguments[0];
		for (int i = 1; i < funct.definition.arguments.size(); i++)
		{
			output += ", ";
			output += funct.typeNames[i];
			output += " ";
			output += funct.definition.arguments[i];
		}
		output += ")";
		return output;
	}
}

function::function(simpleLine inputLine, int execNumber)
{
	def = buildFunction(inputLine, execNumber);
}

void function::getBody(vector<executionLine> allLines)
{
	for (int i = def.executionLine; i < allLines.size(); i++)
	{
		lines.push_back(allLines[i]);
		if (allLines[i].name.value == "end" ||
			allLines[i].name.value == "return")
		{
			break;
		}
	}
}

void function::addTyped(vector<stirng> typeNames)
{

}
