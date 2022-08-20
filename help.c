/****************************************************************************/
/***  ##   # ###### ##     #####_  ******************************************/
/***  ###### ##___  ##     ##  .#  ******************************************/
/***  ##   # ##^^^  ##     #####^  ******************************************/
/***  ##   # ###### ###### ##      ******************************************/
/****************************************************************************
 *
 *	This module prints the help menus for Hazel to the command line...
 *
*/

static void display_help_about ();
static void display_help_flags ();
static void display_help_includes ();
static void display_help_commands ();
static void display_help_arguments ();


void display_help () 
{
	puts(HAZEL_LOGO_LARGE);
	puts(
		"A versatile preprocessor for Javascript, written in C...\n"
		"        http://github.com/Motekye/Hazel\n\n"
		"        USAGE:\n\n"
		"./hazel path/to/script.hz\n\n"
		"        SELECT AN OPTION:\n\n"
		"  [f]   See list of available flags.\n"
		"  [m]   Hazel macro commands.\n"
		"  [i]   Hazel file I/O commands.\n"
		"  [a]   About Hazel.\n\n"
		"press any letter above, then enter. Or just press enter to exit.\n"
		
	);
	char c = getchar();
	if (c=='\n') { return; }
	while (getchar() != '\n');
	switch (c) {
	case 'f': display_help_flags (); return;
	case 'm': display_help_commands (); return;
	case 'i': display_help_includes (); return;
	case 'a': display_help_about (); return;
	}	
}


/* ************************************************************* */

/*
 *        
*/

static void display_help_about ()
{
	puts(HAZEL_LOGO_LARGE);
	puts(
		"Hazel was originally a PHP script called 'TokenJS', written\n"
		"in 2017 to provide some relief from constantly repeating\n"
		"long identifiers or function names in Javascript.\n"
		"Some of the worst offenders...\n\n"
		"\tdocument.getElementsByTagName\n"
		"\telement.previousElementSibling\n"
		"\telement.getBoundingClientRect\n\n"
		"And Hazel provided match macros to drastically shorten these\n"
		"Javascript functions, in a PHP array.\n\n"
		"(hit ENTER to see more...)"
	);
	getchar ();
	puts(
		"Later on, //= commands were added so that macros could be\n"
		"defined inside of documents. Then //i includes were added\n"
		"so long lists of macros could be stored in separate files.\n"
		"A while after that, //f function wrappers were added to\n"
		"better encapsulate code and to match the #define directive\n"
		"available in C. Finally, //b block wrappers were added as\n"
		"a creative addition that I had not seen implemented in any\n"
		"preprocessor I have seen so far.\n\n"
		"But while running Hazel on a large collection of Javascript\n"
		"files for a web application, totalling around 200kb,\n"
		"I couldn't help but notice how slow the PHP script was. It\n"
		"took nearly 30 seconds to process all the files. And at this\n"
		"point I decided the script needed to be ported to a faster\n"
		"language and seriously upgraded.\n\n"
		"(hit ENTER to see more...)"
	);
	getchar ();
	puts(
		"In a few days time, Hazel was rewritten in C. Subtle changes\n"
		"were made, but the syntax remained mostly the same. Now that\n"
		"same 200kb collection of files is processed in under 2 seconds.\n\n"
		"The C version of Hazel also adds //2 pair match closures, to\n"
		"replace the dangerous //p user functions utilized in the PHP\n"
		"version, that allowed any PHP function to be run by name.\n"
		"The string filter that came with the PHP version was replaced\n"
		"by native string filtering methods on the template parameters.\n\n"
		
		
		"This rewrite has inspired me to add more features which I have\n"
		"not even thought of a serious use for. //k keywords are in\n"
		"development as of this release. and more ideas are floating\n"
		"around in my head as to how I can improve Hazel. I have yet to\n"
		"re-include base64 encoded images, either.\n\n"
		
		"\t\t-- August 20th, 2022\n\n"
		
		
		"(hit ENTER to return...)"
	);
	getchar();
	display_help ();
}


/* ************************************************************* */

/*
 *        
*/

