# 第十四章 C++中的代码重用
C++的一个主要目标是促进代码重用，公有继承是实现这种目标的机制之一，但并不是唯一的机制。本章将介绍其它方法。
一种方法是使用是另一个类的对象作为类成员。这种方法称为包含(containment)、组合(composition)、或层次化(layering)。
另一种方法是使用私有或保护继承。
一般来说，包含、私有继承和保护继承用于实现has-a关系，即新的类包含另一个类的对象。
本章还会介绍类模板————另一种重用代码的方法，类模板使我们能够使用通用术语定义类，然后使用模板来创建针对特定类型定义的特殊类。
## 包含对象成员的类
### valarray类简介
valarray类是由头文件valarray支持的，这个类用于处理数值或具有类似特性的类。它支持像将数组中的所有元素相加以及在数组中找出最大和最小的值等操作。valarray被定义为一个模板类，以便能够处理不同的数据类型。
模板特性意味着声明对象时，必须指定具体的数据类型，因此，使用valarray类来声明一个对象时，需要在标识符valarray后面加上一对尖括号，并在其中包含所需的数据类型：
```C++
valarray<int> q_values;//an array of int
valarray<double> weights;//an array of double
double gpa[5]={3.1,3.5,3.8,2.9,3.3};
valarray<double> v1;//an array of double,size 0
valarray<int> v2(8);//an array of 8 int elements
valarray<int> v3(10,8);//an array of 8 int elements,each set to 10
valarray<double> v4(gpa,4);//an array of 4 double elements,intialized to the first 4 elements of gpa
```
从中可知，可以创建长度为零的空数组、指定长度的空数组、所有元素都被初始化为指定值的数组、用常规数组中的值进行初始化的数组。在C++11中，也可以使用初始化列表：
```valarray<int> v5={20,32,17,9}//C++11```
下面是这个类的一些方法：
* operator[]():让您能够访问各个元素
* size():返回包含的元素数
* sum():返回所有元素的总和
* max():返回最大的元素
* min():返回最小的元素
### 接口和实现
* 使用公有继承时，类可以继承接口，可能还有实现（基类的纯虚函数提供接口，但不提供实现）。获得接口是is-a关系的组成部分；
* 而使用包含，类可以获得实现，但不能获得接口，不继承接口是has-a关系的组成部分。
### C++和约束
C++包括让程序员能够限制程序结构的特性，使用explicit防止单参数构造函数的隐式转换，使用const限制方法修改数据等。这样做的根本原因是：在编译阶段出现错误优于在运行阶段出现错误。
### 初始化被包含的对象
对于继承的对象，构造函数在成员初始化列表中使用类名来调用特定的基类构造函数；对于成员对象，构造函数则使用成员名，例如：
 ```C++
Student(const char * str,const double * pd,int n):name(str),scores(pd,n){}
```
这里的name是string对象，scores是valarray对象，初始化列表每一项都调用与之匹配的构造函数。
如果不使用初始化列表语法，C++要求在构建对象的其它部分之前，先构建继承对象的所有成员对象，因此如果省略初始化列表，C++将使用成员对象所属类的默认构造函数。
#### 初始化顺序
**当初始化列表包含多个项目时，这些项目被初始化的顺序为它们被声明的顺序，而不是它们在初始化列表中的顺序。**
## 私有继承 
使用私有继承，基类的公有成员和保护成员都将成为派生类的私有成员，这意味着基类方法将不会成为派生对象公有接口的一部分，但可以在派生类的成员函数中使用它们。
**使用公有继承，基类的公有方法会成为派生类的公有方法，派生类将继承基类的接口，这是is-a关系的一部分；使用私有继承，基类的公有方法将成为派生类的私有方法，派生类不继承基类的接口，这种不完全继承是has-a关系的一部分。**
**包含将对象作为一个命名的成员对象添加到类中，而私有继承将对象作为一个未被命名的继承对象添加到类中。**
使用术语子对象(suboject)来表示通过继承或包含添加的对象。
私有继承与包含提供的特性相同：获得实现，但不获得接口。
### 使用私有继承
使用私有继承，用关键字private而不是public来定义类（private实际上是默认值，因此省略访问限定符也将导致私有继承）。
使用多个基类的继承被称为多重继承(multiple inheritence,MI)。
```C++
class Student::private std::string,private std::valarray<double>
{
    public:
    ...
};
```
上述代码是使用私有继承和多重继承的例子。
#### 初始化基类组件
隐式继承基类组件而不是成员对象，就只能使用用于公有继承的技术来描述基类对象了。
对于继承类，构造函数将使用成员初始化列表语法，它只能使用类名而不是成员对象名来标识构造函数：
```C++
Student(const char * str,const double * pd,int n):std::string(str),ArrayDb(pd,n){}//use class name for inheritance
```
#### 访问基类的方法
使用包含时将使用对象名来调用方法，而使用私有继承时将使用类名和作用域解析符来调用方法。
#### 访问基类对象
如果要使用基类对象本身，可以使用强制类型转换，将派生类强制类型转换为基类：
```C++
const string & Student::Name() const
{
    return (const string &)*this;
}
```
上述方法使用强制类型转换来创建一个引用，该引用指向用于调用该方法的Student对象中继承而来的string对象。
#### 访问基类的友元函数
因为友元不属于类，因此用类名显式的调用并不适合。可以通过显式转换为基类来调用正确的函数：
```C++
ostream & operator<<(ostream & os,const Student & stu)
{
    os<<"Scores for "<<(const string &)stu<<":\n";
    ...
}
```
在私有继承中，不进行显式类型转换的派生类指针或引用，不能赋值给基类指针或引用。
并且，如果两个基类都提供了重载运算符，由于Student类使用多重继承，编译器将无法确定转换为哪个基类。
### 使用包含还是私有继承
既可以使用包含，也可以使用私有继承来建立has-a关系。大多数C++程序员倾向于使用包含。
首先它易于理解，类声明中包含表示被包含类的显式命名对象，代码可以通过名称引用这些对象，而使用继承将使关系更抽象。
其次，继承会引起很多问题，尤其是从多个基类继承时，如包含同名方法的独立的基类或共享祖先的独立基类。
另外，包含能够包括多个同类的子对象，而继承则只能使用一个这样的未命名对象。
然而私有继承的特性比包含更多。例如，假设类包含保护成员(数据成员或成员函数)，则这样的成员在派生类中是可用的，在类外不可用。
另一种需要使用私有继承的情况是需要重新定义虚函数，派生类可以重新定义虚函数，但包含类不能。且使用私有继承重新定义的函数只能在类中使用，不是公有的。
## 保护继承
保护继承是私有继承的变体。保护继承在列出基类时使用关键字protected。
使用保护继承时，基类的公有成员和保护成员都将成为派生类的保护成员。和私有继承相同，基类的接口在派生类中可用，类外不可用。
当从派生类派生出另一个类时，私有继承与保护继承的主要区别会出现：
* 使用私有继承时，第三代类不能使用基类的接口，因为基类的公有方法在派生类中将变成私有方法；
* 使用保护继承时，基类的公有方法在第二代中变成受保护的，因此第三代可以使用它们。
**各种继承方式：**

