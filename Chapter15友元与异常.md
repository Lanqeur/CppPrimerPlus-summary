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