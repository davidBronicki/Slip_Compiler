#pragma once

class scope{
	std::vector<std::string> scopeNames;
public:
	scope();
	scope(std::string starterScope);
	void pushScope(std::string newName);
	void popScope();
	friend scope leastCommonScope(const scope& base, const scope& other);
	friend bool operator==(const scope& base, const scope& other);
};
scope leastCommonScope(const scope& base, const scope& other);
bool operator==(const scope& base, const scope& other);

struct variableSingleShadow{
	std::string typeName;
	scope currentScope;
};

class variable{
	std::vector<variableSingleShadow> shadows;
	int shadowIndex;
	std::vector<variable> members;
	std::string name;
	scope currentScope;
public:
	variable(std::string inputName, scope variableScope,
		std::string typeName);
	void pushScope(std::string newScope);
	void popScope();
	bool exists();
	void addShadow(std::string newType);
	std::string getName();
};
