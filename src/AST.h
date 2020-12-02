/*
* ViviFire Programming Language
*
* Copyright 2012-2020 Brent D. Thorn
*
* You can get the latest version at http://vivifire.com/.
*
* Use of this source code is governed by an MIT-style license that can be
* found in the LICENSE file.
*/

#if !defined(_AST_H_)
#define _AST_H_

#include <inttypes.h>
#include <list>
#include "Scanner.h"
#include "Type.h"

template<class T>
inline void delete_list(std::list<T> &list) {
	for (T::iterator it = list.begin(), end = list.end(); it != end; ++it) delete (*it);
}

namespace AST {

// Forward declarations
struct Assignment;
struct BinaryOp;
struct BooleanCond;
struct Call;
struct Catch;
struct CharConst;
struct CompareCond;
struct ComparisonChain;
struct ComparisonOp;
struct DTConst;
struct Do;
struct ElseIf;
struct Enum;
struct Exit;
struct Expression;
struct RealConst;
struct For;
struct ForEach;
struct Generics;
struct GotoCase;
struct If;
struct IntConst;
struct Library;
struct Loop;
struct Module Where;
struct New;
struct Parameter;
struct Procedure;
struct Program;
struct QualifiedName;
struct Return;
struct Select;
struct Symbol;
struct Throw;
struct Try;
struct UnaryOp;
struct Visitor;
struct Wait;
struct While;

#define TYPEDEFLIST(T) typedef std::list<T *> T##List;

TYPEDEFLIST(Case)
TYPEDEFLIST(CaseExpression)
TYPEDEFLIST(Catch)
TYPEDEFLIST(ElseIf)
TYPEDEFLIST(Expression)
TYPEDEFLIST(ModuleWhere)
TYPEDEFLIST(Parameter)
TYPEDEFLIST(QualifiedName)
TYPEDEFLIST(Statement)

// Enumerations

enum block_type {
	DoBlock, ForBlock, FunctionBlock, MethodBlock, ProgramBlock, PropertyBlock, SubBlock, WhileBlock
};

enum do_type {
	DoWhileLoop, DoUntilLoop, DoLoopWhile, DoLoopUntil
};

enum op_type {
	ConcatOp,
	AddOp, SubOp,
	MulOp, DivOp, ModOp, RemOp,
	PowOp,
	LTOp, LEOp, GTOp, GEOp, EQOp, NEOp,
	NegOp, NotOp,
	AndOp, AndThenOp, OrOp, OrElseOp, XorOp,
	SHLOp, SHROp,
	InOp, IsOp
};

enum param_type {
	NormalParam, ByRefParam, OptionalParam
};

// Nodes

struct Node {
	int line, col;
	
	virtual ~Node() = 0;
	virtual void Accept(Visitor *) = 0;

	void SetLineCol(int l, int c) {
		line = l;
		col = c;
	}
};

struct GenParam : public Node {
	enum { NoMod, InMod, OutMod } mod;
	wchar_t *ident;
};

struct Symbol : public Node {
	wchar_t *ident;
	Type::Type *type;
	
	Symbol() = delete;
	Symbol(wchar_t *id, Type::Type *ty): type(ty) {
		ident = coco_string_create(id);
	}
	virtual ~Symbol() {
		delete ident;
		delete type;
	}

	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Expression : public Node {};

template<typename T>
struct Constant: public Expression {
	T value;

	Constant() = delete;
	Constant(T val): value(val)
};

struct IntConst : public Constant<int64_t> {
	IntConst() = delete;
	IntConst(int64_t val): Constant(val) {}
	IntConst(wchar_t *s);
	virtual void ~IntConst() {}

	virtual void Accept(Visitor *);
};

struct RealConst : public Constant<long double> {
	RealConst() = delete;
	RealConst(long double val): Constant(val) {}
	RealConst(wchar_t *);
	virtual ~RealConst() {}

	virtual void Accept(Visitor *);
};

struct StringConst : public Constant<wchar_t *> {
	StringConst() = delete;
	StringConst(wchar_t *val): Constant(coco_string_create(val)) {}
	virtual ~StringConst() {
		coco_string_delete(value);
	}

	virtual void Accept(Visitor *);
};

struct CharConst : public Constant<uint32_t> {
	CharConst() = delete;
	CharConst(uint32_t val): Constant(val) {}
	CharConst(wchar_t *);
	virtual ~CharConst() {}

	virtual void Accept(Visitor *);
};

struct DTConst : public Const<uint64_t> {
	DTConst() = delete;
	DTConst(uint64_t val): Constant(val) {}
	DTConst(wchar_t *);
	virtual ~DTConst() {}

	virtual void Accept(Visitor *);
};

struct UnaryOp : public Expression {
	op_type oper;
	Expression *expr;
	
