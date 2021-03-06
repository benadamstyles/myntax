/** This grammar definition was generated from parsable/grammar **/
open PackTypes.Parsing;

let grammar = {lineComment: Some(";"), blockComment: Some(("/*", "*/")), rules: [("Start", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Star(NonTerminal("Rule", None))])],
  }), ("Decorator", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Terminal("@", None), NonTerminal("ident", Some("name")), Optional(Group([Terminal("(", None), Star(Group([NonTerminal("decarg", Some("args")), Terminal(",", None)])), Optional(NonTerminal("decarg", Some("args"))), Terminal(")", None)])), CommentEOL])],
  }), ("decarg", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("bool", "", [NonTerminal("bool", None)]),
("string", "", [NonTerminal("string", None)]),
("number", "", [NonTerminal("number", None)])
],
  }), ("bool", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [
("", "", [Terminal("true", None)]),
("", "", [Terminal("false", None)])
],
  }), ("Rule", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Star(NonTerminal("Decorator", Some("decorators"))), NonTerminal("ident", Some("name")), Terminal("=", None), NonTerminal("Choice", Some("choices")), CommentEOL]),
("", "", [Star(NonTerminal("Decorator", Some("decorators"))), NonTerminal("ident", Some("name")), Terminal("=", None), CommentEOL, Plus(Group([Terminal("|", None), NonTerminal("Choice", Some("choices")), CommentEOL]))])
],
  }), ("Choice", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Plus(NonTerminal("Item", None)), Optional(Group([Terminal("--", None), NonTerminal("ident", Some("name"))])), Optional(Group([Terminal(";", None), NonTerminal("rest_of_line", Some("comment"))]))])],
  }), ("Item", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("full_indent", "", [Terminal(">>", None)]),
("indent", "", [Terminal(">", None)]),
("item", "", [NonTerminal("Item_", None)])
],
  }), ("Item_", {
    passThrough: true,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Optional(NoSpaceAfter(Terminal("~", Some("neg")))), Optional(NoSpaceAfter(Terminal("#", Some("lexify")))), Optional(NoSpaceAfter(Group([NoSpaceAfter(Terminal("[", None)), Optional(NoSpaceAfter(NonTerminal("flag", Some("flag")))), NoSpaceAfter(NonTerminal("ident", Some("name"))), Terminal("]", None)]))), Optional(NoSpaceBefore(NonTerminal("noBreak", Some("noBreakBefore")))), Optional(NoSpaceAfter(NonTerminal("noSpace", Some("noSpaceBefore")))), NonTerminal("ItemInner", Some("inner")), Optional(NoSpaceBefore(NonTerminal("noSpace", Some("noSpaceAfter")))), Optional(NoSpaceBefore(NonTerminal("noBreak", Some("noBreakAfter")))), Optional(NoSpaceBefore(NonTerminal("suffix", Some("suffix"))))])],
  }), ("noSpace", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [("", "", [Terminal("&", None)])],
  }), ("noBreak", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [("", "", [Terminal("$", None)])],
  }), ("ItemInner", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [NonTerminal("string", None)]),
("", "", [NonTerminal("ident", None)]),
("nested", "", [Terminal("(", None), NonTerminal("NestedItems", None), Terminal(")", None)]),
("", "", [NonTerminal("char_range", None)]),
("", "", [NonTerminal("char", None)])
],
  }), ("NestedItems", {
    passThrough: true,
    ignoreNewlines: Yes,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Plus(NonTerminal("Item", Some("nested")))])],
  }), ("char_range", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Terminal("'", None), NonTerminal("single", Some("start")), Terminal("..", None), NonTerminal("single", Some("end")), Terminal("'", None)])],
  }), ("char", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Terminal("'", None), NonTerminal("single", Some("char")), Terminal("'", None)])],
  }), ("single", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [
("", "", [Terminal("\\", None), Any(None)]),
("", "", [Not(Terminal("'", None)), Not(Terminal("\n", None)), Any(None)])
],
  }), ("string", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [("", "", [Terminal("\"", None), Star(NonTerminal("strchar", Some("contents"))), Terminal("\"", None)])],
  }), ("strchar", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Terminal("\\", None), Any(None)]),
("", "", [Not(Terminal("\"", None)), Not(Terminal("\n", None)), Any(None)])
],
  }), ("flag", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("bool", "exists", [Terminal("?", None)]),
("array", "", [Terminal(":", None)]),
("string", "contents", [Terminal("@", None)])
],
  }), ("suffix", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("plus", "", [Terminal("+", None)]),
("star", "", [Terminal("*", None)]),
("opt", "", [Terminal("?", None)])
],
  }), ("ident", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [("", "", [Not(NonTerminal("digit", None)), Plus(NonTerminal("identchar", None))])],
  }), ("identchar", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Chars('a', 'z', None)]),
("", "", [Chars('A', 'Z', None)]),
("", "", [Chars('0', '9', None)]),
("", "", [Terminal("_", None)])
],
  }), ("number", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [
("", "", [Terminal("0", None), Not(NonTerminal("identchar", None))]),
("", "", [Not(Terminal("0", None)), Plus(NonTerminal("digit", None)), Not(NonTerminal("identchar", None))])
],
  }), ("digit", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Chars('0', '9', None)])],
  }), ("rest_of_line", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: true,
    docs: None,
    choices: [("", "", [Star(Group([Not(Terminal("\n", None)), Any(None)]))])],
  }), ("eol", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [("", "", [Star(NonTerminal("white", None)), NonTerminal("eee", None)])],
  }), ("eee", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Plus(NonTerminal("eolchar", None))]),
("", "", [EOF])
],
  }), ("eolchar", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Terminal("\n", None)]),
("", "", [Terminal("\r", None)])
],
  }), ("white", {
    passThrough: false,
    ignoreNewlines: Inherit,
    capturesComments: false,
    preserveInnerSpace: false,
    leaf: false,
    docs: None,
    choices: [
("", "", [Terminal(" ", None)]),
("", "", [Terminal("\t", None)])
],
  })]};


