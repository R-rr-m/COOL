
//**************************************************************
//
// Code generator SKELETON
//
// Read the comments carefully. Make sure to
//    initialize the base class tags in
//       `CgenClassTable::CgenClassTable'
//
//    Add the label for the dispatch tables to
//       `IntEntry::code_def'
//       `StringEntry::code_def'
//       `BoolConst::code_def'
//
//    Add code to emit everyting else that is needed
//       in `CgenClassTable::code'
//
//
// The files as provided will produce code to begin the code
// segments, declare globals, and emit constants.  You must
// fill in the rest.
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <vector>
#include <algorithm>

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;

CgenClassTable *codegen_classtable = NULL;

int labelnum = 0;

//
// Three symbols from the semantic analyzer (semant.cc) are used.
// If e : No_type, then no code is generated for e.
// Special code is generated for new SELF_TYPE.
// The name "self" also generates code different from other references.
//
//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
       arg,
       arg2,
       Bool,
       concat,
       cool_abort,
       copy,
       Int,
       in_int,
       in_string,
       IO,
       length,
       Main,
       main_meth,
       No_class,
       No_type,
       Object,
       out_int,
       out_string,
       prim_slot,
       self,
       SELF_TYPE,
       Str,
       str_field,
       substr,
       type_name,
       val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
  arg         = idtable.add_string("arg");
  arg2        = idtable.add_string("arg2");
  Bool        = idtable.add_string("Bool");
  concat      = idtable.add_string("concat");
  cool_abort  = idtable.add_string("abort");
  copy        = idtable.add_string("copy");
  Int         = idtable.add_string("Int");
  in_int      = idtable.add_string("in_int");
  in_string   = idtable.add_string("in_string");
  IO          = idtable.add_string("IO");
  length      = idtable.add_string("length");
  Main        = idtable.add_string("Main");
  main_meth   = idtable.add_string("main");
//   _no_class is a symbol that can't be the name of any 
//   user-defined class.
  No_class    = idtable.add_string("_no_class");
  No_type     = idtable.add_string("_no_type");
  Object      = idtable.add_string("Object");
  out_int     = idtable.add_string("out_int");
  out_string  = idtable.add_string("out_string");
  prim_slot   = idtable.add_string("_prim_slot");
  self        = idtable.add_string("self");
  SELF_TYPE   = idtable.add_string("SELF_TYPE");
  Str         = idtable.add_string("String");
  str_field   = idtable.add_string("_str_field");
  substr      = idtable.add_string("substr");
  type_name   = idtable.add_string("type_name");
  val         = idtable.add_string("_val");
}