	UnaryOp() = delete;
	UnaryOp(op_type op, Expression *ex): oper(op), expr(ex) {}
	virtual ~UnaryOp() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct BinaryOp : public Expression {
	op_type oper;
	Expression *left, *right;
	
	BinaryOp() = delete;
	BinaryOp(Expression *lt, op_type op, Expression *rt): left(lt), oper(op), right(rt) {}
	virtual ~BinaryOp() {
		delete left;
		delete right;
	}

	virtual void Accept(Visitor *);
};

struct ComparisonOp : public BinaryOp {
	double tol;
	
	ComparisonOp() = delete;
	ComparisonOp(Expression *lt, op_type op, Expression *rt, double tol): BinaryOp(lt, op, rt), tol(tol) {}
	virtual ~ComparisonOp() {}

	virtual void Accept(Visitor *);
};

struct ComparisonChain : public Expression {
	struct link {
		op_type op;
		Expression *expr;
	};
	std::list<link> chain;
	
	virtual void Accept(Visitor *);
};

struct BooleanCond : public Expression {
	Expression *cond, *tval, *fval;
	
	BoolCond() = delete;
	BoolCond(Expression *con, Expression *tv, Expression *fv): cond(con), tval(tv), fval(fv) {}
	virtual ~BoolCond() {
		delete cond;
		delete tval;
		delete fval;
	}

	virtual void Accept(Visitor *);
};

struct CompareCond : public Expression {
	Expression *expr1, *expr2;
	int results = 0;
	op_type oper[3] = {0};
	Expression *result[3] = {NULL};

	CompareCond() = delete;
	CompareCond(Expression *ex1, Expression *ex2): expr1(ex1), expr2(ex2) {}
	virtual ~CompareCond() {
		delete expr1;
		delete expr2;
		for (int i = 0; i < 3; i++) delete result[i];
	}

	void Add(op_type op, Expression *res);
	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Statement : public Node {};

struct Assignment : public Statement {
	Symbol *var;
	op_type op;
	Expression *expr;
	
