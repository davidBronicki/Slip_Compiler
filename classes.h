#pragma once

struct classDefinition{
	symbol name;
};
classDefinition buildClass(simpleLine inputLine);
std::string to_string(classDefinition classDef);

