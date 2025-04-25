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
