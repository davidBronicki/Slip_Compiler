#pragma once
#include <vector>
#include <string>
#include <exception>
#include <memory>

#include "errors.h"

struct symbol{
	int lineNumber;
	int charNumber;
	std::string value;
};

class simpleLine{
	std::vector<symbol> symbols;
public:
	simpleLine(std::vector<symbol> inputSymbols);
	std::string toString();
	int lineNumber();
	symbol operator[](int index);
	int size();
};
std::string to_string(simpleLine line);

std::vector<simpleLine> interpretToLines(std::string& slipString);

#include "functions.h"
#include "classes.h"
#include "variables.h"
#include "execLines.h"

class environment{
	scope currentScope;

	std::vector<classDefinition> classes;
	
	std::vector<variable> variables;
	std::vector<executionLine> lines;

	std::vector<functionDefinition> builtInFunctionFootprints;
	std::vector<typedFunction> builtInFunctions;
	std::vector<functionDefinition> functions;
	std::vector<typedFunction> typedFunctions;

	bool variableExists(std::string var);
	bool functionExists(std::string name, int argsLength);
	void validateVariablesInExecutionLine(executionLine line);
	void addFunct(std::string name,
		std::vector<std::string> arguments,
		std::vector<std::string> returnTypes,
		std::vector<std::vector<std::string>> argTypes);
	std::string typeInterpret(executionLine line);
	std::string getReturnType(std::string functionName,
		std::vector<std::string> argTypes)
public:
	environment();
	void interpretLine(simpleLine line);
	void typeInterpret();
 	void update(simpleLine newLine);
	int functListSize();
	functionDefinition getFunct(int index);
	int execListSize();
	executionLine getExecution(int index);
	bool isGlobal();
	std::string toC();
};

// class ensamble{
// 	std::vector<functionDefinition> builtInFunctionFootprints;
// 	std::vector<typedFunction> builtInFunctions;
// 	std::vector<typedFunction> functions;
// 	std::vector<environment> linePointEnvironments;
// 	// std::vector<simpleLine> executionLines;
// 	environment persistantEnvironment;
// 	void addFunct(std::string name,
// 		std::vector<std::string> arguments,
// 		std::vector<std::string> returnTypes,
// 		std::vector<std::vector<std::string>> argTypes)
// 	string typeInterpret(executionLine line);
// public:
// 	ensamble();
// 	void typeInterpret();
// 	void interpretLine(simpleLine line);
// 	std::string toC();
// 	int functListSize();
// 	functionDefinition getFunct(int index);
// 	int execListSize();
// 	executionLine getExecution(int index);
// };

std::string convertToC(std::string slipString);