static char *gc_init_names[] =
  { "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static char *gc_collect_names[] =
  { "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };


//  BoolConst is a class that implements code generation for operations
//  on the two booleans, which are given global names here.
BoolConst falsebool(FALSE);
BoolConst truebool(TRUE);

//*********************************************************
//
// Define method for code generation
//
// This is the method called by the compiler driver
// `cgtest.cc'. cgen takes an `ostream' to which the assembly will be
// emmitted, and it passes this and the class list of the
// code generator tree to the constructor for `CgenClassTable'.
// That constructor performs all of the work of the code
// generator.
//
//*********************************************************

void program_class::cgen(ostream &os) 
{
  if (cgen_debug) std::cerr << "Starting cgen..." << std::endl;
  // spim wants comments to start with '#'
  os << "# start of generated code\n";

  initialize_constants();
  if (cgen_debug) std::cerr << "Constants initialized." << std::endl;
  codegen_classtable = new CgenClassTable(classes,os);
  if (cgen_debug) std::cerr << "Class table built." << std::endl;

  os << "\n# end of generated code\n";
  if (cgen_debug) std::cerr << "Finished cgen." << std::endl;
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_load(char *dest_reg, int offset, char *source_reg, ostream& s)
{
  s << LW << dest_reg << " " << offset * WORD_SIZE << "(" << source_reg << ")" 
    << endl;
}

static void emit_store(char *source_reg, int offset, char *dest_reg, ostream& s)
{
  s << SW << source_reg << " " << offset * WORD_SIZE << "(" << dest_reg << ")"
      << endl;
}

static void emit_load_imm(char *dest_reg, int val, ostream& s)
{ s << LI << dest_reg << " " << val << endl; }

static void emit_load_address(char *dest_reg, char *address, ostream& s)
{ s << LA << dest_reg << " " << address << endl; }

static void emit_partial_load_address(char *dest_reg, ostream& s)
{ s << LA << dest_reg << " "; }

static void emit_load_bool(char *dest, const BoolConst& b, ostream& s)
{
  emit_partial_load_address(dest,s);
  b.code_ref(s);
  s << endl;
}

static void emit_load_string(char *dest, StringEntry *str, ostream& s)
{
  emit_partial_load_address(dest,s);
  str->code_ref(s);
  s << endl;
}

static void emit_load_int(char *dest, IntEntry *i, ostream& s)
{
  emit_partial_load_address(dest,s);
  i->code_ref(s);
  s << endl;
}

static void emit_move(char *dest_reg, char *source_reg, ostream& s)
{ s << MOVE << dest_reg << " " << source_reg << endl; }

static void emit_neg(char *dest, char *src1, ostream& s)
{ s << NEG << dest << " " << src1 << endl; }

static void emit_add(char *dest, char *src1, char *src2, ostream& s)
{ s << ADD << dest << " " << src1 << " " << src2 << endl; }

static void emit_addu(char *dest, char *src1, char *src2, ostream& s)
{ s << ADDU << dest << " " << src1 << " " << src2 << endl; }

static void emit_addiu(char *dest, char *src1, int imm, ostream& s)
{ s << ADDIU << dest << " " << src1 << " " << imm << endl; }

static void emit_div(char *dest, char *src1, char *src2, ostream& s)
{ s << DIV << dest << " " << src1 << " " << src2 << endl; }

static void emit_mul(char *dest, char *src1, char *src2, ostream& s)
{ s << MUL << dest << " " << src1 << " " << src2 << endl; }

static void emit_sub(char *dest, char *src1, char *src2, ostream& s)
{ s << SUB << dest << " " << src1 << " " << src2 << endl; }

static void emit_sll(char *dest, char *src1, int num, ostream& s)
{ s << SLL << dest << " " << src1 << " " << num << endl; }

static void emit_jalr(char *dest, ostream& s)
{ s << JALR << "\t" << dest << endl; }

static void emit_jal(const char *address,ostream &s)
{ s << JAL << address << endl; }

static void emit_return(ostream& s)
{ s << RET << endl; }

static void emit_gc_assign(ostream& s)
{ s << JAL << "_GenGC_Assign" << endl; }

static void emit_disptable_ref(Symbol sym, ostream& s)
{  s << sym << DISPTAB_SUFFIX; }

static void emit_init_ref(Symbol sym, ostream& s)
{ s << sym << CLASSINIT_SUFFIX; }

static void emit_label_ref(int l, ostream &s)
{ s << "label" << l; }

static void emit_protobj_ref(Symbol sym, ostream& s)
{ s << sym << PROTOBJ_SUFFIX; }

static void emit_method_ref(Symbol classname, Symbol methodname, ostream& s)
{ s << classname << METHOD_SEP << methodname; }

static void emit_label_def(int l, ostream &s)
{
  emit_label_ref(l,s);
  s << ":" << endl;
}

static void emit_beqz(char *source, int label, ostream &s)
{
  s << BEQZ << source << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_beq(char *src1, char *src2, int label, ostream &s)
{
  s << BEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bne(char *src1, char *src2, int label, ostream &s)
{
  s << BNE << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bleq(char *src1, char *src2, int label, ostream &s)
{
  s << BLEQ << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blt(char *src1, char *src2, int label, ostream &s)
{
  s << BLT << src1 << " " << src2 << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_blti(char *src1, int imm, int label, ostream &s)
{
  s << BLT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_bgti(char *src1, int imm, int label, ostream &s)
{
  s << BGT << src1 << " " << imm << " ";
  emit_label_ref(label,s);
  s << endl;
}

static void emit_branch(int l, ostream& s)
{
  s << BRANCH;
  emit_label_ref(l,s);
  s << endl;
}

//
// Push a register on the stack. The stack grows towards smaller addresses.
//
static void emit_push(char *reg, ostream& str)
{
  emit_store(reg,0,SP,str);
  emit_addiu(SP,SP,-4,str);
}

//
// Fetch the integer value in an Int object.
// Emits code to fetch the integer value of the Integer object pointed
// to by register source into the register dest
//
static void emit_fetch_int(char *dest, char *source, ostream& s)
{ emit_load(dest, DEFAULT_OBJFIELDS, source, s); }

//
// Emits code to store the integer value contained in register source
// into the Integer object pointed to by dest.
//
static void emit_store_int(char *source, char *dest, ostream& s)
{ emit_store(source, DEFAULT_OBJFIELDS, dest, s); }


static void emit_test_collector(ostream &s)
{
  emit_push(ACC, s);
  emit_move(ACC, SP, s); // stack end
  emit_move(A1, ZERO, s); // allocate nothing
  s << JAL << gc_collect_names[cgen_Memmgr] << endl;
  emit_addiu(SP,SP,4,s);
  emit_load(ACC,0,SP,s);
}

static void emit_gc_check(char *source, ostream &s)
{
  if (source != (char*)A1) emit_move(A1, source, s);
  s << JAL << "_gc_check" << endl;
}


///////////////////////////////////////////////////////////////////////////////
//
// Environment implementation
//
///////////////////////////////////////////////////////////////////////////////

void Environment::EnterScope() {
    var_scopes.push_back(std::vector<Symbol>());
}

void Environment::ExitScope() {
    var_scopes.pop_back();
}

void Environment::AddVar(Symbol sym) {
    var_scopes.back().push_back(sym);
}

void Environment::AddParam(Symbol sym) {
    params.push_back(sym);
}

void Environment::AddObstacle() {
    obstacle_count++;
}

int Environment::LookUpVar(Symbol sym) {
    int offset = obstacle_count;
    for (int i = var_scopes.size() - 1; i >= 0; --i) {
        for (int j = var_scopes[i].size() - 1; j >= 0; --j) {
            if (var_scopes[i][j] == sym) {
                return offset;
            }
            offset++;
        }
    }
    return -1;
}

int Environment::LookUpParam(Symbol sym) {
    for (size_t i = 0; i < params.size(); ++i) {
        if (params[i] == sym) {
            return params.size() - 1 - i;
        }
    }
    return -1;
}

int Environment::LookUpAttrib(Symbol sym) {
    std::map<Symbol, int> idx_tab = m_class_node->GetAttribIdxTab();
    if (idx_tab.find(sym) != idx_tab.end()) {
        return idx_tab[sym];
    }
    return -1;
}


///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Cool has three kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// All string constants are listed in the global "stringtable" and have
// type StringEntry.  StringEntry methods are defined both for String
// constant definitions and references.
//
// All integer constants are listed in the global "inttable" and have
// type IntEntry.  IntEntry methods are defined for Int
// constant definitions and references.
//
// Since there are only two Bool values, there is no need for a table.
// The two booleans are represented by instances of the class BoolConst,
// which defines the definition and reference methods for Bools.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << STRCONST_PREFIX << index;
}

//
// Emit code for a constant String.
// You should fill in the code naming the dispatch table.
//

void StringEntry::code_def(ostream& s, int stringclasstag)
{
  IntEntryP lensym = inttable.add_int(len);

  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s  << LABEL                                             // label
      << WORD << stringclasstag << endl                                 // tag
      << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (len+4)/4) << endl // size
      << WORD;


 /***** Add dispatch information for class String ******/

      s << "String" << DISPTAB_SUFFIX << endl;               // dispatch table
      s << WORD;  lensym->code_ref(s);  s << endl;            // string length
  emit_string_constant(s,str);                                // ascii string
  s << ALIGN;                                                 // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s, int stringclasstag)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,stringclasstag);
}

//
// Ints
//
void IntEntry::code_ref(ostream &s)
{
  s << INTCONST_PREFIX << index;
}

//
// Emit code for a constant Integer.
// You should fill in the code naming the dispatch table.
//

void IntEntry::code_def(ostream &s, int intclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                // label
      << WORD << intclasstag << endl                      // class tag
      << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << endl  // object size
      << WORD; 

 /***** Add dispatch information for class Int ******/

      s << "Int" << DISPTAB_SUFFIX << endl;               // dispatch table
      s << WORD << str << endl;                           // integer value
}


//
// IntTable::code_string_table
// Generate an Int object definition for every Int constant in the
// inttable.
//
void IntTable::code_string_table(ostream &s, int intclasstag)
{
  for (List<IntEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s,intclasstag);
}


//
// Bools
//
BoolConst::BoolConst(int i) : val(i) { assert(i == 0 || i == 1); }

void BoolConst::code_ref(ostream& s) const
{
  s << BOOLCONST_PREFIX << val;
}
  
//
// Emit code for a constant Bool.
// You should fill in the code naming the dispatch table.
//

void BoolConst::code_def(ostream& s, int boolclasstag)
{
  // Add -1 eye catcher
  s << WORD << "-1" << endl;

  code_ref(s);  s << LABEL                                  // label
      << WORD << boolclasstag << endl                       // class tag
      << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << endl   // object size
      << WORD;

 /***** Add dispatch information for class Bool ******/

      s << "Bool" << DISPTAB_SUFFIX << endl;                // dispatch table
      s << WORD << val << endl;                             // value (0 or 1)
}

//////////////////////////////////////////////////////////////////////////////
//
//  CgenClassTable methods
//
//////////////////////////////////////////////////////////////////////////////

//***************************************************
//
//  Emit code to start the .data segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_data()
{
  Symbol main    = idtable.lookup_string(MAINNAME);
  Symbol string  = idtable.lookup_string(STRINGNAME);
  Symbol integer = idtable.lookup_string(INTNAME);
  Symbol boolc   = idtable.lookup_string(BOOLNAME);

  str << "\t.data\n" << ALIGN;
  //
  // The following global names must be defined first.
  //
  str << GLOBAL << CLASSNAMETAB << endl;
  str << GLOBAL; emit_protobj_ref(main,str);    str << endl;
  str << GLOBAL; emit_protobj_ref(integer,str); str << endl;
  str << GLOBAL; emit_protobj_ref(string,str);  str << endl;
  str << GLOBAL; falsebool.code_ref(str);  str << endl;
  str << GLOBAL; truebool.code_ref(str);   str << endl;
  str << GLOBAL << INTTAG << endl;
  str << GLOBAL << BOOLTAG << endl;
  str << GLOBAL << STRINGTAG << endl;

  //
  // We also need to know the tag of the Int, String, and Bool classes
  // during code generation.
  //
  str << INTTAG << LABEL
      << WORD << intclasstag << endl;
  str << BOOLTAG << LABEL 
      << WORD << boolclasstag << endl;
  str << STRINGTAG << LABEL 
      << WORD << stringclasstag << endl;    
}


//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************

void CgenClassTable::code_global_text()
{
  str << GLOBAL << HEAP_START << endl
      << HEAP_START << LABEL 
      << WORD << 0 << endl
      << "\t.text" << endl
      << GLOBAL;
  emit_init_ref(idtable.add_string("Main"), str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Int"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("String"),str);
  str << endl << GLOBAL;
  emit_init_ref(idtable.add_string("Bool"),str);
  str << endl << GLOBAL;
  emit_method_ref(idtable.add_string("Main"), idtable.add_string("main"), str);
  str << endl;
}

void CgenClassTable::code_bools(int boolclasstag)
{
  falsebool.code_def(str,boolclasstag);
  truebool.code_def(str,boolclasstag);
}

void CgenClassTable::code_select_gc()
{
  //
  // Generate GC choice constants (pointers to GC functions)
  //
  str << GLOBAL << "_MemMgr_INITIALIZER" << endl;
  str << "_MemMgr_INITIALIZER:" << endl;
  str << WORD << gc_init_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_COLLECTOR" << endl;
  str << "_MemMgr_COLLECTOR:" << endl;
  str << WORD << gc_collect_names[cgen_Memmgr] << endl;
  str << GLOBAL << "_MemMgr_TEST" << endl;
  str << "_MemMgr_TEST:" << endl;
  str << WORD << (cgen_Memmgr_Test == GC_TEST) << endl;
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************

void CgenClassTable::code_constants()
{
  //
  // Add constants that are required by the code generator.
  //
  stringtable.add_string("");
  inttable.add_string("0");

  stringtable.code_string_table(str,stringclasstag);
  inttable.code_string_table(str,intclasstag);
  code_bools(boolclasstag);
}

// Helpers
std::vector<CgenNodeP> CgenClassTable::GetClassNodes() {
    std::vector<CgenNodeP> nodes;
    for (List<CgenNode> *l = nds; l; l = l->tl()) {
        nodes.push_back(l->hd());
    }
    // Sort to ensure deterministic order (e.g. by name or something, but list order is reverse install order)
    // To match official compiler, maybe we need specific order.
    // The list 'nds' has nodes in reverse order of installation.
    // Let's reverse it to get install order.
    std::reverse(nodes.begin(), nodes.end());
    return nodes;
}


CgenClassTable::CgenClassTable(Classes classes, ostream& s) : nds(NULL) , str(s)
{
   codegen_classtable = this; // Initialize global pointer
   enterscope();
   std::cerr << "Building CgenClassTable..." << std::endl;
   install_basic_classes();
   std::cerr << "Installed basic classes." << std::endl;
   install_classes(classes);
   std::cerr << "Installed user classes." << std::endl;
   build_inheritance_tree();
   std::cerr << "Built inheritance tree." << std::endl;
   
   // Assign tags using DFS
   int tag = 0;
   std::cerr << "Assigning tags..." << std::endl;
   CgenNodeP r = root();
   if (!r) { std::cerr << "FATAL: Root (Object) not found!" << std::endl; exit(1); }
   AssignTags(r, tag);
   std::cerr << "Tags assigned. Max tag: " << tag << std::endl;

   std::cerr << "Generating code..." << std::endl;
   code();
   std::cerr << "Code generation complete." << std::endl;
   exitscope();
}

void CgenClassTable::AssignTags(CgenNodeP node, int& tag) {
    node->set_tag(tag);
    if (node->get_name() == Str) stringclasstag = tag;
    if (node->get_name() == Int) intclasstag = tag;
    if (node->get_name() == Bool) boolclasstag = tag;
    tag++;
    
    for (List<CgenNode> *l = node->get_children(); l; l = l->tl()) {
        AssignTags(l->hd(), tag);
    }
    node->set_max_child_tag(tag - 1);
}

void CgenClassTable::install_basic_classes()
{

// The tree package uses these globals to annotate the classes built below.
  //curr_lineno  = 0;
  Symbol filename = stringtable.add_string("<basic class>");

//
// A few special class names are installed in the lookup table but not
// the class list.  Thus, these classes exist, but are not part of the
// inheritance hierarchy.
// No_class serves as the parent of Object and the other special classes.
// SELF_TYPE is the self class; it cannot be redefined or inherited.
// prim_slot is a class known to the code generator.
//
  addid(No_class,
	new CgenNode(class_(No_class,No_class,nil_Features(),filename),
			    Basic,this));
  addid(SELF_TYPE,
	new CgenNode(class_(SELF_TYPE,No_class,nil_Features(),filename),
			    Basic,this));
  addid(prim_slot,
	new CgenNode(class_(prim_slot,No_class,nil_Features(),filename),
			    Basic,this));

// 
// The Object class has no parent class. Its methods are
//        cool_abort() : Object    aborts the program
//        type_name() : Str        returns a string representation of class name
//        copy() : SELF_TYPE       returns a copy of the object
//
// There is no need for method bodies in the basic classes---these
// are already built in to the runtime system.
//
  install_class(
   new CgenNode(
    class_(Object, 
	   No_class,
	   append_Features(
           append_Features(
           single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
           single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
           single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	   filename),
    Basic,this));

// 
// The IO class inherits from Object. Its methods are
//        out_string(Str) : SELF_TYPE          writes a string to the output
//        out_int(Int) : SELF_TYPE               "    an int    "  "     "
//        in_string() : Str                    reads a string from the input
//        in_int() : Int                         "   an int     "  "     "
//
   install_class(
    new CgenNode(
     class_(IO, 
            Object,
            append_Features(
            append_Features(
            append_Features(
            single_Features(method(out_string, single_Formals(formal(arg, Str)),
                        SELF_TYPE, no_expr())),
            single_Features(method(out_int, single_Formals(formal(arg, Int)),
                        SELF_TYPE, no_expr()))),
            single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
            single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	   filename),	    
    Basic,this));

//
// The Int class has no methods and only a single attribute, the
// "val" for the integer. 
//
   install_class(
    new CgenNode(
     class_(Int, 
	    Object,
            single_Features(attr(val, prim_slot, no_expr())),
	    filename),
     Basic,this));

//
// Bool also has only the "val" slot.
//
    install_class(
     new CgenNode(
      class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename),
      Basic,this));

//
// The class Str has a number of slots and operations:
//       val                                  ???
//       str_field                            the string itself
//       length() : Int                       length of the string
//       concat(arg: Str) : Str               string concatenation
//       substr(arg: Int, arg2: Int): Str     substring
//       
   install_class(
    new CgenNode(
      class_(Str, 
	     Object,
             append_Features(
             append_Features(
             append_Features(
             append_Features(
             single_Features(attr(val, Int, no_expr())),
            single_Features(attr(str_field, prim_slot, no_expr()))),
            single_Features(method(length, nil_Formals(), Int, no_expr()))),
            single_Features(method(concat, 
				   single_Formals(formal(arg, Str)),
				   Str, 
				   no_expr()))),
	    single_Features(method(substr, 
				   append_Formals(single_Formals(formal(arg, Int)), 
						  single_Formals(formal(arg2, Int))),
				   Str, 
				   no_expr()))),
	     filename),
        Basic,this));

}

// CgenClassTable::install_class
// CgenClassTable::install_classes
//
// install_classes enters a list of classes in the symbol table.
//
void CgenClassTable::install_class(CgenNodeP nd)
{
  Symbol name = nd->get_name();

  if (probe(name))
    {
      return;
    }

  // The class name is legal, so add it to the list of classes
  // and the symbol table.
  nds = new List<CgenNode>(nd,nds);
  addid(name,nd);
}

void CgenClassTable::install_classes(Classes cs)
{
  for(int i = cs->first(); cs->more(i); i = cs->next(i))
    install_class(new CgenNode(cs->nth(i),NotBasic,this));
}

//
// CgenClassTable::build_inheritance_tree
//
void CgenClassTable::build_inheritance_tree()
{
  for(List<CgenNode> *l = nds; l; l = l->tl())
      set_relations(l->hd());
}

//
// CgenClassTable::set_relations
//
// Takes a CgenNode and locates its, and its parent's, inheritance nodes
// via the class table.  Parent and child pointers are added as appropriate.
//
void CgenClassTable::set_relations(CgenNodeP nd)
{
  CgenNode *parent_node = probe(nd->get_parent());
  nd->set_parentnd(parent_node);
  parent_node->add_child(nd);
}

void CgenNode::add_child(CgenNodeP n)
{
  children = new List<CgenNode>(n,children);
}

void CgenNode::set_parentnd(CgenNodeP p)
{
  assert(parentnd == NULL);
  assert(p != NULL);
  parentnd = p;
}



void CgenClassTable::code()
{
  std::cerr << "coding global data" << std::endl;
  code_global_data();

  std::cerr << "choosing gc" << std::endl;
  code_select_gc();

  std::cerr << "coding constants" << std::endl;
  code_constants();

//                 Add your code to emit
//                   - prototype objects
//                   - class_nameTab
//                   - dispatch tables
//
  std::cerr << "coding class_nameTab" << std::endl;
  code_class_nameTab();
  std::cerr << "coding class_objTab" << std::endl;
  code_class_objTab();
  std::cerr << "coding dispatchTabs" << std::endl;
  code_dispatchTabs();
  std::cerr << "coding protObjs" << std::endl;
  code_protObjs();

  std::cerr << "coding global text" << std::endl;
  code_global_text();

//                 Add your code to emit
//                   - object initializer
//                   - the class methods
//                   - etc...
  std::cerr << "coding class_inits" << std::endl;
  code_class_inits();
  std::cerr << "coding class_methods" << std::endl;
  code_class_methods();

}


CgenNodeP CgenClassTable::root()
{
   return probe(Object);
}

void CgenClassTable::code_class_nameTab() {
    str << CLASSNAMETAB << LABEL;
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        Symbol class_name = class_node->get_name();
        StringEntry* str_entry = stringtable.lookup_string(class_name->get_string());
        str << WORD;
        str_entry->code_ref(str);
        str << endl;
    }
}

void CgenClassTable::code_class_objTab() {
    str << CLASSOBJTAB << LABEL;
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        str << WORD;
        emit_protobj_ref(class_node->get_name(), str);
        str << endl;
        str << WORD;
        emit_init_ref(class_node->get_name(), str);
        str << endl;
    }
}

void CgenClassTable::code_dispatchTabs() {
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        emit_disptable_ref(class_node->get_name(), str);
        str << LABEL;
        std::vector<std::pair<Symbol, Symbol> > dispatch_info; // method_name -> class_name
        
        // Collect methods including inherited ones
        // We can use a recursive approach or just GetFullMethods if it returns implementation class.
        // Let's rely on CgenNode::GetDispatchInfo
        std::vector<std::pair<Symbol, Symbol> > methods = class_node->GetDispatchInfo();
        
        for (size_t i = 0; i < methods.size(); ++i) {
            str << WORD;
            emit_method_ref(methods[i].second, methods[i].first, str);
            str << endl;
        }
    }
}

void CgenClassTable::code_protObjs() {
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        class_node->code_protObj(str);
    }
}

void CgenClassTable::code_class_inits() {
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        class_node->code_init(str);
    }
}

void CgenClassTable::code_class_methods() {
    std::vector<CgenNodeP> class_nodes = GetClassNodes();
    for (CgenNodeP class_node : class_nodes) {
        if (!class_node->basic()) {
            std::cerr << "Generating methods for class: " << class_node->get_name()->get_string() << std::endl;
            class_node->code_methods(str);
        }
    }
}


///////////////////////////////////////////////////////////////////////
//
// CgenNode methods
//
///////////////////////////////////////////////////////////////////////

CgenNode::CgenNode(Class_ nd, Basicness bstatus, CgenClassTableP ct) :
   class__class((const class__class &) *nd),
   parentnd(NULL),
   children(NULL),
   basic_status(bstatus)
{ 
   stringtable.add_string(name->get_string());          // Add class name to string table
}

std::vector<attr_class*> CgenNode::GetAttribs() {
    std::vector<attr_class*> attribs;
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        if (dynamic_cast<attr_class*>(features->nth(i))) {
            attribs.push_back((attr_class*)features->nth(i));
        }
    }
    return attribs;
}

std::vector<attr_class*> CgenNode::GetFullAttribs() {
    std::vector<attr_class*> attribs;
    if (parentnd && name != Object) {
        attribs = parentnd->GetFullAttribs();
    }
    std::vector<attr_class*> my_attribs = GetAttribs();
    attribs.insert(attribs.end(), my_attribs.begin(), my_attribs.end());
    return attribs;
}

std::map<Symbol, int> CgenNode::GetAttribIdxTab() {
    std::vector<attr_class*> attribs = GetFullAttribs();
    std::map<Symbol, int> idx_tab;
    for (size_t i = 0; i < attribs.size(); ++i) {
        idx_tab[attribs[i]->GetName()] = i;
    }
    return idx_tab;
}

std::vector<method_class*> CgenNode::GetMethods() {
    std::vector<method_class*> methods;
    for (int i = features->first(); features->more(i); i = features->next(i)) {
        if (dynamic_cast<method_class*>(features->nth(i))) {
            methods.push_back((method_class*)features->nth(i));
        }
    }
    return methods;
}

// Returns method name and class name where it is defined
std::vector<std::pair<Symbol, Symbol> > CgenNode::GetDispatchInfo() {
    std::vector<std::pair<Symbol, Symbol> > info;
    if (parentnd && name != Object) {
        info = parentnd->GetDispatchInfo();
    }
    
    std::vector<method_class*> my_methods = GetMethods();
    for (method_class* method : my_methods) {
        // Check if overrides
        bool overridden = false;
        for (size_t i = 0; i < info.size(); ++i) {
            if (info[i].first == method->GetName()) {
                info[i].second = name;
                overridden = true;
                break;
            }
        }
        if (!overridden) {
            info.push_back(std::make_pair(method->GetName(), name));
        }
    }
    return info;
}

std::vector<method_class*> CgenNode::GetFullMethods() {
    // This is not enough for dispatch table generation because we need to know the defining class
    // Use GetDispatchInfo instead.
    // But for index table, we just need names.
    return std::vector<method_class*>(); // Placeholder
}

std::map<Symbol, int> CgenNode::GetDispatchIdxTab() {
    std::vector<std::pair<Symbol, Symbol> > info = GetDispatchInfo();
    std::map<Symbol, int> idx_tab;
    for (size_t i = 0; i < info.size(); ++i) {
        idx_tab[info[i].first] = i;
    }
    return idx_tab;
}


void CgenNode::code_protObj(ostream& s) {
    std::vector<attr_class*> attribs = GetFullAttribs();
    
    s << WORD << "-1" << endl;  // GC
    s << name << PROTOBJ_SUFFIX << LABEL;
    s << WORD << class_tag << "\t# class tag" << endl;
    s << WORD << (DEFAULT_OBJFIELDS + attribs.size()) << "\t# size" << endl;
    s << WORD << name << DISPTAB_SUFFIX << endl;
    
    for (attr_class* attr : attribs) {
        Symbol type = attr->GetType();
        if (type == Int) {
            s << WORD;
            inttable.lookup_string("0")->code_ref(s);
            s << "\t# int(0)" << endl;
        } else if (type == Bool) {
            s << WORD;
            falsebool.code_ref(s);
            s << "\t# bool(0)" << endl;
        } else if (type == Str) {
            s << WORD;
            stringtable.lookup_string("")->code_ref(s);
            s << "\t# str()" << endl;
        } else {
            s << WORD << "0\t# void" << endl;
        }
    }
}

void CgenNode::code_init(ostream& s) {
    s << name << CLASSINIT_SUFFIX << LABEL;
    
    emit_addiu(SP, SP, -12, s);
    emit_store(FP, 3, SP, s);
    emit_store(SELF, 2, SP, s);
    emit_store(RA, 1, SP, s);
    emit_addiu(FP, SP, 4, s);
    emit_move(SELF, ACC, s);
    
    if (parentnd && name != Object) {
        emit_jal((std::string(parentnd->get_name()->get_string()) + CLASSINIT_SUFFIX).c_str(), s);
    }
    
    std::vector<attr_class*> attribs = GetAttribs();
    // We need indices relative to FULL attributes
    std::map<Symbol, int> idx_tab = GetAttribIdxTab();
    
    for (attr_class* attr : attribs) {
        int idx = idx_tab[attr->GetName()];
        if (!dynamic_cast<no_expr_class*>(attr->init)) {
            Environment env;
            env.m_class_node = this;
            attr->init->code(s, env);
            
            // LookUpAttrib logic: idx + 3
            emit_store(ACC, 3 + idx, SELF, s);
            
            if (cgen_Memmgr == 1) { // GC
                emit_addiu(A1, SELF, 4 * (3 + idx), s);
                emit_gc_assign(s);
            }
        }
    }
    
    emit_move(ACC, SELF, s);
    
    emit_load(FP, 3, SP, s);
    emit_load(SELF, 2, SP, s);
    emit_load(RA, 1, SP, s);
    emit_addiu(SP, SP, 12, s);
    emit_return(s);
}

void CgenNode::code_methods(ostream& s) {
    std::vector<method_class*> methods = GetMethods();
    for (method_class* method : methods) {
        std::cerr << "  Generating method: " << method->GetName()->get_string() << std::endl;
        method->code(s, this);
    }
}


//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `cool-tree.h'  Sample code for
//   constant integers, strings, and booleans are provided.
//
//*****************************************************************

// Helper for method code
void method_class::code(ostream& s, CgenNode* class_node) {
    emit_method_ref(class_node->get_name(), name, s);
    s << LABEL;
    
    emit_addiu(SP, SP, -12, s);
    emit_store(FP, 3, SP, s);
    emit_store(SELF, 2, SP, s);
    emit_store(RA, 1, SP, s);
    emit_addiu(FP, SP, 4, s);
    emit_move(SELF, ACC, s);
    
    Environment env;
    env.m_class_node = class_node;
    for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
        env.AddParam(formals->nth(i)->GetName());
    }
    
    expr->code(s, env);
    
    emit_load(FP, 3, SP, s);
    emit_load(SELF, 2, SP, s);
    emit_load(RA, 1, SP, s);
    emit_addiu(SP, SP, 12, s);
    
    int arg_num = GetArgNum();
    emit_addiu(SP, SP, arg_num * 4, s);
    
    emit_return(s);
}


void assign_class::code(ostream &s, Environment env) {
    expr->code(s, env);
    
    int idx;
    if ((idx = env.LookUpVar(name)) != -1) {
        emit_store(ACC, idx + 1, SP, s);
        if (cgen_Memmgr == 1) {
             emit_addiu(A1, SP, 4 * (idx + 1), s);
             emit_gc_assign(s);
        }
    } else if ((idx = env.LookUpParam(name)) != -1) {
        emit_store(ACC, idx + 3, FP, s);
        if (cgen_Memmgr == 1) {
             emit_addiu(A1, FP, 4 * (idx + 3), s);
             emit_gc_assign(s);
        }
    } else if ((idx = env.LookUpAttrib(name)) != -1) {
        emit_store(ACC, idx + 3, SELF, s);
        if (cgen_Memmgr == 1) {
            emit_addiu(A1, SELF, 4 * (idx + 3), s);
            emit_gc_assign(s);
        }
    }
}

void static_dispatch_class::code(ostream &s, Environment env) {
    std::vector<Expression> actuals = GetActuals();
    for (size_t i = 0; i < actuals.size(); ++i) {
        actuals[i]->code(s, env);
        emit_push(ACC, s);
        env.AddObstacle();
    }
    
    expr->code(s, env);
    
    int label_not_void = labelnum++;
    emit_bne(ACC, ZERO, label_not_void, s);
    emit_load_string(ACC, stringtable.lookup_string(env.m_class_node->get_filename()->get_string()), s);
    emit_load_imm(T1, 1, s); // Line number? We don't have line number easily here without modifying tree or using global.
    // The skeleton doesn't pass line number. But tree_node has line_number.
    // emit_load_imm(T1, line_number, s); 
    // tree_node has get_line_number().
    emit_load_imm(T1, get_line_number(), s);
    emit_jal("_dispatch_abort", s);
    emit_label_def(label_not_void, s);
    
    // Static dispatch: use type_name to find dispatch table
    std::string dispatch_tab = std::string(type_name->get_string()) + DISPTAB_SUFFIX;
    emit_load_address(T1, (char*)dispatch_tab.c_str(), s);
    
    // Find method index
    CgenNodeP class_node = codegen_classtable->GetClassNode(type_name);
    int idx = class_node->GetDispatchIdxTab()[name];
    
    emit_load(T1, idx, T1, s);
    emit_jalr(T1, s);
}

void dispatch_class::code(ostream &s, Environment env) {
    std::vector<Expression> actuals = GetActuals();
    for (size_t i = 0; i < actuals.size(); ++i) {
        actuals[i]->code(s, env);
        emit_push(ACC, s);
        env.AddObstacle();
    }
    
    expr->code(s, env);
    
    int label_not_void = labelnum++;
    emit_bne(ACC, ZERO, label_not_void, s);
    emit_load_string(ACC, stringtable.lookup_string(env.m_class_node->get_filename()->get_string()), s);
    emit_load_imm(T1, get_line_number(), s);
    emit_jal("_dispatch_abort", s);
    emit_label_def(label_not_void, s);
    
    emit_load(T1, 2, ACC, s); // dispatch table
    
    Symbol type = expr->get_type();
    Symbol class_name = (type == SELF_TYPE) ? env.m_class_node->get_name() : type;
    
    CgenNodeP class_node = codegen_classtable->GetClassNode(class_name);
    int idx = class_node->GetDispatchIdxTab()[name];
    
    emit_load(T1, idx, T1, s);
    emit_jalr(T1, s);
}

void cond_class::code(ostream &s, Environment env) {
    pred->code(s, env);
    emit_fetch_int(T1, ACC, s);
    
    int label_false = labelnum++;
    int label_finish = labelnum++;
    
    emit_beq(T1, ZERO, label_false, s);
    
    then_exp->code(s, env);
    emit_branch(label_finish, s);
    
    emit_label_def(label_false, s);
    else_exp->code(s, env);
    
    emit_label_def(label_finish, s);
}

void loop_class::code(ostream &s, Environment env) {
    int label_start = labelnum++;
    int label_finish = labelnum++;
    
    emit_label_def(label_start, s);
    pred->code(s, env);
    emit_fetch_int(T1, ACC, s);
    emit_beq(T1, ZERO, label_finish, s);
    
    body->code(s, env);
    emit_branch(label_start, s);
    
    emit_label_def(label_finish, s);
    emit_move(ACC, ZERO, s);
}

void typcase_class::code(ostream &s, Environment env) {
    expr->code(s, env);
    
    int label_not_void = labelnum++;
    emit_bne(ACC, ZERO, label_not_void, s);
    emit_load_string(ACC, stringtable.lookup_string(env.m_class_node->get_filename()->get_string()), s);
    emit_load_imm(T1, get_line_number(), s);
    emit_jal("_case_abort2", s);
    emit_label_def(label_not_void, s);
    
    emit_load(T1, 0, ACC, s); // T1 = class tag
    
    // Sort cases by class tag (descending order of inheritance depth? No, specific logic required)
    // The manual says: pick the closest ancestor.
    // A simple way: Iterate through cases, check if object is instance of case type.
    // If multiple match, pick the one with "smallest" distance (highest tag in that branch).
    // Better approach:
    // Generate code to check each case.
    // Since tags are DFS ordered (if we did it right), subclass tags are range [T, T + size].
    // But we need to find the "best" match.
    // Standard approach:
    // For each case branch, we know the Type.
    // We can check if object tag is descendant of Type tag.
    // If yes, this is a candidate.
    // Among candidates, pick the one with most specific type (largest tag usually, or deepest in tree).
    // Or, we can sort branches by type depth descending. The first match is the best match.
    
    // Let's sort cases.
    std::vector<Case> cases_vec = GetCases();
    
    // Sort by inheritance depth (descending).
    // So if we sort cases by depth descending, the first match is the best match.
    
    struct CaseSorter {
        CgenClassTable* ct;
        CaseSorter(CgenClassTable* c) : ct(c) {}
        bool operator()(Case a, Case b) {
            branch_class* ba = static_cast<branch_class*>(a);
            branch_class* bb = static_cast<branch_class*>(b);
            
            int depth_a = 0;
            CgenNodeP na = ct->GetClassNode(ba->type_decl);
            while (na->get_parentnd()) { depth_a++; na = na->get_parentnd(); }
            
            int depth_b = 0;
            CgenNodeP nb = ct->GetClassNode(bb->type_decl);
            while (nb->get_parentnd()) { depth_b++; nb = nb->get_parentnd(); }
            
            return depth_a > depth_b; // Descending
        }
    };
    
    std::sort(cases_vec.begin(), cases_vec.end(), CaseSorter(codegen_classtable));
    
    int label_finish = labelnum++;
    
    for (Case c : cases_vec) {
        int label_next = labelnum++;
        
        branch_class* branch = static_cast<branch_class*>(c);
        
        CgenNodeP type_node = codegen_classtable->GetClassNode(branch->type_decl);
        int tag = type_node->get_tag();
        int max_tag = type_node->get_max_child_tag();
        
        // Check if T1 (obj tag) is in range of type_node's subtree.
        // Range: [tag, max_tag]
        
        emit_blti(T1, tag, label_next, s);
        emit_bgti(T1, max_tag, label_next, s);
        
        // Match!
        
        // Push result to stack (bind variable)
        emit_push(ACC, s);
        env.EnterScope();
        env.AddVar(branch->name);
        
        branch->expr->code(s, env);
        
        env.ExitScope();
        emit_addiu(SP, SP, 4, s);
        emit_branch(label_finish, s);
        
        emit_label_def(label_next, s);
    }
    
    emit_jal("_case_abort", s);
    emit_label_def(label_finish, s);
}

void block_class::code(ostream &s, Environment env) {
    for (int i = body->first(); body->more(i); i = body->next(i)) {
        body->nth(i)->code(s, env);
    }
}

void let_class::code(ostream &s, Environment env) {
    init->code(s, env);
    
    if (dynamic_cast<no_expr_class*>(init)) {
        if (type_decl == Str) {
            emit_load_string(ACC, stringtable.lookup_string(""), s);
        } else if (type_decl == Int) {
            emit_load_int(ACC, inttable.lookup_string("0"), s);
        } else if (type_decl == Bool) {
            emit_load_bool(ACC, BoolConst(0), s);
        } else {
            emit_move(ACC, ZERO, s);
        }
    }
    
    emit_push(ACC, s);
    env.EnterScope();
    env.AddVar(identifier);
    
    body->code(s, env);
    
    env.ExitScope();
    emit_addiu(SP, SP, 4, s);
}

void plus_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_jal("Object.copy", s);
    
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    emit_add(T3, T1, T2, s);
    emit_store(T3, 3, ACC, s);
}

void sub_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_jal("Object.copy", s);
    
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    emit_sub(T3, T1, T2, s);
    emit_store(T3, 3, ACC, s);
}

void mul_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_jal("Object.copy", s);
    
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    emit_mul(T3, T1, T2, s);
    emit_store(T3, 3, ACC, s);
}

void divide_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_jal("Object.copy", s);
    
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    emit_div(T3, T1, T2, s);
    emit_store(T3, 3, ACC, s);
}

void neg_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_jal("Object.copy", s);
    emit_load(T1, 3, ACC, s);
    emit_neg(T1, T1, s);
    emit_store(T1, 3, ACC, s);
}