|特征|公有继承|保护继承|私有继承|
|-|-|-|-|
|公有成员变成|派生类的公有成员|派生类的保护成员|派生类的私有成员|
|保护成员变成|派生类的保护成员|派生类的保护成员|派生类的私有成员|
私有成员变成|只能通过基类接口访问|只能通过基类接口访问|只能通过基类接口访问|
能否隐式向上转换|是|是(但只能在派生类中)|否
### 使用using重新定义访问权限
使用保护派生或者私有派生时，基类的公有成员将成为保护成员或私有成员，假设要让基类的方法在派生类外面可用，方法之一是定义一个使用该基类方法的派生类方法：
```C++
double Student::sum() const//public Student method
{
    return std::valarray<double>::sum();//use privately-inherited method
}
```
另一种方法是，将函数调用包装在另一个函数调用中，即使用using声明来指出派生类可以使用特定的基类成员，即使采用的是私有派生：
```C++
class Student:private std::string,private std::valarray<double>
{
    ...
    public:
    using std::valarray<double>::max;
    using std::valarray<double>::min;
    ...
};
```
上述using声明使得valarray<double>::max()和valarray<double>::min()可用，就像它们是Student的公有方法一样。**using声明只使用成员名——没有圆括号、函数特征标和返回类型。**
using声明只适用于继承，不适用于包含。
## 多重继承
MI描述的是有多个直接基类的类，与单继承一样，公有MI表示的是is-a关系，私有MI和保护MI表示的是has-a关系。
MI面临的两个主要问题是：
* 从两个不同的基类继承同名方法；
* 从两个或更多相关基类那里继承同一个类的多个实例。
为解决这些问题，需要使用一些新规则和不同的语法。
### 导致麻烦的例子
定义一个抽象基类Worker，并使用它派生出Waiter类和Singer类，然后使用MI从Waiter类和Singer类派生出SingingWaiter类。这里使用两个独立的派生来使基类Worker被继承，这将导致MI大多数的麻烦。
* 因为Singer和Waiter都继承了一个Worker组件，因此SingingWaiter将包含两个Worker组件。当将派生类对象的地址赋给基类指针时，会出现二义性：
```C++
SingingWaiter ed;
Worker * pw=&ed;//ambiguous
```
因为ed中包含两个Worker对象，有两个地址可供选择，所以应使用类型转换来指定对象：
```C++
Worker * pw1=(Waiter *) &ed;//the Worker in Waiter
Worker * pw2=(Singer *) &ed;//the Worker in Singer
```
这会使基类指针引用不同的对象复杂化。因此C++在引入多重继承的同时，引入了虚基类(virtual base class)。
### 虚基类
虚基类使得从多个类（它们的基类相同）派生出的对象只继承一个基类对象。通过在类声明中使用关键字virtual，可以使Worker被用作Singer和Waiter的虚基类(virtual和public的顺序不重要)：
```C++
class Singer:virtual public Worker{...};
class Waiter:public virtual Worker{...};
```
然后在SingingWaiter类使用MI，SingingWaiter对象将只包含Worker对象的一个副本。本质上说，继承的Singer和Waiter对象共享一个Worker对象，而不是各自引入自己的Worker对象副本。现在SingingWorker只包含了一个Worker子对象，所以可以使用多态。
#### 新的构造函数规则
Waiter和Singer的构造函数都会初始化基类Worker，为了避免这种冲突，C++在基类是虚的时，禁止信息通过中间类自动传递给基类，因此在SingingWaiter的构造函数中调用Waiter和Singer的构造函数不会调用基类Worker的构造函数，而是使用Worker的默认构造函数。
如果不希望使用默认构造函数来构造虚基类对象，则需要显式的调用所需的基类构造函数：
```C++
SingingWorker(const Worker & wk,int p=0,int v=Singer::other):Worker(wk),Waiter(wk,p),Singer(wk,v){}
```
上述代码将显式的调用构造函数Worker(const Worker &)，**这种用法对于虚基类是合法的，对于非虚基类，是非法的。**
#### 同名方法的调用
假设没有在SingingWaiter类中重新定义Show()方法，并试图使用SingingWaiter对象调用继承的Show()方法：
* 对于单继承，如果没有重新定义Show(),则将使用最近祖先中的定义；
* 在多重继承中，每个直接祖先都有一个Show()函数，这使得上述调用是二义性的。

