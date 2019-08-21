#include "LineEd.h"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::getline;
using std::cin;
using std::cout;
using std::endl;
using std::advance;
using std::size_t;




/**************************************************
 
 Description:
 LineEd class ctor. Set filename to parameter,
 currentLine to 1, saved to false
 
 Parameters:
 filename: the file that to be loaded, could be
        empty string
 
 **************************************************/
LineEd::LineEd(string filename) : filename(filename), currentLine(1), saved(false) {
	if (!filename.empty()) {
		loadFile();
	}
	else {
		cout << "\"?\" [New File]" << endl;
	}
}




/**************************************************
 
 Description:
 LineEd class dtor
 
 **************************************************/
LineEd::~LineEd() {}




/**************************************************
 
 Description:
 Initialize the LineEd object. If filename is given,
 load the file to buffer. Then entering command
 mode
 
 **************************************************/
void LineEd::run() {
   
    while(determineCommand());
}




/**************************************************
 
 Description:
 Load the entire file contents to buffer. Output
 the number of lines that have been read to the
 buffer
 
 **************************************************/
void LineEd::loadFile() {
    ifstream inFile(filename.c_str());
    string line;
    while (getline(inFile, line)) {
        buffer.push_back(line);
    }
    currentLine = buffer.size();
    cout << "\"" << filename << "\" " << currentLine;
    if (1 < currentLine) {
        cout << " lines" << endl;
    } else {
        cout << " line" << endl;
    }
}




/**************************************************
 
 Description:
 Handle command issued by user, create Command
 object to trim, parse determnine the command case,
 then call the corresponding function to execute
 the command
 
 Returns:
 Return false if command issued by user is quit
 command, true otherwise
 
 **************************************************/
bool LineEd::determineCommand() {
    bool notQuit = true;
    string userCommand;
    cout << "? ";
    getline(cin, userCommand);
    Command c(userCommand);
    c.parse(currentLine, buffer.size());
    Cmd cmd = c.getCommand();
    int x = c.getX();
    int y = c.getY();
    switch (cmd) {
        case APPEND_AFTER:
            saved = false;
            appendAfter();
            break;
        case INSERT_BEFORE:
            saved = false;
            insertBefore(x);
            break;
        case PASTE_BELOW:
            saved = false;
            pasteBelow(x);
            break;
        case DELETE:
            saved = false;
            deletes(x, y);
            break;
        case CUT:
            saved = false;
            cut(x, y);
            break;
        case JOIN:
            saved = false;
            join(x, y);
            break;
        case PRINT:
            print(x, y);
            break;
        case CHANGE:
            saved = false;
            change(x, y);
            break;
        case MOVE_UP:
            moveUp(1);
            break;
        case MOVE_DOWN:
            moveDown(1);
            break;
        case GO:
            currentLine = x;
            print(x, x);
            break;
		case WRITE:
			saved = true;
			write();
			break;
		case QUIT:
			quit(notQuit);
			break;
        case INVALID_RANGE:
            cout << "Bad address" << endl;
            break;
        case INVALID_COMMAND:
            cout << "bad command" << endl;
            break;
        case EMPTY_BUFFER:
            cout << "File empty . Must use one of I,A,Q,V commands" << endl;
            break;
    }
    return notQuit;
}

/**************************************************

 Description:
 Handle command issued by user, create Command
 object to trim, parse determnine the command case,
 then call the corresponding function to execute
 the command

 Returns:
 Return false if command issued by user is quit
 command, true otherwise

 **************************************************/
bool LineEd::runCommand(string testCommand) {

	bool notQuit = true;
	string userCommand= testCommand;
	Command c(userCommand);
	c.parse(currentLine, buffer.size());
	Cmd cmd = c.getCommand();
	int x = c.getX();
	int y = c.getY();
	switch (cmd) {
	case APPEND_AFTER:
		saved = false;
		appendAfter();
		break;
	case INSERT_BEFORE:
		saved = false;
		insertBefore(x);
		break;
	case PASTE_BELOW:
		saved = false;
		pasteBelow(x);
		break;
	case DELETE:
		saved = false;
		deletes(x, y);
		break;
	case CUT:
		saved = false;
		cut(x, y);
		break;
	case JOIN:
		saved = false;
		join(x, y);
		break;
	case PRINT:
		print(x, y);
		break;
	case CHANGE:
		saved = false;
		change(x, y);
		break;
	case MOVE_UP:
		moveUp(1);
		break;
	case MOVE_DOWN:
		moveDown(1);
		break;
	case GO:
		currentLine = x;
		print(x, x);
		break;
	case WRITE:
		saved = true;
		write();
		break;
	case QUIT:
		quit(notQuit);
		break;
	case INVALID_RANGE:
		cout << "Bad address" << endl;
		break;
	case INVALID_COMMAND:
		cout << "bad command" << endl;
		break;
	case EMPTY_BUFFER:
		cout << "File empty . Must use one of I,A,Q,V commands" << endl;
		break;
	}
	return notQuit;
}




