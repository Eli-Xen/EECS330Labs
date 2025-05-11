#ifndef __MYINFIXCALCULATOR_H__
#define __MYINFIXCALCULATOR_H__

#include <algorithm>
#include <string>

#include "MyStack.h"
#include "MyVector.h"

//g++ --std=c++11 MainTest.cpp -o Lab5 
// for i in {0..9}; do
//     ./Lab5 Inputs/input_$i.txt > output_$i.txt
//     python3 GradingScript.py output_$i.txt Outputs/output_$i.txt
// done

class MyInfixCalculator{

  public:
    
    MyInfixCalculator()
    {}

    ~MyInfixCalculator()
    {}

    double calculate(const std::string& s)
    {   MyVector<std::string> infix, postfix; //declares infix and postfix vectors 
        tokenize(s, infix); 
        infixToPostfix(infix, postfix); 
        
        // std::cout << "Infix Tokens: ";
        // for (size_t i = 0; i < infix.size(); i++) {
        //     std::cout << infix[i] << " ";
        // }
        // std::cout << std::endl;

        // // Print the postfix expression
        // std::cout << "Postfix Tokens: ";
        // for (size_t i = 0; i < postfix.size(); i++) {
        //     std::cout << postfix[i] << " ";
        // }
        // std::cout << std::endl;
        
        return calPostfix(postfix); 
    }

  private:
    bool isNeg(const std::string& token) //checks if a token is a negative number or just a negative sign 
    {   if (token[0]=='-' && token.length()>1 && isDigit(token[1])) //if token begins with - and is longer than 1 char and second char is a digit then its a negative number 
            return true; 
        else return false; 
    }

    // returns operator precedance; the smaller the number the higher precedence
    // returns -1 if the operator is invalid
    // does not consider parenthesis
    int operatorPrec(const char c) const
    {   switch(c)
        {
            case '*':
                return 2;
            case '/':
                return 2;
            case '+':
                return 3;
            case '-':
                return 3;
            default:
                return -1;
        }
    }

    bool isValidParenthesis(const char c) const // checks if a character corresponds to a valid parenthesis                 
    {   switch(c)
        {
            case '(':
                return true;
            case ')':
                return true;
            default:
                return false;
        }
    }

    bool isDigit(const char c) const // checks if a character corresponds to a valid digit
    {   if(c >= '0' && c <= '9')
            return true;
        return false;
    }

    // computes binary operation given the two operands and the operator in their string form
    double computeBinaryOperation(const std::string& ornd1, const std::string& ornd2, const std::string& opt) const
    {   double o1 = std::stod(ornd1); //std::stod() converts a string to a double 
        double o2 = std::stod(ornd2);
        switch(opt[0])
        {   case '+':
                return o1 + o2;
            case '-':
                return o1 - o2;
            case '*':
                return o1 * o2;
            case '/':
                return o1 / o2;
            default:
                std::cout << "Error: unrecognized operator: " << opt << std::endl;
                return 0.0;
        }
    }


    // tokenizes an infix string s into a set of tokens (operands or operators)
    void tokenize(const std::string& s, MyVector<std::string>& tokens)
    {   std::string currentToken; //stores digits and decimal points until the current number is finished 
        for (size_t i = 0; i < s.length(); i++) {
            //Main loop
            if (isDigit(s[i]) || s[i] == '.') //appends num/decimal point 
                currentToken += s[i];
                        
            else if (s[i] == '(' || s[i] == ')') //if we find parenthesis, put current token into vector  
            {   if (currentToken != "") //if token isnt empty, push collected token and then clear it 
                {   tokens.push_back(currentToken);
                    currentToken = ""; //clear current token 
                }
                tokens.push_back(std::string(1,s[i])); //Add the parenthesis 
                //currentToken = "";
            }
            //Check if the current char is a negitive sign and not minus operator 
            else if (s[i] == '-' && (i == 0 || operatorPrec(s[i-1]) != -1 || s[i-1] == '(')) //if at -, and (at string[0] or previous i is operator or parenthesis)
                currentToken += s[i]; //add it to the token (should always be the start of a token)
    
            //Checks if the current char is a normal one length operator
            else if (operatorPrec(s[i]) != -1) //if its an operator
            {   if (currentToken != "") //if token isnt empty add it to vector...
                {   tokens.push_back(currentToken);
                    currentToken = "";
                }
                tokens.push_back(std::string(1,s[i])); //then make operator the current token 
            }
        }
        
        if (currentToken != "") //pushes the last token to the vector if there is one 
            tokens.push_back(currentToken); 
    }
    
    //checks for digit (put onto postfix), check for open parenthesis (push to operator stack), check for closed parenthesis (put all operands from stack to postfix), if operator compare precedence of current to top of stack and if stack is >= precedence add it to postfix then push current operator 
    void infixToPostfix(MyVector<std::string>& infix_tokens, MyVector<std::string>& postfix_tokens) //converts a set of infix tokens to a set of postfix tokens
    {   MyStack<std::string> stck; //stack to hold operators and parenthesis 
        for (const std::string& token : infix_tokens) //for token in tokens
        {   if (isDigit(token[0])) //if first token is a digit... 
            {    postfix_tokens.push_back(token); }//...put it onto postfix 
            else if (isNeg(token)) //if the token is negative number 
            {   postfix_tokens.push_back(token); } //put it onto postfix 

            else if (token == "(") //push open parenthesis 
            {   stck.push(token);}
                        
            else if (token == ")") //for closing parenthesis...
            {   while (!stck.empty() && stck.top() != "(") //until the stack is empty or reaches a "("...
                {   postfix_tokens.push_back(stck.top()); //push all operators from stack 
                    stck.pop(); 
                }
                if (!stck.empty() && stck.top() == "(") //after that it gets rid of open parenthesis 
                {    stck.pop(); }
            }

            else if (token=="+" || token=="-" || token=="*" || token =="/") 
            {   while (!stck.empty() && stck.top()!="(" && (operatorPrec(stck.top()[0])<=operatorPrec(token[0]))) //if token is operator; while stack not empty, pop from the stack until ( or operator of lower precedence is found 
                {   postfix_tokens.push_back(stck.top()); //add top of stack operator to postfix  
                    stck.pop();
                }
                stck.push(token); // push the current operator
            }
        }
        
        while (!stck.empty()) //pop remaining
        {   postfix_tokens.push_back(stck.top());
            stck.pop();
        }
    }

    // calculates the final result from postfix tokens
    double calPostfix(const MyVector<std::string>& postfix_tokens) const
    {   MyStack<std::string> solve; //stack for solving the equation 
        for (const std::string& token : postfix_tokens)
        {   if (isDigit(token[0]) || (token[0]=='-' && token.length()>1 && isDigit(token[1]))) //does this include negatives?? ik it uses ascii; we need token[0] because it wants a char 
                solve.push(token); 
            else //if (!isDigit(token)) //if token and not operand 
            {   std::string op2=solve.top(); //first operand 
                solve.pop(); 
                std::string op1=solve.top(); //second operand 
                solve.pop(); 
                double result=computeBinaryOperation(op1, op2, token); //passes operators and operand token
                solve.push(std::to_string(result)); //convert to string to store on stack; push back to stack incase there is * or other operators 
            }
        }
        double final=std::stod(solve.top()); //std::stod() converts string to double 
        solve.pop(); 
        return final; 
    }
};

#endif // __MYINFIXCALCULATOR_H__