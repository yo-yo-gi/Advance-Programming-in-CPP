/*
 * LineList.h
 *
 *  Created on: May 24, 2019
 *      Author: Yogesh Nimbhorkar
 */

#include "LineList.h"
#include <cstring>
#include <iostream>

using namespace std;

/** Default constructor */
LineList::LineList() {
	Node * dHead = new Node { Line { "" } };		//new dummy head
	head = dHead;

	// create dummy tail node
	Node * dTail = new Node { Line { "" } };		//new dummy tail
	tail = dTail;

	// connect head to tail and tail to head
	dHead->setNext(dHead);
	dTail->setPrev(dHead);
}

/** Destructor */
LineList::~LineList() {
	removeAll();
}

/** Copy constructor
 *
 * @param rhs reference to type LineList
 */
LineList::LineList(const LineList & rhs) {
	deepCopy(rhs);
}

/** Copy assignment
 *
 * @param rhs reference to type LineList
 * @return *this current object of LineList
 */
const LineList & LineList::operator=(const LineList & rhs) {
	if (this != &rhs) {
		removeAll();					//deleting given list
		deepCopy(rhs);					//deep copy
	}
	return *this;
}

/** Inserts line at the front of the this list
 *
 * @param line reference to type Line
 */
void LineList::push_front(const Line & line) {
	Node * newNode = new Node { line, head, head->getNext() };			//calling constructor

	head->getNext()->setPrev(newNode);

	head->setNext(newNode);												//setting new node after head

	++theSize;															//addup size
}

/** Inserts line at the end of the this list
 *
 * @param line reference to type Line
 */
void LineList::push_back(const Line & line) {
	Node* newNode = new Node { line, tail->getPrev(), tail };

	tail->getPrev()->setNext(newNode);								//setting new node before tail

	tail->setPrev(newNode);

	++theSize;															//addup size
}

/** Remove the first node in this list */
void LineList::pop_front() {
	Node * temp = head->getNext();

	if (temp != tail) {
		temp->getNext()->setPrev(head);

		head->setNext(temp->getNext());								//pointing to new node

		--theSize;													//reduce size

		delete temp;												//releasing poped node
		temp = nullptr;
	}
}

/** Remove the last node in this list */
void LineList::pop_back() {
	// create temp pointer to popping node to deallocate space later
	Node * temp = tail->getPrev();

	if (temp != head) {
		temp->getPrev()->setNext(tail);								//pointing to new node

		tail->setPrev(temp->getPrev());

		--theSize;													//reduce size

		delete temp;												//releasing poped node
		temp = nullptr;
	}
}

/** Returns the size of this list
 *
 * @return theSize size of this LineList
 */
int LineList::size() const {
	return theSize;
}

/** Returns whether this list is empty
 *
 * @return out reference to type ostream
 */
bool LineList::empty() const {
	return (head->getNext() == tail) && (tail->getPrev() == head)
			&& (theSize == 0);											//Checking for size elementing haed and tail
}

/** Inserts a new line at position k in this list
 *
 * @param line object of type Line
 * @param k index to put new line
 */
void LineList::insert(const Line & line, int k) {
	if (validIndex(k)) {
		if (k == 1) {
			push_front(line);
			return;
		}

		Node * pre = find(k - 1);

		Node * pos = find(k);

		// create new node pointing to the prev and next to be nodes
		Node * newNode = new Node { line, pre, pos };

		pos->setPrev(newNode);

		pre->setNext(newNode);
		++theSize;
	} else {
		cout << "Its not valid index." << endl;
	}
}

/** Removes node at position k in this list
 *
 * @param k position to be removed
 */
void LineList::remove(int k) {
	if (validIndex(k)) {

		Node * kthNode = find(k);									// get pointer to the k node

		kthNode->getPrev()->setNext(kthNode->getNext());			// update pointers to skip k node
		kthNode->getNext()->setPrev(kthNode->getPrev());

		--theSize;
		delete kthNode;
		kthNode = nullptr;
	} else {
		cout << "Its not valid index." << endl;
	}
}

/** Overloads operator << as a friend function */
void LineList::print() const {
	Node * currentNode = head;
	for (int n = 0; n < theSize; ++n) {
		currentNode = currentNode->getNext();
		cout << currentNode->getData() << endl;
	}
}

/** Returns the node at a given position in the list.
 *
 * @ param i The position of the node.
 * @return current->getData() node at position k, if exists; otherwise, null.
 */
Line LineList::get(int i) const {
	if (i < 1 || i > size()) {
		return NULL; // notifies bad index
	} else {
		Node * current = head;
		for (int n = 0; n < i; ++n) {
			current = current->getNext();
		}
		return current->getData();
	}
}

/**  Deep copies the supplied LineList
 *
 * @param original reference to type LineLists
 */
void LineList::deepCopy(const LineList & original) {
	head = new Node { Line { "" } };

	tail = new Node { Line { "" }, head };									// tail points to head

	head->setNext(tail);													// head points to tail
	theSize = original.size();

	if (original.empty())
		return; //
	Node * current = original.head->getNext();
	while (current != original.tail) {
		push_back(current->getData());
		current = current->getNext();
	}
}

/** Removes all nodes without keeping DUMMY HEAD and TAIL */
void LineList::removeAll() {
	while (!empty()) {
		pop_front();
	}
	delete head;
	head = nullptr;
	delete tail;
	tail = nullptr;
}

/** Check whether an element exists at a given index before attempting to access that element.
 * @param i the index to be checked.
 * @return true if index i is valid, false otherwise.
 */
bool LineList::validIndex(int i) const {
	return !(i < 1 || i > size());
}

/** Returns a pointer to the node at a given position in the list.
 *
 * @param index position of the node.
 * @return current pointer to the node at position k, if exists; otherwise, nullptr.
 */
LineList::Node * LineList::find(int i) const {
	if (i < 1 || i > size()) {
		return nullptr;
	} else {
		Node * current = head;
		for (int n = 0; n < i; ++n) {
			current = current->getNext();
		}
		return current;
	}
}