可以使用作用域解析符来指定选择的Show()函数；
更好的方法是在SingingWaiter中重新定义的Show()中，使用模块化的方式，在Worker定义只显示Worker组件的方法，在Waiter中定义只显示Waiter组件的方法，在Singer中定义只显示Singer组件的方法，并在Show()中将这些组件组合起来。这些组件方法只能在类内部使用，作为协助公有接口的辅助方法，因此可以声明为保护方法。
```C++
void Worker::Data() const
{
    cout<<"Name: "<<fullname<<"\n";
    cout<<"Employee ID: "<<id<<"\n";
}
void Waiter::Data() const
{
    cout<<"Panache rating: "<<panache<<"\n";
}
void Singer::Data() const
{
    cout<<"Vocal range: "<<pv[voice]<<"\n";
}
void SingerWaiter::Data() const
{
    Singer::Data();
    Waiter::Data();
}
void SingingWaiter::Show() const
{
    cout<<"Category:singing waiter\n";
    Worker::Data();
    Data();
}
```
与此相似，其它Show()方法可以组合适当的Data()组件。
#### 混合使用虚基类和非虚基类
当类通过多条虚途径和非虚途径继承某个特定的基类时，该类将包含一个表示所有的虚途径的基类子对象，和分别表示各条非虚途径的多个基类子对象。
### MI小结
不使用虚基类的MI，这种形式的MI不会引入新的规则，但是如果一个类从两个不同的类那里继承了两个同名的成员，则需要在派生类中使用类限定符来区分它们，否则编译器会指出二义性。
使用虚基类的MI，当派生类使用关键字virtual来指示派生时，基类就成为虚基类，主要变化是从虚基类的一个或多个实例派生而来的类将只包含一个基类对象，为实现这种特性，必须满足这些要求：
* 有间接虚基类的派生类包含直接调用间接基类构造函数的构造函数，这对于间接非虚基类来说是非法的；
* 通过优先规则解决名称二义性。
## 类模板 
继承和包含并不总是能够满足重用代码的需要。例如之前设计的Stack类和Queue类都是容器类(container class)，容器类设计用来存储其它对象或数据类型。除了存储的对象类型的不同，这些容器类的代码是相同的。与其编写新的类声明，不如编写一个泛型(即独立于类型的)栈或队列，然后将具体的类型作为参数传递给这个类，这样就可以使用通用的代码生成不同类型值的栈或队列。
C++的类模板为生成通用的类声明提供了一种更好的方法，模板提供参数化(parameterized)类型，即能够将类型名作为参数传递给接收方来建立类或函数。
### 定义类模板
模板类以下面这样的代码开头：
```template <typename Type>```
关键字template告诉编译器，将要定义一个模板。
在关键字typename后面使用泛型标识符进行标识，常用T或Type进行命名。这里的Type称为类型参数(type parameter)，它类似于变量，但只能赋给它们类型而不是数字等。当模板被调用时，Type将被具体的类型名如int或string取代。
使用模板函数替换原有类的类方法，每个函数都必须以相同的模板声明开头：
```template <typename Type>```
另外还需要改变类限定符，例如从```Stack::```改为```Stack<Type>::```。
如果在类声明中定义了方法(内联定义)，则可以省略模板前缀和类限定符。
需要注意这些：
* 这些模板不是类和成员函数，它们是C++编译指令，说明了如何生成类和成员函数的定义。模板的具体实现被称为实例化(instantiation)或具体化(specialization)。
* 由于模板不是函数，它们不能单独编译，模板必须与特定的模板实例化请求一起使用。最简单的方法是将所有模板信息放在一个头文件里，并在要使用这些模板文件的文件中包含该头文件。
* 仅在程序包含模板并不能生成模板类，而必须请求实例化，因此需要声明一个类型为模板类的对象，方法是使用所需的具体类型替换泛型名：
```C++
Stack<int> kernels;//create a stack of ints
Stack<string> colonels;//create a stack of string objects
```
上面的代码创建两个栈，一个用于存储int，另一个用于存储string对象。
* 注意必须显式提供所需的类型，这与常规的函数模板不同，因为编译器需要根据函数的参数类型来确定生成哪种函数。
### 深入讨论模板类
既然可以将内置类型或类对象用作类模板的类型，指针是否也可以呢？
以栈为例，在栈测试程序中我们一般用一个固定类型的变量po表示pop()函数和push()函数接收的参数。
当po是指针变量的话，它首先必须被初始化分配内存空间才能使用；即便用new给po分配了内存空间，由于每次传递给栈的只是一个指针地址，虽然po指向的内容一直在改变，但是传入栈的地址一直是同一个，导致栈并没有保存每次读取的数据，而只是保存了指针变量po最后指向的数据。
#### 正确使用指针栈
使用指针栈的方法之一是，让调用程序提供一个指针数组，其中每个指针都指向不同的数据，然后把这些指针放在栈里才是有意义的，因为每个指针都将指向不同的字符串。
同时需要注意，创建不同的指针是调用程序的责任，而不是栈的责任，栈的任务是管理指针，而不是创建指针。
接下来我们将设计一个模板栈类Stack，它的大小将不再是固定的，而是可变的。为此我们需要让Stack构造函数能够接受一个可选大小的参数，这同时涉及在内部使用动态数组，因此还需要设计一个析构函数，一个复制构造函数和一个赋值运算符。具体代码实现见于他处。
强调一点：**实际的模板函数将类型定义为```Stack<Type>```，可以在模板声明或模板函数定义中使用Stack，但是在类外，即指定返回类型或使用作用域解析符时，必须使用完整的```Stack<Type>```。**
#### 非类型参数
对于这种形式的模板头：
```C++
template <typename T,int n>
```
关键字typename指出T为类型参数，int指出n的类型为int，这种指定特殊类型而不是泛型名的参数，称为非类型(non-type)或表达式(expression)参数。假如有如下声明：
```C++
ArrayTP<double,12> eggweights;
```
这将导致编译器定义名为ArrayTP<double,12>的类，并创建一个该类的对象eggweights，定义类时，编译器将使用double替换T，使用12替换n。
表达式参数有一定限制：
* 表达式参数可以是整型、枚举、引用或指针，因此浮点类型如double是不合法的。但是double *这种指针是合法的；
* 模板代码不能修改参数的值，也不能使用参数的地址。所以不能使用像n++,&n等表达式。
* 实例化模板时，用作表达式参数的值必须是常量表达式。
#### 模板多功能性
模板类可以用作基类，组件类，或其它模板的类型参数。
需要特别注意的是，对于这样的代码：
```C++
Array < Stack<int> > asi;
```
C++98要求使用至少一个空白字符将两个>符号分开，以免于运算符>>混淆，C++11不要求这样做。
#### 递归使用模板
对于这种形式的代码：
```C++
ArrayTP<ArrayTP<int,5>,10> twodee;
```
这使得twodee是一个包含10个元素的数组，其中每个元素都是包含一个5个int元素的数组，它与常规的二维数组等价：
```C++
int twodee[10][5];
```
#### 使用多个类型参数
模板可以包含多个类型参数，假设希望类可以保存两种类型的值，则可以创建并使用Pair模板来保存，标准模板库提供了类似的模板，名为pair。
```C++
template <typename T1,typename T2>
class Pair
{
    private:
    T1 a;
    T2 b;
    public:
    ...
};
```
#### 默认类型模板
类模板的另一项特性是，可以为类型参数提供默认值：
```C++
template <typename T1,typename T2=int>class Topo{...};
```
这样，如果省略T2的值，编译器将使用int。
标准模板库经常使用该特性，将默认类型设置为类。
**虽然可以为类模板类型参数提供默认值，但不能为函数模板参数提供默认值。但是可以为非类型参数提供默认值，这对于类模板和函数模板都适用。**
### 模板的具体化 
类模板与函数模板相似，都有隐式实例化，显式实例化和显式具体化，它们统称为具体化(specialization)。模板以泛型的方式描述类，而具体化是使用具体的类型生成类声明。
#### 显式实例化 
隐式实例化(implicit instantiation)即当创建一个或多个对象时，指出所需的类型，然后编译器才会根据通用模板生成具体的类定义。在此之前编译器不会生成类的隐式实例化。
当使用关键字template并指出所需类型来声明类时，编译器将生成类声明的显式实例化(explicit instantiation)，声明必须位于模板定义所在的名称空间中，例如：
```C++
template class ArrayTP<string,1000>;//generate the class
```
在这种情况下，虽然没有创建或提及类对象，编译器也根据通用模板来生成具体化。
#### 显式具体化 
显式具体化(explicit specialization)是特定类型(用于替换模板中的泛型)的定义。具体化类模板的格式如下：
```C++
template <> class Classname<specialized-type-name>{...};
```
当提供一个显式模板具体化时，如果具体化模板与通用模板都与实例化请求匹配时，编译器将使用具体化版本。
#### 部分具体化
C++还允许部分具体化(partial specialization)，即部分限制模板的通用性。例如，部分具体化可以给参数类型之一指定具体的类型：
```C++
//general template
template <typename T1,typename T2> class Pair{...};
//specialization with T2 set to int
template <typename T1> class Pair<T1,int>{...};
```
关键字template后面的<>声明是没有被具体化的类型参数，因此上述第二个声明将T2具体化为int，但是T1保持不变。**如果指定所有类型，则<>内将为空，这将导致显式具体化。**
如果有多个模板可供选择，编译器将使用具体化程度最高的模板。
也可以通过为指针提供特殊版本来部分具体化现有的模板：
```C++
template <typename T>//general version
class Feeb{...};
template <typename T*>//pointer partial specialization
class Feeb{...};//modified code
```
如果提供的类型不是指针，则编译器将使用通用版本；如果提供的是指针，则编译器将使用指针具体化版本：
```C++
Feeb<char> fb1;//use general Feeb template,T is char
Feeb<char *> fb2;//use Feeb T* specialization,T is char
```
如果没有进行部分具体化，则第二个声明将使用通用版本，将T转化为char*类型；如果进行了部分具体化，则第二个声明将使用具体化模板，将T转换为char。
### 成员模板
模板可用作结构、类或模板类的成员。要完全实现STL的设计，必须使用这项特性。
以下是一个简短的模板类示例，该模板类将另一个模板类和模板函数作为其成员，并且在类外定义模板方法：
```C++
template <typename T>
class beta
{
    private:
    template <typename V>//declaration
    class hold;
    hold<T> q;
    hold <int> n;
    public:
    beta(T t,int i):q(t),n(i){}
    template<typename U>//declaration
    U blab(U u,T t);
    void Show() const {q.show;n.show();}
};

//member definition
template <typename T>
template <typename V>
class beta<T>::hold
{
    private:
    V val;
    public:
    hold(V v=0):val(v){}
    void show() const {std::cout<<val<<std::endl;}
    V Value() const {return val;}
};

//member definition
template <typename T>
template <typename U>
U beta<T>::blab(U u,T t)
{
    return (n.Value()+q.Value())*u/t;
}
```
上述定义将T、U和V用作模板参数，可以看出模板是嵌套的。定义还必须指出hold和blab是```beta<T>```类的成员，这是通过作用域解析符来完成的。
### 将模板用作参数
模板可以包含类型参数如typename T和非类型参数如int n。
模板还可以包含本身就是模板的参数，这种参数是模板新增的特性，用于实现STL。
在如下的程序里：
```C++
#include <iostream>
#include "stack.h"
template <template <typename T> class Thing>
class Crab
{
    private:
    Thing<int> s1;
    Thing<double> s2;
    public:
    ...
};
int main()
{
    Crab<Stack> nebula;
    //Stack must match template <typename T> class Thing
    ...
    return 0;
}
```
开头的代码中，template <typename T> class Thing是模板参数，template <typename T> class是类型，Thing是参数。
在主函数内声明Crab<Stack> nebula;后，```Thing<int>```将被实例化为```Stack<int>```，而```Thing<double>```将被实例化为```Stack<double>```。总之，模板参数Thing将被替换为声明Crab对象时被用作模板参数的模板类型。
可以混合使用模板参数和常规参数，例如：
```C++
template<template <typename T> class Thing,typename U,typename V>
class Crab
{
    private:
    Thing<U> s1;
    Thing<V> s2;
    ...
};
```
现在成员s1和s2可存储的数据类型为泛型。
模板参数T表示一种模板类型，而类型参数U和V表示非模板类型。
### 模板类和友元
