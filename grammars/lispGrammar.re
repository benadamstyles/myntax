
open Asttypes;
open Parsetree;
module H = Ast_helper;

module DSL = PackTypes.DSL;

[@lineComment ";"];
[@blockComment ("(**", "*)")];

[@name "Start"]
[%%rule (
  "ModuleBody",
  ([@nodes "Structure"]structures) => structures
)];

[@name "ModuleBody"]
[%%passThroughRule "Structure+"];

[@name "Structure"]
[%%rules [
  ("let", {|"("& "def" LetPair &")"|},
    (~loc, [@node "LetPair"]pair) => H.Str.value(~loc, Nonrecursive, [pair])
  ),
  ( "let_rec", {|"("& "def-rec" LetPair+ &")"|},
    (~loc, [@nodes "LetPair"]pairs) => H.Str.value(~loc, Recursive, pairs)
  ),
  ( "type", {|"("& "type" TypeBody &")"|},
    (~loc, [@nodes "TypePair"]pairs) => H.Str.type_(pairs),
  ),
  ( "module", {|"("& "module" capIdent ModuleExpr &")"|},
    (~loc, [@text "capIdent"](name, nameLoc), [@node "ModuleExpr"]expr) => H.Str.module_(~loc, H.Mb.mk(
      Location.mkloc(name, nameLoc),
      expr
    )),
  ),
  ( "open", {|open longCap|},
    (~loc, [@node "longCap"]lident) => H.Str.open_(~loc, H.Opn.mk(lident))
  ),
  ( "eval", "Expression",
    (~loc, [@node "Expression"]expr) => H.Str.eval(~loc, expr)
  )
]];

[@name "TypeBody"]
[%%passThroughRule "TypePair+"];

[@name "ModuleExpr"]
[%%rules [
  (
    "arrow",
    {|"("& "=>" "[" "]" Structure* &")"|},
    () => ()
  ),
  (
    "structure",
    {|"("& "str" Structure* &")"|},
    () => ()
  ),
  (
    "ident",
    {|longCap|},
    () => ()
  ),
  (
    "functor_call",
    {|"("& longCap ModuleExpr+ &")"|},
    () => ()
  )
]];

[@name "LetPair"]
[%%rule (
  {|Pattern Expressoin|},
  ([@node "Pattern"]pattern, [@node "Expression"]expr) => H.Vb.mk(pattern, expr)
)];

[@name "TypePair"]
[%%rule (
  {|TypeName TypeDecl|},
  (~loc, [@node "TypeName"](name, vbls), [@node "TypeDecl"]kind) => {
    H.Type.mk(
      ~loc,
      ~params=vbls,
      ~kind,
      name,
    )
  },
)];

[@name "TypeName"]
[%%rules [
  (
    "vbl",
    {|"("& lowerIdent typeVariable+ &")"|},
    (~loc, [@text "lowerIdent"](name, loc), [@texts "typeVariable"]vbls) => (
      Location.mkloc(name, loc),
      vbls |> List.map(((name, loc)) => (H.Typ.var(~loc, name), Invariant)),
    )
  ),
  (
    "plain",
    {|lowerIdent|},
    (~loc, [@text "lowerIdent"](name, loc)) => (
      Location.mkloc(name, loc),
      []
    )
  )
]];


