#include "symbolParsing.h"
#include <math.h>

using namespace std;

//scope class implementation
scope::scope()
{

}
scope::scope(string starterScope)
{
	scopeNames.push_back(starterScope);
}
void scope::pushScope(string newName)
{
	scopeNames.push_back(newName);
}
void scope::popScope(){
	scopeNames.pop_back();
}
scope leastCommonScope(const scope& base, const scope& other)
{
	int minVal = min(base.scopeNames.size(), other.scopeNames.size());
	scope output;
	for (int i = 0; i < minVal; i++)
	{
		if (base.scopeNames[i] == other.scopeNames[i])
		{
			output.pushScope(base.scopeNames[i]);
		}
		else
		{
			return output;
		}
	}
}
bool operator==(const scope& base, const scope& other)
{
	if (base.scopeNames.size() != other.scopeNames.size()){
		return false;
	}
	for (int i = 0; i < base.scopeNames.size(); i++){
		if (base.scopeNames[i] != other.scopeNames[i]){
			return false;
		}
	}
	return true;
}

//variable class implementation
variable::variable(string inputName, scope variableScope,
	string typeName)
{
	shadowIndex = 0;
	name = inputName;
	shadows.push_back(variableSingleShadow{typeName, variableScope});
	currentScope = variableScope;
}

void variable::pushScope(string newScope)
{
	currentScope.pushScope(newScope);
}

void variable::popScope()
{
	for (int i = shadows.size() - 1; i >= 0; i--)
	{
		if (shadows[i]->currentScope == currentScope)
		{
			shadows.erase(shadows.begin() + i);
			shadowIndex--;
		}
	}
	currentScope.popScope();
}

string variable::getName(){
	return name;
}

bool variable::exists()
{
	return shadows.size() > 0;
}

void variable::addShadow(string newType){
	shadows.push_back(variableSingleShadow{newType, currentScope});
	shadowIndex = shadows.size() - 1;
}
