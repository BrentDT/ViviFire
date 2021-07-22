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

#if !defined(_CODEGEN_H_)
#define _CODEGEN_H_

#include "AST.h"

using namespace AST;

class CodeGen : public Visitor {
public:
	virtual void Visit(Assignment *);
	virtual void Visit(BinaryOp *);
	virtual void Visit(Block *);
	virtual void Visit(Call *);
	virtual void Visit(Case *);
	virtual void Visit(CaseExpression *);
	virtual void Visit(CaseIs *);
	virtual void Visit(CaseTo *);
	virtual void Visit(Catch *);
	virtual void Visit(ComparisonChain *);
	virtual void Visit(ComparisonOp *);
	virtual void Visit(Do *);
	virtual void Visit(DTConst *);
	virtual void Visit(For *);
	virtual void Visit(ForEach *);
	virtual void Visit(FPConst *);
	virtual void Visit(GotoCase *);
	virtual void Visit(Exit *);
	virtual void Visit(IfOp *);
	virtual void Visit(IntConst *);
	virtual void Visit(Library *);
	virtual void Visit(Program *);
	virtual void Visit(Return *);
	virtual void Visit(Select *);
	virtual void Visit(Throw *);
	virtual void Visit(Try *);
	virtual void Visit(UnaryOp *);
	virtual void Visit(While *);
};

#endif // _CODEGEN_H_
