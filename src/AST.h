/*
 * ViviFire Programming Language
 *
 * Copyright 2021 Brent D. Thorn
 *
 * You can get the latest version at http://vivifire.com/.
 *
 * Use of this source code is governed by an MIT-style license that can be
 * found in the LICENSE file.
 */

#if !defined(_AST_H_)
#define _AST_H_

#include <cstdint>
#include <inttypes.h>
#include <list>
#include "Parser.h"
#include "Scanner.h"
#include "Type.h"

template<typename T>
inline void delete_list(std::list<T> &list) {
	for(auto &it: list) delete it; list.clear();
}

namespace AST {

enum op_type {
	PLUS, MINUS, NOT,
	SHL, SHR,
	POWER,
	TIMES, SLASH, MOD, REM,
	CONCAT,
	NULL_COALESCING,
	LESS, LESS_EQUAL, GREATER, GREATER_EQUAL, EQUAL, NOT_EQUAL,
	AND, AND_THEN, OR, OR_ELSE, XOR
};

enum data_type {
	DATETIME, INT, REAL, STRING, VERSION
};

enum block_type {
	DO, FOR, FUNCTION, METHOD, PROPERTY, SUB, WHILE
};

// Forward declarations
struct Assignment;
//struct BaseUnit;
struct BeginLoop;
struct BinaryOp;
struct Block;
struct BooleanConditional;
struct Call;
//struct CallBase;
//struct CallConstructor;
struct Case;
struct CaseExpr;
struct Catch;
struct CharConst;
struct Class;
struct CompareConditional;
struct ComparisonChain;
struct ComparisonOp;
//struct Declare;
struct DTConst;
struct Do;
struct ElseIf;
struct Enum;
//struct Event;
struct Exit;
struct Expr;
struct Field;
struct For;
struct ForEach;
struct GotoCase;
struct Ident;
struct If;
struct IntConst;
struct Library;
struct Loop;
struct ModuleWhere;
//struct Namespace;
struct New;
//struct Object;
struct Parameter;
struct Procedure;
struct Program;
//struct Property;
struct Raise;
//struct RaiseEvent;
struct RealConst;
//struct ReDim;
struct Require;
struct Return;
struct Select;
struct Statement;
//struct Struct;
struct Symbol;
//struct Trait;
struct Try;
struct TypeParam;
struct UnaryOp;
//struct Unit;
struct Visitor;
struct When;
struct While;
//struct Yield;

typedef std::list<wchar_t *> StringList;

#define TYPEDEFLIST(T) typedef std::list<T *> T##List;

TYPEDEFLIST(Case)
TYPEDEFLIST(CaseExpr)
TYPEDEFLIST(Catch)
TYPEDEFLIST(ElseIf)
TYPEDEFLIST(Expr)
TYPEDEFLIST(Field)
TYPEDEFLIST(Ident)
TYPEDEFLIST(ModuleWhere)
TYPEDEFLIST(Parameter)
TYPEDEFLIST(Require)
TYPEDEFLIST(Statement)
TYPEDEFLIST(TypeParam)
TYPEDEFLIST(When)

///////////////////////////////////////////////////////////////////////////
// Nodes

struct Node {
	int line, col;
	
	Node() = delete;
	Node(int line, int col): line(line), col(col) {}
	virtual ~Node() = 0;
	
	virtual void Accept(Visitor *) = 0;
};

struct Field : public Node {
	Symbol *sym;
	// TODO: dimensions
	Expr *init;
	
	Field(Symbol *sym, Expr *init, int line, int col): Node(line, col), sym(sym), init(init) {}
	virtual ~Field() {
		delete sym;
		delete init;
	}
	
	virtual void Accept(Visitor *);
};

struct Ident : public Node {
	wchar_t *id;

	Ident(wchar_t *id, int line, int col): Node(line, col), id(id) {}
	~Ident() {
		coco_string_delete(id);
	}

