#include "symbolParsing.h"

#include <iostream>

using namespace std;

#define print(input) cout << input << endl

//////environment implementation//////////////////////

//helper functions
bool isFunction(simpleLine inputLine){
	return inputLine[0].value == "def";
}

bool isClass(simpleLine inputLine){
	return inputLine[0].value == "class";
}

bool isLiteral(string parsedValue){
	if (parsedValue[0] == '"'){
		return true;
	}
	if (parsedValue == "true" ||
		parsedValue == "false")
	{
		return true;
	}
	try
	{
		stod(parsedValue);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

string literalTypify(string parsedValue)
{
	if (parsedValue[0] == '"'){
		return "string";
	}
	if (parsedValue == "true" ||
		parsedValue == "false")
	{
		return "bool";
	}
	try
	{
		stoll(parsedValue);
		return "long long";
	}
	catch(...)
	{
		return "double";
	}
}

//member functions
void environment::addFunct(string name,
	vector<string> arguments,
	vector<string> returnTypes,
	vector<vector<string>> argTypes)
{
	int i = 0;
	for (; i < builtInFunctionFootprints.size(); i++){
		if (builtInFunctionFootprints[i].name.value == name){
			break;
		}
	}
	if (i == builtInFunctionFootprints.size()){
		builtInFunctionFootprints.push_back({
			{-1, -1, name}, -1, arguments});
	}
	for (int j = 0; j < returnTypes.size(); j++){
		builtInFunctions.push_back({
			builtInFunctionFootprints[i],
			argTypes[j], returnTypes[j]});
	}
}

environment::environment()
{
	vector<string> numerics{
		"*", "/", "+", "-"
	};
	for (string op : numerics){
		addFunct(op, {"base", "other"},
			{
				"long long",
				"double",
				"double"
			},
			{
				{"long long", "long long"},
				{"long long", "double"},
				{"double", "double"}
			});
	}
	vector<string> stringNumerics{
		"+", "-"
	};
	for (string op : stringNumerics)
	{
		addFunct(op, {"base", "other"},
			{
				"string"
			},
			{
				{"string", "string"}
			});
	}
	addFunct("print", {"input"},
		{
			"void"
		},
		{
			{"string"}
		});
	currentScope = scope("global");
}

bool environment::variableExists(string var)
{
	for (variable item : variables)
	{
		if (var == item.getName())
		{
			return true;
		}
	}
	return false;
}

bool environment::functionExists(string name, int argsLength)
{
	for (functionDefinition def : builtInFunctionFootprints)
	{
		if (def.name.value == name &&
			def.arguments.size() == argsLength)
		{
			return true;
		}
	}
	for (functionDefinition def : functions)
	{
		if (def.name.value == name &&
			def.arguments.size() == argsLength)
		{
			return true;
		}
	}
	return false;
}

string environment::getReturnType(string functionName,
	vector<string> argTypes)
{
	functionDefinition functFootprint;
	for (typedFunction def : builtInFunctions)
	{
		if (def.definition.name.value == functionName &&
			def.definition.arguments.size() == argTypes.size())
		{
			functFootprint = def.definition;
			int i = 0;
			for (; i < argTypes.size(); i++)
			{
				if (def.argTypes[i] != argTypes[i])
				{
					break;
				}
			}
			if (i == argTypes.size())
			{
				return def.returnType;
			}
		}
	}
	for (typedFunction def : typedFunctions)
	{
		if (def.definition.name.value == functionName &&
			def.definition.arguments.size() == argTypes.size())
		{
			functFootprint = def.definition;
			int i = 0;
			for (; i < argTypes.size(); i++)
			{
				if (def.argTypes[i] != argTypes[i])
				{
					break;
				}
			}
			if (i == argTypes.size())
			{
				return def.returnType;
			}
		}
	}
	
}

string environment::typeInterpret(executionLine line)
{
	if (functionExist(line.name.value, line.arguments.size()))
	{
		vector<string> argTypes;
		for (executionLine item : line.arguments)
		{
			argTypes.push_back(typeInterpret(
				item));
			if (argTypes.back() == "")
			{
				return "";
			}
		}
		return getReturnType(line.name.value, argTypes);
	}
	else
	{
		throw slipSyntaxError("function was never declared",
			line.name.value, line.name.lineNumber,
			lin.name.charNumber);
	}
}

void environment::validateVariablesInExecutionLine(executionLine line)
{
	if (line.name.value == "="){
		if (line.arguments[0].isVariable){
			if (isLiteral(line.arguments[0].name.value)){
				throw slipSyntaxError("cannot assign to a literal",
					line.arguments[0].name.value, 
					line.arguments[0].name.lineNumber,
					line.arguments[0].name.charNumber);
			}
			if (!variableExists(line.arguments[0].name.value)){
				string typeAssignment = typeInterpret(line.arguments[1]);
				variables.push_back(variable(
					line.name.value, currentScope,
					typeAssignment));
			}
		}
		else
		{
			throw slipSyntaxError("assignment must be performed \
				on a single variable",
				line.name.value,
				line.name.lineNumber, line.name.charNumber);
		}
		validateVariablesInExecutionLine(line.arguments[1]);
	}
	else{
		if (line.isVariable){
			if (!variableExists(line.name.value) &&
				!isLiteral(line.name.value) ){
				throw slipSyntaxError("variable not yet declared",
					line.name.value,
					line.name.lineNumber, line.name.charNumber);
			}
		}
		else{
			for (executionLine item : line.arguments){
				validateVariablesInExecutionLine(item);
			}
		}
	}
}

void environment::interpretLine(simpleLine newLine){
	if (isFunction(newLine)){
		// print("function");
		functions.push_back(buildFunction(
			newLine, lines.size()));
		currentScope.pushScope(functions.back().name.value);
		for (variable var : variables){
			var.pushScope(functions.back().name.value);
		}
		for (string arg : functions.back().arguments)
		{
			if (variableExists(arg)){
				for (variable var : variables)
				{
					if (arg == var.getName()){
						var.addShadow();
					}
				}
			}
			else
			{
				variables.push_back(variable(arg, currentScope));
			}
		}
	}
	else if (isClass(newLine)){
		// print("class");
		classes.push_back(buildClass(newLine));
		currentScope.pushScope(classes.back().name.value);
		for (variable var : variables){
			var.pushScope(classes.back().name.value);
		}
	}
	else{
		// print("line");
		lines.push_back(buildExecutionLine(newLine));
		validateVariablesInExecutionLine(lines.back());
		if (lines.back().name.value == "end" ||
			lines.back().name.value == "return")
		{
			currentScope.popScope();
			for (variable var : variables){
				var.popScope();
			}
			for (int i = variables.size() - 1; i >= 0; i--){
				if (!variables[i].exists()){
					variables.erase(variables.begin() + i);
				}
			}
		}
	}
	// currentLine++;
}
void environment::typeInterpret(){
	
}
string environment::toC(){
	return "";
}
int environment::functListSize(){
	return functions.size();
}
functionDefinition environment::getFunct(int index){
	return functions[index];
}
int environment::execListSize(){
	return lines.size();
}
executionLine environment::getExecution(int index){
	return lines[index];
}
bool environment::isGlobal(){
	return currentScope == scope("global");
}


string convertToC(string slipString){
	vector<simpleLine> lines = interpretToLines(slipString);
	environment theEnsamble;
	for (simpleLine line : lines){
		theEnsamble.interpretLine(line);
	}
	print("");
	for (int i = 0; i < theEnsamble.functListSize(); i++){
		print(to_string(theEnsamble.getFunct(i)));
	}
	print("");
	for (int i = 0; i < theEnsamble.execListSize(); i++){
		print(to_string(theEnsamble.getExecution(i)));
	}
	theEnsamble.typeInterpret();
	return theEnsamble.toC();
}
