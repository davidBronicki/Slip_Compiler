#pragma once

// using namespace std;

class slipCompileError : public std::exception{
	std::string description;
	std::string lineSnippet;
	int lineNumber;
	int characterNumber;
protected:
	std::string whatString;
	std::string standardBuild();
public:
	slipCompileError(std::string inputDescription,
			std::string inputLineSnippet,
			int inputThrowLine,
			int inputThrowCharacter);
	virtual const char* what() const throw();
};

class slipParsingError : public slipCompileError{
	// const char* whatString;
public:
	slipParsingError(std::string inputDescription,
			std::string inputLineSnippet,
			int inputThrowLine,
			int inputThrowCharacter);
	slipParsingError(std::string inputDescription,
			int inputThrowLine,
			int inputThrowCharacter);
	slipParsingError(std::string inputDescription,
			int inputThrowLine);
	slipParsingError(std::string inputDescription);
};

class slipSyntaxError : public slipCompileError{
	// const char* whatString;
public:
	slipSyntaxError(std::string inputDescription,
			std::string inputLineSnippet,
			int inputThrowLine,
			int inputThrowCharacter);
	slipSyntaxError(std::string inputDescription,
			int inputThrowLine,
			int inputThrowCharacter);
	slipSyntaxError(std::string inputDescription,
			int inputThrowLine);
	slipSyntaxError(std::string inputDescription);
};
