#pragma once

struct executionLine{
	symbol name;
	std::vector<executionLine> arguments;
	bool isVariable;
};
executionLine buildExecutionLine(simpleLine inputLine);
std::string to_string(executionLine execLine);
