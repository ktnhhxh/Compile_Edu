#include"sym.h"
//<macros>
//<data>
	struct table
		cns={SCOPE_CONSTANTS},
		ids={SCOPE_GLOBAL};
	Table constants=&cns;
	Table idt=&ids;
	int level=SCOPE_GLOBAL;
//<functions>

//符号表初始化函数
void symInit(){
	idt->previous=constants;
}

//install函数为给定的name分配一个符号
//并把该符号加到对应给定作用域层数相对应的表中
//该函数将返回一个指向符号的指针
Symbol install(String&name,int level){
	struct symbol sym;
	sym.name=name;
	sym.scope=level;
	Table t=idt;
	while(t->level>level)
		t=t->previous;
	t->m[name]=sym;
	return &t->m[name];
}

//新定义一个枚举常量的标识符
//name是这个枚举常量的名字
//val是这个枚举常量的常量值
//t是这个枚举常量的类型
//c是这个符号出现的位置
Symbol newEnumConst(String&name,int val,Type t,Coordinate c){
	Symbol sym=install(name,SCOPE_CONSTANTS);
	sym->addressed=0;
	sym->sclass=SCLASS_CONST;
	sym->src=c;
	sym->temporary=0;
	sym->type=t;
	sym->u.ev=val;
	return sym;
}

//找到名字叫name的标识符
//没找到就返回NULL
Symbol findSymbol(String name){
	for(Table t=idt;t;t=t->previous){
		auto it=t->m.find(name);
		if(it!=t->m.end())
			return &it->second;
	}return NULL;
}

//新定义一个标识符
//name是这个标识符的名字
//t是这个标识符的类型
//s是这个标识符的存储类型
//c是这个符号出现的位置
Symbol newSymbol(String name,Type t,enum SCLASS s,Coordinate c){
	Symbol sym=install(name,level);
	sym->sclass=s;
	sym->src=c;
	sym->temporary=0;
	sym->type=t;
	return sym;
}

//告诉符号表,进入了一个作用域
void enterScope(){
	Table newTable=new struct table();
	newTable->level=++level;
	newTable->previous=idt;
	idt=newTable;
}

//告诉符号表,退出了一个作用域
//符号表会把不合时宜的符号删除
void exitScope(){
	--level;
	Table oldTable=idt;
	idt=idt->previous;
	delete oldTable;
}

//下一个临时变量id
int tempid=1;
//向符号表申请一个临时变量
Symbol newTemp(){
	static char str[11];
	sprintf(str,"$%d",tempid++);
	String name=str;
	Symbol sym=install(name,level);
	sym->temporary=1;
	return sym;
}