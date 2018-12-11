#pragma once
#include"CommonHead.h"
//<exported macros>
#define HASHSIZE 256
//<exported types>

	typedef struct coord{
		// String file;//该定义的文件名
		unsigned x,y;//行号和字符位置
	}Coordinate;

	typedef union value{
		char sc;
		short ss;
		int i;
		unsigned char uc;
		unsigned short us;
		unsigned u;
		float f;
		double d;
		void* p;
	}Value;

	typedef struct symbol{
		//name域存这个符号的字符串表示
		String name;
		//scope域说明了符号是常量,标号,全局变量,参数或局部变量
		int scope;
		//src域说明了这个符号定义出现的位置
		Coordinate src;
		//符号的扩展存储类型
		//有:无=0,SCLASS_AUTO=1,SCLASS_CONST
		//SCLASS_STATIC,SCLASS_EXTERN,SCLASS_TYPEDEF
		int sclass;
		//type域保存了变量,函数,常量,结构,联合和枚举等类型信息
		struct type* type;
		union{
			//enum constants value
			int ev;
			//constants
			struct{
				//常量的值存放在v中
				Value v;
				//有些(浮点)常量不能存储在指令中,所以编译器
				//生成一个静态变量并将其初始化成该浮点常量的值
				//对于这些常量,loc指向了产生的变量所对应的符号
				struct symbol* loc;
			}c;
		}u;
		//<symbol flags>
		//如果该符号是临时变量,则temporary为1
		unsigned temporary:1;
		//如果该类型的常量可以作为指令的一部分
		//即不能被寻址,则addressed的值为0
		unsigned addressed:1;
	}* Symbol;

	//注:在第k层声明的局部变量,其scope域等于LOCAL+k
	enum SCOPE{SCOPE_CONSTANTS=1,SCOPE_GLOBAL,SCOPE_PARAM,SCOPE_LOCAL};//scope

	enum SCLASS{SCLASS_AUTO=1,SCLASS_CONST,SCLASS_STATIC,SCLASS_EXTERN,SCLASS_TYPEDEF};//sclass

	typedef struct table{

		//作用域
		int level;

		//外层作用域对应的table
		struct table* previous;

		//保存符号
		std::hash_map<String,struct symbol>m;

	}* Table;

//<exported typedefs>
//<exported functions>

	//符号表初始化函数
	void symInit();

	//新定义一个枚举常量的标识符
	//name是这个枚举常量的名字
	//val是这个枚举常量的常量值
	//t是这个枚举常量的类型
	//c是这个符号出现的位置
	Symbol newEnumConst(String name,int val,struct type* t,Coordinate c);

	//找到名字叫name的标识符
	//没找到就返回NULL
	Symbol findSymbol(String name);

	//新定义一个标识符
	//name是这个标识符的名字
	//t是这个标识符的类型
	//s是这个标识符的存储类型
	//c是这个符号出现的位置
	Symbol newSymbol(String name,type* t,enum SCLASS s,Coordinate c);

	//告诉符号表,进入了一个作用域
	void enterScope();

	//告诉符号表,退出了一个作用域
	//符号表会把不合时宜的符号删除
	void exitScope();

	//向符号表申请一个临时变量
	Symbol newTemp();

//<exported data>

	// //静态或常量
	// extern Table constants;
	// //声明为extern的标识符
	// extern Table externals;
	// //保存具有文件作用域的标识符
	// extern Table globals;

	//标识符
	extern Table idt;
	// //level和对应的表表示了一个作用域
	// extern int level;