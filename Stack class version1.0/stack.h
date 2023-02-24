//stack.h--class definition for the stack ADT
#ifndef STACK_H_
#define STACK_H_

typedef unsigned long Item;

class Stack
{
	enum {MAX=5};//constant specific to class
	Item items[MAX];//holds stack items
	int top;//index for top stack item
	public:
	    Stack();
	    bool isempty() const;
	    bool isfull() const;
	    //push() returns false if stack already is full,true otherwise
	    bool push(const Item & item);//add item to stack
	    //pop() returns true if stack already is full,false otherwise
	    bool pop(Item & item);//pop top into item
};
#endif