	virtual void Accept(Visitor *);
};

struct Symbol : public Node {
	wchar_t *id;
	Type::Type *type;
	
	Symbol(wchar_t *id, Type::Type *type, int line, int col): Node(line, col), id(id), type(type) {}
	virtual ~Symbol() {
		coco_string_delete(id);
		delete type;
	}
	
	virtual void Accept(Visitor *);
};

struct TypeParam : public Node {
	int mod; // _In or _Out
	Ident *id;
	
	TypeParam(int mod, Ident *id, int line, int col): Node(line, col), mod(mod), id(id) {}
	virtual ~TypeParam() {
		delete id;
	}
	
	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Expr : public Node {
	Type::Type *type = nullptr;
	
	Expr(int line, int col): Node(line, col) {}
	virtual ~Expr() = 0;
	
	virtual bool IsLValue() const { return false; }
	virtual bool IsConst() const { return false; }
};

///////////////////////////////////////////////////////////////////////////

template<typename T>
struct Constant : public Expr {
	T val;
	
	Constant(int line, int col): Expr(line, col), val() {}
	Constant(T val, int line, int col): Expr(line, col), val(val) {}
	virtual ~Constant() {}
	
	virtual bool IsConst() const { return true; }
};

struct CharConst : public Constant<uint32_t> {
	CharConst(uint32_t val, int line, int col): Constant<uint32_t>(val, line, col) {}
	CharConst(wchar_t *s, int line, int col);
	virtual ~CharConst() {}

	virtual void Accept(Visitor *);
};

struct DTConst : public Constant<uint64_t> {
	DTConst(uint64_t val, int line, int col): Constant<uint64_t>(val, line, col) {}
	DTConst(wchar_t *s, int line, int col);
	virtual ~DTConst() {}

	virtual void Accept(Visitor *);
};

struct IntConst : public Constant<long long> {
	IntConst(long long val, int line, int col): Constant<long long>(val, line, col) {}
	IntConst(wchar_t *s, int line, int col);
	virtual ~IntConst() {}

	virtual void Accept(Visitor *);
};

struct RealConst : public Constant<long double> {
	RealConst(long double val, int line, int col): Constant<long double>(val, line, col) {}
	RealConst(wchar_t *, int line, int col);
	virtual ~RealConst() {}

	virtual void Accept(Visitor *);
};

struct StringConst : public Constant<wchar_t *> {
	StringConst(wchar_t *val, int line, int col): Constant<wchar_t *>(val, line, col) {}
	virtual ~StringConst() {
		coco_string_delete(val);
	}

	virtual void Accept(Visitor *);
};

///////////////////////////////////////////////////////////////////////////

struct UnaryOp : public Expr {
	op_type op; // NOT, etc.
	Expr *expr;
	
	UnaryOp(op_type op, Expr *expr, int line, int col): Expr(line, col), op(op), expr(expr) {}
	virtual ~UnaryOp() {
		delete expr;
	}
	
	virtual bool IsConst() const {
		return expr->IsConst();
	}
	
	virtual void Accept(Visitor *);
};

struct BinaryOp : public Expr {
	int op; // _plus, etc.
	Expr *lexpr, *rexpr;
	
	BinaryOp(Expr *lexpr, int op, Expr *rexpr, int line, int col):
		Expr(line, col), lexpr(lexpr), op(op), rexpr(rexpr) {}
	virtual ~BinaryOp() {
		delete lexpr;
		delete rexpr;
	}
	
	virtual bool IsConst() const {
		return (lexpr->IsConst() && rexpr->IsConst());
	}
	virtual void Accept(Visitor *);
};

struct ComparisonOp : public BinaryOp {
	double tol;
	
	ComparisonOp(Expr *lexpr, int op, Expr *rexpr, double tol, int line, int col):
		BinaryOp(lexpr, op, rexpr, line, col), tol(tol) {}
	virtual ~ComparisonOp() {}