/**************************************************
 
 Description:
 Append text into the buffer after line x, the
 current address is set to the last line entered
 
 Parameters:
 x: line address 1
 
 **************************************************/
void LineEd::appendAfter() {
	cout << "Entering input mode ." << endl;
    string line;
	getline(cin, line);
    while (getline(cin, line)) {
		if (line == ".")break;
        buffer.push_back(line);
		currentLine= buffer.size();
    }
    cin.clear();
	cout << "Entering command mode ." << endl;
}




/**************************************************
 
 Description:
 Insert text into the before after line x, the
 current address is set to the last line entered
 
 Parameters:
 x: line address 1
 
 **************************************************/
void LineEd::insertBefore(int x) {
	cout << "Entering input mode ." << endl;
    list<string>::iterator it = buffer.begin();
    advance(it, x-1);
    currentLine = x-1;
    string line;
    while (getline(cin, line)) {
		if (line == ".")break;
        buffer.insert(it, line);
        ++currentLine;
    }
    cin.clear();
	cout << "Entering command mode ." << endl;
}




/**************************************************
 
 Description:
 Paste text from the clipboard into the buffer
 below line x. The current address is set to the
 last line entered
 
 Parameters:
 x: line address 1
 
 **************************************************/
void LineEd::pasteBelow(int x) {
    if (0 == clipboard.size()) return;
    list<string>::iterator it_b = buffer.begin();
    advance(it_b, x-1);
    currentLine = (0 == buffer.size()) ? x-1 : x;
    string line;
    for (vector<string>::iterator it_v = clipboard.begin(); it_v != clipboard.end(); ++it_v){
        line = *it_v;
        buffer.insert(it_b, line);
        ++currentLine;
    }
}





/**************************************************
 
 Description:
 Delete the line range x through y from the buffer.
 If there is a line after the delete line range,
 then the current address is set to that line.
 Otherwise, the current address is set to the line
 before the deleted line range.
 
 Parameters:
 x: line address 1
 y: line address 2
 
 **************************************************/
void LineEd::deletes(int x, int y) {
    currentLine = (y < buffer.size()) ? x : x-1;
    if (0 == currentLine) ++currentLine;
    list<string>::iterator itr1 = buffer.begin();
	list<string>::iterator itr2 = buffer.begin();
    advance(itr1, x-1);
	advance(itr2, y);
        buffer.erase(itr1,itr2);
}




/**************************************************
 
 Description:
 Cut the line range x through y from the buffer
 into the clipboard. If there is a line after the
 cut line range, then the current address is set to
 that line. Otherwise the current address is set to
 the line beore the cut line range
 
 Parameters:
 x: line address 1
 y: line address 2
 
 **************************************************/
void LineEd::cut(int x, int y) {
    clipboard.clear();
    list<string>::iterator it = buffer.begin();
    advance(it, x-1);
    for (int i = x; i <= y; ++i) {
        clipboard.push_back(*(it++));
    }
    deletes(x, y);
}



/**************************************************
 
 Description:
 Join the line range x through y together on one
 line at address x, sugh that each line in turn is
 appended to line x, separated by a single space.
 Line x becomes the current line
 
 Parameters:
 x: line address 1
 y: line address 2
 
 **************************************************/
void LineEd::join(int x, int y) {
    list<string>::iterator it = buffer.begin();
    advance(it, x-1);
	string temp;
    for (int i = x-1; i <y; ++i) {
		temp += *it;
		++it;
    }
	deletes(x,y);
	it = buffer.begin();
	advance(it, x-1);
	currentLine = x;
	buffer.insert(it, temp);


}




/**************************************************
 
 Description:
 Print the line range x through y without affecting
 the current line address
 
 Parameters:
 x: line address 1
 y: line address 2
 
 **************************************************/
void LineEd::print(int x, int y) {
    list<string>::iterator it = buffer.begin();
    advance(it, x-1);
    for (int i = x; i <= y; ++i) {
        if (i == currentLine) {
            cout << i << "> " << *it << endl;
        } else {
            cout << i << ": " << *it << endl;
        }
        ++it;
    }
}