void lt_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    
    int label_true = labelnum++;
    int label_finish = labelnum++;
    
    emit_blt(T1, T2, label_true, s);
    emit_load_bool(ACC, BoolConst(0), s);
    emit_branch(label_finish, s);
    
    emit_label_def(label_true, s);
    emit_load_bool(ACC, BoolConst(1), s);
    
    emit_label_def(label_finish, s);
}

void eq_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_move(T2, ACC, s);
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    
    int label_true = labelnum++;
    int label_finish = labelnum++;
    
    // If T1 == T2 (pointers equal), true
    emit_beq(T1, T2, label_true, s);
    
    // Call equality_test
    // equality_test expects args in T1, T2. Returns Bool in A0 (ACC).
    // Wait, runtime equality_test:
    // It compares T1 and T2.
    // If they are basic types (Int, Str, Bool), compares values.
    // Else compares pointers.
    emit_load_bool(ACC, BoolConst(1), s);
    emit_load_bool(A1, BoolConst(0), s);
    emit_jal("equality_test", s);
    // Result in ACC.
    // Wait, equality_test returns values in A0?
    // Let's assume standard equality_test usage.
    
    // Actually, generated code usually looks like:
    // T1 = e1, T2 = e2
    // beq T1 T2 label_true
    // jal equality_test
    // label_true: ...
    
    // equality_test takes A0 and A1? Or T1 and T2?
    // Trap handler doc says: equality_test compares objects in $t1 and $t2.
    // Result in $a0 (true/false object).
    
    emit_branch(label_finish, s); // Result of equality_test is in ACC
    
    emit_label_def(label_true, s);
    emit_load_bool(ACC, BoolConst(1), s);
    
    emit_label_def(label_finish, s);
}

