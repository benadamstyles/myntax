let getContents = (input) =>
  switch input {
  | "-" => Sysop.readStdin()
  | x => Sysop.readFile(x)
  };

let printImpl = (implementation) => Printast.implementation(Format.std_formatter, implementation);
let pprintImpl = (implementation) => Pprintast.structure(Format.std_formatter, implementation);

let getGrammar = (raw) => {
  let start = Unix.gettimeofday();
  switch (Runtime.parse(GrammarGrammar.grammar, "Start", raw)) {
  | PackTypes.Result.Failure(maybeResult, (charsParsed, failure)) =>
    print_string(PackTypes.Error.genErrorText(raw, failure));
    failwith("Unable to parse grammar")
  | PackTypes.Result.Success(result) =>
    let mid = Unix.gettimeofday();
    let res = GrammarOfGrammar.convert(result);
    (res, mid -. start, Unix.gettimeofday() -. mid)
  }
};

let out_binary = (ast: Parsetree.structure, input_name) => {
  output_string(stdout, Config.ast_impl_magic_number);
  output_value(stdout, input_name);
  output_value(stdout, ast)
};

let getResult = (grammar, contents) => {
  let start = Unix.gettimeofday();
  switch (Runtime.parse(grammar, "Start", contents)) {
  | PackTypes.Result.Failure(maybeResult, (charsParsed, failure)) =>
    print_string(PackTypes.Error.genErrorText(contents, failure));
    exit(1)
  | PackTypes.Result.Success(result) => (result, Unix.gettimeofday() -. start)
  }
};

let getResult = (grammarFile, input) => {
  let contents = getContents(input);
  let grammarRaw = getContents(grammarFile);
  let (grammar, parseTime, convertTime) = getGrammar(grammarRaw);
  Printf.eprintf("Parse time %f, convert time %f\n", parseTime, convertTime);
  /* Runtime.debug := true; */
  let (result, parseTime) = getResult(grammar, contents);
  Printf.eprintf("Main parse time: %f", parseTime);
  (result, grammar, contents)
};

/*
 let main grammarFile input printType => {
   let (result, grammar) = getResult grammarFile input;

   switch printType {
     | Bin => out_binary (OcamlOfReason.convert result);
     | Debug => printImpl (OcamlOfReason.convert result);
     | Pretty => {
       /* print_endline (PackTypes.Result.show_result (OcamlOfReason.convertFrom (OcamlOfReason.convert result))); */
       /* failwith "no impl" */
       switch (PrettyPrint.toString grammar result) {
         | Some x => print_endline x
         | None => failwith "Failed to pretty print :("
       }
     }
     | RoundPretty => {
       let round = (OcamlOfReason.convertFrom (OcamlOfReason.convert result));
       /* failwith "no impl" */
       switch (PrettyPrint.toString grammar round) {
         | Some x => print_endline x
         | None => failwith "Failed to pretty print :("
       }
     }
     | Dump => {
       print_endline (PackTypes.Result.show_result result);
     }
     | RoundDump => {
       print_endline (PackTypes.Result.show_result (OcamlOfReason.convertFrom (OcamlOfReason.convert result)));
     }
   }
 }; */
/* let (grammarFile, input, printType) = switch Sysop.argv {
     | [|_, "bin", grammarFile, input|] =>  (grammarFile, input, Bin)
     | [|_, "dump", grammarFile, input|] =>  (grammarFile, input, Dump)
     | [|_, "pretty", grammarFile, input|] =>  (grammarFile, input, Pretty "")
     | [|_, "round-pretty", grammarFile, input|] =>  (grammarFile, input, RoundPretty)
     | [|_, "round-dump", grammarFile, input|] =>  (grammarFile, input, RoundDump)
     | [|_, grammarFile, input|] =>  (grammarFile, input, Debug)
     | _ => failwith "Usage: run grammarfile inputfile"
   };


   main grammarFile input printType;
   */
type printType =
  | Bin
  | Pretty(string)
  | Debug
  | DebugLisp
  | BinLisp
  | Dump
  | RoundPretty
  | RoundDump;

let (command, grammarFile, input) =
  switch Sysop.argv {
  | [|_, "pretty", grammarFile, input, output|] => (Pretty(output), grammarFile, input)
  | [|_, command, grammarFile, input|] => (
      switch command {
      | "bin" => Bin
      | "dump" => Dump
      | "pretty" => Pretty("-")
      | "debug-lisp" => DebugLisp
      | "bin-lisp" => BinLisp
      | "round-pretty" => RoundPretty
      | "round-dump" => RoundDump
      | _ => failwith("Invalid command")
      },
      grammarFile,
      input
    )
  | [|_, grammarFile, input|] => (Debug, grammarFile, input)
  | _ => failwith("Usage: [command=debug] grammarfile inputfile")
  };

let (result, grammar, raw) = getResult(grammarFile, input);

switch command {
| Bin => out_binary(OcamlOfReason.convert(result), input)
| Debug => printImpl(OcamlOfReason.convert(result))
/* | DebugLisp => printImpl(LispToOcaml.convert(result)) */
| DebugLisp => pprintImpl(LispToOcaml.convert(result, input, raw))
| BinLisp => out_binary(LispToOcaml.convert(result, input, raw), input)
| Pretty(dest) =>
  /* print_endline (PackTypes.Result.show_result (OcamlOfReason.convertFrom (OcamlOfReason.convert result))); */
  /* failwith "no impl" */
  switch (PrettyPrint.toString(grammar, result)) {
  | Some(x) =>
    switch dest {
    | "-" => print_endline(x)
    | _ => output_string(open_out(dest), x)
    }
  | None => failwith("Failed to pretty print :(")
  }
| RoundPretty =>
  let round = OcamlOfReason.convertFrom(OcamlOfReason.convert(result));
  /* failwith "no impl" */
  switch (PrettyPrint.toString(grammar, round)) {
  | Some(x) => print_endline(x)
  | None => failwith("Failed to pretty print :(")
  }
| Dump =>
  /* print_endline (PackTypes.Result.show_result result); */
  ()
| RoundDump =>
  /* print_endline (PackTypes.Result.show_result (OcamlOfReason.convertFrom (OcamlOfReason.convert result))); */
  ()
};
