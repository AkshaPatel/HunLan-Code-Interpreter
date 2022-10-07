#ifndef _PROJECT3_H
#define _PROJECT3_H

#include <string>
#include <vector>
#include <unordered_map>


class MyParser {
public:
	MyParser();
	~MyParser();

	std::vector<std::string> operator()(std::string& input);
	void check_input(std::vector<std::string>& parsed_input);

private:
	std::vector<char> delimiters{ ' ', '\t' };
};


class MyManager {
public:
    MyManager();
    ~MyManager();

	long long convert_string_to_ll(std::string& numerical_string);

    bool process(std::vector<std::string>& parsed_userinput);
	bool NUMBER(std::string& parsed_userinput);
	bool STRING(std::string& parsed_userinput);
	bool PRINT(std::string& user_input);
	bool VARIABLE(std::vector<std::string>& parsed_userinput);

private:
	std::unordered_map<std::string, std::string> STRING_variables;

    std::unordered_map<std::string, long long> NUMBER_variables;
};



#endif