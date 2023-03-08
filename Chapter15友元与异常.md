# 第十五章 友元、异常和其它
本章先介绍一些C++最初就有的特性，包括友元类，友元成员函数和嵌套类，它们是在其它类中声明的类；然后介绍C++语言新增的一些特性，包括异常运行阶段类型识别(RTTI)和改进后的类型转换控制。
## 友元
类并非只能拥有友元函数，也可以将类作为友元。在这种情况下，友元类的所有方法都可以访问原始类的私有成员和公有成员。
另外也可以做更严格的限制，只将特定的成员函数指定为另一个类的友元。哪些函数、成员函数或类为友元是由类定义的，不能从外部改变，它们并不违反面向对象的编程思想，反而提高了公有接口的灵活性。
### 友元类
设想一个类成为另一个类的友元的情形：有一个Tv类和一个Remote类分别表示电视机和遥控器，它们不是is-a关系或者has-a关系，实际上，遥控器能够改变电视机的状态，这表明应将Remote类作为Tv类的一个友元。类友元用来表示一些关系，如一个遥控器可用于多台电视机。
下面是定义部分：
```C++ 
class Tv
{
    public:
    friend class Remote;//Remote can access Tv private parts
    enum{Off,On};
    enum{MniVal,MaxVal=20};
    enum{Antenna,Cable};
    enum{TV,DVD};
    Tv(int s=Off,int mc=125):state(s),volume(5),maxchannel(mc),channel(2),mode(Cable),input(TV){}
    void onoff{state=(state==On)?Off:On;}
    bool ison() const {return state==On;}
    bool volup();
    bool voldown();
    void chanup();
    void chandown();
    void set_mode(){mode=(mode==Antenna)?Cable:Antenna;}
    void set_input(){input=(input==TV)?DVD:TV;}
    void settings() const;//display all settings
    private:
    int state;//on or off
    int volume;//assumed to be digitized
    int maxchannel;//maximum number of channels
    int channel;//current channel setting
    int mode;//broadcast or cable
    int input;//TV or DVD
};
class Remote
{
    private:
    int mode;//controls TV or DVD
    public:
    Remote(int m=Tv::TV):mode(m){}
    bool volup(Tv & t){return t.volup();}
    bool voldown(Tv & t){return t.voldown();}
    void onoff(Tv & t){t.onoff();}
    void chanup(Tv & t){t.chanup();}
    void chandown(Tv & t){t.chandown();}
    void set_chan(Tv & t,int c){t.channel=c;}
    void set_mode(Tv & t){t.set_mode;}
    void set_input(Tv & t){t.set_input();}
};
```
* 在程序中，用一组状态成员表示Tv类，包括开/关，频道设置，音量设置，有线电视或天线调节模式，TV调谐或A/V输入；
* 定义中包括一些被定义为枚举的常数；
* ```friend class Remote```语句使Remote成为友元类；
* 友元声明可以位于公有、私有或保护部分，其位置不重要；
* 由于Remote类提到了Tv类，所以编译器必须了解Tv类后才能处理Remote类，因此最简单的方法是先定义Tv类。也可以使用前向声明(forward declaration).
* 所有的Remote方法都将一个Tv对象引用作为参数，这表明遥控器必须针对特定的电视机；
* 很多方法使用了条件运算符？：在两种状态之间切换，如果两种状态分别为true(1)和false(0)，则可以使用按位运算符来简化代码：
```C++
void onoff(){state^=1;}
```
在单个char变量中可以存储多达8个双状态设置，可以使用按位运算符的切换位方法分别对它们进行切换。
#### 按位运算符
按位运算符有6个，分别是<<,>>,~,&,|,^.
* 左移运算符
```value<<shift```
value是被操作的整数值，shift是要移动的位数。例如：
```13<<3```
上面的代码将13的所有位都向左移3位，腾出的位置用0填充，超出边界的位被舍弃；左移x位相当于乘以2^x，因此移位后的值为```13*2^3=104```.
左移运算符不会修改变量的值，如果要修改必须使用赋值运算符，如x<<=2;
* 右移运算符
右移运算符语法与左移相同，右移后，对于无符号整数，腾出的位置用0填充，超出边界的位置被删除；对于有符号整数，腾出的位置可能用0填充，也可能用原来最左边的位填充，取决于C++实现。
向右移动x位相当于除以2^x。
* 逻辑按位运算符
逻辑按位运算符用于值的每一位，而不是整个值。
* 位非(求反)运算符~
~运算符将每一位转换为它的反面，新值是原值的补值。
* 按位运算符OR(|)
|对两个整数值进行操作，生成一个新的整数值。它表示逻辑“或”的关系，只有相应位至少有1个1，则新值的相应位才为1，否则为0；
* 按位运算符XOR(^)
^表示异或的关系，如果相应位有1个1，则新值的相应位为1；如果相应位都为0或1，则新值的相应位为0；
* 按位运算符AND(&)
&表示逻辑“与”的关系，只有相应位都为1，新值的相应位才为1，否则为0；
* 按位运算符技术
lottabits表示一个值，bit表示特定位的值，位从右向左编号，从0开始。由于各个位都对应2的幂，因此我们用术语位bit表示2的幂，这对应于特定位为1，其它所有位为0的情况。
* 打开位
 ```lottabits|=bit；```
