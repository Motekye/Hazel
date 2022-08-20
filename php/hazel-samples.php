<?php

	// 	Sample function hooks for Hazel...


	/*

		FUNCTION HOOK:		SUPPORTING:


		hazel_string_filter, 	hazel_strf

		hazel_base64_image, 	hazel_b64i

		hazel_php_variable, 	hazel_val, hazel_vref, hazel_spill

		hazel_if_variable
		
	*/



	/* ************************************************************************** */

	
	/*

		//k	strf	hazel_string_filter

		strf(func;<>;""){ String_Data... }

		Stack multiple predefined filters or PHP functions
		that take the full block as a single argument. Filters 
		rendered from first to last, ignored if invalid.

		Includes: "" '' <> filters built-in...

	*/


	// process string in { } with php function by name:
	function hazel_string_filter($i,$a,$b,$e){
		if(!$a[0]){ return $b; }

		// process each filter in order...
		for($j=0;$j<count($a);$j++){ 
			$b = hazel_strf($a[$j],$b); }

		return $b; // finished
	}

	// process built-in string function or hook once:
	function hazel_strf($f,$b){ switch($f){

	  // escape contents in single or double quotes:
	  case '""': case "''": $o = ''; $d = '';
		$q = substr($f,0,1);
		for($p=0;$p<strlen($b);$p++){ $c=substr($b,$p,1);
			if($c=="\n"){ $o.="\\n"; }
		else	if($c=="\r"){ $o.="\\r"; }
		else	if($c=="\t"){ $o.="\\t"; }
		// TODO: more escape sequences?
		else	if($c==$q||$c=='\\'){ $o.="\\$c"; }
		else { $o.=$c; } $d = $c; }
		return $q.$o.$q;

	  // escape HTML entities and line breaks:
	  case '<>': $o = ''; $d = '';
		for($p=0;$p<strlen($b);$p++){ $c=substr($b,$p,1);
			if($c=="\n"&&$d!="\r"){ $o.="<br/>"; }
		else	if($c=="\r"){ $o.="<br/>"; }
		else	if($c=="<"){ $o.="&lt;"; }
		else	if($c==">"){ $o.="&gt;"; }
		else	if($c=="&"){ $o.="&amp;"; }
		// TODO: HTML entities for upper-ascii?
		else { $o.=$c; } $d = $c; } 
		return $o;

	  // php function as filter by name with one argument:
	  default: if(!function_exists($f)){ return $b; }
		return call_user_func($f,$b);
		
	} }



/* ************************************************************************** */


	/*

		//p	img64	hazel_base64_image

		myimage = img64(my/image.png);
		myimages = img64(my/1.jpeg,my/2.gif,my/3.png);
		
		Include a single image as base64 string, or a 
		javascript array of images as base64 strings if 
		multiple arguments are provided...

	*/


	// get base64 image as function:	TODO: support glob()
	function hazel_base64_image($i,$a){
		// no image, empty string:
		if(count($a)==0){ return '""'; }
		// one image as string:
		if(count($a)==1){ return '"'.hazel_b64i($a[0]).'"'; }
		// array of images as strings:
		$o = "["; for($j=0;$j<count($a);$j++){
			if($j>0){ $o.=','; }
			$o.='"'.hazel_b64i($a[$j]).'"';
		} return $o."]";
	}

	// get single base64 image from path:
	function hazel_b64i($n){ $m='';
		switch(substr($n,strrpos($n,'.'))){
		case '.gif': $m='gif'; break; 
		case '.jpeg': case '.jpg': $m='jpg'; break; 
		case '.png': $m='png'; break; 
		// TODO: more mime-types to support...
		default: return ""; }
		return 'data:image/'.$m.';base64,'.
			base64_encode(file_get_contents($n));
	}



