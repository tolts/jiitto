
## A quick overview

```
(print "Hello world!")
```

Above is a simple hello world program where print is the label to be called and "Hello world" the variable. 'label', 'function' and 'method' are synonymous in regards to their use. In this language their implementation is more akin to labels in assembly, hence the name.

## Syntax

The syntax is heavly inspired from lisp.

### Expressions

An expression is of the form
```
(label var1 var2 ...)
```
where ```label``` and ```var1 var2 ...``` are valid ones (have been declared prior to the execution).

Expressions are executed one by one. One expression can be in another one in which case the former is executed before the latter.

They are surrounded by parenthesis which are the only way to recognise one expressions from another. This allows for the following which is equivalent ti the first script in the A quick overview section
```
(print "Hello ")(print "world!")
```

### Whitespaces

Whitespaces are all equivalent. Thus,
```
(print               "Ja")
```
is equivalent to
```
(print "Ja")
```

### Labels

Everything is a label. More precisly, everything is a pointer. This rule allows for
```
("Hello world!" print)
```
The above must be understood as applying a 'function' to a 'variable'. In becomes more interesting with operations: ```(2 + 2)``` This expression returns 4 where '+' is the label called