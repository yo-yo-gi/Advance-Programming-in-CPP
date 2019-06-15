/*
 * Line.cpp
 *
 *  Created on: May 24, 2019
 *      Author: Yogesh Nimbhorkar
 */

#include "Line.h"

using namespace std;

/** Constructs this line, assigning linePtr a pointer
 * to a deep copy of the supplied C-string text
 *
 * @param text pointer to char
 */
Line::Line(const char* text) {
	lineLength = strlen(text);					//calculate length
	capacity = lineLength + 1;					//assign capacity
	linePtr = new char[lineLength + 1];			//assign linePtr
	strcpy(linePtr, text);						//copy text
}

/** Copy Constructor assigning recieved line to linePtr
 *
 * @param line reference to Line object
 */
Line::Line(const Line& line) {
	lineLength = line.lineLength;				//calculate length
	capacity = lineLength + 1;					//assign capacity
	linePtr = new char[lineLength + 1];			//assign linePtr
	strcpy(linePtr, line.linePtr);				//copy text
}

/** Overloading assignment operator '=' to copy line object
 *
 * @param rhs object of type line to be assigned
 * @return *this pointer to this object
 */
const Line & Line::operator=(const Line & rhs) {
	if (this == &rhs)
		return *this;
	else {
		delete[] linePtr;						//deallocate linePtr
		lineLength = rhs.lineLength;			//deallocate linePtr
		capacity = lineLength + 1;
		linePtr = new char[lineLength + 1];
		strcpy(linePtr, rhs.linePtr);
	}
	return *this;
}

/** Destructor to release dynamic memory created and owned by this line
 *
 */
Line::~Line() {
	lineLength = 0;			//assign length as zero
	capacity = lineLength;	//assign capacity as zero
	delete[] linePtr;		//deallocating linePtr
	linePtr = nullptr;
}

/** Returns C-style version of this line
 *
 * @return linePtr pointer to the first character
 */
const char * Line::cstr() const {
	return linePtr;
}

/** Returns length of this line
 *
 * @return lineLength length of line
 */
int Line::length() const {
	return lineLength;
}

/** Returns whether this line is empty
 *
 * @return true if line length is zero else false
 */
bool Line::empty() const {
	return !(lineLength > 0);
}

/** Returns whether this line is full
 *
 * @return true if linelength is greater than or equal to capacity else false
 */
bool Line::full() const {
	return (lineLength >= capacity);
}

/** Returns capacity of this line
 *
 * @return capacity capacity of the this line
 */
int Line::getCapacity() const {
	return capacity;
}

/** Doubles capacity if this line is full  */
void Line::resize() {
	int updatedCapacity = getCapacity() * 2;				//doubling capacity
	char * charArray = new char[updatedCapacity + 1];		//creating new array with updated capacity
	strcpy(charArray, linePtr);								//copying old text to new arry
	capacity = updatedCapacity;
	delete[] linePtr;										//deleting and deallocating old linePtr
	linePtr = charArray;
}

/** Appends ch to the end of this line
 *
 * @param ch character type to be appended
 */
void Line::push_back(const char & ch) {
	if (full())
		resize();
	linePtr[lineLength] = ch;				//appending new character at the end
	linePtr[lineLength + 1] = '\0';			//adding null character  at the end
	lineLength++;							//increading length
}

/** Removes the last character in this line
 *
 */
void Line::pop_back() {
	if (!empty()) {
		linePtr[lineLength - 1] = '\0';		//putting new character at the end
		lineLength--;						//decreasing length
	}
}

/** Overloads operator << as a friend function
 *
 * @param out reference to type ostream
 * @param line object of type Line
 * @return out reference to type ostream
 */
std::ostream & operator<<(ostream & out, const Line & line) {
	if (line.lineLength > 0)
		out << line.linePtr;
	return out;
}

/** Overloads operator >> as a friend function
 *
 * @param in reference to type istream
 * @param line object of type Line *
 * @return in reference to istream
 */
std::istream & operator>>(istream & in, Line & line) {
	char c { };
	while (in.get(c)) {
		if (line.full())							//chekcing if full
			line.resize();							//if full then resize
		line.push_back(c);
	}
	return in;
}

