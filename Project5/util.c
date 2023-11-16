#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */
void printToken(TokenType token, const char* tokenString)
{
    switch (token)
    {
    case ELSE:
       
    case IF:
       
    case INT:
 
    case RETURN:
   
    case VOID:
    
    case WHILE:
        fprintf(listing, "reserved word: %s\n", tokenString);
        break;
    case PLUS:     fprintf(listing, "+\n");  break;
    case MINUS:    fprintf(listing, "-\n");  break;
    case TIMES:    fprintf(listing, "*\n");  break;
    case OVER:     fprintf(listing, "/\n");  break;
    case LT:       fprintf(listing, "<\n");  break;
    case LE:       fprintf(listing, "<=\n"); break;
    case GT:       fprintf(listing, ">\n");  break;
    case GE:       fprintf(listing, ">=\n"); break;
    case EQ:       fprintf(listing, "==\n"); break;
    case NE:       fprintf(listing, "!=\n"); break;
    case ASSIGN:   fprintf(listing, "=\n");  break;
    case SEMI:     fprintf(listing, ";\n");  break;
    case COMMA:    fprintf(listing, ",\n");  break;
    case LPAREN:   fprintf(listing, "(\n");  break;
    case RPAREN:   fprintf(listing, ")\n");  break;
    case LBRACKET: fprintf(listing, "[\n");  break;
    case RBRACKET: fprintf(listing, "]\n");  break;
    case LBLOCK:   fprintf(listing, "{\n");  break;
    case RBLOCK:   fprintf(listing, "}\n");  break;
    case ENDFILE:  fprintf(listing, "%s %s\n", "ENDFILE", "EOF"); break;
    case NUM: fprintf(listing, "NUM, val = %s\n", tokenString); break;
    case ID: fprintf(listing, "ID, name = %s\n", tokenString); break;
    case ERROR: fprintf(listing, "ERROR: %s\n", tokenString); break;
    default: /* should never happen */
        fprintf(listing, "Unknown token: %d\n", token);
    }
}

/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode* newStmtNode(StmtKind kind) {
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0;i < MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = StmtK;
        t->kind.stmt = kind;
        t->lineno = lineno;
    }
    return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode* newExpNode(ExpKind kind) {
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL) fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0;i < MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = ExpK;
        t->kind.exp = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}

/* Function newDeclNode creates a new declaration
 * node for syntax tree construction
 */
TreeNode* newDeclNode(DeclKind kind) {
    TreeNode* t = (TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else {
        for (i = 0;i < MAXCHILDREN;i++) t->child[i] = NULL;
        t->sibling = NULL;
        t->nodekind = DeclK;
        t->kind.decl = kind;
        t->lineno = lineno;
        t->type = Void;
    }
    return t;
}


char* copyString(char* s)
{
    int n;
    char* t;
    if (s == NULL) return NULL;
    n = strlen(s) + 1;
    t = malloc(n);
    if (t == NULL)
        fprintf(listing, "Out of memory error at line %d\n", lineno);
    else strcpy(t, s);
    return t;
}


static int indentno = 0;


#define INDENT indentno+=2
#define UNINDENT indentno-=2

static void printSpaces(void) {
    int i;
    for (i = 0;i < indentno;i++)
        fprintf(listing, " ");
}

void printTypes(TreeNode* tree) {
    if (tree->child[0] != NULL) {
        switch (tree->child[0]->type) {
        case Integer:
            fprintf(listing, "int");
            break;
        case Void:
            fprintf(listing, "void");
            break;

        default: return;
        }
    }
    else {
        switch (tree->type) {
        case Integer:
            fprintf(listing, "int");
            break;
        case Void:
            fprintf(listing, "void");
            break;

        default: return;
        }
    }
}


void printTree(TreeNode* tree)
{
    int i;
    INDENT;
    while (tree != NULL) {


        if (tree->nodekind == StmtK) {
            printSpaces();
            switch (tree->kind.stmt) {
            case SelectionK:
                fprintf(listing, "If\n");
                break;
            case IterationK:
                fprintf(listing, "While\n");
                break;
            case CompoundK:
                fprintf(listing, "Compound statement\n");
                break;
            case ReturnK:
                fprintf(listing, "Return\n");
                break;
            case ExpressionK:
                fprintf(listing, "Expression\n");
                break;
            default:
                fprintf(listing, "Unknown stmtNode kind\n");
                break;
            }
        }

        else if (tree->nodekind == ExpK) {
            if (tree->kind.exp != TypeK) printSpaces();
            switch (tree->kind.exp) {
            case OpK:
                fprintf(listing, "Op: ");
                printToken(tree->attr.op, "\0");
                break;
            case ConstK:
                fprintf(listing, "Const: %d\n", tree->attr.val);
                break;
            case IdK:
                fprintf(listing, "Id: %s\n", tree->attr.name);
                break;
            case TypeK: break;
            case ArrIdK:
                fprintf(listing, "ArrId \n");
                break;
            case CallK:
                fprintf(listing, "Call Function : %s\n", tree->attr.name);
                break;
            case CalcK:
                fprintf(listing, "Operator : ");
                printToken(tree->child[1]->attr.op, "\0");
                break;
            default:
                fprintf(listing, "Unknown ExpNode kind\n");
                break;
            }
        }

        else if (tree->nodekind == DeclK) {
            printSpaces();
            switch (tree->kind.decl) {
            case FunK:
                fprintf(listing, "Function Declaration:  ");
                printTypes(tree);
                fprintf(listing, " %s()\n", tree->attr.name);
                break;
            case VarK:
                fprintf(listing, "Variable Declaration:  ");
                printTypes(tree);
                fprintf(listing, " %s;\n", tree->attr.name);
                break;
            case ArrVarK:
                fprintf(listing, "Array Variable Declaration:  ");
                printTypes(tree);
                fprintf(listing, " %s[%d];\n", tree->attr.arr.name, tree->attr.arr.size);
                break;
            case ArrParamK:
                fprintf(listing, "Array Parameter: %s\n", tree->attr.name);
                break;
            case ParamK:
                fprintf(listing, "Parameter: ");
                printTypes(tree);
                if (tree->attr.name != NULL) {
                    fprintf(listing, " %s\n", tree->attr.name);
                }
                else {
                    fprintf(listing, " void\n");
                }
                break;
            default:
                fprintf(listing, "Unknown Declaration\n");
                break;
            }
        }
        else fprintf(listing, "Unknown node kind\n");
        for (i = 0;i < MAXCHILDREN;i++) if (tree->child[i] != NULL) printTree(tree->child[i]);
        tree = tree->sibling;
    }
    UNINDENT;
}
