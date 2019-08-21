#include "Command.h"

#include "ctype.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::stringstream;




/**************************************************
 
 Description:
 Command class ctor. Set command variable as user
 issued, set cmd to INVALID_COMMAND, set both line
 address variable x and y to -1
 
 Parameters:
 command: command issued by user
 
 **************************************************/
Command::Command(string command) : command(command), cmd(INVALID_COMMAND), x(-1), y(-1) {}




/**************************************************
 
 Description:
 Command class dtor
 
 **************************************************/
Command::~Command() {}




/**************************************************
 
 Description:
 Parse the trimmed command into four parts, int,
 char, int, char. Then check the constraint of two
 line addresses' range, determine the command type.
 If all valid, set twp line address variables and
 cmd type variable
 
 Parameters:
 currentLine: # of current line
 bufferSize: # of buffer size (# of last line)
 
 **************************************************/
void Command::parse(int currentLine, int bufferSize) {
    // translate two int parameters to corresponding string
    ostringstream oss_cl, oss_bs;
    oss_cl << currentLine;
    oss_bs << bufferSize;
    string originalCommand = command;
	//cout << "Un TRIMED: " << command << endl;
    // trim the command
    trim(oss_cl.str(), oss_bs.str());
     //cout << "TRIMED: " << command << endl;
    
    // parse the command into four parts
    istringstream iss(command);
    char c_1 = '\0', c_2 = '\0';


	iss >> c_2; // may be command
	iss.clear();
    iss >> x;
    iss.clear();
    iss >> c_1; // may be punctuation or command
    iss.clear();
    iss >> y;
    iss.clear();

	if (x > bufferSize) x = bufferSize;
	if (y > bufferSize) y = bufferSize;
	if (x < 1) x = 1;
	if (y < 1) y = 1;	
	if (x > y) {
		int temp = y; y = x; x = temp;
	}
	
   

     //cout << "PARSED: " << c_2<< "|" << x << "|" << c_1 << "|" << y  << endl;
    
    // check empty buffer, check range, determind command type
    if (0 == bufferSize && 1 == originalCommand.length()) { // empty buffer case
        if ("a" == originalCommand) {
            cmd = APPEND_AFTER;
        } else if ("i" == originalCommand) {
            cmd = INSERT_BEFORE;
        } else if ("v" == originalCommand) {
            cmd = PASTE_BELOW;
        } else if ("u" == originalCommand) {
            cmd = MOVE_UP;
		} else if ("n" == originalCommand) {
			cmd = MOVE_DOWN;
        } else if ("w" == originalCommand) {
            cmd = WRITE;
        } else if ("q" == originalCommand) {
            cmd = QUIT;
        } else if ("," == originalCommand) {
            cmd = INVALID_RANGE;
        } else {
            cmd = EMPTY_BUFFER;
        }
    } else if ('a' == c_2) { // append after
            cmd = APPEND_AFTER;
    } else if ('i' == c_2) { // insert before
            cmd = (x < 1 || x > bufferSize) ? INVALID_RANGE : INSERT_BEFORE;
    } else if ('v' == c_2) { // paste below
        cmd = (x < 1 || x > bufferSize) ? INVALID_RANGE : PASTE_BELOW;
    } else if ('d' == c_2) { // delete
        cmd = (x < 1 || x > y || y > bufferSize) ? INVALID_RANGE : DELETE;
    } else if ('x' == c_2) { // cut
        if ('x' == c_1) y = x;
        cmd = (x < 1 || x > y || y > bufferSize) ? INVALID_RANGE : CUT;
    } else if ('j' == c_2) { // join
        cmd = (x < 1 || x > y || y > bufferSize) ? INVALID_RANGE : JOIN;
    } else if ('p' == c_2) { // print
        cmd = (x < 1 || x > y || y > bufferSize) ? INVALID_RANGE : PRINT;
    } else if ('c' == c_2) { // change
        cmd = (x < 1 || x > y || y > bufferSize) ? INVALID_RANGE : CHANGE;
    } else if ('n' == c_2) { // move down
        cmd = MOVE_DOWN;
    } else if ('u' == c_2) { // move up
        cmd = MOVE_UP;
    } else if ('g' == c_2) { // move up
        cmd = (x < 1 || x > bufferSize) ? INVALID_RANGE : GO;
    } else if ('w' == c_2) { // write
        cmd = WRITE;
    } else if ('q' == c_2) { // quit
        cmd = QUIT;
    }
}



//need to chage everything in this method put all p related above and handle evry other thing below


/**************************************************
 
 Description:
 Interpret user issued command. Change '$' and '.'
 to line number, fill up the missing line address
 and command.
 
 Parameters:
 currentLine: # of current line
 bufferSize: # of buffer size (# of last line)
 
 **************************************************/
