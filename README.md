# Hazel Preprocessor

A versatile preprocessor for JavaScript, written in C.

Compile `main.c`:

```
gcc main.c -o hazel
```

Massive re-write of Hazel in C. New version is over 10x faster
than the PHP script and includes new features.

## Changes from PHP version...

* Removed dangerous //p and //k commands.
* String escape / trim now native to arguments.
* Comment line initials can now be invalid syntax.
* Added //2 pair matches in closures.
* Added //q quiet includes.
* Added //o output file.

View the folder `php` for the older version of Hazel.

## TODO list...

* Refine minify, minify on //j files.
* Add //m for base64 encoded images.
* Add //k block wrapper keywords.
* Add //e to alias environment variables.
* *Maybe* //x for exec (with flag to allow!)
* Write PHP extension.

## List of commands...

```
//= exact match macros
//v variable name macros
//s string match macros
//c comment line initials
//f function wrappers
//b block wrappers
//2 pair matches in closures
//i include another file
//j include unmodified file
//q include file quietly
//o set output file
```

For complete documentation, run `hazel --help`


