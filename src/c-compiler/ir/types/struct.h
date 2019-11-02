/** Handling for record-based types with fields
 * @file
 *
 * This source file is part of the Cone Programming Language C compiler
 * See Copyright Notice in conec.h
*/

#ifndef struct_h
#define struct_h

// Field-containing types (e.g., struct, trait, etc.)
// - fields holds all owned and trait-inherited fields
// - nodelist holds owned methods and static functions and variables
// - namespace is the dictionary of all owned and inherited named nodes
typedef struct StructNode {
    INsTypeNodeHdr;
    NodeList fields;
} StructNode;

typedef struct FieldDclNode FieldDclNode;

StructNode *newStructNode(Name *namesym);

// Add a field node to a struct type
void structAddField(StructNode *type, FieldDclNode *node);

void structPrint(StructNode *node);

// Name resolution of a struct type
void structNameRes(NameResState *pstate, StructNode *node);

// Type check a struct type
void structTypeCheck(TypeCheckState *pstate, StructNode *name);

int structEqual(StructNode *node1, StructNode *node2);
int structCoerces(StructNode *to, StructNode *from);

#endif