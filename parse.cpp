#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include <memory>

using namespace std;

#define print(input) cout << input << endl

#include "symbolParsing.h"

int main(){
	string test = 
R"(def stringAdd(thing1,  thing2 )
	temp= thing1 +thing2;
	return temp;

//this is a comment
def world( )
	print("world");//this is another comment
	end;
//and a third comment
print( stringAdd("hel", "lo "));
world();

a = 1;
b = 2;
c = a+ b;
d = a * (b  +a) << 2;
print(c);)";
	try{
		convertToC(test);
	}
	catch(slipCompileError& e){
		print(e.what());
	}
	catch(exception& e){
		print(e.what());
	}
	return 0;
}
