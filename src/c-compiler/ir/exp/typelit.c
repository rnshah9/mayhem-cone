/** Handling for list nodes (e.g., type literals)
 * @file
 *
 * This source file is part of the Cone Programming Language C compiler
 * See Copyright Notice in conec.h
*/

#include "../ir.h"

// Serialize a list node
void typeLitPrint(FnCallNode *node) {
    if (node->objfn)
        inodePrintNode(node->objfn);
    INode **nodesp;
    uint32_t cnt;
    inodeFprint("[");
    for (nodesFor(node->args, cnt, nodesp)) {
        inodePrintNode(*nodesp);
        if (cnt)
            inodeFprint(",");
    }
    inodeFprint("]");
}

// Is the type literal actually a literal?
int typeLitIsLiteral(FnCallNode *node) {
    INode **nodesp;
    uint32_t cnt;
    for (nodesFor(node->args, cnt, nodesp)) {
        INode *arg = *nodesp;
        if (arg->tag == NamedValTag)
            arg = ((NamedValNode*)arg)->val;
        if (!litIsLiteral(arg))
            return 0;
    }
    return 1;
}

// Type check an array literal
void typeLitArrayCheck(PassState *pstate, FnCallNode *arrlit) {

    // Get element type from first element
    // Type of array literal is: array of elements whose type matches first value
    INode *first = nodesGet(arrlit->args, 0);
    if (!isExpNode(first)) {
        errorMsgNode((INode*)first, ErrorBadArray, "Array literal element must be a typed value");
        return;
    }
    INode *firsttype = ((ITypedNode*)first)->vtype;
    ((ArrayNode*)arrlit->vtype)->elemtype = firsttype;

    // Ensure all elements are consistently typed (matching first element's type)
    INode **nodesp;
    uint32_t cnt;
    for (nodesFor(arrlit->args, cnt, nodesp)) {
        if (!itypeIsSame(((ITypedNode*)*nodesp)->vtype, firsttype))
            errorMsgNode((INode*)*nodesp, ErrorBadArray, "Inconsistent type of array literal value");
    }
}

// Type check a struct literal
// Verify correct name/type of each literal element against struct's properties
void typeLitStructCheck(PassState *pstate, FnCallNode *arrlit, StructNode *strnode) {
    INode **nodesp;
    uint32_t cnt;
    uint32_t argi = 0;
    for (imethnodesFor(&strnode->methprops, cnt, nodesp)) {
        if ((*nodesp)->tag != VarDclTag)
            break;
        VarDclNode *prop = (VarDclNode *)*nodesp;

        // If element has been specified, be sure it is the right type
        if (argi < arrlit->args->used) {
            INode **litval = &nodesGet(arrlit->args, argi);
            if (!iexpCoerces(*nodesp, litval))
                errorMsgNode((INode*)*litval, ErrorBadArray, "Literal value's type does not match expected field's type");
        }
        // Append default value if no value specified
        else if (prop->value) {
            nodesAdd(&arrlit->args, prop->value);
        }
        else {
            errorMsgNode((INode*)arrlit, ErrorBadArray, "Not enough values specified on struct literal");
            break;
        }
        ++argi;
    }
    if (argi < arrlit->args->used)
        errorMsgNode((INode*)arrlit, ErrorBadArray, "Too many values specified on struct literal");
}

// Check the list node
void typeLitWalk(PassState *pstate, FnCallNode *arrlit) {
    INode **nodesp;
    uint32_t cnt;
    for (nodesFor(arrlit->args, cnt, nodesp))
        inodeWalk(pstate, nodesp);

    switch (pstate->pass) {
    case NameResolution:
        break;
    case TypeCheck:
    {
        if (arrlit->args->used == 0) {
            errorMsgNode((INode*)arrlit, ErrorBadArray, "Literal list may not be empty");
            return;
        }

        INode *littype = itypeGetTypeDcl(arrlit->vtype);
        if (littype->tag == ArrayTag)
            typeLitArrayCheck(pstate, arrlit);
        else if (littype->tag == StructTag)
            typeLitStructCheck(pstate, arrlit, (StructNode*)littype);
        else
            errorMsgNode((INode*)arrlit, ErrorBadArray, "Unknown type literal type for type checking");

    }
    }
}
