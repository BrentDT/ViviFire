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
	node->left->Accept(this);
	node->right->Accept(this);
}

void CodeGen::Visit(Block *node) {
///	for (StatementList::iterator it = node->stmts.begin(), end = node->stmts.end(); it != end; ++it)
///		(*it)->Accept(this);
node->Accept(this);
}

void CodeGen::Visit(Call *node) {}

void CodeGen::Visit(Case *node) {
	for (CaseExpressionList::iterator it = node->exprs->begin(), end = node->exprs->end(); it != end; ++it)
		(*it)->Accept(this);
}

void CodeGen::Visit(CaseExpression *node) {
	node->expr->Accept(this);
}

void CodeGen::Visit(CaseIs *node) {
	node->expr->Accept(this);
}

void CodeGen::Visit(CaseTo *node) {
	node->expr->Accept(this);
	node->max->Accept(this);
}

void CodeGen::Visit(Catch *node) {
//TODO
}

void CodeGen::Visit(ComparisonChain *node) {
	for (std::list<ComparisonChain::link>::iterator it = node->chain.begin(), end = node->chain.end(); it != end; ++it) {
		it->expr->Accept(this);
	}
}

void CodeGen::Visit(ComparisonOp *node) {
	node->left->Accept(this);
	node->right->Accept(this);
}

void CodeGen::Visit(Do *node) {
	node->cond->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(DTConst *node) {}

void CodeGen::Visit(For *node) {
	node->var->Accept(this);
	node->init->Accept(this);
	node->max->Accept(this);
	if (node->step) node->step->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(ForEach *node) {
	node->var->Accept(this);
	node->coll->Accept(this);
	node->Accept(this);
}

void CodeGen::Visit(FPConst *node) {}

void CodeGen::Visit(GotoCase *node) {}

void CodeGen::Visit(Exit *node) {}

void CodeGen::Visit(IfOp *node) {
	node->cond->Accept(this);
	node->tval->Accept(this);
	node->fval->Accept(this);
}

void CodeGen::Visit(IntConst *node) {}

void CodeGen::Visit(LibraryModule *node) {
	node->Accept(this);
}

void CodeGen::Visit(Return *node) {
	if (node->expr) node->expr->Accept(this);
}

void CodeGen::Visit(Select *node) {
	node->test->Accept(this);
	for (CaseList::iterator it = node->cases->begin(), end = node->cases->end(); it != end; ++it)
		(*it)->Accept(this);
	if (node->elseb) node->elseb->Accept(this);
}

void CodeGen::Visit(Throw *node) {
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

void CodeGen::Visit(Wait *node) {
	fprintf(stdout, "; Wait\n");
}

void CodeGen::Visit(While *node) {
	node->cond->Accept(this);
	node->Accept(this);
}

