/*
 * Line.h
 *
 *  Created on: May 24, 2019
 *      Author: Yogesh Nimbhorkar
 */

#include <cstring>
#include <iostream>

#ifndef LINE_H_
#define LINE_H_

class Line {

private:
//	Stores a pointer to the first character
	char *linePtr { nullptr };
//	Length of this line
	int lineLength { 0 };
//	Storage capacity of this line
	int capacity { 0 };

public:
//	Constructor
	Line(const char *);
//	Constructor
	Line(const Line &);
//	Overloading operator '='
	const Line & operator =(const Line &);
//	Destructor
	virtual ~Line();
// Return C-Style version of line
	const char * cstr() const;
//	Returns length of line
	int length() const;
//	Returns whether this line is empty
	bool empty() const;
//	Teturns wheather this line is full
	bool full() const;
//	Returns capacity of this line
	int getCapacity() const;
//	Doubles capacity if this line is full
	void resize();
//	Appends ch to the end of this line
	void push_back(const char &);
//	Removes the last character in this line
	void pop_back();
//	Overloads operator << as a friend
	friend std::ostream & operator<<(std::ostream &, const Line &);
//	Overloads operator>> as a friend
	friend std::istream & operator>>(std::istream &, Line &);
//	Overloads operator == as a friend
	friend bool operator ==(const Line & line1, const Line & line2) {
		int result = strcmp(line1.linePtr, line2.linePtr);
		if (result == 0)
			return true;
		else
			return false;
	}
//	Overloads operator != as a friend
	friend bool operator !=(const Line line1, const Line line2) {
		return !(line1 == line2);
	}
};

#endif /* LINE_H_ */