static void display_help_flags ()
{
	puts(
		"        SELECT A FLAG:\n\n"
		"  [h]   -h --help flag.\n"
		"  [o]   -o --out --output flag.\n"
		"  [m]   -m --min --minify flag.\n"
		"  [q]   -q --quiet flag.\n"
		"  [n]   -n --nest --max-nest flag\n"
		"  [/]   Return to previous menu.\n\n"
		"press any letter above, then enter.\n"
	);
	char c = getchar();
	while (getchar() != '\n');
	switch (c) {
	case '/': display_help (); return;
		
	case 'h':
		puts(
			"\n"
			"\t-h --help\n\n"
			"Use this flag, or run Hazel with no parameters to see this\n"
			"help file.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'o':
		puts(
			"\n"
			"\t-o --out --output\n\n"
			"Use this flag along with a filename as the next argument\n"
			"to specify an output file. Otherwise Hazel will spill the\n"
			"translated document to the console.\n\n"
			"\tUSAGE:\n\n"
			"./hazel source.hz -o script.js\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'm':
		puts(
			"\n"
			"\t-m --min --minify\n\n"
			"Using this flag will minify all output, stripping line\n"
			"breaks and unnecessary white space. All included files\n"
			"will also do this.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'q':
		puts(
			"\n"
			"\t-q --quiet\n\n"
			"This flag will prevent streaming of any data, as if\n"
			"the original file was included with the //q command.\n\n"
			"Using //o in your documents will override this behavior.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'n':
		puts(
			"\n"
			"\t-n --nest --max-nest\n\n"
			"Hazel limits the code buffer to 1024 include levels. These are\n"
			"shared by file includes and macro instances. This limit is set to\n"
			"avoid runaway self or circular references. This should be\n"
			"enough for even the most complex scripts. But if for some\n"
			"reason you need to increase this number, you can use this flag.\n\n"
			"\tUSAGE:\n\n"
			"./hazel path/to/complex.hz -n 4096\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	
	
	default: break;
	}
	while (getchar() != '\n');
	display_help_flags ();
}


/* ************************************************************* */

/*
 *        
*/

static void display_help_includes ()
{
	puts(
		"        SELECT A COMMAND:\n\n"
		"  [i]   //i include a hazel file.\n"
		"  [q]   //q quiet include (no output).\n"
		"  [j]   //j include plain javascript.\n"
		"  [o]   //o specify output file.\n"
		"  [/]   Return to previous menu.\n\n"
		"press any letter above, then enter.\n"
	);
	char c = getchar();
	while (getchar() != '\n');
	switch (c) {
	case '/': display_help (); return;
	
	case 'i':
		puts(
			"\n"
			"\t//i path/to/file.hz\n\n"
			"Include another hazel file by name.\n\n"
			"For safety reasons, includes are limited to files\n"
			"in the original file's directory, or any sub-directory.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'q':
		puts(
			"\n"
			"\t//q path/to/file.hz\n\n"
			"Include another hazel file by name... *quietly*\n\n"
			"This command will not stream any output, but will only\n"
			"execute hazel commands in the target file. Use this\n"
			"command if you have a file full of shortcuts to include,\n"
			"but would rather not stream any of the empty space in the\n"
			"file. You can also use the -q flag for the same function\n"
			"on the original source file\n\n"
			"For safety reasons, includes are limited to files\n"
			"in the original file's directory, or any sub-directory.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'j':
		puts(
			"\n"
			"\t//j path/to/file.js\n\n"
			"Include a raw Javascript file by name.\n"
			"This will include a file without any hazel functionality.\n\n"
			"For safety reasons, includes are limited to files\n"
			"in the original file's directory, or any sub-directory.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'o':
		puts(
			"\n"
			"\t//o path/to/output.js\n"
			"\t//o .\n\n"
			"Set the output buffer. This is like using the -o flag\n"
			"internally. If you have a large number of files to run\n"
			"Hazel on, you can make a master file with //i and //o\n"
			"commands to produce all of the files in one operation.\n\n"
			"Use //o . to reset the output stream to the console.\n\n"
			"For safety reasons, output files are limited to locations\n"
			"in the original file's directory, or any sub-directory.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	
	
	default: break;
	}
	while (getchar() != '\n');
	display_help_includes ();
}


/* ************************************************************* */

/*
 *
*/

static void display_help_commands ()
{
	puts(
		"        SELECT A COMMAND:\n\n"
		"  [.]   General macro options.\n"
		"  [=]   //= exact macros.\n"
		"  [v]   //v variable macros.\n"
		"  [s]   //s string macros.\n"
		"  [c]   //c comment line initials.\n"
		"  [f]   //f function wrappers.\n"
		"  [b]   //b block wrappers.\n"
		"  [k]   //k block keywords.\n"
		"  [2]   //2 pair match closures.\n"
		"  [#]   Template argument list.\n"
		"  [/]   Return to previous menu.\n\n"
		"press any letter above, then enter.\n"
	);
	char c = getchar();
	while (getchar() != '\n');
	switch (c) {
	case '/': display_help (); return;
	
	case '#': display_help_arguments (); return;
	
	case '.':
		puts(
			"\n"
			"Hazel commands must appear on a new line. They take the form\n"
			"of special line comments, beginning with either // or ##.\n"
			"If ## is used, and the code is not minified, the ## will be\n"
			"converted to // in the output.\n\n"
			"All commands begin with (slash)(slash)(something)(tab) or\n"
			"(slash)(slash)(something)(space), where (something) is any\n"
			"recognized command symbol. Then most commands will require\n"
			"a name or match string, followed by another space or tab,\n"
			"then the replace string, or template to use.\n\n"
			"examples...\n"
			"//= (<> \tdocument.getElementsByTagName(\n"
			"//v BODY\tdocument.body\n"
			"//b forln\tfor(#1=0;#1<#0.length;#1++){ #B }\n\n"
			"(hit ENTER to see more...)"
		);
		getchar ();
		puts(
			"Sometimes you want templates to span multiple lines. You can\n"
			"do this by starting the next lines off with //_ or ##_. This\n"
			"lets you carry long templates over multiple lines.\n\n"
			"eg...\n\n"
			"//b forln\tif(#0.length==0){ #E } else {\n"
			"//_ \tfor(#1=0;#1<#0.length;#1++){ #B } }\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case '=':
		puts(
			"\n"
			"\t//= name replace\n\n"
			"Defines exact match macros. The macro name may be any\n"
			"non-whitespace characters. Hazel will replace this string\n"
			"with the replace value in the output.\n\n"
			"If the macro would otherwise be a valid identifier, it's\n"
			"better to use the //v command instead.\n\n"
			"Exact macros are nesting. A macro may contain the name of\n"
			"another macro of any type. Hazel provides limited protection\n"
			"against circular or self references.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'v':
		puts(
			"\n"
			"\t//v name replace\n\n"
			"Defines variable match macros. The macro name must be a\n"
			"valid javascript variable name. Hazel will only match this\n"
			"macro if the complete identifier is a match. This avoids\n"
			"partial matches of your macro.\n\n"
			"For instance...\n"
			"//v macr\tmy_macro\n\n"
			"will match: macr\nbut will not match: macro\n\n"
			"If you want partial identifier matches, or matches containing\n"
			"non-identifier characters, you should use //= instead.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 's':
		puts(
			"\n"
			"\t//s name replace\n\n"
			"Defines string match macros. The macro name may be any\n"
			"non-whitespace characters. Hazel will only match these\n"
			"macros inside of \"double quoted\" string literals.\n\n"
			"Other macros are ignored inside of strings, and single\n"
			"quoted strings are always unmodified.\n\n"
			"Also note that these macros do not nest. String macros\n"
			"inject behind the cursor, so you cannot use a string\n"
			"macro that references other macros.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'c':
		puts(
			"\n"
			"\t//c name\n"
			"\t//c name off\n"
			"\t//c name -\n"
			"\t//c name on\n"
			"\t//c name +\n\n"
			"Comment lines define a string which, when appearing at the\n"
			"start of a line, will clear that line in the output.\n"
			"Use this feature to hide or show lines of debugging code with\n"
			"a single command. You may use + or 'on' to define the comment\n"
			"line as enabled (showing), or - or 'off' to define the comment\n"
			"line as disabled (hidden). Using no parameter will make an\n"
			"disabled (hidden) comment line.\n\n"
			"(hit ENTER to see more...)"
		);
		getchar ();
		puts(
			"When enabled, a comment line init will be replaced by spaces.\n"
			"That way, sequences that would otherwise cause a syntax error\n"
			"may be used as an init.\n\n"
			"eg...\n\n"
			"//c !!!\n\n"
			"function myfn(){\n"
			"!!!\tconsole.log(arguments.length);\n"
			"\t(SOME CODE...)\n"
			"}\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'f':
		puts(
			"\n"
			"\t//f name template\n\n"
			"Defines function wrapper macros. The macro name may be any\n"
			"non-whitespace characters. These macros must have a ( open\n"
			"bracket after them, though there may be spaces, tabs or line\n"
			"breaks before the ( open bracket. If the ( bracket is not\n"
			"present then the macro will be ignored.\n\n"
			"Hazel will render the template using the arguments provided\n"
			"in the function call. These appear as (#0 #1...) numbered\n"
			"tags in the template. In function wrappers, the arguments are\n"
			"separated by , comma. Commas that appear inside of other bracket\n"
			"sets will not interfere with your arguments. Hazel is wise to\n"
			"strings and bracket depth when reading the arguments.\n\n"
			"(hit ENTER to see more...)"
		);
		getchar ();
		puts(
			"In addition to numbered arguments, function wrappers may use\n"
			"any of the arguments: #N #X #R #A #C #P or #I.\n"
			"For a detailed description of what these do, consult the\n"
			"Template argument list in the previous menu.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'b':
		puts(
			"\n"
			"\t//b name template\n\n"
			"Defines block wrapper macros. This is a feature which makes\n"
			"Hazel rather unique as far as preprocessors go. You can use\n"
			"these macros to define new blocks that behave like for(){}\n"
			"or while(){}, with arguments and a closure.\n\n"
			"The macro name may be any non-whitespace characters.\n"
			"These macros must have a ( open bracket after them, though\n"
			"there may be spaces, tabs or line breaks before the ( open\n"
			"bracket. If the ( bracket is not present then the macro will\n"
			"be ignored, just like //f function wrappers.\n\n"
			"Hazel will render the template using the arguments provided\n"
			"in the function call. These appear as (#0 #1...) numbered\n"
			"tags in the template. In block wrappers, the arguments are\n"
			"separated by ; semi-colon. Semi-colons that appear inside of\n"
			"other bracket sets will not interfere with your arguments.\n"
			"Hazel is wise to strings and bracket depth.\n\n"
			"(hit ENTER to see more...)"
		);
		getchar ();
		puts(
			"In addition to numbered arguments, block wrappers may use\n"
			"any of the arguments: #N #X #R #A #C #P #I #B or #E.\n"
			"For a detailed description of what these do, consult the\n"
			"Template argument list in the previous menu.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'k':
		puts(
			"\n"
			"\t//k name template\n\n"
			"  <<< THIS FEATURE IS NOT YET AVAILABLE IN THIS VERSION >>>\n\n"
			"Defines block keyword macros. These must follow a block wrapper\n"
			"definition and defines keywords which may appear in any of the\n"
			"block wrapper's closures. Sort of like how a 'while' construct has\n"
			"'continue' and 'break' keywords.\n\n"
			"The template will share all of the argument values with the block\n"
			"wrapper. You may use any #0 #1... numbered argument, as well as the\n"
			"arguments #N #X #R #A #C #P #I, but you may not use #B or #E, since\n"
			"these keywords appear inside those blocks.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case '2':
		puts(
			"\n"
			"\t//2 pairs\n"
			"\t/*  match  replace\n"
			"\t    match  replace\n"
			"\t    ...\n"
			"\t*/\n\n"
			"Defines pair matches that are translated in special closures.\n\n"
			"Give the closure a name, then //2 expects a block comment on the\n"
			"next line, with lines containing as many match/replace terms as\n"
			"you want. These will only be translated inside the closure, but\n"
			"are unconditional. That means they are translated in strings and\n"
			"regular expressions.\n\n"
			"(hit ENTER to see more...)"
		);
		getchar ();
		puts(
			"eg...\n\n"
			"//2 HTMLesc\n"
			"/*\n"
			"\t&\t&amp;\n"
			"\t<\t&lt;\n"
			"\t>\t&gt;\n"
			"*/\n\n"
			"usage...\n\n"
			"html = HTMLesc {\"<title>Things & Stuff</title>\"};\n\n"
			"becomes...\n\n"
			"html = \"&lt;title&gt;Things &amp; Stuff&lt;/title&gt;\";\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	
		
	default: break;
	}
	while (getchar() != '\n');
	display_help_commands ();
}


/* ************************************************************* */

/*
 *
*/

static void display_help_arguments ()
{
	puts(
		"        SELECT A COMMAND:\n\n"
		"  [.]   Argument formatting.\n"
		"  [0]   #0 #1... numbered arguments.\n"
		"  [n/x] #N #X next argument.\n"
		"  [r]   #R rest of the arguments.\n"
		"  [a]   #A all arguments.\n"
		"  [c]   #C #c argument count.\n"
		"  [p]   #P #p parameter count.\n"
		"  [i]   #I #i instance number.\n"
		"  [b/e] #B #E block and else block.\n"
		"  [/]   Return to previous menu.\n\n"
		"press any letter above, then enter.\n"
	);
	char c = getchar();
	while (getchar() != '\n');
	switch (c) {
	case '/': display_help_commands (); return;
		
	case '.':
		puts(
			"\n"
			"\t#_0 untrimmed argument.\n\n"
			"Hazel trims white space and new lines from the ends of\n"
			"arguments by default. If you want the space to remain,\n"
			"put an underscore after the # symbol in the template.\n\n"
			"\t#'0' stringified argument.\n"
			"\t#\"0\" stringified argument.\n\n"
			"By putting quotes around the argument, after the # symbol,\n"
			"you can escape an argument in double or single quotes.\n"
			"All backslashes, new lines, tabs and matching quotes will\n"
			"be escaped with a \\ and their respective escape character.\n\n"
			"\t#_\"0\" untrimmed, stringified argument.\n\n"
			"You can combine the two filters, putting _ before the quotes.\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case '0':
		puts(
			"\n"
			"\t#0 #1 numbered arguments.\n\n"
			"These will inject the numbered arguments provided to\n"
			"the function or block wrapper into your template.\n\n"
			"eg...\n\n"
			"//f DEB\tConsole.log(#0)\n"
			"//f EL\t#0 = document.createElement(#1); #0.innerHTML = #2\n"
			"//f POW\tMath.pow(#0,#1)\n\n"
			"DEB('testing arguments...');\n"
			"EL(mydiv,'div','This is my div...');\n"
			"POW(2,10);\n\n"
			"(hit ENTER to return...)"
		);
		break;
	
	case 'n':
	case 'x':
		puts(
			"\n"
			"\t#N next argument (from start).\n"
			"\t#X next argument (from end).\n\n"
			"After Hazel has finished injecting all of the numbered\n"
			"arguments, the parameters in the template are swept again,\n"
			"processing these two values.\n\n"
			"#N will reference unused arguments from the left, while\n"
			"#X will reference unused arguments from the right.\n\n"
			"eg...\n\n"
			"//f MYFN\tarr = [#0, #X, #N, #3]\n\n"
			"MYFN(a,b,c,d);\n\n"
			"output...\n\n"
			"arr = [a, c, b, d];\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'r':
		puts(
			"\n"
			"\t#R rest of the arguments.\n\n"
			"After Hazel has finished injecting all of the numbered\n"
			"arguments, and the #N and #X injects, the remaining\n"
			"arguments can be injected with #R. These arguments will\n"
			"always be separated by a comma, whether you are using a\n"
			"block or function wrapper template.\n\n"
			"eg...\n\n"
			"//f MYFN\tarr = [#0, #X, #1, [#R]]\n\n"
			"MYFN(a,b,c,d,e,f,g);\n\n"
			"output...\n\n"
			"arr = [a, g, b, [c,d,e,f]];\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'a':
		puts(
			"\n"
			"\t#A all of the arguments.\n\n"
			"Similar to the #R inject, #A will inject arguments all\n"
			"separated by commas. The difference being that #A will\n"
			"inject all of the supplied arguments. This does not affect\n"
			"whether arguments have been used for #N, #X or #R.\n\n"
			"eg...\n\n"
			"//f MYFN\tarr = [#0, #1, #2, [#A]]\n\n"
			"MYFN(a,b,c,d);\n\n"
			"output...\n\n"
			"arr = [a, b, c, [a,b,c,d]];\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'c':
		puts(
			"\n"
			"\t#C #c argument count.\n\n"
			"Using #C will inject the number of arguments provided to\n"
			"the function or block wrapper. This might differ from the count\n"
			"of parameters in the template. #N, #X and #R are based on the\n"
			"provided argument count, so if you need the number of arguments\n"
			"given to the wrapper for your variadic macro, this is what\n"
			"you would want to use.\n\n"
			"#C will give this number 1-based, while #c is 0-based.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'p':
		puts(
			"\n"
			"\t#P #p parameter count.\n\n"
			"Using #P will inject the number of parameters defined in\n"
			"the original template. This is based on the highest numbered\n"
			"parameter in the template, so if you skip numbers, they will\n"
			"still be counted towards the total.\n\n"
			"#P will give this number 1-based, while #p is 0-based.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'i':
		puts(
			"\n"
			"\t#I #i instance count.\n\n"
			"Hazel remembers how many times a macro is used and you may\n"
			"inject this number using #I. If a macro populated an array\n"
			"and you wanted each usage to increment the index, you can\n"
			"use #I in your template instead of passing the number in\n"
			"as an argument.\n\n"
			"#I will give this number 1-based, while #i is 0-based.\n\n"
			"(hit ENTER to return...)"
		);
		break;
		
	case 'b':
	case 'e':
		puts(
			"\n"
			"\t#B block and #E else block.\n\n"
			"These two parameters are available in block wrapper templates.\n"
			"#B will inject the {closure} immediately after the block\n"
			"wrapper's arguments.\n\n"
			"After the first closure, you may put an 'else' keyword and\n"
			"another closure, just like the else after an if(){} block.\n"
			"Use #E to inject this closure if you are using it.\n\n"
			"(hit ENTER to return...)"
		);
		break;



	default: break;
	}
	while (getchar() != '\n');
	display_help_arguments ();
}


/* ************************************************************* */