/*

@ignoreNewlines
TypeDecl =
| "{"& TypeObjectItem+ &"}" -- record
| TypeConstructor+ -- constr
| CoreType -- core

TypeConstructor =
| longCap -- no_args
| "("& longCap CoreType+ &")" -- args

TypeObjectItem =
| shortAttribute CoreType -- normal
| shortAttribute -- punned

CoreType =
| longIdent -- constr_no_args
| typeVariable -- variable
| "("& longIdent CoreType+ &")" -- constructor

@leaf
typeVariable = '\'' lowerIdent

@ignoreNewlines
Expression =
| "("& "["& [index]Expression &"]" [array]Expression &")" -- array_index
| "("& [attr]string [object]Expression &")" -- js_object_attribute
| "("& attribute Expression &")" -- record_attribute
| "("& "let" "["& (Pattern Expression)+ &"]" Expression+ &")" -- let
| "("& "open" ModuleExpr Expression+ &")" -- open

| "("& "if" [test]Expression [yes]Expression [no]Expression &")" -- open

| "("& "module" capIdent ModuleExpr Expression+ &")" -- module
    ; not 100% sure I want to do this :P but it could be so handy!!
| "("& "loop" "["& (Pattern Expression)+ &"]" Expression+ &")" -- loop_recur
| Arrow -- arrow
| "("& "->>" [target]Expression ThreadItem+ &")" -- threading_last
| "("& "->" [target]Expression ThreadItem+ &")" -- threading
| Switch -- switch
| "("& [constr]longCap [args]Expression+ &")" -- constructor
| "("& "," [args]Expression+ &")" -- tuple
| "("& [fn]Expression FnCallArg+ &")" -- fn_call
| "["& [items]Expression* ("..."& [spread]Expression)? &"]" -- array_literal
| "{"& ("..."& [spread]Expression)? ObjectItem+ &"}" -- object_literal
| longCap -- empty_constr
| longIdent -- ident
| attribute -- attribute
| operator -- op
| constant -- const

FnCallArg =
| argLabel "=" Expression -- labeled
| argLabel -- punned
| Expression -- expr

Switch = "("& "switch" [target]Expression SwitchBody &")"

@ignoreNewlines
SwitchBody = SwitchCase+

SwitchCase = SwitchCond Expression
SwitchCond =
| Pattern "when" Expression -- when
| Pattern -- plain

ThreadItem =
| attribute -- attribute
| longIdent -- ident
| longCap -- emptyconstr
| "("& [constr]longCap [args]Expression+ &")" -- constructor
| "("& [fn]Expression [args]Expression+ &")" -- fn_call

ObjectItem =
| attribute Expression -- normal
| attribute -- punned

Arrow = "("& "=>" FnArgs Expression* &")"

FnArgs =
| lowerIdent -- single
| "()" -- unit
| "_" -- ignored
| "["& FnArgItems &"]" -- multiple

@ignoreNewlines
@passThrough
FnArgItems = FnArg+

@ignoreNewlines
FnArg =
| argLabel "as" Pattern -- destructured
| argLabel &"=?" -- optional
| argLabel &"="& Expression -- defaulted
| argLabel -- labeled
| Pattern -- unlabeled

@ignoreNewlines
Pattern =
| lowerIdent -- ident
| longCap -- empty_constr
| constant
| "()" -- unit
| "_" -- ignored
| "["& [item]Pattern* ("..."& [spread]Pattern)? &"]" -- array
| "("& "," [item]Pattern* &")" -- tuple
| "("& [constr]longCap [args]Pattern+ &")" -- constructor
| "{"& PatternObjectItem+ &"}" -- object
| "(|"& Pattern+ &")" -- or

PatternObjectItem =
| attribute Pattern -- normal
| attribute -- punned

@leaf
argLabel = '~' lowerIdent

@passThrough
Parened = "("& Expression & ")"

attribute = ':' longIdent
shortAttribute = ':' lowerIdent

longIdent = (longCap_ ".")? lowerIdent
longCap = longCap_ ~"."

longCap_ =
  | longCap_ "." capIdent -- dot
  | capIdent -- lident

constant =
  | [val]float -- float
  | [val]int64 -- int
  | [val]string -- string
  | [val]char -- char

@leaf
capIdent = ~(reserved ~identchar) 'A..Z' identchar*
@leaf
lowerIdent = ~(reserved ~identchar) 'a..z' identchar*

identchar =
  | alpha
  | digit
  | "_"

@leaf
int64 =  digit+ ~identchar

@leaf
float = digit+ '.' digit+

@leaf
string = "\"" strchar* "\""
strchar =
  | "\\" any
  | ~"\"" ~"\n" ~"\\" any

@leaf
char = "'" charchar "'"
charchar =
  | "\\" any
  | ~"'" ~"\n" ~"\\" any

reserved =
  | "fun"
  | "let"
  | "and"
  | "as"
  | "type"
  | "switch"
  | "exception"
  | "of"
  | "module"
  | "rec"
  | "open"
  | "import"
  | "try"
  | "catch"
  | "from"

alpha =
  | 'a..z'
  | 'A..Z'
digit = '0..9'

@leaf
operator = ~reservedOps opChar+ ~identchar

reservedOps =
| "=>"
| "..."

opChar =
  |"!"
  |"$"
  |"%"
  |"&"
  |"*"
  |"+"
  |"-"
  |"."
  |"/"
  ;|":"
  |"<"
  |"="
  |">"
  |"?"
  |"@"
  |"^"
  |"|"
  |"~"
 */






