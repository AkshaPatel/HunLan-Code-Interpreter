#include "Project3.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

MyParser::MyParser() {

}

std::vector<std::string> MyParser::operator()(std::string& input) {
    std::vector<std::string> parsed_command;

    std::string token = "";
    int num_of_quotes = 0;

    for (int i=0; i<input.length(); i++) {
        if(input[i] == '#') {
            break;
        }
        if (input[i] == '"')
            num_of_quotes++;
    }

    int i = 0;

    if (num_of_quotes == 2 || num_of_quotes == 0) {
        while (i < input.length()) {
            if (input[i] == delimiters[0] || input[i] == delimiters[1]) {
                if (token != "") {
                    parsed_command.push_back(token);
                    token = "";
                }
            }
            else if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '=') {
                if (token != "") {
                    parsed_command.push_back(token);
                }

                token = input[i];
                parsed_command.push_back(token);
                token = "";
            }
            else if (input[i] == '"') {
                if (token != "") {
                    parsed_command.push_back(token);
                }

                // I include the quotes in the parsed input to differentiate 
                // between variable names and strings when doing PRINT functionality.
                token = input[i];
                i++;

                while (input[i] != '"') {
                    token += input[i];
                    i++;
                }

                token += input[i];

                parsed_command.push_back(token);
                token = "";
            }
            else if (input[i] == '#') {
                if (token != "") {
                    parsed_command.push_back(token);
                }
                
                check_input(parsed_command);
                return parsed_command;
            }
            else {
                token += input[i];
            }

            if (i == (input.length() - 1) && token != "")
                parsed_command.push_back(token);

            i++;
        }
    }
    else {
        parsed_command = {"BAD"};
    }

    check_input(parsed_command);
    return parsed_command;
}

void MyParser::check_input(std::vector<std::string>& parsed_input) {
    if (parsed_input.size() == 0) {

    }
    else if (parsed_input[0] == "NUMBER") {
        if (parsed_input.size() != 2) {
            parsed_input = {"BAD"};
        }

        if (isdigit(parsed_input[1][0])) {
            parsed_input = {"BAD"};
        }
    }
    else if (parsed_input[0] == "STRING") {
        if (parsed_input.size() != 2) {
            parsed_input = {"BAD"};
        }

        if (isdigit(parsed_input[1][0])) {
            parsed_input = {"BAD"};
        }
    }
    else if (parsed_input[0] == "PRINT") {
        if (parsed_input.size() != 2) {
            parsed_input = {"BAD"};
        }
    }
}

MyParser::~MyParser() {

}


MyManager::MyManager() {

}

long long MyManager::convert_string_to_ll(std::string& numerical_string) {
    return stoll(numerical_string);
}

bool MyManager::process(std::vector<std::string>& parsed_userinput) {
    if (parsed_userinput.size() == 0) {
        return false;
    }
    else if (parsed_userinput[0] == "BAD") {
        return true;
    }
    else if (parsed_userinput[0] == "NUMBER") {
        return NUMBER(parsed_userinput[1]);
    }
    else if (parsed_userinput[0] == "STRING") {
        return STRING(parsed_userinput[1]);
    }
    else if (parsed_userinput[0] == "PRINT") {
        return PRINT(parsed_userinput[1]);
    }
    else {
        return VARIABLE(parsed_userinput);
    }
}

bool MyManager::NUMBER(std::string& parsed_userinput) {
    if (isdigit(parsed_userinput[0])) {
        return true;
    }

    NUMBER_variables.insert(std::pair<std::string, long long>(parsed_userinput, 0));
    
    return false;
}

bool MyManager::STRING(std::string& parsed_userinput) {
    if (isdigit(parsed_userinput[0])) {
        return true;
    }

    STRING_variables.insert(std::pair<std::string, std::string>(parsed_userinput, ""));

    return false;
}

bool MyManager::PRINT(std::string& user_input) {
    // count the number of quotes to tell if input is a variable or a string 

    int num_of_quotes = 0;

    for (int i=0; i < user_input.length(); i++) {
        if (user_input[i] == '"')
            num_of_quotes++;
    }

    if (num_of_quotes == 2) {
        std::string str_without_quotes = user_input.substr(1, user_input.length() - 2);

        for (int i=0; i < str_without_quotes.length(); i++) {
            if(int(str_without_quotes[i]) == 92 && int(str_without_quotes[i+1]) == 110) {
                std::cout << '\n';
                i++;
            }
            else {
                std::cout << str_without_quotes[i];
            }
        }
    }
    else if (num_of_quotes == 0) {
        if (STRING_variables.find(user_input) != STRING_variables.end()) {
            
            std::cout << STRING_variables[user_input];
        }
        else if (NUMBER_variables.find(user_input) != NUMBER_variables.end()) {
            std::cout << NUMBER_variables[user_input];
        }
        else {
            std::cout << user_input;
        }
    }
    
    return false;
    
}