	virtual void Accept(Visitor *);
};

struct ComparisonChain : public Expr {
	struct link {
		int op; // _less_than or _less_or_equal
		Expr *expr;
		link(int op, Expr *expr) : op(op), expr(expr) {}
		~link() {
			delete expr;
		}
	};
	Expr *expr1;
	std::list<link *> chain;
	
	ComparisonChain(Expr *expr1, int line, int col): Expr(line, col), expr1(expr1) {}
	virtual ~ComparisonChain() {
		delete expr1;
		delete_list(chain);
	}
	
	void Add(int op, Expr *expr) {
		chain.push_back(new link(op, expr));
	}
	virtual void Accept(Visitor *);
};

struct BooleanConditional : public Expr {
	Expr *cond, *tval, *fval;
	
	BooleanConditional(Expr *con, Expr *tval, Expr *fval, int line, int col):
		Expr(line, col), cond(con), tval(tval), fval(fval) {}
	virtual ~BooleanConditional() {
		delete cond;
		delete tval;
		delete fval;
	}

	virtual void Accept(Visitor *);
};

struct CompareConditional : public Expr {
	Expr *expr1, *expr2;
	struct {
		int op = 0; // _less_than, etc.
		Expr *expr = NULL;
	} result[3];
	int results = 0;
	enum {
		NOP = 0, LT = 1, EQ = 2, LE = 3, GT = 4, NE = 5, GE = 6, ALL = 7
	};
	int	comps = NOP;

	CompareConditional(Expr *expr1, Expr *expr2, int line, int col):
		Expr(line, col), expr1(expr1), expr2(expr2) {}
	virtual ~CompareConditional() {
		delete expr1;
		delete expr2;
		for (int i = 0; result[i].expr; i++) delete result[i].expr;
	}

	bool Add(int op, Expr *expr);
	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Statement : public Node {
	Statement(int line, int col): Node(line, col) {}
	virtual ~Statement() = 0;
};

struct Assignment : public Statement {
	Symbol *var;
	int op; // _plus for +=, etc.
	Expr *expr;
	