/**************************************************
 
 Description:
 Prompts for and reads the text to be changed, and
 then promps for and reads the replacement text.
 Searches each line in the line range for an
 occurence of the specified string and changes all
 matched strings to the replacement text.
 
 Parameters:
 x: line address 1
 y: line address 2
 
 **************************************************/
void LineEd::change(int x, int y) {
    string changeFrom, changeTo;
    int occurence = 0;
    size_t found;
	getline(cin, changeFrom);
    cout << "Change? ";
    getline(cin, changeFrom);
    cout << "    To? ";
    getline(cin, changeTo);
    list<string>::iterator it = buffer.begin();
    advance(it, x-1);
	list<string>::iterator initIt = it;

    for (int i = x; i <= y; ++i) {
        found = 0;
		bool lineIncrementFlag = false;
        while (true) {
            found = (*it).find(changeFrom, found);
            if (string::npos == found) break;
			lineIncrementFlag = true;
            (*it).replace(found, changeFrom.length(), changeTo);
            found += changeTo.length();
            ++occurence;
        }

		if (lineIncrementFlag) {
			currentLine = x + (std::distance(initIt, it));
		}
        ++it;
    }
    cout << "Changed " << occurence << " occurence(s)" << endl;
}




/**************************************************
 
 Description:
 Move the current line up by 1 lines provided that
 there are x lines above the current line; otherwise,
 prints the message top of file reached and sets the
 current line to last line in the buffer. If omitter,
 x = 1.
 
 Parameters:
 x: 1
 
 **************************************************/
void LineEd::moveUp(int x) {
    for (int i = 0; i < x; ++i) {
        --currentLine;
        if (currentLine < 1) {
            cout << "BOF reached" << endl;
            currentLine = 1;
            break;
        }
    }
	if (currentLine != 0)
	{
		list<string>::iterator it = buffer.begin();
		for (int i = 1; i < currentLine; ++i) {
			++it;
		}
		cout << *it << endl;
	}
}




/**************************************************
 
 Description:
 Move the current line down by x lines provided that
 there are x lines above the current line; otherwise,
 prints the message end of file reached and sets the
 current line to last line in the buffer. If omitter,
 x = 1.
 
 Parameters:
 x: 1
 
 **************************************************/
void LineEd::moveDown(int x) {
    for (int i = 0; i < x; ++i) {
        ++currentLine;
        if (currentLine > buffer.size()) {
            cout << "EOF reached" << endl;
            currentLine = buffer.size();
            break;
        }
    }
	if (currentLine!=0)
	{	
	list<string>::iterator it = buffer.begin();
	for (int i = 1; i < currentLine; ++i) {
		++it;
	}
	cout << *it << endl;
	}
}






/**************************************************

 Description:
 Write out entire buffer to its associated file. If
 the buffer is not associated with a user named file,
 it prompts for and reads the name of the associate
 file.

 **************************************************/
void LineEd::write() {

	cout << "Save changes to the file "+ filename +" (y or n)?";
	string answer;
	cin >> answer;
	if (filename.empty() || "n"==answer) {
		cout << "Enter the name of a file to write to: ";
		cin >> filename;
		cin.ignore();
	}
	ofstream outFile;
	outFile.open(filename.c_str());
	for (list<string>::iterator it = buffer.begin(); it != buffer.end(); ++it) {
		outFile << *it << endl;
	}
	outFile.close();
	string unit = (buffer.size() > 1) ? " lines" : " line";
	cout << buffer.size() << unit + " written to file: \"" << filename << "\"" << endl;
}




/**************************************************

 Description:
 Quit led. Before quitting, however, it gives the
 user a last change to save the buffer. If the user
 takes the change, it simulates the w command. Set
 notQUit to false

 Parameters:
 notQuit: determine if quit command is received or
		  not.

 **************************************************/
void LineEd::quit(bool& notQuit) {
	notQuit = false;
	if (true == saved) {
		cout << "No changes after the last 'w' command issued" << endl;
		cout << "Bye" << endl;
	}
	else {
		cout << "Do you wish to save the changes (y or n)? ";
		string answer;
		while (cin >> answer) {
			if ("y" == answer) {
				write();
				cout << "Bye" << endl;
				break;
			}
			else if ("n" == answer) {
				cin.ignore();
				break;
			}
			else {
				cout << "Bad answer: " << answer << endl;
				cout << "Enter y for yes and n for no" << endl;
				cout << "Save changes to a file (y/n)? ";
			}
		}
	}
}