bool MyManager::VARIABLE(std::vector<std::string>& parsed_userinput) {
    
    //If a variable is being set
    if (parsed_userinput.size() == 3) {
        if (STRING_variables.find(parsed_userinput[2]) != STRING_variables.end()) {
            if (STRING_variables.find(parsed_userinput[0]) != STRING_variables.end()) {
                if (parsed_userinput[1] == "=") {
                    STRING_variables[parsed_userinput[0]] = STRING_variables[parsed_userinput[2]];
                    return false;
                }
            }
        }
        else if (NUMBER_variables.find(parsed_userinput[2]) != NUMBER_variables.end()) {
            if (NUMBER_variables.find(parsed_userinput[0]) != NUMBER_variables.end()) {
                if (parsed_userinput[1] == "=") {
                    NUMBER_variables[parsed_userinput[0]] = NUMBER_variables[parsed_userinput[2]];
                    return false;
                }
            }
        }
        else {
            if (STRING_variables.find(parsed_userinput[0]) != STRING_variables.end()) {
                if (parsed_userinput[1] == "=") {
                    std::string str_without_quotes = parsed_userinput[2].substr(1, parsed_userinput[2].length() - 2);
                    STRING_variables[parsed_userinput[0]] = str_without_quotes;
                    return false;
                }
            }

            if (NUMBER_variables.find(parsed_userinput[0]) != NUMBER_variables.end()) {
                if (parsed_userinput[1] == "=") {
                    for (int i = 0; i < parsed_userinput[2].size(); i++) {
                        if(!isdigit(parsed_userinput[2][i])) {
                            return true;
                        }
                    }
                    long long number = convert_string_to_ll(parsed_userinput[2]);
                    NUMBER_variables[parsed_userinput[0]] = number;
                    return false;
                }
            }
        }
    }
    else {
        //If there is two mathematical symbols next to each other return an error:
        for (int i=0; i < parsed_userinput.size()-1; i++) {
            if ((parsed_userinput[i] == "+" || parsed_userinput[i] == "-" || parsed_userinput[i] == "*") &&
                (parsed_userinput[i+1] == "+" || parsed_userinput[i+1] == "-" || parsed_userinput[i+1] == "*"))
                return true;
        }

        // This will calculate a number and assign it to a NUMBER variable
        if (NUMBER_variables.find(parsed_userinput[0]) != NUMBER_variables.end() && parsed_userinput[1] == "=") {
            bool is_there_multiplication = true;
            long long multiplication_sum = 1;

            for (int i=2; i < parsed_userinput.size(); i++) {
                if (NUMBER_variables.find(parsed_userinput[i]) != NUMBER_variables.end()) {
                    parsed_userinput[i] =  std::to_string(NUMBER_variables[parsed_userinput[i]]);
                }
            }

            while (is_there_multiplication) {
                is_there_multiplication = false;

                for (int i=2; i < parsed_userinput.size(); i++) {
                    if (parsed_userinput[i] == "*") {
                        multiplication_sum *= convert_string_to_ll(parsed_userinput[i-1]) * convert_string_to_ll(parsed_userinput[i+1]);
                        parsed_userinput[i] = std::to_string(multiplication_sum);
                        parsed_userinput.erase(parsed_userinput.begin() + (i+1));
                        parsed_userinput.erase(parsed_userinput.begin() + (i-1));
                        multiplication_sum = 1;
                        is_there_multiplication = true;
                    }
                }
            }

            long long sum = 0;
            //Put the first value into sum
            if (NUMBER_variables.find(parsed_userinput[2]) != NUMBER_variables.end()) {
                sum += NUMBER_variables[parsed_userinput[2]];
            }
            else {
                sum += convert_string_to_ll(parsed_userinput[2]);
            }
            
            for (int i=3; i < parsed_userinput.size()-1; i++) {
                if (parsed_userinput[i] == "+") {
                    sum += convert_string_to_ll(parsed_userinput[i+1]);
                }
                else if (parsed_userinput[i] == "-") {
                    sum -= convert_string_to_ll(parsed_userinput[i+1]);
                }
                i++;
            }

            NUMBER_variables[parsed_userinput[0]] = sum;
            return false;
        }
    }

    return true;
}

MyManager::~MyManager() {

}

int main(int argc, char *argv[]) {
   
   MyManager engine;
   MyParser parser;

   std::string userInput;

   std::fstream infile;
   infile.open(argv[1], std::ios::in);
   bool shouldStop = false;

    int line_number = 1;

    if (infile.is_open()) {
        while(getline(infile, userInput)) {

            std::vector<std::string> parsed_input = parser(userInput);
            shouldStop = engine.process(parsed_input);

            if (shouldStop) {
                std::cout << "ERROR: Bad instruction in Line " << line_number << '\n';
                infile.close();
            }
            line_number++;
        }
    }

    infile.close(); 
    return 0;
}