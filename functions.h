#pragma once
#include "execLines.h"

struct functionDefinition{
	symbol name;
	int executionLine;
	std::vector<std::string> arguments;
};
functionDefinition buildFunction(simpleLine inputLine, int execNumber);
std::string to_string(functionDefinition functDef);

struct typedFunction
{
	// functionDefinition definition;
	std::vector<std::string> typeNames;
	std::string returnType;
};
std::string to_string(typedFunction funct);

class function
{
	functionDefinition def;
	std::vector<typedFunction> instances;
	std::vector<executionLine> lines;
public:
	function(simpleLine inputLine, int execNumber);
	addTyped(std::vector<std::string> typeNames);
	std::string toString();
}

std::string to_string(function funct);
