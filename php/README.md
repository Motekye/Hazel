# Hazel Preprocessor

Macro-based preprocessor for JavaScript, written in PHP.

Uses string-based interpretation without use of outside 
libraries. Native functions used are well documented for 
future ports to other languages.

Package includes the following files...

```
hazel.php ............. plain hazel program
hazelr.php ............ with reporting feature
hazel-samples.php ..... sample function hooks
hz.php ................ sample invocation
def.hz ................ sample shortcuts
csssh.php ............. CSS Short-hand macros
```

The sample invocation is a basic PHP script that includes
hazel, then fetches files with a glob from the querystring.
If the destination is a folder, all .hz files within will be 
included in the output. Usage...

```html
<script type="text/javascript" src="hz.php?my/file.hz"></script>
<script type="text/javascript" src="hz.php?my/scripts/"></script>
<script type="text/javascript" src="hz.php?api/class-*.hz"></script>
```

You may also disable minification by opening the querystring
with the `!` character. This will stream all whitespace and 
comments, including your hazel commands, and is useful for
debugging...

```html
<script type="text/javascript" src="hz.php?!my/file.hz"></script>
```

The sample invocation also includes _hazel-samples.php_ and runs
hazel() over _def.hz_ prior to loading the request. This is
how you would define your own set of custom function hooks
and common macros to all your projects. I included my own
with this package as examples, but I would assume anyone 
using Hazel would want to make their own shortcuts.

Make copies of and edit _hz.php_ and _def.hz_ to suit your needs.

Hazel (c) 2017-2021 - 1002px.com - released under MIT license.

## Using Hazel

Hazel uses different types of macro shortcuts to truncate
javascript (or any similar language) to your specification.
Shortcuts all take the form (slash)(slash)(something)(tab),
and allow you to alter the syntax of javascript with match
macros, or wrappers that transliterate function calls and 
new structural blocks resembling for(){} or while(){}...

```javascript
//=	exact match macro
//s	macros exclusively in strings
//v	words without : after or . before
//f	function wrapper template
//b	block wrapper template
//p	procedure hook for function wrapper
//k	procedure hook for block wrapper
//i	include another .hz file
//j	include unaltered script
//c	define starter for comment line

//_	carry long definitions over lines
```

You may use `##` instead of `//` if you want to preserve 
syntax highlighting on the command. If you are not minifying
the code, these will be turned into `//` commands.

## Comment lines and includes

```javascript
//c	/*!*/

(function(){
	do.something();
/*!*/	console.log(something);
	return something;
})();
```

Comment lines with `//c` or `##c` expect a single term, which 
when found at the beginning of a line, will strip that line
from the output. This is useful for adding and removing lines
of debugging code by only changing a single command.

```javascript
//i	path/to/script.hz
//j	path/to/script.js
```

Include another Hazel script, or plain javascript file. Do not
put quotes around the file names.

## Function and block wrappers

```javascript
##f	if<>	if(#0&&#0.nodeType==1)

// USE...

if<>(elm){ do.something(elm); }
else { do.something_else(); }
```

```javascript
##b	after	setTimeout(function(){#B},#0);

// USE...

after(1000){ do.something(); }
```

```javascript
##b	forln	for(#1=0;#1<#0.length;#1++){#B}

// USE...

forln(arr;i){ do.something(arr[i]); }
```

```javascript
//=	(<>	document.createElement(
//v	BODY	document.body
//=	..AP	.appendChild
//=	..IH	.innerHTML
//=	..SA	.setAttribute
//=	..SC(	.setAttribute('class',

// USE...

var EL = (<>'div');
BODY..AP(EL);
EL..IH = "Hello World.";
EL..SA('id','ex-002');
EL..SC("example");
```

Any block wrapper you create may also have an _else_ 
clause associated with it. Use `#E` in your template
to include the else clause somewhere.

```javascript
##b	forln	if(!#0||!#0.length){#E}
##_             else { for(#1=0;#1<#0.length;#1++){#B} }

// USE...

forln(arr;i){ do.something(arr[i]); }
else { do.something_else(); }
```

The previous example executing the _else_ clause if the
first argument is not iterable or has 0 length, otherwise
executing the block once for each iteration.

## Template Arguments

Templates for function and block wrappers allow
numbered arguments `#0`, `#1`... `#99`. You may 
also use any of the following values as of this 
version...

```
#C	Argument count
#A	All arguments separated by ,
#N	Shift argument from left
#X	Pop argument from right
#R	Remainder of arguments
#B	Block for block wrappers
#E	Else block for block wrappers
#I	Instance number (0-based)
```

The numbered arguments are resolved first, then
the `#N` and `#X` commands in order, then the 
rest of the commands are plugged in afterwards.
`#A` will supply _all arguments regardless, 
separated by commas_, `#R` will only supply the
arguments not caught by other commands.

While the arguments in a block wrapper are separated
by `;`, the output of `#A` or `#R` will still be 
separated by comma `,` for better injectability.

`#C` spills an integer for the number of arguments
provided, `#I` is an _instance number_. Hazel
remembers how many times a shortcut is used, and 
that number is available for use in your template.

## Function wrapper procedure hooks

It is very easy to write PHP functions to handle 
function and block wrappers. This allows you the
ability to add server-side functions to hazel.
The _hazel-samples.php_ file contains a number of
sample function and block wrapper hooks to use 
for inspiration.

