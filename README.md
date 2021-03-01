# hazel
Preprocessor for JavaScript, written in PHP.
Package includes the following files...

hazel.php ............. plain hazel program
hazelr.php ............ with reporting feature
hazel-samples.php ..... sample function hooks
hz.php ................ sample invocation
def.hz ................ sample shortcuts

The sample invocation is a basic PHP script that includes
hazel, then fetches files with a glob from the querystring.
If the destination is a folder, all .hz files within will be 
included in the output. Usage...

<script type="text/javascript" src="hz.php?my/file.hz"></script>
<script type="text/javascript" src="hz.php?my/scripts/"></script>
<script type="text/javascript" src="hz.php?api/class-*.hz"></script>

The sample invocation also includes samples.php and runs
hazel() over def.hz prior to loading the request. This is
how you would define your own set of custom function hooks
and common macros to all your projects. I included my own
with this package as examples, but I would assume anyone 
using Hazel would want to make their own shortcuts.

Make copies of and edit hz.php to suit your needs.

Hazel (c) 2017-2021 - 1002px.com - released under MIT license.
