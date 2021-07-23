#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void getChar();					//Declaring the functions
void addChar();
void printAndErrorCheck();
char nextChar();
int isKeyword();
void writeBrackets(char c);
void checkCurlyBrackets(char c);
void checkBrackets(char c);
void lexical();

FILE* ceng;						//Input file
FILE *lex;						//Output file
char nextCh;					//Current char value
char lexeme[30];				//Storing the parsed value
int i = 0;						//Index value of lexeme
int upperClass;					//Storing the ancestor token

#define LETTER 1				//Numberizing the tokens
#define DIGIT 2
#define UNDERSCORE 3
#define OPERATOR 4
#define ENDOFLINE 7
#define IDENTIFIER 6
#define STRING 5
#define OTHERS 8
#define E 11
#define BRACKETS 9
#define KEYWORDS 10
#define COMMENT 12
#define SPACE 13

int charClass;					//Storing the current token
char stack[30];					//To check parantheses ()[] are balanced or unbalanced
int stackPointer = 0;		
char lastElementOfStack;
char curly_stack[30];			//To check parantheses {} are balanced or unbalanced	
int curly_stackPointer = 0;
char curly_lastElementOfStack;

char keywords[18][15] = { "break","case","char","const","continue","do","else","enum","float","for","goto","if","int", "long","record","return","static","while" };

void printAndErrorCheck() {		//Checking for identifier and operator errors, printing the value to the screen if there is no error.

	if (upperClass == IDENTIFIER) {		
		if (charClass == OPERATOR || charClass == ENDOFLINE || charClass == BRACKETS || charClass == SPACE) { 
			if (i > 20) {
				fprintf(lex, "Size is not allowed!\n");	//Identifier size error (20 characters.)
				exit(0);
			}
							
			else {
				if(isKeyword() == 0)					//Identifier token
					fprintf(lex, "IDENTIFIER(%s)\n", lexeme);
				else if (isKeyword() == 1) 				//Keyword token
					fprintf(lex, "KEYWORD(%s)\n", lexeme);
			}
		} 
		else {
			while (!(charClass == OPERATOR || charClass == ENDOFLINE || charClass == BRACKETS)) {
				addChar();
				getChar();

			}
			fprintf(lex, "Identifiers may include only letters, digits or underscore! %s\n", lexeme);
			exit(0);
			i = 0;
		}
	}
	else if (upperClass == OPERATOR) {
		if (i > 2) {								//Operator size error
			fprintf(lex, "Operator size can be up to 2!\n");
			exit(0);
		}
		else {
			if ((lexeme[0] == '+' && lexeme[1] == '\0') || (lexeme[0] == '-' && lexeme[1] == '\0') || (lexeme[0] == '*' && lexeme[1] == '\0') || (lexeme[0] == '/' && lexeme[1] == '\0') || (lexeme[0] == '+' && lexeme[1] == '+') || (lexeme[0] == '-' && lexeme[1] == '-') || (lexeme[0] == ':' && lexeme[1] == '=')) {
				fprintf(lex, "OPERATOR(%s)\n", lexeme);	//Operator token
			}
			else {									//Invalid operator
				fprintf(lex, "There is no such operator! %s\n", lexeme);
				exit(0);
			}
		}
	}
}

void addChar() {									//Filling the lexeme array

	lexeme[i] = nextCh;	
	i++;
}

char nextChar() {									//Current character of the file
	
	    char x = fgetc(ceng);
		if (feof(ceng)) {							//If the file ends, will return E
			return E;
		}
		else {
			if (x > 64 && x < 91) {					//Making the character lower case 
				x = x + 32;
			}
			return x;
		}
}

void getChar() {									//Determining type of the token

	if ((nextCh = nextChar()) != E) {
		if (nextCh == ';') {
			charClass = ENDOFLINE;
		}
		else if ((nextCh > 96 && nextCh < 123) || (nextCh > 64 && nextCh < 91)) {		//Letters (a-z, A-Z)
			charClass = LETTER;
		}
		else if (nextCh > 47 && nextCh < 58) {											//Digits (0-9)
			charClass = DIGIT;
		}
		else if (nextCh == 95) {
			charClass = UNDERSCORE;
		}
		else if (nextCh == '+' || nextCh == '-' || nextCh == '*' || nextCh == '/' || nextCh == ':' || nextCh == '=') {
			charClass = OPERATOR;
		}
		else if (nextCh == 34) {
			charClass = STRING;
		}
		else if (nextCh == '*') {
			charClass = COMMENT;
		}
		else if (nextCh == '(' || nextCh == ')' || nextCh == '[' || nextCh == ']' || nextCh == '{' || nextCh == '}') {
			charClass = BRACKETS;
		}
		else if (nextCh == ' ') {
			charClass = SPACE;
		}
		else
			charClass = OTHERS;
	}
	else {
		charClass = E;
	}
}

