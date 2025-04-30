# Froglang syntax specification

## Function declarations:
Function are declared using the following syntax:
```
fn <function_name>(<argname1>: <type1>, <argname2>: <type2>, ...) : <return_type>
```

## Variable declarations:
Immutable variables can be declared using the following syntax:
```
<varname> : <type> = <expr>
```

<<<<<<< HEAD
## Returning
To return a value from a function we use the ret keyword
```
ret <expr>
```

## If-else statements
The language currently supports if-else statements in the form of
```
if <block> else <block>
```
=======
## Iverson notation:
Logical values can be explicitly cast to the corresponding integer (0 for false and 1 for true) using the Iversion notation.
```
[<logical_expression>]
```
In the future this will return a "polymorphic type" which automatically casts to an identity element of the correct type on any operation.
# Syntax yet unimplemented

## Type casts
Types are not implicitly castable, instead type casts need to be explicit. The syntax is not ready yet though
>>>>>>> 1977f70 (Updated the syntax doc)
