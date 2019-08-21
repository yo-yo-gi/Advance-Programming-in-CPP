#ifndef COMMAND_H
#define COMMAND_H

#include <string>

using std::string;

// enum for command type
enum Cmd { APPEND_AFTER, INSERT_BEFORE, PASTE_BELOW, DELETE, CUT, JOIN, PRINT, CHANGE, MOVE_UP, MOVE_DOWN, GO, WRITE, QUIT, INVALID_RANGE, INVALID_COMMAND, EMPTY_BUFFER };

class Command {
  private:
    string command; // command issued by user
    Cmd cmd; // command type
	int x{ 1 }, y{1}; // two line addresses
  public:
    Command(string);
    ~Command();
    void parse(int, int);
    void trim(string, string);
    bool checkInt(string) const;
    Cmd getCommand() const;
    int getX() const;
    int getY() const;
	void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr);
	bool checkIfInvalid(string command);
	bool startsWithComma(char c1);
	bool startsWithDigit(string str);
	bool startsWithValidCommand(string str);
	bool perfectlyValidCommand(string str);
};

#endif
