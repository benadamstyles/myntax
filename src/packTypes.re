let module Parsing = {
  type grammar = list production[@@deriving show]
  and production = (string, list choice)[@@deriving show] /* rule name -> e1 | e2 | ... */
  and choice = (string, string, list parsing)[@@deriving show] /* choice name, comment, sequence */
  and parsing =
    | Star parsing (option string)      /* e* */
    | Plus parsing (option string)      /* e+ */
    | Optional parsing (option string)  /* e? */
    | Any (option string) /* any */
    | EOF /* EOF */
    | Group (list parsing)  /* ( e ... ) */
    | Lookahead parsing  /* &e */
    | Not parsing        /* !e */
    | Lexify parsing     /* # somelexrule */
    | NonTerminal string (option string)/* nonterminal 'name' */
    | Terminal string (option string)   /* terminal */
    | Chars char char (option string)   /* [a-z] */
    | Empty              /* epsilon */[@@deriving show];
};
include Parsing;

let unwrapOr a b => {
  switch a {
    | Some x => x
    | None => b
  }
};

let module Result = {
  type resultType =
    | Terminal string
    | Lexical string string
    | Iter
    | Nonlexical string [@@deriving yojson];

  let resultTypeDescription rt => switch rt {
    | Terminal s => "Terminal(" ^ s ^ ")"
    | Lexical s l => "Lexical(" ^ l ^ "," ^ l ^ ")"
    | Iter => "Iter"
    | Nonlexical s => "Nonlexical(" ^ s ^ ")"
  };

  type result = {
    start: int,
    cend: int,
    typ: resultType,
    label: option string,
    children: list result,
  } [@@deriving yojson];

  /* type partial = {
    path: list string,
    expected: string,
    position: int,
    lno: int,
    cno: int,
  } [@@deriving yojson]; */

  type errs = list (bool, parsing);
  type partial = (int, (int, errs));

  type parserMatch =
    | Success result
    | Failure (option result) partial;

};
include Result;
