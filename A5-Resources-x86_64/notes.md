# Notes

- main can have any signature.
- every return statement should obey the return type of the function.
- void functions cannot have return statements, since `return;` is not allowed.
- non-void functions must have atleast one return statement with the appropriate type. 
- it is ok if some paths control paths don't have a return statement.

- function overloading is not allowed
- return value of a function call cannot be ignored.          
- for every global variable declaration, make an entry in the data section in the order of declaration.
- for every string literal used anywhere in the program, make an entry in the data section in the order they appear in in the file.

- locals: in the order of declaration, below the PFP.
- shared temps: in the order of their id, below the locals.
- params: above the RA, in REVERSE order as in the function signature.

- --show-asm is set to true by default. 

- return seems to require its own stemp. Standby.

- --show-ast, --show-tac, --show-rtl and --show-asm all show functions in alphabetical order.

- For some unfathomable reason, all functions other than `main` are appended by `_`.

# To-Do
- Check the SPIM generated for move, different types of compute statements, read and write RTL statements.
- RTL for return statement.
- Enforce that the main function must be defined.

# Gameplan
- Before we even think about function calls, we need to firm up single function, basically return types and return statements.
