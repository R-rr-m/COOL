
#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include <iostream>
#include <vector>
#include "tree.h"
#include "cool.h"
#include "stringtab.h"
#define yylineno curr_lineno;
extern int yylineno;

inline Boolean copy_Boolean(Boolean b) {return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(ostream& stream, int padding, Boolean b)
	{ stream << pad(padding) << (int) b << "\n"; }

void dump_Symbol(ostream& stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;

class CgenNode;

class Environment {
public:
    int LookUpVar(Symbol sym);
    int LookUpParam(Symbol sym);
    int LookUpAttrib(Symbol sym);
    void EnterScope();
    void ExitScope();
    void AddVar(Symbol sym);
    void AddParam(Symbol sym);
    void AddObstacle();
    CgenNode* m_class_node;
    
    // Internal data structures
    std::vector<std::vector<Symbol> > var_scopes;
    std::vector<Symbol> params;
    int obstacle_count;

    Environment() : m_class_node(0), obstacle_count(0) {
        EnterScope(); // Default scope
    }
};

#define Program_EXTRAS                          \
virtual void cgen(ostream&) = 0;		\
virtual void dump_with_types(ostream&, int) = 0; 

#define program_EXTRAS                          \
void cgen(ostream&);     			\
void dump_with_types(ostream&, int);            

#define Class__EXTRAS                   \
virtual Symbol get_name() = 0;  	\
virtual Symbol get_parent() = 0;    	\
virtual Symbol get_filename() = 0;      \
virtual void dump_with_types(ostream&,int) = 0; 

#define class__EXTRAS                                  \
Symbol get_name()   { return name; }		       \
Symbol get_parent() { return parent; }     	       \
Symbol get_filename() { return filename; }             \
void dump_with_types(ostream&,int);                    

#define Feature_EXTRAS                                        \
virtual void dump_with_types(ostream&,int) = 0; 

#define Feature_SHARED_EXTRAS                                       \
void dump_with_types(ostream&,int);    

#define method_EXTRAS                                 \
int GetArgNum() { return formals->len(); }            \
Symbol GetName() { return name; }                     \
void code(ostream&, CgenNode*);

#define attr_EXTRAS                                   \
Symbol GetName() { return name; }                     \
Symbol GetType() { return type_decl; }

#define Formal_EXTRAS                              \
virtual void dump_with_types(ostream&,int) = 0; \
virtual Symbol GetName() = 0;

#define formal_EXTRAS                           \
void dump_with_types(ostream&,int);             \
Symbol GetName() { return name; }

#define Case_EXTRAS                             \
virtual void dump_with_types(ostream& ,int) = 0;

#define branch_EXTRAS                                   \
void dump_with_types(ostream& ,int);

#define Expression_EXTRAS                    \
Symbol type;                                 \
Symbol get_type() { return type; }           \
Expression set_type(Symbol s) { type = s; return this; } \
virtual void code(ostream&, Environment) = 0; \
virtual void dump_with_types(ostream&,int) = 0;  \
void dump_type(ostream&, int);               \
Expression_class() { type = (Symbol) NULL; }

#define Expression_SHARED_EXTRAS           \
void code(ostream&, Environment); 			   \
void dump_with_types(ostream&,int); 

#define dispatch_EXTRAS \
std::vector<Expression> GetActuals() { \
    std::vector<Expression> ret; \
    for (int i = actual->first(); actual->more(i); i = actual->next(i)) { \
        ret.push_back(actual->nth(i)); \
    } \
    return ret; \
}

#define static_dispatch_EXTRAS \
std::vector<Expression> GetActuals() { \
    std::vector<Expression> ret; \
    for (int i = actual->first(); actual->more(i); i = actual->next(i)) { \
        ret.push_back(actual->nth(i)); \
    } \
    return ret; \
}

#define typcase_EXTRAS \
std::vector<Case> GetCases() { \
    std::vector<Case> ret; \
    for (int i = cases->first(); cases->more(i); i = cases->next(i)) { \
        ret.push_back(cases->nth(i)); \
    } \
    return ret; \
}

#endif