	Assignment(Symbol *var, int op, Expr *expr, int line, int col):
		Statement(line, col), var(var), op(op), expr(expr) {}
	virtual ~Assignment() {
		delete var;
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct New : public Statement {
	Symbol *var;
	ExprList args;
	StatementList stms;

	New(Symbol *var, int line, int col): Statement(line, col), var(var) {}
	virtual ~New() {
		delete var;
		delete_list(args);
		delete_list(stms);
	}

	void Add(Expr *arg) {
		args.push_back(arg);
	}
	void Add(Statement *stm) {
		stms.push_back(stm);
	}
	virtual void Accept(Visitor *);
};

struct Call : public Statement {
	Symbol *proc;
	ExprList args;
	
	Call(Symbol *proc, int line, int col): Statement(line, col), proc(proc) {}
	virtual ~Call() {
		delete proc;
		delete_list(args);
	}

	void Add(Expr *arg) {
		args.push_back(arg);
	}
	virtual void Accept(Visitor *);
};

struct Enum : public Statement {
	bool flags;
	Symbol *sym;
	FieldList flds;
	
	Enum(bool flags, Symbol *sym, int line, int col): Statement(line, col), flags(flags), sym(sym) {}
	virtual ~Enum() {
		delete sym;
		delete_list(flds);
	}
	
	void Add(Field *fld) {
		flds.push_back(fld);
	}
	
	virtual void Accept(Visitor *);
};

struct Exit : public Statement {
	Ident *id;
	std::list<block_type> blks;
	
	Exit(Ident *id, int line, int col): Statement(line, col), id(id) {}
	Exit(block_type blk, int line, int col): Statement(line, col), id(nullptr) {
		Add(blk);
	}
	virtual ~Exit() {
		delete id;
		blks.clear();
	}

	void Add(block_type blk) {
		blks.push_back(blk);
	}
	virtual void Accept(Visitor *);
};

struct GotoCase : public Statement {
	Expr *expr; // NULL for Goto Case Else
		double tol;

	GotoCase(Expr *expr, double tol, int line, int col):
		Statement(line, col), expr(expr), tol(tol) {}
	virtual ~GotoCase() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct Return : public Statement {
	Expr *expr;
	
	Return(Expr *expr, int line, int col): Statement(line, col), expr(expr) {}
	virtual ~Return() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct Raise : public Statement {
	Expr *expr;
	
	Raise(Expr *expr, int line, int col): Statement(line, col), expr(expr) {}
	virtual ~Raise() {
		delete expr;
	}

	virtual void Accept(Visitor *);
};

struct Require : public Statement {
	IdentList lib;
	
	Require(int line, int col): Statement(line, col) {}
	~Require() {
		delete_list(lib);
	}
	
	void Add(Ident *id) {
		lib.push_back(id);
	}

	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct Block : public Statement {
	StatementList stms;
	Block *parent;
	
	Block(Block *parent, int line, int col): Statement(line, col), parent(parent) {}
	virtual ~Block() {
		delete_list(stms);
	}
	
	void Add(Statement *stm) {
		stms.push_back(stm);
	}

	virtual void Accept(Visitor *);
};

struct ModuleWhere: public Statement {
	data_type type;
	wchar_t *id, *val;

	ModuleWhere(wchar_t *id, wchar_t *val, data_type type, int line, int col):
		Statement(line, col), id(id), val(val), type(type) {
		if (type== VERSION) normalize();
	}
	virtual ~ModuleWhere() {
		coco_string_delete(id);
		coco_string_delete(val);
	}

	void normalize(); // Removes unnecessary 0s.

	virtual void Accept(Visitor *);
};

struct Program : public Block {
	Ident *id;
	bool procStart;
	ModuleWhereList wheres;
	RequireList reqs;

	Program(Ident *id, bool procStart, int line, int col):
		Block(nullptr, line, col), id(id), procStart(procStart) {}
	virtual ~Program() {
		delete id;
		delete_list(wheres);
		delete_list(reqs);
	}

	void Add(ModuleWhere *where) {
		wheres.push_back(where);
	}
	void Add(Require *req) {
		reqs.push_back(req);
	}
	
	virtual void Accept(Visitor *);
};

struct Library : public Block {
	Ident *id;
	ModuleWhereList wheres;
	RequireList reqs;
	
	Library(Ident *id, int line, int col): Block(nullptr, line, col), id(id) {}
	virtual ~Library() {
		delete id;
		delete_list(wheres);
		delete_list(reqs);
	}

	void Add(ModuleWhere *where) {
		wheres.push_back(where);
	}
	void Add(Require *req) {
		reqs.push_back(req);
	}
	
	virtual void Accept(Visitor *);
};

struct Class : public Block {
	Symbol *sym;
	TypeParamList generics;
	
	Class(Symbol *sym, int line, int col):
		Block(nullptr, line, col), sym(sym) {}
	virtual ~Class() {
		delete sym;
		delete_list(generics);
	}
	
	bool Add(TypeParam *param) {
		generics.push_back(param);
	}
	
	virtual void Accept(Visitor *);
};

struct ElseIf : public Block {
	Expr *cond;
	
	ElseIf(Block *parent, Expr *cond, int line, int col): Block(parent, line, col), cond(cond) {}
	virtual ~ElseIf() {
		delete cond;
	}

	virtual void Accept(Visitor *);
};

struct If : public Block {
	Expr *cond;
	ElseIfList elifs;
	Block *el = nullptr;
	
	If(Block *parent, Expr *cond, int line, int col): Block(parent, line, col), cond(cond) {}
	virtual ~If() {
		delete cond;
		delete_list(elifs);
		delete el;
	}
	void Add(ElseIf *elif) {
		assert(elif != nullptr);
		elifs.push_back(elif);
	}
	void SetElse(Block *blk) {
		assert(blk != nullptr && el == nullptr);
		el = blk;
	}

	virtual void Accept(Visitor *);
};

struct CaseExpr : public Node {
	Expr *expr, *expr2 = NULL;
	int op = 0; // _less_than, etc.
	
	CaseExpr(Expr *expr, int line, int col): // 'Case expr'.
		Node(line, col), expr(expr) {}
	CaseExpr(Expr *expr, Expr *expr2, int line, int col): // 'Case expr To expr2'.
		Node(line, col), expr(expr), expr2(expr2) {}
	CaseExpr(int op, Expr *expr, int line, int col): // 'Case Is < expr'
		Node(line, col), op(op), expr(expr) {}
	virtual ~CaseExpr() {
		delete expr;
		delete expr2;
	}
	virtual void Accept(Visitor *);
};

struct Case : public Block {
	CaseExprList exps;
	
	Case(Block *parent, int line, int col): Block(parent, line, col) {}
	virtual ~Case() {
		delete_list(exps);
	}
	void Add(CaseExpr *expr) {
		exps.push_back(expr);
	}
	bool IsCaseElse() const {
		return exps.empty();
	}

	virtual void Accept(Visitor *);
};

struct Select : public Block {
	Expr *expr;
	double tol;
	CaseList cases;
	
	Select(Block *parent, Expr *expr, double tol, int line, int col): Block(parent, line, col), expr(expr), tol(tol) {}
	virtual ~Select() {
		delete expr;
		delete_list(cases);
	}
	bool Add(Case *acase) {
		if (acase->IsCaseElse()) {
			if (cases.empty()) return false; // 'Else' can't be first.
			// TODO: false if existing 'Else'
		}
		cases.push_back(acase);
		return true;
	}
	
	virtual void Accept(Visitor *);
};

struct Catch : public Block {
	Symbol *sym; // catch-all if NULL.
	
	Catch(Block *parent, Symbol *sym, int line, int col): Block(parent, line, col), sym(sym) {}
	virtual ~Catch() {
		delete sym;
	}
	
	bool IsCatchall() const {
		return (sym == nullptr);
	}

	virtual void Accept(Visitor *);
};

struct Try : public Block {
	CatchList catches;
	Block *finally = NULL;
	
	Try(Block *parent, int line, int col): Block(parent, line, col) {}
	virtual ~Try() {
		delete_list(catches);
		delete finally;
	}
	
	bool Add(Catch *acatch); // false if dups.
	
	virtual void Accept(Visitor *);
};

/*---------------------------------------------------------------------------*/

struct When : public Block {
	Ident *id; // 'Else' if NULL
	
	When(Block *parent, Ident *id, int line, int col): Block(parent, line, col), id(id) {}
	virtual ~When() {
		delete id;
	}

	virtual void Accept(Visitor *);
};

struct BeginLoop : public Block {
	std::list<wchar_t *> ids;
	WhenList whens;
	
	BeginLoop(Block *parent, int line, int col): Block(parent, line, col) {}
	virtual ~BeginLoop() {
		delete_list(ids);
		delete_list(whens);
	}
	
	bool Add(wchar_t *id); // false if dups or is NULL, 'DONE', or 'NONE'.
	bool Add(When *when); // false if dups.
	
	virtual void Accept(Visitor *);
};

///////////////////////////////////////////////////////////////////////////

struct Loop : public Block {
	Loop(Block *parent, int line, int col): Block(parent, line, col) {}
	virtual ~Loop() = 0;
};

struct Do : public Loop {
	enum order {
		DoWhileLoop, DoUntilLoop, DoLoopWhile, DoLoopUntil
	} ord;
	Expr *cond;
	
	Do(Block *parent, order ord, Expr *cond, int line, int col): Loop(parent, line, col), ord(ord), cond(cond) {}
	virtual ~Do() {
		delete cond;
	}
	
	virtual void Accept(Visitor *);
};

struct For : public Loop {
	Symbol *var;
	Expr *init, *last, *step;
	
	For(Block *parent, Symbol *var, Expr *init, Expr *last, Expr *step, int line, int col):
		Loop(parent, line, col), var(var), init(init), last(last), step(step) {}
	virtual ~For() {
		delete var;
		delete init;
		delete last;
		delete step;
	}
	
	virtual void Accept(Visitor *);
};

struct ForEach : public Loop {
	Symbol *var;
	Expr *cont;
	
	ForEach(Block *parent, Symbol *var, Expr *cont, int line, int col): Loop(parent, line, col), var(var), cont(cont) {}
	virtual ~ForEach() {
		delete var;
		delete cont;
	}
	
	virtual void Accept(Visitor *);
};

struct While : public Loop {
	Expr *cond;
	
	While(Block *parent, Expr *cond, int line, int col): Loop(parent, line, col), cond(cond) {}
	virtual ~While() {
		delete cond;
	}
	
	virtual void Accept(Visitor *);
};

struct Parameter : public Symbol {
	int mod; // _ByRef, etc.
	
	Parameter(int mod, wchar_t *id, Type::Type *type, int line, int col):
		Symbol(id, type, line, col), mod(mod) {}
	virtual ~Parameter() {}
	
	virtual void Accept(Visitor *);
};

struct Procedure : public Block {
	Symbol *sym;
	TypeParamList generics;
	ParameterList params;
	
	Procedure(Block *parent, Symbol *sym, int line, int col):
		Block(parent, line, col), sym(sym) {}
	virtual ~Procedure() {
		delete sym;
		delete_list(generics);
		delete_list(params);
	}
	
	void Add(TypeParam *param) {
		generics.push_back(param);
	}
	void Add(Parameter *param) {
		params.push_back(param);
	}

	virtual void Accept(Visitor *);
};

struct Visitor {
	virtual void Visit(Assignment *) = 0;
	virtual void Visit(BeginLoop *) = 0;
	virtual void Visit(BinaryOp *) = 0;
	virtual void Visit(Block *) = 0;
	virtual void Visit(Call *) = 0;
	virtual void Visit(Case *) = 0;
	virtual void Visit(CaseExpr *) = 0;
	virtual void Visit(Catch *) = 0;
	virtual void Visit(CharConst *) = 0;
	virtual void Visit(Class *) = 0;
	virtual void Visit(ComparisonChain *) = 0;
	virtual void Visit(ComparisonOp *) = 0;
	virtual void Visit(Do *) = 0;
	virtual void Visit(DTConst *) = 0;
	virtual void Visit(ElseIf *) = 0;
	virtual void Visit(Enum *) = 0;
	virtual void Visit(Exit *) = 0;
	virtual void Visit(Field *) = 0;
	virtual void Visit(For *) = 0;
	virtual void Visit(ForEach *) = 0;
	virtual void Visit(GotoCase *) = 0;
	virtual void Visit(If *) = 0;
	virtual void Visit(BooleanConditional *) = 0;
	virtual void Visit(CompareConditional *) = 0;
	virtual void Visit(Ident *) = 0;
	virtual void Visit(IntConst *) = 0;
	virtual void Visit(Library *) = 0;
	virtual void Visit(ModuleWhere *) = 0;
	virtual void Visit(New *) = 0;
	virtual void Visit(Parameter *) = 0;
	virtual void Visit(Procedure *) = 0;
	virtual void Visit(Program *) = 0;
	virtual void Visit(Raise *) = 0;
	virtual void Visit(RealConst *) = 0;
	virtual void Visit(Require *) = 0;
	virtual void Visit(Return *) = 0;
	virtual void Visit(Select *) = 0;
	virtual void Visit(Symbol *) = 0;
	virtual void Visit(Try *) = 0;
	virtual void Visit(UnaryOp *) = 0;
	virtual void Visit(When *) = 0;
	virtual void Visit(While *) = 0;
};

} // namespace AST

#endif // _AST_H_