它将对应的位设为1，而不管这一位以前的值是多少。因为在逻辑“或”关系里，bit的其它位都为0，所以不论lottabits的其它位的值为多少，新值的对应位的值都不变；而bit对应的位为1，所以新值中对应位的值一定为1；
* 切换位
 ```lottabits^=bit;```
逻辑“异或”关系同理，bit的其它位为0，新值的对应位值不变；而bit对应的位是1，lottabits对应位如果是1，新值就为0，如果为0，新值就为1，即切换该位的值。
* 关闭位
 ```lottabits&=~bit;```
首先求反bit，其它所有位为1，在逻辑“与”关系下，新值的对应位不变；而bit对应的位为0，所以新值的对应位一定是0，即关闭位。
* 测试位的值
 ```if(lottabits&bit)```
因为bit中只有对应位为1，其它位都为0，所以在逻辑“与”关系里新值的其它位一定为0，当lottabits对应位为1时，新值的对应位才为1(测试结果为true),否则新值的所有位都是0，测试结果为false。
### 友元成员函数
大多数Remote方法都是用Tv类的公有接口实现的，这意味着这些方法不是真正需要作为友元。唯一直接访问Tv成员的Remote方法是Remote::set_chan()，因此它是唯一需要作为友元的方法。
虽然可以选择仅让特定的类成员成为另一个类的友元：
```C++
class Tv
{
    friend void Remote::set_chan(Tv & t,int c);
    ...
};
```
但是对于编译器来说，必须要知道Remote的定义才能处理这条语句，否则它并不知道Remote是一个类，set_chan()是这个类的方法；
但如果将Remote的定义放在Tv的定义前面，Remote的方法又提到了Tv对象；
避开这种循环依赖的方法是使用前向声明(forward declaration)：
```C++
class Tv;//forward declaration
class Remote {...};
class Tv{...};
//put Remote method definitions here
```
不能调换Remote和Tv类的声明顺序，因为Remote的一个方法被声明为Tv类的友元，编译器必须首先看到Remote类的声明和set_chan()的声明；
对于之前的Remote声明里包含的内联代码，由于调用了Tv类的方法，所以编译器必须首先看到Tv类的声明，解决方法是使Remote声明中只包含方法声明，并将实际定义放在Tv类之后,使用关键字inline使其成为内联方法。
### 其它友元关系
想象一种交互式遥控器，一些Remote方法可以影响Tv对象，而一些Tv方法也可以影响Remote对象，这可以通过让类互为对方的友元来实现。需要注意的是，对于使用友元类对象的方法，其原型可以在友元类之前声明，但是必须在友元类之后定义，以便编译器有足够的信息编译该方法：
```C++
class Tv
{
    friend class Remote;
    ...
};
class Remote
{
    friend class Tv;
    ...
};
//put Tv method definitions here
```
 需要使用的友元的另一种情况是，函数需要访问两个类的私有数据，例如对两个类的内部时钟进行同步。将函数作为两个类的友元，而不是一个类的成员，另一个类的友元。
 ### 嵌套类
 在另一个类中声明的类被称为嵌套类(nested class)，包含类的成员函数可以创建和使用被嵌套类的对象；除非嵌套类声明位于公有部分，才能在包含类外面使用嵌套类，而且必须使用作用域解析符。
 对类进行嵌套不是包含关系，包含是将类对象作为另一个类的成员，而嵌套类不创建类成员，只是定义了一种在包含嵌套类声明的类中才有效的类型。
 对类进行嵌套通常是为了帮助实现另一个类，并避免名称冲突。
 如第十二章里的Queue类示例，嵌套了结构定义。
 现在将Node结构更改为有构造函数的Node类：
 ```C++
 class Queue
 {
    //class scope definitions
    //Node is a nested definition local to this class
    class Node
    {
        public:
        Item item;
        Node * next;
        Node(const Item & i):item(i),next(0){}
    };
    ...
 };
 ```
 该构造函数将节点的item成员初始化为i，并将next指针设置为0即空指针。接下来使用构造函数重新编写enqueue():
 ```C++
 bool Queue::enqueue(const Item & item)
 {
    if(isfull())
    return false;
    Node * add=new Node(item);//create,innitialize node
    //on failure,new throws std::bad_alloc exception
    ...
 }
 ```
 这使得enqueue()的代码更短更安全，因为它自动进行初始化。
 这个例子在类声明中定义了构造函数，假设在方法文件中定义构造函数，则定义必须使用作用域解析符：
 ```C++
 Queue::Node::Node(const Item & i):item(i),next(0){}
 ```
 #### 嵌套类和访问权限
 嵌套类的声明位置决定了嵌套类的作用域，即决定了程序的哪些部分可以创建这种类的对象；其次嵌套类的公有、保护和私有部分控制了对类成员的访问。
 1. 作用域
 如果嵌套类在类的私有部分声明，则只有该类知道它，比如被嵌套在Queue声明中的Node类。对于从Queue派生而来的类，Node也是不可见的，因为派生类不能直接访问基类的私有部分；
 如果嵌套类在类的保护部分声明，这种情况下派生类将知道嵌套类，并可以直接创建这种类型的对象；
 如果嵌套类在类的公有部分声明，则允许包含它的类、派生类以及类外使用它。在类外使用时，必须使用类限定符。
 2. 访问控制
 类可见后，起决定作用的是访问控制。在Queue示例中，Node类的所有成员都被声明为公有的，因为Node类是在Queue类的私有部分声明的，只有Queue的方法可以直接访问Node的成员。
 #### 模板中的嵌套
 可以将Queue类定义转换为模板，包含嵌套类不会带来额外的问题：
 ```C++
 //queuetp.h -- queue template with s nested class
 #ifndef QUEUETP_H_
 #define QUEUETP_H_
 template <typename Item>
 class QueueTP
 {
    private:
    enum{Q_SIZE=10};
    //Node is a nested class definition
    class Node
    {
        public:
        Item item;
        Node * next;
        Node(const Item & i):item(i),next(0){}
    };
    Node * front;
    Node * rear;
    int items;//current number of items in Queue
    const int qsize;//maximum number of items in Queue
    ...
 };
 //QueueTP methods
 template<typename Item>
 bool QueueTP::enqueue(const Item & item)
 {
    ...
 }
 ...
 #endif
 ```
 ## 异常
 程序有时会遇到运行阶段错误，导致程序终止，例如程序可能试图打开一个不可用的文件，请求过多的内存，或者遭遇不能容忍的值，一般由程序员来预防这种意外情况。
 以计算两个数的调和平均数为例，两个数的调和平均数是指这两个数倒数的平均值的倒数，表达式为：
 ```2.0*x*y/(x+y)```
 如果x+y为0，则会导致被零除的情况，不同编译器会有不同的反应，最好编写在所有系统上都受控的代码：
 ### 调用abort()
 abort()函数原型位于头文件cstdlib中，它的典型实现是向标准错误流发送消息abnormal program termination(程序异常终止)，然后终止程序，它还返回一个随实现而异的值，告诉操作系统或父进程，处理失败。abort()是否刷新文件缓冲区取决于实现，也可以使用exit()，该函数刷新文件缓冲区但不显示消息。
 #### 返回错误码
 一种比异常终止更灵活的方法是使用函数返回值来指出问题，例如将函数的返回值定义为bool，让返回值指出成功还是失败了。
 ### 异常机制
 现在介绍如何使用异常机制来处理错误，C++异常是对程序运行过程中发生异常情况的一种响应，异常提供了将控制权从程序的一个部分传递到另一部分的途径。对异常的处理有3个组成部分：
 * 引发异常(throw)；
 * 使用处理程序捕获异常(catch)；
 * 使用try块；
 可以不调用abort()函数，而是引发异常来触发异常机制：
 ```C++
 #include <iostream>
 double hmean(double a,double b);
 int main()
 {

 }
 double hmean(double a,double b)
 {
    if(a==-b)
    throw "bad hmean() arguments:a=-b not allowed";
    return 2.0*a*b/(a+b);
 }
 ```
 * throw关键字表示引发异常，紧随其后的值指出了异常的特征，异常类型可以是字符串或其它C++类型，通常为类类型，后面的示例会介绍；执行throw语句类似于执行返回语句，因为它也将终止函数的执行，但throw不是将控制权返回给调用程序，而是导致程序沿函数调用序列后退，直至找到包含try块的函数，在这个例子里throw将控制权返回给main()，程序将在main()里寻找与引发的异常类型匹配的异常处理程序(位于try块后面)；
 * 程序使用异常处理程序(exception handler)来捕获异常，catch关键字表示捕获异常，处理程序以关键字catch开头，随后是位于括号中的类型声明，它指出了异常处理程序要响应的异常类型，然后是一个花括号括起的代码块，指出要采取的措施；catch关键字和异常类型用作标签，指出当异常被引发时，程序应跳转到这个位置执行，异常处理程序页被称为catch块；
 * try标识其中特定的异常可能被激活的代码块，它后面跟一个或多个catch块，try块由关键字try指示，关键字try后面是一个代码块，表明需要注意这些代码引发的异常；如果其中的某条语句导致异常被引发，则后面的catch块将对异常进行处理，如果异常在try块外引发，将无法处理异常；
 * 执行完try块中的语句后，如果没有引发任何异常，则程序跳过try块后面的catch块，直接执行处理程序后面的第一条语句；