/* ************************************************************************** */


	/*

		//p	phpv	hazel_php_variable

		username = phpv($_SESSION.user.name);
		
		Include the javascript formatted value of a PHP variable
		using a dotted reference for array members, each term
		beginning with a $ being a $GLOBAL, and any other term 
		being an array key for the previous array.

		PHP arr[20] and arr["20"] are the same index, so keys
		should not need quotes to distinguish themselves...
	
		phpv($data.user.$userno.$cno.post.16);		vs.

		echo $GLOBALS['data']['user'][$GLOBALS['userno']
			[$GLOBALS['cno']]]['post'][16];

		An array in PHP is one dimensional, nesting of arrays
		and keys is not required in this format, as long as
		the term to the left of the . is an array and the 
		term to the right is a key. The reference is processed
		from right to left, to ensure there is always a key,
		not an array, to the right of the . In this manner, any
		nested array structure may be expressed in this 
		flattened, dotted form.

		user = phpv($_SESSION.user.name,$_SESSION.user.icon);

		Use multiple arguments to create an array structure
		from different values. You may also address an array
		to have the array structure spilled out in JavaScript.

		user = phpv($_SESSION.user);

	*/


	// get value of dotted php variable reference(s):
	function hazel_php_variable($i,$a){
		// no variables, return null:
		if(count($a)==0){ return 'null'; }
		// one variable injected:
		if(count($a)==1){ return hazel_val($a[0]); }
		// array of variables injected:
		$o = "["; for($j=0;$j<count($a);$j++){
			if($j>0){ $o.=','; }
			$o.=hazel_val($a[$j]);
		} return $o."]";
	}

	// get value of one variable from dotted array reference:
	function hazel_val($s){ $a = hazel_vref($s);
		// resolve array/key pairs from right to left...
		for($i=count($a)-2;$i>-1;$i--){
		// TODO: support for fetching Object properties as well
		if(gettype($a[$i])=='array'&&gettype($a[$i+1])!='array'){
		  $j = $i+1; $w = $a[$j]; // get next valid key:
		  while($w==''&&$j<count($a)){ $j++; $w = $a[$j]; }
		  // save updated value, invalidating key:
		  if(array_key_exists($w,$a[$i])){
			$a[$i] = $a[$i][$w]; $a[$j] = '';
		} } }
		// check for any untranslated particles:
		// TODO: some kind of error response
		for($i=1;$i<count($a);$i++){ 
			if($a[$i]!=''){ return 'undefined'; } }
		// spill final value copied to first particle:
		return hazel_spill($a[0]);
	}

	// get flat list of arrays and array keys from reference:
	function hazel_vref($r){ 
		// reference must start with valid $ @ # % ! prefix: 
		if(strpos("$@#%!",substr($r,0,1))===false){ return null; }
		// does not currently allow nesting with () or []:
		$a = explode('.',$r);
		// loop through array mapping each particle...
		for($i=0;$i<count($a);$i++){ $c = substr($a[$i],0,1);
		  // map prefixes to a superglobal array:
		  if(strpos("$@#%!",$c)!==false){ switch($c){
		    case '$': $a[$i] = $GLOBALS[substr($a[$i],1)]; break;
		    // TODO: some kind of session error?
		    case '@': if(!session_started()){ return null; }
			$a[$i] = $_SESSION[substr($a[$i],1)]; break;
		    case '#': $a[$i] = $_POST[substr($a[$i],1)]; break;  
		    case '%': $a[$i] = $_GET[substr($a[$i],1)]; break;  
		    case '!': $a[$i] = $_SERVER[substr($a[$i],1)]; break;
		    // TODO: more prefixes like _COOKIES?
		  } }
		  // integer array key: (strings left untouched)
		  if(strpos("0123456789",$c)!==false){ $a[$i] = intval($a[$i]); }
		// flat list of array of arrays and keys:
		} return $a;
	}

	/*

		This function will spill any PHP variable as a 
		javascript primitive, or array / object.

		Function runs itself for array structures.

		Uses hazel_strf to escape quoted string values...

	*/

	// spill the value of any PHP variable as valid javascript:
	function hazel_spill($v){ $o = ''; switch(gettype($v)){

	// dump value of primitives and strings to javascript...
	case 'NULL': return 'null'; case 'unknown type': return 'undefined';
	case 'boolean': if($v){ return 'true'; } else { return 'false'; }
	case 'integer': case 'double': return $v;
	case 'string': return hazel_strf('""',$v);

	// spill array structures recursively...
	case 'array': $o = "["; for($j=0;$j<count($v);$j++){
		if($j>0){ $o.=','; } $o.=hazel_spill($v[$j]);
	} return $o."]";

	// TODO: case 'object':

	// TODO: case 'resource': case 'resource (closed)': 

	} }



/* ************************************************************************** */


	/*

		//k	ifv	hazel_if_variable

		ifv($_SESSION.user.loggedin){ Code... } else { Code... }
		
		Evaluate the against the truthyness of a single PHP
		variable reference in this version, no = < > or math.
		Only one parameter is supported as well.

		Uses same variable reference style and functions as
		the hazel_php_variable function wrapper hook...

	*/


	// server-side 'if' with PHP evaluation:
	function hazel_if_variable($i,$a,$b,$e){

		if(hazel_val($a[0])){ return $b; } 

		else { return $e; }

		// TODO: get some math up on in here.

	}



/* ************************************************************************** */

//  	HAZEL PREPROCESSOR (c) 2017-2021 -- http://1002px.com

?>
