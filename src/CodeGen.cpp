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

#include <list>
#include <stdio.h>
#include "CodeGen.h"

using namespace AST;

void CodeGen::Visit(Assignment *node) {
	node->var->Accept(this);
	node->expr->Accept(this);
}

void CodeGen::Visit(BinaryOp *node) {
	node->lexpr->Accept(this);
	node->rexpr->Accept(this);
}

void CodeGen::Visit(Block *node) {
///	for (StatementList::iterator it = node->stmts.begin(), end = node->stmts.end(); it != end; ++it)
///		(*it)->Accept(this);
node->Accept(this);
}

void CodeGen::Visit(Call *node) {}

void CodeGen::Visit(Case *node) {
}

void CodeGen::Visit(CaseExpr *node) {
	node->expr->Accept(this);
}

void CodeGen::Visit(Catch *node) {
//TODO
}

void CodeGen::Visit(ComparisonChain *node) {
}

void CodeGen::Visit(ComparisonOp *node) {
	node->lexpr->Accept(this);
	node->rexpr->Accept(this);
}

void CodeGen::Visit(Do *node) {
	node->cond->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(DTConst *node) {}

void CodeGen::Visit(For *node) {
	node->var->Accept(this);
	node->init->Accept(this);
	node->last->Accept(this);
	if (node->step) node->step->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(ForEach *node) {
	node->var->Accept(this);
	node->cont->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(RealConst *node) {}

void CodeGen::Visit(GotoCase *node) {}

void CodeGen::Visit(Exit *node) {}

void CodeGen::Visit(BooleanConditional *node) {
	node->cond->Accept(this);
	node->tval->Accept(this);
	node->fval->Accept(this);
}

void CodeGen::Visit(IntConst *node) {}

void CodeGen::Visit(Library *node) {
	node->Accept(this);
}

void CodeGen::Visit(Return *node) {
	if (node->expr) node->expr->Accept(this);
}

void CodeGen::Visit(Select *node) {
	node->expr->Accept(this);
	for (auto it: node->cases) it->Accept(this);
}

void CodeGen::Visit(Raise *node) {
	node->expr->Accept(this);
}

void CodeGen::Visit(Try *node) {
// TODO
}

void CodeGen::Visit(UnaryOp *node) {
	node->expr->Accept(this);
}

void CodeGen::Visit(Program *node) {
	node->Accept(this);
}

void CodeGen::Visit(While *node) {
	node->cond->Accept(this);
	node->Accept(this);
}

