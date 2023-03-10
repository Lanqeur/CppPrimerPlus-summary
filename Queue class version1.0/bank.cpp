//bank.cpp--use the Queue interface
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include "queue.h"
int main()
{
    using namespace std;
    Queue st;//create an empty queue
    char ch;
    unsigned long id;
    cout<<"Please enter A to add a member to the rear,\n"
    <<"R to remove a member from the front, or Q to quit.\n";
    while(cin>>ch&&toupper(ch)!='Q')
    {
        while(cin.get()!='\n')
        continue;
        if(!isalpha(ch))
        {
            cout<<'\a';
            continue;
        }
        switch(ch)
        {
            case 'A':
            case 'a':cout<<"Enter a ID number to add: ";
                     cin>>id;
                     if(st.isfull())
                     cout<<"queue already full\n";
                     else
                     st.enqueue(id);
                     break;
            case 'R':
            case 'r':if(st.isempty())
                    cout<<"queue already empty\n";
                    else
                    {
                        st.dequeue(id);
                        cout<<"ID #"<<id<<" removed\n";
                    }
                    break;
                    
        }
    cout<<"Please enter A to add a member to the rear,\n"
    <<"R to remove a member from the front, or Q to quit.\n";
    }
    cout<<"Bye\n";
    return 0;
}
/*const int MIN_PER_HR=60;

bool newcustomer(double x); //is there a new customer?

int main()
{
    using std::cin;
    using std::cout;
    using std::endl;
    using std::ios_base;
    std::srand(std::time(0));
    
    cout<<"Welcome to Lanqeur Bank!\n"<<"Enter maximum size of queue: ";
    int qs;
    cin>>qs;
    Queue line(qs);//line queue holds up to qs people
    
    cout<<"Enter the number of simulation hours: ";
    int hours;
    cin>>hours;
    //simulation will run 1 cycle per minute
    long cyclelimit=MIN_PER_HR*hours;
    
    cout<<"Enter the average number of customers per hour: ";
    double perhour;
    cin>>perhour;
    double min_per_cust=MIN_PER_HR/perhour;
    Item temp;//new customer data
    long turnaways=0;//turned away by full queue
    long customers=0;//join the queue
    long served=0;//served during the simulation
    long sum_line=0;//cumulative line length
    int wait_time=0;//time until autoteller is free
    long line_wait=0;//cumulative time in line
    
    //running the simulation
    for(int cycle=0;cycle<cyclelimit;cycle++)
    {
        if(newcustomer(min_per_cust))//have newcustomer
        {
            if(line.isfull())
            turnaways++;
            else
            {
                customers++;
                temp.set(cycle);//cycle=time of arrive
                line.enqueue(temp);//add newcomer to line
            }
            if(wait_time<=0&&!line.isempty())
            {
                line.dequeue(temp);//attend next customer
                wait_time=temp.ptime();//for wait_time minutes
                line_wait+=cycle-temp.when();
                served++;
            }
            if(wait_time>0);
            wait_time--;
            sum_line+=line.queuecount();
        }
        
        //reporting results
        if(customers>0)
        {
            cout<<"customers accepted: "<<customers<<endl;
            cout<<"customers served: "<<served<<endl;
            cout<<"turnaways: "<<turnaways<<endl;
            cout<<"average queue size: ";
            cout.precision(2);
            cout.setf(ios_base::fixed,ios_base::floatfield);
            cout<<(double) sum_line/cyclelimit<<endl;
            cout<<"average wait time: "
            <<(double) line_wait/served<<" minutes\n";
        }
        else
        cout<<"No customers!\n";
        return 0;
    }
}
//x=average time,int minutes,betwen customers
bool newcustomer(double x)
{
    return (std::rand()*x/RAND_MAX<1);
}*/

