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

class CodeGen : public AST::Visitor {
public:
	virtual void Visit(AST::Assignment *);
	virtual void Visit(AST::BinaryOp *);
	virtual void Visit(AST::Block *);
	virtual void Visit(AST::BooleanConditional *);
	virtual void Visit(AST::Call *);
	virtual void Visit(AST::Case *);
	virtual void Visit(AST::CaseExpr *);
	virtual void Visit(AST::Catch *);
	virtual void Visit(AST::ComparisonChain *);
	virtual void Visit(AST::ComparisonOp *);
	virtual void Visit(AST::Do *);
	virtual void Visit(AST::DTConst *);
	virtual void Visit(AST::For *);
	virtual void Visit(AST::ForEach *);
	virtual void Visit(AST::GotoCase *);
	virtual void Visit(AST::Exit *);
	virtual void Visit(AST::IntConst *);
	virtual void Visit(AST::Library *);
	virtual void Visit(AST::Program *);
	virtual void Visit(AST::Raise *);
	virtual void Visit(AST::RealConst *);
	virtual void Visit(AST::Return *);
	virtual void Visit(AST::Select *);
	virtual void Visit(AST::Try *);
	virtual void Visit(AST::UnaryOp *);
	virtual void Visit(AST::While *);
};

#endif // _CODEGEN_H_