void leq_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_push(ACC, s);
    env.AddObstacle();
    
    e2->code(s, env);
    emit_load(T1, 1, SP, s);
    emit_addiu(SP, SP, 4, s);
    emit_move(T2, ACC, s);
    
    emit_load(T1, 3, T1, s);
    emit_load(T2, 3, T2, s);
    
    int label_true = labelnum++;
    int label_finish = labelnum++;
    
    emit_bleq(T1, T2, label_true, s);
    emit_load_bool(ACC, BoolConst(0), s);
    emit_branch(label_finish, s);
    
    emit_label_def(label_true, s);
    emit_load_bool(ACC, BoolConst(1), s);
    
    emit_label_def(label_finish, s);
}

void comp_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_load(T1, 3, ACC, s);
    
    int label_true = labelnum++;
    int label_finish = labelnum++;
    
    emit_beqz(T1, label_true, s);
    emit_load_bool(ACC, BoolConst(0), s);
    emit_branch(label_finish, s);
    
    emit_label_def(label_true, s);
    emit_load_bool(ACC, BoolConst(1), s);
    
    emit_label_def(label_finish, s);
}

void int_const_class::code(ostream& s, Environment env)  
{
  emit_load_int(ACC,inttable.lookup_string(token->get_string()),s);
}

void string_const_class::code(ostream& s, Environment env)
{
  emit_load_string(ACC,stringtable.lookup_string(token->get_string()),s);
}

