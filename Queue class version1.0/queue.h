//queue.h--class definition for the queue ADT
#ifndef QUEUE_H_
#define QUEUE_H_
typedef unsigned long Item;
class Queue
{
    private:
        //class scope definitions
        enum {SIZE=10};
        //Node is a nested structure defintion local to class
        struct Node
        {
            Item item;//data stored in the node
            struct Node * next;//pointer to next node
        };
        //private class member
        Node * front;//pointer to front of queue
        Node * rear;//pointer to rear of queue
        int items;//current number of queue
        const int qsize;//maximum number of items in queue
        Queue(const Queue & q):qsize(0){}//preemptive definition
        Queue & operator=(const Queue & q){return *this;}
    public:
        Queue(int qs=SIZE);//create queue with a limit
        ~Queue();
        bool isempty() const;
        bool isfull() const;
        int queuecount() const;
        bool enqueue(const Item & item);//add item to end
        bool dequeue(Item & item);//remove item from front
};
#endif
//This queue contain Customer items
/*class Customer
{
    private:
        long arrive;//arrival time for customer
        int processtime;//processing time for customer
    public:
        Customer(){arrive=processtime=0;}
        void set(long when);
        long when() const {return arrive;}
        int ptime() const {return processtime;}
};
typedef Customer Item;*/