int isKeyword() {											//Checking if the word is a keyword.

	for (int i = 0; i < 18; i++) {
		if (!strcmp(lexeme, keywords[i])) {
			return 1;
		}
	}
	return 0;
}

void writeBrackets(char charr) {							//Printing the brackets token

	if (charr == '(') {
		fprintf(lex, "LeftPar\n");
	}
	else if (charr == '[') {
		fprintf(lex, "LeftSquareBracket\n");
	}
	else if (charr == '{') {
		fprintf(lex, "LeftCurlyBracket\n");
	}
	else if (charr == ')') {
		fprintf(lex, "RightPar\n");
	}
	else if (charr == ']') {
		fprintf(lex, "RightSquareBracket\n");
	}
	else if (charr == '}') {
		fprintf(lex, "RightCurlyBracket\n");
	}
}

void checkCurlyBrackets(char _char) {								//Checks if curly brackets are equal

	if (_char == '{') {												//Filling the stack
		curly_stack[curly_stackPointer] = _char;
		curly_lastElementOfStack = curly_stack[curly_stackPointer];
		curly_stackPointer++;
	}
	else if (_char == '}' && curly_lastElementOfStack == '{') {		//Removing appropriate elements from stack.
		curly_stack[curly_stackPointer - 1] = '\0';
		curly_stackPointer -= 1;
		if (curly_stackPointer != 0) {
			curly_lastElementOfStack = curly_stack[curly_stackPointer - 1];
		}
		else {														
			curly_lastElementOfStack = '\0';
		}
	}
	else {															//Checking wrong situations. 
		curly_stackPointer++;
	}
}

void checkBrackets(char _char) {									//Checks if brackets are equal

	if (_char == '(' || _char == '[') {								//Filling the stack
		stack[stackPointer] = _char;
		lastElementOfStack = stack[stackPointer];
		stackPointer++;
	}
	else if (_char == ')' && lastElementOfStack == '(' || _char == ']' && lastElementOfStack == '[') { //Removing appropriate elements from stack.
		stack[stackPointer - 1] = '\0';
		stackPointer -= 1;
		if (stackPointer != 0) {
			lastElementOfStack = stack[stackPointer - 1];
		}
		else {
			lastElementOfStack = '\0';
		}
	}
	else {														//Checking wrong situations. 
		stackPointer++;
	}
}

