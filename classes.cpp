#include "symbolParsing.h"

using namespace std;


//class definition
classDefinition buildClass(simpleLine inputLine){
	if (inputLine.size() < 2){
		throw slipSyntaxError("poor class definition");
	}
	return {inputLine[1]};
}
string to_string(classDefinition classDef){
	return classDef.name.value;
}