void Command::trim(string currentLine, string bufferSize) {
	// remove white spaces
	for (string::iterator it = command.begin(); it != command.end(); ++it) {
		if (isspace(*it)) {
			command.erase(it--);
		}
	}
	if (command=="1$") { // 1$ case
		command = "z-1,-1";
	}
	findAndReplaceAll(command, ".", currentLine);
	findAndReplaceAll(command, "$", bufferSize);


	if (perfectlyValidCommand(command)) {  //for best case scenario
		command = command;
	}	
	// interpret command issued by user
	else if (command.empty()) { // empty command case
		command = "p" + currentLine + "," + currentLine;
	}
	else if (checkInt(command)) { // single integer case
		command = "p" + command + "," + command;
	}
	else if ("n" == command || "u" == command) { // 'n' and 'u' case
		command = command + "1";
	}
	else if ("," == command) { // ',' case
		command = "p" + currentLine + "," + currentLine;
	}
	else if ("." == command || "$" == command) { // translate '.' and '$'
		string lineNumber = ("." == command) ? currentLine : bufferSize;
		command = "p" + lineNumber + "," + lineNumber;
	}
	else if ("a" == command || "i" == command || "v" == command ||
		"d" == command || "x" == command || "u" == command ||
		"j" == command || "p" == command || "n" == command ||
		"c" == command || "g" == command || "w" == command || 
		"q" == command || "p" == command) { // single command variable case
		command = command + currentLine + "," + currentLine;
	}
	else if (checkIfInvalid(command)) { // check for general invalidity
		command = "z-1,-1";
	}
	else if (startsWithComma(command.at(0))) { //for case ,y to p.,y
		string linenumber = command.substr(1);
		command = "p" + currentLine + "," + linenumber;
	}
	else if (startsWithDigit(command)) { //for case x, to px,x  and		x, y to px, y

		if ((command.find(",") + 1) == command.length()) //for case x, to px,x 
		{
			string sub = command.substr(0, command.find(","));
			command = "p" + sub + "," + sub;
		}
		else if (checkInt(command.substr(0, command.find(","))) && checkInt(command.substr(command.find(",") + 1))) {		//for case 		x, y to px, y			
			command = "p" + command;
		}
		else
			command = "z-1,-1";
	}
	else if (startsWithValidCommand(command)) { //for case zx to zx,x		z, y to z., y			zx, to zx, x

		if (checkInt(command.substr(1))) { //for case zx to zx,x
			command = command + "," + command.substr(1);
		}
		else if (checkInt(command.substr(2))) { //for case z,y to z., y
			int commapos = command.find(",");
			command = command.substr(0, commapos) + currentLine + command.substr(commapos);
		}
		else if ((command.find(",") + 1) == command.length()) { //for case zx, to zx, x
			int commapos = command.find(",");
			command = command + command.substr(1);
			command = command.substr(0, command.length() - 2);
		}
		else
			command = "z-1,-1";

	}
	else
		command = "z-1,-1";


}



/**************************************************
 
 Description:
 Interpret user issued command. Change '$' and '.'
 to line number, fill up the missing line address
 and command
 
 Parameters:
 s: string that to be checked, whether it is an
    integer or not
 
 Returns:
 Return true if s is an integer, false otherwise
 
 **************************************************/
bool Command::checkInt(string s) const {
    stringstream ss;
    ss << s;
    int x = 0;
    ss >> x;
    if (1 == ss.good()) { // check case like "12p"
        return false;
    } else if (0 == x && '0' != s[0]) { // check case like ,12
        return false;
    }
    return true;
}




/**************************************************
 
 Description:
 Simple getter function, get the cmd type
 
 Returns:
 Return cmd variable.
 
 **************************************************/
Cmd Command::getCommand() const {
    return cmd;
}




/**************************************************
 
 Description:
 Simple getter function, get the first line address
 
 Returns:
 Return the first line address
 
 **************************************************/
int Command::getX() const {
    return x;
}




/**************************************************
 
 Description:
 Simple getter function, get the second line address
 
 Returns:
 Return the second line address
 
 **************************************************/
int Command::getY() const {
    return y;
}

void Command::findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}

bool Command::checkIfInvalid(string command)
{
	bool result = false;
	for (int i = 1; i < command.length(); i++) {
		string str = command.substr(i, 1);
		if (!(checkInt(str) || str == ",")) {
		result = true;
		break;
	}
	}
	return result;
}

/*
 * Case Sensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
	bool Command::startsWithComma(char c1)
{
		string mainStr, toMatch;
		mainStr = c1;
		toMatch = ',';
	// std::string::find returns 0 if toMatch is found at starting
	if (mainStr.find(toMatch) == 0)
		return true;
	else
		return false;
}

	/*
 * Case Sensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
	bool Command::startsWithDigit(string str2)
	{
		// Find position of ',' using find() 
		int pos = str2.find(",");

		// Copy substring upto pos 
		string sub = str2.substr(0,pos);

		return checkInt(sub);
	}

	/*
* Case Sensitive Implementation of startsWith()
* It checks if the string 'mainStr' starts with given string 'toMatch'
*/
	bool Command::startsWithValidCommand(string str)
	{
		bool result = false;
		// Copy first char 
		string sub = str.substr(0, 1);

		if(	"a" == sub || "i" == sub || "v" == sub ||
			"d" == sub || "x" == sub || "u" == sub ||
			"j" == sub || "p" == sub || "n" == sub ||
			"c" == sub || "g" == sub || "w" == sub || "q" == sub) {
			result = true;
		}

		return result;
	}


	/*
* Case Sensitive Implementation of startsWith()
* It checks if the string 'mainStr' starts with given string 'toMatch'
*/
	bool Command::perfectlyValidCommand(string str) //zx,y
	{
		bool result = false;
		if (str.find(",") == std::string::npos) return result;
		// Copy first char 
		int commapos;
		commapos = str.find(",");
		string sub = str.substr(0, 1);
		string firstInt = str.substr(1, commapos-1);
		string secondInt = str.substr(commapos+1);

		if (("a" == sub || "i" == sub || "v" == sub ||
			"d" == sub || "x" == sub || "u" == sub ||
			"j" == sub || "p" == sub || "n" == sub ||
			"c" == sub || "g" == sub || "w" == sub || "q" == sub) && checkInt(firstInt) && checkInt(secondInt)) {
			result = true;
		}

		return result;
	}
