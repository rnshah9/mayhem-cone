/** AST Node List
 * @file
 *
 * This source file is part of the Cone Programming Language C compiler
 * See Copyright Notice in conec.h
*/

#ifndef nodes_h
#define nodes_h

typedef struct AstNode AstNode;
typedef struct NamedAstNode NamedAstNode;
typedef struct OwnerAstNode OwnerAstNode;
typedef struct Name Name;

#include <stdint.h>

// *** Nodes: Dynamically-sized array of AstNodes ***

// Header for a variable-sized structure holding a list of AstNodes
// The nodes immediately follow the header
typedef struct Nodes {
	uint32_t used;
	uint32_t avail;
} Nodes;

// Helper Functions
Nodes *newNodes(int size);
void nodesAdd(Nodes **nodesp, AstNode *node);
NamedAstNode *nodesFind(Nodes *nodes, Name *name);

#define nodesNodes(nodes) ((AstNode**)((nodes)+1))
#define nodesFor(nodes, cnt, nodesp) nodesp = (AstNode**)((nodes)+1), cnt = (nodes)->used; cnt; cnt--, nodesp++
#define nodesGet(nodes, index) ((AstNode**)((nodes)+1))[index]
#define nodesLast(nodes) nodesGet(nodes, nodes->used-1)

#endif