void lexical() {

	getChar();
	while (charClass != E) {									//Read until end of the file

		for (int j = 0; j < 30; j++){							//Clear the lexeme 
			lexeme[j] = '\0';
		}

		switch (charClass){										//Token cases

		case LETTER:

			upperClass = IDENTIFIER;							//Identifier check
			addChar();											//Storing the identifier
			getChar();											//Next character

			while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE){	//Read until end of the identifier 
				addChar();							
				getChar();
			}
			printAndErrorCheck();								//Controlling the identifier
			i = 0;												//Set index of the lexeme to 0
			break;


		case OPERATOR:

			upperClass = OPERATOR;								//Operator check
			addChar();	
			getChar();
			
			while (charClass == OPERATOR){						//Read until end of the operator 
				addChar();
				getChar();
			}
			while (charClass == SPACE) {						//Ignoring the spaces
				getChar();
			}
			printAndErrorCheck();								//Controlling the operator
			i = 0;
			break;


		case STRING:

			getChar();
			int flag = 0;
			while (flag == 0) {
				if (feof(ceng)) {								//Situation of file ends
					flag = 2;
				}
				if (charClass == STRING) {						//End of the string
					flag = 1;
				}
				getChar();	
			}
			if (flag == 1)
				fprintf(lex, "StringConst()\n");						//String token
			if (flag == 2){
				fprintf(lex, "Strings Must Terminated Before The File End!\n");		//Error of uncompleted string
				exit(0);
			}	
			break;


		case DIGIT:
			
			addChar();
			getChar();
			int counter = 1;								    //Size of the integer
			int flag2 = 0;										//Determining the errors
			while (charClass == DIGIT){
				addChar();
				getChar();
				counter++;
				if (charClass == LETTER) {						//Starting with digit and contiuning with the letter
					flag2 = 1;
					while (charClass != ENDOFLINE)
						getChar();
					fprintf(lex, "Identifiers can not start with a number!\n");		//Rule violation of identifier initializing 
					exit(0);		
					break;
				}
				
				if (counter == 10) {											//Exceeding the maximum integer size
					flag2 = 1;
					while (charClass != ENDOFLINE)								//Read until en of the line
						getChar();
					fprintf(lex, "Integer size cannot be more than 10 digit!\n");
					exit(0);		
					break;
				}
			}
			while(!(charClass == OPERATOR || charClass == SPACE || charClass == ENDOFLINE)) {	//End of the integer
				flag2 = -1;
				addChar();
				getChar();
			}

			while(charClass == SPACE) {											//Rule violation of integer initializing 
				getChar();
				if (charClass == DIGIT || charClass == LETTER) {
					while (charClass != OPERATOR && charClass != ENDOFLINE) {
						addChar();
						getChar();
					}
					flag2 = -2;
					fprintf(lex, "Multiple integers can not be initialized at once. %s\n", lexeme);
					exit(0);
					break;

				}
			}
			if (counter < 10 && flag2 == 0)
				fprintf(lex, "IntConst(%s)\n", lexeme);				//Integer token
			else if (flag2 == -1) {
				fprintf(lex, "Integers can include only digits!%s\n", lexeme);
				exit(0);
			}
			i = 0;
			break;


		case COMMENT:

			getChar();
			flag = 3;
			while (flag == 3) {										//Uncompleted comment error
				if (feof(ceng)) {
					fprintf(lex, "Comments Must Terminated Before The File End!\n");
					exit(0);		
					flag = 4;
				}
				char temp = nextCh;
				if (nextCh == '*'){									//Comment has completed
					getChar();	
					if(nextCh == ')')
						break;
				}
				if(temp != '*')										//Next char
					getChar();		
			}
			break;


		case ENDOFLINE:

			getChar();
			while (nextCh == '\n' || nextCh == '\r') {			//Ignoring the newline characters.
				if (nextCh == '\n') {							//Checking whether the parentheses are balanced at the end of each line.				
					if (stackPointer != 0) {
						fprintf(lex, "Unbalanced paranthesis.\n");
						exit(0);
					}
					memset(stack, 0, sizeof(stack));			//Resetting the stack, stack values, pointer 
					stackPointer = 0;							//and the last element of the stack at the end of each line.	
					lastElementOfStack = '\0';
				}
				getChar();
			}
			fprintf(lex, "END OF LINE\n");								//End of line token
			i = 0;
			break;


		case OTHERS:

			if (nextCh == '\n' || nextCh == '\r') {						//Ignoring the new line character
				getChar();
				break;
			}
			addChar();
			getChar();
			while (charClass != OPERATOR && charClass != ENDOFLINE) {	//Rule violance of initializing (invalid characters)
				addChar();
				getChar();
			}
			fprintf(lex, "Rule violance of initializing! Contains invalid character(s)...%s\n", lexeme);
			exit(0);
			i = 0;
			break;


		case UNDERSCORE:

			addChar();
			getChar();
			while (charClass != OPERATOR && charClass != ENDOFLINE) {
				addChar();
				getChar();
			}
			fprintf(lex, "Rule violance of initializing! Contains invalid character(s)...%s\n", lexeme);
			exit(0);
			i = 0;
			break;


		case BRACKETS:

			if(nextCh == '('){											//Checking the bracket (
				getChar();
				if(nextCh == '*'){										//Comment control
					charClass = COMMENT;
					break;
				}
				else{
					checkBrackets('(');									//Printing the bracket tokens
				}
			}														
			if (nextCh == '[' || nextCh == ')' || nextCh == ']') {		//Checking the brackets ()[]
				checkBrackets(nextCh);
				getChar();
			}
			else if (nextCh == '{' || nextCh == '}') {					//Checking the curly brackets {}
				checkCurlyBrackets(nextCh);
				getChar();
			}
			break;


		case SPACE:

			getChar();																	//Ignring the space
			break;
		}
	}

	if (charClass == E) {
		fprintf(lex, "END OF FILE\n");						//End of file token
		if (curly_stackPointer != 0) {						//Checking whether the curly brackets are balanced at the end of file.
			fprintf(lex, "Unbalanced curly brackets.\n");					
			exit(0);
		}
	}
}

int main() {

	ceng = fopen("code.Ceng", "r");		//Reading the input file
	lex = fopen("code.lex", "w");
	if (ceng == NULL) {
		fprintf(lex, "No File!");
		exit(0);
	}
	lexical();
	fclose(ceng);
	fclose(lex);
	return 0;
}