	Assignment() = delete;
	Assignment(Symbol *v, op_type op, Expression *ex): var(v), op(op), expr(ex) {}
	virtual ~Assignment() {
		delete var;
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct New : public Statement {
	// TODO: class id
	wchar_t *ident;
	ExpressionList args;
	StatementList stats;

	New() = delete;
	New(wchar_t *id) {
		ident = coco_string_create(id);
	}
	virtual ~New() {
		coco_string_delete(ident);
		delete_list(args);
		delete_list(stats);
	}

	Add(Expression *arg) {
		args.push_back(arg);
	}
	void Add(Statement *s) {
		stats.push_back(s);
	}
	virtual void Accept(Visitor *);
};

struct Call : public Statement {
	Symbol *proc;
	ExpressionList args;
	
	Call() = delete;
	Call(Symbol *proc): proc(proc) {}
	virtual ~Call() {
		delete proc;
		delete_list(args);
	}

	void Add(Expression *arg) {
		args.push_back(arg);
	}
	virtual void Accept(Visitor *);
};

struct Enum : public Statement {
	struct constant: public Statement {
		wchar_t *ident;
		int64_t value;
		
		constant(wchar_t *id, int64_t val, int ln, int c): value(val), line(ln), col(c) {
			ident = coco_string_create(id);
		}
		~constant() {
			coco_string_delete(ident);
		}
	};

	bool flags;
	wchar_t *ident;
	int64_t value;
	std::list<constant *> consts;
	
	Enum() = delete;
	Enum(bool flags, wchar_t *id): flags(flags) {
		ident = coco_string_create(id);
	}
	virtual ~Enum() {
		coco_string_delete(ident);
		delete_list(consts);
	}
	void Add(wchar_t *id, int64_t val, int ln, int c) {
		if (!flags) { value = val; } else { value |= val; }
		consts.push_back(new constant(id, val, ln, c);
	}
	void Add(wchar_t *id, int ln, int c);
	virtual void Accept(Visitor *);
};

struct Exit : public Statement {
	wchar_t *ident;
	std::list<block_type> blocks;
	
	Exit() = delete;
	Exit(wchar_t *id) {
		ident = coco_string_create(id);
	}
	Exit(block_type b): ident(NULL) {
		blocks.push_back(b);
	}
	virtual ~Exit() {
		coco_string_delete(ident);
		delete_list(blocks);
	}

	void Add(block_type b) {
		blocks.push_back(b);
	}
	virtual void Accept(Visitor *);
};

struct GotoCase : public Statement {
	Expression *expr; // NULL for Goto Case Else
		double tol;

	GotoCase() = delete;
	GotoCase(Expression *ex, double tol = 0.0): expr(ex), tol(tol) {}
	virtual ~GotoCase() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct Return : public Statement {
	Expression *expr;
	
	Return(Expression *ex = NULL): expr(ex) {}
	virtual ~Return() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct Throw : public Statement {
	Expression *expr;
	
	Throw() = delete;
	Throw(Expression *ex): expr(ex) {}
	virtual ~Throw() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Block : public Statement {
	StatementList stats;
	Block *parent;
	
	Block() = delete;
	Block(Block *scope): parent(scope) {}
	virtual ~Block() {
		delete_list(stats);
	}
	
	void Add(Statement *stat) {
		stats.push_back(stat);
	}
	virtual void Accept(Visitor *);
};

struct ModuleWhere: public Statement {
	enum data_type { StringType, VersionType } type;
	wchar_t *ident, *value;

	ModuleWhere(wchar_t *id, wchar_t *val, data_type t): type(t) {
		ident = coco_string_create(id);
		value = coco_string_create(val);
		if (type == VersionType) normalize();
	}
	virtual ~ModuleWhere() {
		coco_string_delete(ident);
		coco_string_delete(value);
	}

	virtual void Accept(Visitor *);
	void normalize(); // Removes unnecessary 0s.
};

struct QualifiedName: public Expression {
	std::list<wchar_t *> ids;
	void Add(wchar_t *id) {
		ids.push_back(coco_string_create(id));
	}
	virtual ~QualifiedName() {
		for (QualifiedNameList::iterator it = ids.begin(), end = ids.end(); it != end; ++it) coco_string_delete(*it);
	}
	virtual void Accept(Visitor *);
};

struct Program : public Block {
	wchar_t *ident;
	bool proc;
	ModuleWhereList wheres;
	QualifiedNameList requires;

	Program() = delete;
	Program(wchar_t *id, bool proc): proc(proc) {
		ident = coco_string_create(id);
	}
	virtual ~Program() {
		coco_string_delete(ident);
		delete_list(wheres);
		delete_list(requires);
	}

	void Add(ModuleWhere *where) {
		wheres.push_back(where);
	}
	void Add(QualifiedName *require) {
		requires.push_back(require);
	}
	virtual void Accept(Visitor *);
};

struct Library : public Block {
	wchar_t *ident;
	ModuleWhereList wheres;
	
	Library() = delete;
	Library(wchar_t *id) {
		ident = coco_string_create(id);
	}
	~LibraryModule() {
		coco_string_delete(ident);
		delete_list(wheres);
	}

	void Add(ModuleWhere *where) {
		wheres.push_back(where);
	}
	virtual void Accept(Visitor *);
};

struct Class : public Block {
	Symbol ident;
	Generics *generics;
	Class *base;
};

struct Constructor : public Procedure {
	Constructor(ParameterList *params): Procedure(NULL, params) {}
};

struct Destructor : public Procedure {
	Destructor(): Procedure(NULL, NULL) {}
};

struct ElseIf : public Block {
	Expression *cond;
	
	ElseIf(Expression *cond): cond(cond), Block() {}
	virtual ~ElseIf() {
		delete cond;
	}
	virtual void Accept(Visitor *);
	ElseIf() = delete;
};

struct If : public Block {
	Expression *cond;
	ElseIfList elseifs;
	Block *elsebl;
	
	If() = delete;
	If(Expression *con): cond(con), elsebl(NULL) {}
	virtual ~If() {
		delete cond;
		delete_list(elseifs);
		delete elsebl;
	}

	void Add(ElseIf *elif) {
		elseifs.push_back(elif);
	}
	void Else(Block *bl) {
		elsebl = bl;
	}
	virtual void Accept(Visitor *);
};

struct CaseExpression : public Node {
	Expression *expr;
	
	CaseExpression(= delete;
	CaseExpression(Expression *expr): expr(expr) {}
	virtual ~CaseExpression() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct CaseIs : public CaseExpression {
	op_type op;
	
	CaseIs() = delete;
	CaseIs(op_type op, Expression *expr): op(op), CaseExpression(expr) {}
	virtual ~CaseIs() {}

	virtual void Accept(Visitor *);
};

struct CaseTo : public CaseExpression {
	Expression *max;
	
	CaseTo() = delete;
	CaseTo(Expression *min, Expression *max): CaseExpression(min), max(max) {}
	virtual ~CaseTo() {
		delete max;
	}

	virtual void Accept(Visitor *);
};

struct Case : public Block {
	CaseExpressionList exprs;
	
	Case(): Block() {}
	virtual ~Case() {
		delete_list(exprs);
	}

	void Add(CaseExpression *ex) {
		exprs.push_back(ex);
	}
	virtual void Accept(Visitor *);
};

struct Select : public Statement {
	Expression *test;
	double tol;
	CaseList *cases;
	Block *elsebl;
	
	Select() = delete;
	Select(Expression *ex, double tol): test(ex), tol(tol) {}
	virtual ~Select() {
		delete test;
		delete_list(cases);
		delete elsebl;
	}

	void Add(Case *bl) {
		cases.push_back(bl);
	}
	void Else(Block *bl) {
		elsebl = bl;
	}
	virtual void Accept(Visitor *);
};

struct Catch : public Block {
	Expression *expr;

	virtual ~Catch() {
		delete expr;
	}
	virtual void Accept(Visitor *);
};

struct Try : public Block {
	CatchList catches;
	Block *finally;

	virtual ~Try() {
		for (CatchList::iterator it = catches.begin(), end = catches.end(); it != end; ++it) delete *it;
	}
	virtual void Accept(Visitor *);
	void Add(Catch *cat) { catches.push_back(cat); }
};

/*---------------------------------------------------------------------------*/

struct Loop : public Block {
	// TODO: WHEN statements
	
	Loop(Block *after, Block *other): Block(), after(after), other(other) {}
	virtual ~Loop() {}
	virtual void Accept(Visitor *);
	Loop() = delete;
};

struct Do : public Loop {
	do_type type;
	Expression *cond;
	
	Do(do_type type, Expression *cond): type(type), cond(cond), Loop() {}
	virtual ~Do() {
		delete cond;
	}
	virtual void Accept(Visitor *);
	Do() = delete;
};

struct For : public Loop {
	Symbol *var;
	Expression *init, *max, *step;
	
	For(Symbol *var, Expression *init, Expression *max, Expression *step): var(var), init(init), max(max), step(step), Loop() {}
	virtual ~For() {
		delete var;
		delete init;
		delete max;
		delete step;
	}
	virtual void Accept(Visitor *);
	For() = delete;
};

struct ForEach : public Loop {
	Symbol *var;
	Expression *coll;
	
	ForEach(Symbol *var, Expression *coll): var(var), coll(coll), Loop() {}
	virtual ~ForEach() {
		delete var;
		delete coll;
	}
	virtual void Accept(Visitor *);
	ForEach() = delete;
};

struct While : public Loop {
	Expression *cond;
	
	While(Expression *cond): cond(cond), Loop() {}
	virtual ~While() {
		delete cond;
	}
	virtual void Accept(Visitor *);
	While() = delete;
};

struct Parameter : public Symbol {
	param_type modifiers;
	
	Parameter(param_type mods, wchar_t *id, Type::Type *type): modifiers(mods), Symbol(id, type) {}
	Parameter() = delete;
};

struct Procedure : public Block {
	Symbol *id;
	Generics *generics;
	ParameterList *params;
	
	Procedure(Symbol *id, ParameterList *params): id(id), params(params), Block() {}
	virtual ~Procedure() {
		delete id;
		delete_list(params);
	}
	Procedure() = delete;
};

struct Visitor {
	virtual void Visit(Assignment *) = 0;
	virtual void Visit(BinaryOp *) = 0;
	virtual void Visit(Block *node) {
		for (StatementList::iterator it = node->stmts.begin(), end = node->stmts.end(); it != end; ++it)
			(*it)->Accept(this);
	}
	virtual void Visit(Call *) = 0;
	virtual void Visit(Case *) = 0;
	virtual void Visit(CaseExpression *) = 0;
	virtual void Visit(CaseIs *) = 0;
	virtual void Visit(CaseTo *) = 0;
	virtual void Visit(Catch *) = 0;
	virtual void Visit(CharConst *) = 0;
	virtual void Visit(Class *) = 0;
	virtual void Visit(ComparisonChain *) = 0;
	virtual void Visit(ComparisonOp *) = 0;
	virtual void Visit(Constructor *) = 0;
	virtual void Visit(Destructor *) = 0;
	virtual void Visit(Do *) = 0;
	virtual void Visit(DTConst *) = 0;
	virtual void Visit(ElseIf *) = 0;
	virtual void Visit(Enum *) = 0;
	virtual void Visit(Exit *) = 0;
	virtual void Visit(For *) = 0;
	virtual void Visit(ForEach *) = 0;
	virtual void Visit(GotoCase *) = 0;
	virtual void Visit(If *) = 0;
	virtual void Visit(BooleanCond *) = 0;
	virtual void Visit(CompareCond *) = 0;
	virtual void Visit(IntConst *) = 0;
	virtual void Visit(Library *) = 0;
	virtual void Visit(New *) = 0;
	virtual void Visit(Program *) = 0;
	virtual void Visit(RealConst *) = 0;
	virtual void Visit(Return *) = 0;
	virtual void Visit(Select *) = 0;
	virtual void Visit(Symbol *) = 0;
	virtual void Visit(Throw *) = 0;
	virtual void Visit(Try *) = 0;
	virtual void Visit(UnaryOp *) = 0;
	virtual void Visit(While *) = 0;
};

} // namespace AST

#endif // _AST_H_
