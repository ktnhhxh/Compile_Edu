#pragma once
#include"CommonHead.h"
//<exported macros>
//<exported types>

//下面是for的几个状态
enum FOR_STATE{
	FOR_CMP=1,//比较表达式即将开始识别,需要处理初始化表达式
	FOR_INC,//自增表达式即将开始识别,需要处理比较表达式
	FOR_BLOCK,//受控块即将开始识别,需要处理自增表达式
	FOR_END,//整个for循环识别完毕,需要加一个标签
	FOR_BREAK,//遇到了break,需要goto
	FOR_CONTINUE//遇到了continue,需要goto
};
//下面是if的几个状态
enum IF_STATE{
	IF_THEN=1,//表达式即将开始识别then
	IF_ELSE,//即将开始识别else
	IF_END//整个if识别完毕
};
enum OP{
	ADD,//'+'
	SUB,//'-'
	MUL,//'*'
	DIV,//'/'
	MOD,//'%'
	SHL,//'<<'
	SHR,//'>>'
	NOT,//'~'
	NEG,//'-'
	INC,//'++'
	DEC,//'--'
	LEA,//'&'
	PTR,//'*'
	EQU,//'=='
	NEQ,//'!='
	GTR,//'>'
	LES,//'<'
	GEQ,//'>='
	LEQ,//'<='
	AND,//'&&'
	OR,//'||'
	BTA,//'&'
	BTO,//'|'
	BTX,//'^'
	BTN,//'!'
	MOV//'='
};
//<exported typedefs>
//<exported functions>

	//后端初始化函数
	void backEndInit();

	//告诉后端,编译完成,发送结束四元式
	void sendEnd();

	//告诉后端,生成一个id为labelId的标签
	void sendLabel(int labelId);

	//告诉后端,跳转到id为labelId的标签
	void sendGt(int labelId);

	//告诉后端,生成id为forId的循环的第state阶段四元式和代码
	void sendFor(int forId,enum FOR_STATE state);

	//告诉后端,生成id为ifId的条件的第state阶段四元式和代码
	void sendIf(int ifId,enum IF_STATE state);

	//告诉后端,生成(op,first,second,result)这样的四元式
	void sendOp(enum OP op,struct symbol* first,struct symbol* second,struct symbol* result);

	//通知后端,声明了一个s符号
	void tellVar(struct symbol* s);

	//通知后端,声明了一个t类型
	void tellType(struct type* t);

//<exported data>