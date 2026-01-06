#include <assert.h>
#include <stdio.h>
#include "emit.h"
#include "cool-tree.h"
#include "symtab.h"
#include <vector>
#include <map>

enum Basicness     {Basic, NotBasic};
#define TRUE 1
#define FALSE 0

class CgenClassTable;
typedef CgenClassTable *CgenClassTableP;

class CgenNode;
typedef CgenNode *CgenNodeP;

class CgenClassTable : public SymbolTable<Symbol,CgenNode> {
private:
   List<CgenNode> *nds;
   ostream& str;
   int stringclasstag;
   int intclasstag;
   int boolclasstag;


// The following methods emit code for
// constants and global declarations.

   void code_global_data();
   void code_global_text();
   void code_bools(int);
   void code_select_gc();
   void code_constants();
   
   void code_class_nameTab();
   void code_class_objTab();
   void code_dispatchTabs();
   void code_protObjs();
   void code_class_inits();
   void code_class_methods();

// The following creates an inheritance graph from
// a list of classes.  The graph is implemented as
// a tree of `CgenNode', and class names are placed
// in the base class symbol table.

   void install_basic_classes();
   void install_class(CgenNodeP nd);
   void install_classes(Classes cs);
   void build_inheritance_tree();
   void set_relations(CgenNodeP nd);
   void AssignTags(CgenNodeP node, int& tag);
public:
   CgenClassTable(Classes, ostream& str);
   void code();
   CgenNodeP root();
   
   CgenNodeP GetClassNode(Symbol name) { 
       CgenNodeP n = probe(name);
       if (!n) {
           std::cerr << "FATAL: Class " << name->get_string() << " not found!" << std::endl;
           exit(1);
       }
       return n; 
   }
   std::vector<CgenNodeP> GetClassNodes();
};


class CgenNode : public class__class {
private: 
   CgenNodeP parentnd;                        // Parent of class
   List<CgenNode> *children;                  // Children of class
   Basicness basic_status;                    // `Basic' if class is basic
                                              // `NotBasic' otherwise
   int class_tag;
   int max_child_tag;

public:
   CgenNode(Class_ c,
            Basicness bstatus,
            CgenClassTableP class_table);

   void add_child(CgenNodeP child);
   List<CgenNode> *get_children() { return children; }
   void set_parentnd(CgenNodeP p);
   CgenNodeP get_parentnd() { return parentnd; }
   int basic() { return (basic_status == Basic); }
   
   void set_tag(int t) { class_tag = t; }
   int get_tag() { return class_tag; }
   void set_max_child_tag(int t) { max_child_tag = t; }
   int get_max_child_tag() { return max_child_tag; }
   
   std::vector<method_class*> GetMethods();
   std::vector<method_class*> GetFullMethods();
   std::map<Symbol, int> GetDispatchIdxTab();
   
   std::vector<attr_class*> GetAttribs();
   std::vector<attr_class*> GetFullAttribs();
   std::map<Symbol, int> GetAttribIdxTab();
   std::vector<std::pair<Symbol, Symbol> > GetDispatchInfo();
   
   void code_protObj(ostream& s);
   void code_init(ostream& s);
   void code_methods(ostream& s);
};

class BoolConst 
{
 private: 
  int val;
 public:
  BoolConst(int);
  void code_def(ostream&, int boolclasstag);
  void code_ref(ostream&) const;
};