void bool_const_class::code(ostream& s, Environment env)
{
  emit_load_bool(ACC, BoolConst(val), s);
}

void new__class::code(ostream &s, Environment env) {
    if (type_name == SELF_TYPE) {
        emit_load_address(T1, "class_objTab", s);
        emit_load(T2, 0, SELF, s);  // T2 = self tag
        emit_sll(T2, T2, 3, s);      // * 8
        emit_addu(T1, T1, T2, s);
        emit_push(T1, s);
        emit_load(ACC, 0, T1, s);    // protObj
        emit_jal("Object.copy", s);
        emit_load(T1, 1, SP, s);
        emit_addiu(SP, SP, 4, s);
        emit_load(T1, 1, T1, s);     // init
        emit_jalr(T1, s);
    } else {
        std::string protobj = std::string(type_name->get_string()) + PROTOBJ_SUFFIX;
        emit_load_address(ACC, (char*)protobj.c_str(), s);
        emit_jal("Object.copy", s);
        std::string init = std::string(type_name->get_string()) + CLASSINIT_SUFFIX;
        emit_jal((char*)init.c_str(), s);
    }
}

void isvoid_class::code(ostream &s, Environment env) {
    e1->code(s, env);
    emit_move(T1, ACC, s);
    
    int label_true = labelnum++;
    int label_finish = labelnum++;
    
    emit_beqz(T1, label_true, s);
    emit_load_bool(ACC, BoolConst(0), s);
    emit_branch(label_finish, s);
    
    emit_label_def(label_true, s);
    emit_load_bool(ACC, BoolConst(1), s);
    
    emit_label_def(label_finish, s);
}

void no_expr_class::code(ostream &s, Environment env) {
    emit_move(ACC, ZERO, s);
}

void object_class::code(ostream &s, Environment env) {
    if (name == self) {
        emit_move(ACC, SELF, s);
        return;
    }
    
    int idx;
    if ((idx = env.LookUpVar(name)) != -1) {
        emit_load(ACC, idx + 1, SP, s);
    } else if ((idx = env.LookUpParam(name)) != -1) {
        emit_load(ACC, idx + 3, FP, s);
    } else if ((idx = env.LookUpAttrib(name)) != -1) {
        emit_load(ACC, idx + 3, SELF, s);
    }
}

// Global variable definition
// CgenClassTable *codegen_classtable = NULL; // Moved to top