```javascript
//p	mfn	my_hazel_function

mfn(arg1,arg2);
```

Use the `//p` hazel command to define a function
wrapper procedure. Then define the alias and the
name of the PHP function that handles this wrapper.

```php
function my_hazel_function($ins,$arg){
	return "some code";
}
```

This function receives the _instance number_, which is how
many times this shortcut has appeared starting with zero. 
The second argument is an array of the arguments separated 
by `,` as individual strings. Hazel tracks bracket depth 
and is wise to _strings_ and _regular expressions_ and can 
accurately find the correct `,`s to split the parameters by.

## Block wrapper procedure hooks

Similarly, you may make custom block wrappers.

```javascript
//k	mbk	my_hazel_block

mbk(arg1;arg2){ Code... } else { Code... }
```

The PHP function handling the block gets two extra 
arguments for the `#B` block and `#E` else block.

```php
function my_hazel_block($ins,$arg,$blk,$els){
	return "some code";
}
```

The `else` clauses are entirely optional. They are
checked for with every custom block, but if you don't
use them the clause is simply ignored.

# Sample hooks included

These functions are defined in _hazel-samples.php_ and
provide some added functionality and inspiration for
writing new function and block wrapper hooks.

#### hazel_string_filter

```javascript
//k	strf	hazel_string_filter

mystring = strf(func;<>;""){ unescaped string data... };
```

Filters the contents of the block with a list of 
functions or 3 built-in filters: `""` `''` and `<>`.
Any PHP function may be a filter, the function should
take the string contents as a single argument and 
return the finished string. Functions like `rtrim` or 
`urlencode` are already perfect for this.

The filters are executed from left to right, so the 
example runs _func_ over the block, then escapes HTML
entities, then escapes the string in double quotes.

#### hazel_base64_image

```javascript
//p	img64	hazel_base64_image

myimage = img64(my/image.png);
myimages = img64(my/1.jpeg,my/2.gif,my/3.png);
```

Allows you to get an image on your server, escaped
as a base64 data-uri. Providing multiple arguments
will produce an _array of strings_ with the data-uris
of each argument, in order.

#### hazel_php_variable

```javascript
//p	phpv	hazel_php_variable

username = phpv($_SESSION.user.name);
```

This module allows you to spill the value of any PHP
variable in javascript using the phpv() function, or 
whatever shortcut you choose.

This module uses a dotted notation for arrays and keys.
Because arrays may only be one dimensional in PHP, as 
long as the arrays are in the correct place you do not
need to specify nesting structure for arrays and keys...

```php
phpv($data.user.$userno.$cno.post.16);		// vs:

echo $GLOBALS['data']['user'][$GLOBALS['userno']
	[$GLOBALS['cno']]]['post'][16];
```

Any particle beginning with the characters `$ @ # % !`
indicates a superglobal key for `$GLOBALS`, `$_SESSION`,
`$_POST`, `$_GET` and `$_SERVER` respectively. Any 
particle with no _sigil_ is a string or integer key.
The indices 16 and '16' are the same index anyway, so 
quotes are not needed for string keys.

#### hazel_if_variable

```javascript
//k	ifv	hazel_if_variable

username = ifv($_SESSION.loggedin){ ... } else { ... }
```

The final module adds a _single variable evaluation_ as
of this version. There is no math or < > = comparison.
The module will select the block or else block based on 
the _truthyness_ of the PHP variable.

## Sample macros included

The file def.hz contains a set of macros that radically 
alter the syntax of javascript and also invokes the four
sample hooks in the previous section. The hz.php script is
setup to include this file automatically.

You will undoubtedly want to change or edit this file to
your specification.

## Macro recursivity

All match macros and wrappers insert their output ahead
of the pointer, rather than behind it. This means that
the injected code will again be checked for matches and
wrappers. This makes compounded shortcuts like this one
possible...

```javascript
//k	strf	hazel_string_filter
//=	(#	document.getElementById(
//=	:st	.style
//=	:csssh{	:st=strf(csssh;""){

// USE...

(#"navi"):csssh{ PSF; T:0; R:0; W:100^; H:400^; B#000; };

// TO PRODUCE...

document.getElementById("navi").style =
	" position:fixed; top:0; right:0; width:100px; 
	height:400px; background-color:#000; ";
```

Using a function also included in this package called 
CSSSH (CSS Short Hand) for crunching down CSS properties.
Any CSS preprocessor could work as long as the function
takes one parameter and outputs the finished string.

The example shows a `:csssh{` macro that itself contains
more macros. Your macros may be as recursive as you need,
but there is no protection from macros with infinite 
recursion in this release.

## Feedback and future considerations

Considering adding `#L`, `#U` and `#^` for looping part of
a template while any arguments remain, or while there are 
enough arguments to address every `#N` or `#X` in the loop.

Open to new ideas for template commands or general Hazel 
commands. The language previously had command-level logic
for evaluating single variables...

```
//?	if
//!	if-not
//~	else
//%	select
//:	case
//;	break; case
//x	end
//xxx	(stackable)
//x*	end everything
```

But after adding block wrapper PHP function hooks, it seemed 
pointless and needlessly complex to have two completely 
different types of logical structures, so that was removed.
