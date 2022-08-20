<?php
/******************************************************************************/
/***  ##   # .####, ###### ###### ##      *************************************/
/***  ###### ##   #   _##  ##___  ##      *************************************/
/***  ##   # ######  ##*   ##^^^  ##      *************************************/
/***  ##   # ##   # ###### ###### ######  *************************************/
/******************************************************************************/


	// 	ENVIRONMENT VARIABLES...


	$HZcomln = array();	//	(count, term)
	$HZexact = array();	//	(count, term, replace)
	$HZvalue = array();	//	(count, term, replace)
	$HZquote = array();	//	(count, term, replace)
	$HZfuncw = array();	//	(count, term, template)
	$HZblock = array();	//	(count, term, template)
	$HZprofn = array();	//	(count, term, fn-name)
	$HZprobk = array();	//	(count, term, fn-name)
	
	$HZsrc = '';		//	all source code
	$HZpos = 0;		//	position in source
	$HZlen = 0;		//	length of source
	$HZout = '';		//	output data
	$HZbuf = '';		//	current buffer variable
	$HZlcl = '';		//	last code literal
	$HZlcp = -1;		//	last code position


  // 	FUNCTIONS DEFINED...


  // EXECUTE:	hazel, HZexe, HZpar, HZtmp

  // COMMANDS:	HZltm, HZglb, HZexm, HZvrm, HZfnw, HZprf, HZbkw, HZprb

  // SOURCE:	HZgo2, HZadv, HZout, HZnil, HZlst, HZeof, HZchr, HZerr, 
  //		HZats, HZids, HZnnl, HZwch, HZskp, HZins, HZlit, HZwsp, 
  //		HZnxc, HZstr, HZ2nb, HZrgx, HZ2ar

  // REPORTING:	HZtrk, HZrep

  // REP. VARS:	$HZterms, $HZcount, $HZliteral, $HZlastchr, $HZkeywords


  // 	PHP FUNCTIONS USED...


  //	array, strlen, strpos, array_push, explode, count, trim, ltrim, 
  //	substr, join, glob, file_get_contents, gettype, 
  //	function_exists, call_user_func


/******************************************************************************/


function  hazel  ( $src, $min=true, $rep=false ){
	
	
	// reset environment:
	$GLOBALS['HZsrc'] = $src;
	$GLOBALS['HZpos'] = 0;
	$GLOBALS['HZlen'] = strlen($src);
	$GLOBALS['HZout'] = '';
	$GLOBALS['HZbuf'] = 'HZout';
	//$GLOBALS['HZlcl'] = '';	TODO: reset needed?
	//$GLOBALS['HZlcp'] = -1;


	// .....................................................
	for(;$GLOBALS['HZpos']<$GLOBALS['HZlen'];HZadv()){
	// .....................................................


	    // translate any type of match in codespace...		(CSM)
	    if(

	    //  exact match	variable name		(matches)
		HZexm() || 	HZvrm() || 

	    //  fn wrapper	block wrapper		(wrappers)
		HZfnw() || 	HZbkw() || 

	    //  procedure	pr block		(procedures)
		HZprf() ||	HZprb()


	    ){ $GLOBALS['HZpos']--; continue; }


	// .....................................................


	// check for white-space and comments / hazel commands...
	$n = HZwsp(); if(strlen($n)>0){ HZadv(strlen($n)-1);
		if(!$min){ HZout($n); } else if(HZwch()){ HZout(' '); }
/*RPR*/		if($rep){ HZrep(' '); } 
		continue; }
	
	// catalogue all literal javascript values...
	$n = HZlit(); if($n!=''){ HZadv(strlen($n)-1); $n=HZltm($n); HZout($n); 
		$GLOBALS['HZlcl']=$n; $GLOBALS['HZlcp']=$GLOBALS['HZpos'];
/*RPR*/		if($rep){ HZrep($n); } 
		continue; }


		HZerr("UNRECOGNIZED LITERAL VALUE");


	// .....................................................
	}
	// .....................................................


/*RPR*/		if($rep){ HZrep("\n"); }

		return $GLOBALS['HZout']; }


/******************************************************************************/





/******************************************************************************/
/***  ###### ##   # ######  ####_ ##   # ###### ######  ***********************/
/***  ##___   ###*  ##___  ##   ` ##   #   ##   ##___   ***********************/
/***  ##^^^   _###  ##^^^  ##   _ ##   #   ##   ##^^^   ***********************/
/***  ###### #   ## ######  ####` `####`   ##   ######  ***********************/
/******************************************************************************/


// check for and execute hazel command at position...
function HZexe($p=-1){
	if($p==-1){ $p=$GLOBALS['HZpos']; }
	$d=HZchr($p,2); if($d!='//'&&$d!='##'){ return ''; }
	$z=HZchr($p+2,1); // check for known Hazel command:
	if(strpos('=vsfbpkijc_',$z)===false){ return ''; }
	// get parameters and execute command...
	list($i,$a,$o)=HZpar($p); $y=''; $j=$i; $g='';
	switch($z){
							// INCLUDE FILES:
	  case 'i': HZins(HZglb($a),$i); break;
	  case 'j': HZout(HZglb($a)); break;		// COMMENT LINES:

	  case 'c': array_push($GLOBALS['HZcomln'],0,trim($a)); break;
  
	  case '=': $y='exact'; break;			// MACROS, WRAPPERS:
	  case 'v': $y='value'; break; case 's': $y='quote'; break;
	  case 'f': $y='funcw'; break; case 'b': $y='block'; break;
	  case 'p': $y='profn'; break; case 'k': $y='probk'; break;

	} if($y){ $x=explode("\t",$a,2); $m=$x[0]; $n='';
		if(count($x)==2){ $n=$x[1]; }
		array_push($GLOBALS['HZ'.$y],0,trim($m),ltrim($n)); }
	
	// return up to end point:
	return $o; }


/******************************************************************************/


// get hazel command parameter line (and carried lines using //_)...
function HZpar($p=-1){ $o='';
	if($p==-1){ $p=$GLOBALS['HZpos']; } $s=$GLOBALS['HZsrc'];
	// get parameters up to new line...
	$a=''; for($i=$p+3;$i<strlen($s);$i++){ $c=substr($s,$i,1);
	  if($c=="\r"){ if(substr($s,$i+1,1)=="\n"){ $i++; } $c="\n"; }
	  if($c=="\n"){ break; } $a.=$c; }
	$a=trim($a); $o="//".HZchr($p+2,$i-$p-2);
	// skip leading whitespace: (allows indentation)
	$k=''; for($j=$i+1;$j<strlen($s);$j++){ 
	  $c=substr($s,$j,1); if($c==' '||$c=="\t"){ continue; }
	  // found carried line...
	  $d=substr($s,$j,4); if($d=="//_\t"||$d=="##_\t"){
	    // skip all spaces and tabs, then get contents to newline:
	    for($j+=4;$j<strlen($s);$j++){ $c=substr($s,$j,1);
	      if($c==' '||$c=="\t"){ continue; } break; }
	    for(;$j<strlen($s);$j++){ $c=substr($s,$j,1);
	      if($c=="\r"){ if(substr($s,$j+1,1)=="\n"){ $j++; } $c="\n"; }
	      if($c=="\n"){ break; } $k.=$c; } 
	    // adv, add param line, always present // not ##
	    $i=$j; $a.=' '.$k; $o.="\r\n//_\t".$k; continue; }
	break; }
	// return exit position, parameters and unchanged source:
	return array($i,$a,$o); }


/******************************************************************************/


// construct a template for function and block wrappers...
function HZtmp($t,$i,$a,$b='',$e=''){
	// count arguments ahead of time to track final values:
	$y=array(); for($j=0;$j<count($a);$j++){ array_push($y,false); }

	// break template apart into particles separated by injects...
	$x=array(''); $l=0; for($j=0;$j<strlen($t);$j++){
	$c=substr($t,$j,1); if($c=='#'){ $k=''; $d=substr($t,$j+1,1);
	  // check for template commands with # and a letter:
	  // TODO: add #L #U #^ or #; for arguments-left loops?
	  switch($d){ case 'B': case 'E': case 'I':
	    case 'A': case 'R': case 'N': case 'X': case 'C': $k=$d; }
	  // check for #0-#99 for numbered arguments: TODO: better than this...
	  if($k==''){ for($k=count($a)-1;$k>-1;$k--){
	    if(substr($t,$j+1,strlen($k))==$k){ break; } } }
	  // no argument found or invalid command, use empty string: 
	  if($k!=-1){ array_push($x,$k,''); $j+=strlen($k); $l+=2; 
	    continue; } } $x[$l].=$c; }
	// ...........................................................

	// remember which particles are translated already:
	$z=array(); for($j=0;$j<count($x);$j++){ array_push($z,false); }
	// skip through template converting numbered arguments:
   	for($j=1;$j<count($x);$j+=2){ if(gettype($x[$j])=='integer'){
	  $y[$x[$j]]=true; $x[$j]=$a[$x[$j]]; $z[$j]=true; 
	} }
	// then again, incrementing #N and #X arguments:
	for($j=1;$j<count($x);$j+=2){ if($z[$j]){ continue; } switch($x[$j]){
	  case 'N': for($k=0;$k<count($y);$k++){ if(!$y[$k]){ break; } }
		if($k==count($y)){ $x[$j]=''; } 
		else { $y[$k]=true; $x[$j]=$a[$k]; } break;
	  case 'X': for($k=count($y)-1;$k>-1;$k--){ if(!$y[$k]){ break; } }
		if($k==count($y)){ $x[$j]=''; } 
		else { $y[$k]=true; $x[$j]=$a[$k]; } break;
	} }
	// then finally convert special static values:
	for($j=1;$j<count($x);$j+=2){ if($z[$j]){ continue; } switch($x[$j]){
	  case 'R': $x[$j]=''; for($k=0;$k<count($y);$k++){ 
		if(!$y[$k]){ if($x[$j]!=''){ $x[$j].=', '; } 
		$x[$j].=$a[$k]; } } break;	
	  case 'A': $x[$j]=join(',',$a); $z[$j]=true; break;
	  case 'C': $x[$j]=count($a); $z[$j]=true; break;
	  case 'B': $x[$j]=$b; $z[$j]=true; break;
	  case 'E': $x[$j]=$e; $z[$j]=true; break;
	  case 'I': $x[$j]=$i; $z[$j]=true; break;
	} }
	return join($x); }


/******************************************************************************/





/******************************************************************************/
/***   ####_  ####  ##   # ##   # .####, ##,  # #####, .####_  ****************/
/***  ##   ` ##`  # ### ## ### ## ##   # ###, # ##   # ##__ ^  ****************/
/***  ##   _ ##, .# ## * # ## * # ###### ## #,# ##   # __^^##  ****************/
/***   ####`  ####  ##   # ##   # ##   # ##  ## #####` *####   ****************/
/******************************************************************************/


// check literal type and do matches...
function HZltm($s){ $c=substr($s,0,1); switch($c){
	case '"': case "'": case "`": 
	$s=$c.HZstm(substr($s,1,-1)).$c; break; } return $s; }

// return an amalgam of all files or an empty string...
function HZglb($n){ $o=''; if(substr($n,0,1)=='/'){ 
	  $n = $_SERVER['DOCUMENT_ROOT'].$n; }
	$g = glob($n); for($i=0;$i<count($g);$i++){ 
	  $o.=file_get_contents($g[$i]); } return $o; }

// check for exact match and inject value, advancing...			(CSM)
function HZexm(){ for($i=count($GLOBALS['HZexact'])-3;$i>-1;$i-=3){
	if(HZats($GLOBALS['HZexact'][$i+1])){
	  HZadv(strlen($GLOBALS['HZexact'][$i+1])); 
	  HZins($GLOBALS['HZexact'][$i+2]);
	  $GLOBALS['HZexact'][$i]++; return true; } } return false; }

// convert all matches found in a string and return...
function HZstm($s){ $o=''; for($i=0;$i<strlen($s);$i++){ $c=substr($s,$i,1);
	for($j=count($GLOBALS['HZquote'])-3;$j>-1;$j-=3){
	  if(substr($s,$i,strlen($GLOBALS['HZquote'][$j+1]))
	  ==$GLOBALS['HZquote'][$j+1]){ $o.=$GLOBALS['HZquote'][$j+2];
	  $i+=strlen($GLOBALS['HZquote'][$j+1])-1; $GLOBALS['HZquote'][$j]++;
	continue 2; } } $o.=$c; } return $o; }

// check for a variable name match and inject value, advancing...	(CSM)
function HZvrm(){ if(!HZids()||HZats('.',-1)){ return false; }
	for($i=count($GLOBALS['HZvalue'])-3;$i>-1;$i-=3){
	  $m = $GLOBALS['HZvalue'][$i+1]; 
	  if(HZats($m)&&!HZats(':',strlen($m))){
	    HZadv(strlen($GLOBALS['HZvalue'][$i+1])); 
	    HZins($GLOBALS['HZvalue'][$i+2]);
	    $GLOBALS['HZvalue'][$i]++; return true; } } return false; }

// check for function wrapper and execute, advancing...			(CSM)
function HZfnw(){ if(!HZids()){ return false; }
	for($i=0;$i<count($GLOBALS['HZfuncw']);$i+=3){
	  $m = $GLOBALS['HZfuncw'][$i+1]; if(HZats($m.'(')){
	    // get arguments in brackets:
	    HZadv(strlen($m)); $t = $GLOBALS['HZfuncw'][$i+2];
	    list($p,$a) = HZ2ar(','); HZadv(strlen($p)+2);
	    // return finished template:
	    HZins(HZtmp($t,$GLOBALS['HZfuncw'][$i],$a));
	    $GLOBALS['HZfuncw'][$i]++;
	return true; } } return false; }

// check for PHP procedure function wrapper and exectute, advancing...	(CSM)
function HZprf(){ if(!HZids()){ return false; }
	for($i=0;$i<count($GLOBALS['HZprofn']);$i+=3){
	  $m = $GLOBALS['HZprofn'][$i+1]; if(HZats($m.'(')){
	    // get arguments in brackets:
	    HZadv(strlen($m)); $t = $GLOBALS['HZprofn'][$i+2];
	    list($p,$a) = HZ2ar(','); HZadv(strlen($p)+2);
	    // check for PHP function and execute:
	    $f = $GLOBALS['HZprofn'][$i+2];
	    if(!function_exists($f)){ HZerr('ILLEGAL FUNCTION '.$f); }
	    HZins(call_user_func($f,$GLOBALS['HZprofn'][$i],$a));
	    $GLOBALS['HZprofn'][$i]++;
	return true; } } return false; }

// check for block wrapper and exectute, advancing...			(CSM)
function HZbkw(){ if(!HZids()){ return false; }
	for($i=0;$i<count($GLOBALS['HZblock']);$i+=3){
	  $m = $GLOBALS['HZblock'][$i+1]; if(HZats($m.'(')){
	    // get arguments in brackets and block:
	    HZadv(strlen($m)); $t = $GLOBALS['HZblock'][$i+2];
	    list($p,$a) = HZ2ar(';'); HZadv(strlen($p)+2);
	    $b = HZ2nb(); HZadv(strlen($b)+2);
	    // check for else block after instance:
	    $e = ''; $j = $GLOBALS['HZpos']; $n = HZwsp(); 
	    if(HZats('else',strlen($n))){
	      $j+=strlen($n)+4; $n = HZwsp(strlen($n)+4); $j+=strlen($n);
	      if(HZchr($j)=='{'){ $e = HZ2nb($j); $j+=strlen($e); HZgo2($j+2);
	    } } // return finished template:
	    HZins(HZtmp($t,$GLOBALS['HZblock'][$i],$a,$b,$e));
	    $GLOBALS['HZblock'][$i]++;
	return true; } } return false; }

// check for PHP procedure block wrapper and exectute, advancing...	(CSM)
function HZprb(){ if(!HZids()){ return false; }
	for($i=0;$i<count($GLOBALS['HZprobk']);$i+=3){
	  $m = $GLOBALS['HZprobk'][$i+1]; if(HZats($m.'(')){
	    // get arguments in brackets and block:
	    HZadv(strlen($m)); $t = $GLOBALS['HZprobk'][$i+2];
	    list($p,$a) = HZ2ar(';'); HZadv(strlen($p)+2);
	    $b = HZ2nb(); HZadv(strlen($b)+2);
	    // check for else block after instance:
	    $e = ''; $j = $GLOBALS['HZpos']; $n = HZwsp(); 
	    if(HZats('else',strlen($n))){
	      $j+=strlen($n)+4; $n = HZwsp(strlen($n)+4); $j+=strlen($n);
	      if(HZchr($j)=='{'){ $e = HZ2nb($j); $j+=strlen($e); HZgo2($j+2);
	    } } // check for PHP function and execute:
	    $f = $GLOBALS['HZprobk'][$i+2];
	    if(!function_exists($f)){ HZerr('ILLEGAL FUNCTION '.$f); }
	    HZins(call_user_func($f,$GLOBALS['HZprobk'][$i],$a,$b,$e));
	    $GLOBALS['HZprobk'][$i]++;
	return true; } } return false; }


/******************************************************************************/





/******************************************************************************/
/***  .####_  ####  ##   # #####,  ####_ ######  ******************************/
/***  ##__ ^ ##`  # ##   # ##  .# ##   ` ##___   ******************************/
/***  __^^## ##, .# ##   # ####*  ##   _ ##^^^   ******************************/
/***  *####   ####  `####` ##  ^#  ####` ######  ******************************/
/******************************************************************************/


// go to a specific position...
function HZgo2($n){ $GLOBALS['HZpos']=$n; }

// advance a number of characters...
function HZadv($n=1){ $GLOBALS['HZpos']+=$n; }

// output data to buffer...
function HZout($s){ $GLOBALS[$GLOBALS['HZbuf']].=$s; }

// check if output is empty...
function HZnil(){ if(HZlst()==''){ return true; } return false; }

// get last character(s) outputted...
function HZlst($n=1){ return substr($GLOBALS[$GLOBALS['HZbuf']],-1*$n); }

// check for end of file...
function HZeof(){ if($GLOBALS['HZpos']>=$GLOBALS['HZlen'])
	{ return true; } return false; }

// get character(s) from source...
function HZchr($p=-1,$l=1){ if($p==-1){ $p = $GLOBALS['HZpos']; }
	return substr($GLOBALS['HZsrc'],$p,$l); }

// print error message and end transmission...
function HZerr($s){ HZout("\r\n// HAZEL ERROR >>> $s!!!\r\n");
	$GLOBALS['HZpos'] = $GLOBALS['HZlen']; }

// check if at a sequence in source...
function HZats($s,$p=0){ $p+=$GLOBALS['HZpos']; if(substr($GLOBALS['HZsrc'],
	$p,strlen($s))==$s){ return true; } return false; }

// check if current character starts identifier...
function HZids(){ if(HZnil()){ return true; } $c=HZlst(); if(HZnnl($c))
	{ return true; } return false; }

// character is not a letter character...
function HZnnl($c){ if($c==''){ return true; }
	if(strpos("\r\n\t ,.:;=})]>{([<+-*/%!?&|~^'\"`",$c)===false)
	{ return false; } return true; }

// check if spaces required between words...
function HZwch(){ if(HZnil()){ return false; }
	if(!HZnnl(HZchr($GLOBALS['HZpos']+1))
	&& !HZnnl(substr($GLOBALS['HZout'],-1))){ return true; } return false; }

// get any characters in string from source, advancing...
function HZskp($m){ $o=''; for($i=$GLOBALS['HZpos'];$i<$GLOBALS['HZlen'];$i++){
	$c=HZchr($i); if(strpos($m,$c)==false){ break; } $o.=$c; HZadv(); } 
	return $o; }

// include a string at position in source...
function HZins($s,$p=-1){ if($p==-1){ $p=$GLOBALS['HZpos']; } 
	$GLOBALS['HZsrc']=substr($GLOBALS['HZsrc'],0,$p).$s.substr(
	$GLOBALS['HZsrc'],$p); $GLOBALS['HZlen']=strlen($GLOBALS['HZsrc']); }

// get next codespace literal at position...
function HZlit($p=-1){ if($p==-1){ $p=$GLOBALS['HZpos']; } 
	$c = substr($GLOBALS['HZsrc'],$p,1); switch($c){
	case '"': case '`': case "'": return HZstr($p);
	// get next comment or regular expression:
	case '/': $n=HZnxc($p); if(!$n){ $n=HZrgx($p); } return $n;
	default: return $c; } }

// get all comments and white-space relative to position...
function HZwsp($p=0){ $p+=$GLOBALS['HZpos'];
	$o=''; for($i=$p;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i); switch($c){
	case ' ': case "\t": case "\r": case "\n": $o.=$c; continue 2;
	case '/': case '#': $n=HZexe($i); if($n==''){ $n=HZnxc($i); } 
	  if($n!=''){ $o.=$n; $i+=strlen($n)-1; continue 2; }
	} break; } return $o; }

// get next line or block comment at position...
function HZnxc($p=-1){ if($p==-1){ $p=$GLOBALS['HZpos']; } $c=HZchr($p,2);
	if($c=='//'){ $o=''; // line comments:
	  for($i=$p;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i); $o.=$c;
	    if($c=="\r"){ if(HZchr($i+1)=="\n"){ $o.="\n"; } }
	    if($c=="\r"||$c=="\n"){ break; } } return $o; }
	if($c=='/*'){ $o=''; // block comments:
	  for($i=$p;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i); $o.=$c; 
	    if($c=="/"&&HZchr($i-1)=="*"){ break; } } return $o; } return ''; }

// get string literal at position...
function HZstr($p=-1){ if($p==-1){ $p=$GLOBALS['HZpos']; } 
	$s=$GLOBALS['HZsrc']; $o=''; $z=substr($s,$p,1);
	for($i=$p+1;$i<strlen($s);$i++){ $c=substr($s,$i,1);
	  if($c=="\\"){ $o.=substr($s,$i,2); $i++; continue; }
	  if($c=="\r"){ if(substr($s,$i+1,1)=="\n"){ $i++; } $c="\n"; }
	  if($c=="\n"&&$z!='`'){ $c="\\n"; continue; } // < auto escape
	  if($c==$z){ return $z.$o.$z; } $o.=$c; } return ''; }

// get up to next matching end bracket, codewise...
function HZ2nb($p=-1){ if($p==-1){ $p=$GLOBALS['HZpos']; } $z=HZchr($p);
	$o=''; for($i=$p+1;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i);
	  // resolving bracket structure:
	  if($c=='('||$c=='['||$c=='{'){ $z.=$c; }
	  if(($c==')'&&substr($z,-1)=='(') || ($c==']'&&substr($z,-1)=='[')
	  || ($c=='}'&&substr($z,-1)=='{') ){ $z=substr($z,0,-1); }
	  if(strlen($z)==0){ return $o; }
	  // advance in source by whole literal:
	  $c=HZlit($i); $o.=$c; $i+=strlen($c)-1; } }

// get javascript regular expression literal or just slash...
function HZrgx($i){ if(HZchr($i)!='/'){ return ''; } $m=0;
	$l=substr($GLOBALS['HZlcl'],-1); $o='/'; $k=$GLOBALS['HZlcp'];
	if($l==''||strpos(",.:;=>{([<+-*/%!?&|~^",$l)===false
	&& HZchr($GLOBALS['HZlcp']-5,6)!='return'){ return '/'; }
	for($i++;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i);
	  if($c=="\r"||$c=="\n"){ return '/'; }
	  if($c=="\\"){ $o.=HZchr($i,2); $i++; continue; }
	  if($c=="]"){ $m=0; } else if($c=="["){ $m=1; }
	  elseif($c=='/'){ if($m==0){ break; } } $o.=$c; } $o.='/';
	for($i++;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i);
	  if(strpos("igmsuy",$c)!==false){ $o.=$c; } 
	  else { break; } } return $o; }

// get argument set string to array, codewise...
function HZ2ar($d,$p=-1){ $a=array(''); $l=0;
	if($p==-1){ $p=$GLOBALS['HZpos']; } $z=HZchr($p);
	$o=''; for($i=$p+1;$i<$GLOBALS['HZlen'];$i++){ $c=HZchr($i);
	  // tracking levels of bracket structure:
	  if($c=='('||$c=='['||$c=='{'){ $z.=$c; }
	  if(($c==')'&&substr($z,-1)=='(') || ($c==']'&&substr($z,-1)=='[')
	  || ($c=='}'&&substr($z,-1)=='{') ){ $z=substr($z,0,-1); }
	  if(strlen($z)==0){ return array($o,$a); }
	  // delimiter on top level? add another argument to the output:
	  if($c==$d&&$z=='('){ array_push($a,''); $l++; $o.=','; continue; }
	  // advance in source by whole literal:
	  $c=HZlit($i); $o.=$c; $a[$l].=$c; $i+=strlen($c)-1;
	  // invalid literal found...
	  if(strlen($c)==0){ return array('ERROR',null); } } }


/******************************************************************************/





/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/





/******************************************************************************/
/***  #####, ###### #####_  ####  #####, ######  ******************************/
/***  ##  .# ##___  ##  .# ##`  # ##  .#   ##    ******************************/
/***  ####*  ##^^^  #####^ ##, .# ####*    ##    ******************************/
/***  ##  ^# ###### ##      ####  ##  ^#   ##    ******************************/
/******************************************************************************/


	$HZterms = array();		// all terms used
	$HZcount = array();		// count of each use

	$HZliteral = '';		// building literal...
	$HZlastchr = '';


// TODO: keep track of local and global variables...
function HZtrk($w){

	}

// recieve literal or character for report...
function HZrep($w){ $fn=false; 

	$l=$GLOBALS['HZliteral']; $p=substr($l,0,1);
	$q=substr($GLOBALS['HZliteral'],-1);
	
	// white space and symbols break literals:
	if($w==' '||$w=="\n"||HZnnl(substr($w,-1))||HZnnl($q)){ $fn=true; }

	// as well as non symbols after symbols:
	if(HZnnl($p)&&!HZnnl($w)){ $fn=true; }

	// unless the symbols complete a valid operator:
	if($w!=' '&&HZnnl($p)&&HZnnl($q)&&strpos(
	"++ += -- -= *= /= %= **= <<<= >>>= &= ^= |= === !== >= <= && ||",
	$l.$w)!==false){ $fn=false; }
	
	// dot builds next literal:
	if($l=='.'){ $fn=false; }
	if($p=='.'&&!HZnnl($w)){ $fn=false; }
	if($w=='.'&&strpos('0123456789',$p)!==false){ $fn=false; }
	if($l=='-'&&strpos('0123456789',$w)!==false){ $fn=false; }
	
	// colon can also latch onto an identifier:
	if($w==':'&&!HZnnl($q)&&$p!='.'
	&& strpos('0123456789',$q)===false){ $fn=false; }
	// but all literals end after a color:
	if($q==':'){ $fn=true; }

	// finishing literal...
	if($fn){ if($l!=' '&&$l!=""){
		for($i=0;$i<count($GLOBALS['HZterms']);$i++){
		  if($GLOBALS['HZterms'][$i]==trim($l)){
		    $GLOBALS['HZcount'][$i]++; break; } }
		if($i==count($GLOBALS['HZterms'])){
		  array_push($GLOBALS['HZterms'],trim($l));
		  array_push($GLOBALS['HZcount'],1); }
		// record context of literal:
	} $GLOBALS['HZliteral'] = ''; }

	/* add to literal: */ $GLOBALS['HZliteral'].=$w;

	if($w!="\n"){ return false; }	// finishing report...
	// ================================
	$sc = array(); $un = array(); $ct = array(); 
	$k = '                //  ';

	$o = $k."SHOWING COUNT OF ALL OPERATORS USED  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n"; $n=''; $m=''; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = substr($GLOBALS['HZterms'][$i],0,1);
	  $d = substr($GLOBALS['HZterms'][$i],1,1);
	  $e = $GLOBALS['HZterms'][$i];
	  if($c==''){ continue; } if($d==''){ $d=' '; }
	  if($c=='/'&&($e!='/'&&$e!='/=')){ continue; }
	  if(!HZnnl($c)||strpos("\"'`.",$c)!==false){ continue; }
	  if($c=='-'&&strpos("0123456789",$d)!==false){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n .= substr("       ",0,-1*strlen($GLOBALS['HZterms'][$i]));
	    $n .= $GLOBALS['HZterms'][$i]; $GLOBALS['HZterms'][$i]=''; 
	    $m .= substr("       ",0,-1*strlen($GLOBALS['HZcount'][$i]));
	    $m .= $GLOBALS['HZcount'][$i];
	    if(strlen($n)>108){ $o.="    $n\r\n    $m\r\n\r\n"; $n=''; $m=''; }
	} $o.="    $n\r\n    $m\r\n\r\n\r\n";
	$sc['o']=$o; $un['o']=$u; $ct['o']=$a;

	$o = $k."SHOWING USAGE OF JAVASCRIPT KEYWORDS  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = $GLOBALS['HZterms'][$i]; if($c==''
	  || strpos($GLOBALS['HZkeywords']," $c ")===false){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  '.$c;
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['k']=$o; $un['k']=$u; $ct['k']=$a;

	$o = $k."SHOWING LITERAL NUMBERS FOUND  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = substr($GLOBALS['HZterms'][$i],0,1); 
	  $d = substr($GLOBALS['HZterms'][$i],-1);
	  if($c==''||(($c!='-'&&$c!='.')
	  && strpos("0123456789",$c)===false)){ continue; }
	  if(strpos("0123456789",$d)===false
	  && substr($GLOBALS['HZterms'][$i],0,2)!='0x'){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  ('.$GLOBALS['HZterms'][$i].')';
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['n']=$o; $un['n']=$u; $ct['n']=$a;

	$o = $k."SHOWING OCCURENCES OF STRING LITERALS  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = substr($GLOBALS['HZterms'][$i],0,1); 
	  if($c==''||strpos("\"'`",$c)===false){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  '.$GLOBALS['HZterms'][$i];
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['s']=$o; $un['s']=$u; $ct['s']=$a;

	$o = $k."SHOWING OCCURENCES OF REGULAR EXPRESSIONS  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = substr($GLOBALS['HZterms'][$i],0,1); 
	  if($c!='/'){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  '.$GLOBALS['HZterms'][$i];
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['r']=$o; $un['r']=$u; $ct['r']=$a;

	$o = $k."SHOWING PROPERTY DECLARATIONS AND USAGES  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){
	  $c = substr($GLOBALS['HZterms'][$i],0,1);
	  $d = substr($GLOBALS['HZterms'][$i],-1);
	  if($c==''||($c!='.'&&$d!=':')){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  '.$GLOBALS['HZterms'][$i];
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['p']=$o; $un['p']=$u; $ct['p']=$a;

	$o = $k."SHOWING ALL IDENTIFIER VARIABLE NAMES  ";
	while(strlen($o)<120){ $o.='.'; } 
	$o .= "\r\n\r\n\r\n    "; $x=0; $u=0; $a=0;
	for($i=0;$i<count($GLOBALS['HZterms']);$i++){ 
	  if($GLOBALS['HZterms'][$i]==''){ continue; }
	    $u++; $a+=$GLOBALS['HZcount'][$i];
	    $n=$GLOBALS['HZcount'][$i].'  '.$GLOBALS['HZterms'][$i];
	    $n=substr('       ',strlen($GLOBALS['HZcount'][$i])).$n;
	    $y=24-(strlen($n)%24); for($j=0;$j<$y;$j++){ $n.=' '; }
	    if($x+strlen($n)>128){ $o.="\r\n\r\n    "; $x=0; } 
	    $o.=$n; $x+=strlen($n); $GLOBALS['HZterms'][$i]='';
	} $o.="\r\n\r\n\r\n";
	$sc['v']=$o; $un['v']=$u; $ct['v']=$a;

	// ================================

	$rd = "okvpnrs";

	$r = '';
	$s = $k."GENERAL DOCUMENT STATISTICS";
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = strlen($GLOBALS['HZout']); while(strlen($n)<10){ $n=' '.$n; }
	$s = $k.'TOTAL SIZE: '.$n.'  bytes  ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['k']; while(strlen($n)<12){ $n=' '.$n; }
	$s = $k.'KEYWORDS: '.$n.'      unique:  ';
	$n = $un['k']; while(strlen($n)<10){ $n=' '.$n; } 
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['v']; while(strlen($n)<9){ $n=' '.$n; }
	$s = $k.'IDENTIFIERS: '.$n.'      unique:  ';
	$n = $un['v']; while(strlen($n)<10){ $n=' '.$n; }
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['p']; while(strlen($n)<10){ $n=' '.$n; }
	$s = $k.'PROPERTIES: '.$n.'      unique:  ';
	$n = $un['p']; while(strlen($n)<10){ $n=' '.$n; }
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['n']; while(strlen($n)<13){ $n=' '.$n; }
	$s = $k.'NUMBERS: '.$n.'      unique:  ';
	$n = $un['n']; while(strlen($n)<10){ $n=' '.$n; }
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['r']; while(strlen($n)<14){ $n=' '.$n; }
	$s = $k.'REGEXP: '.$n.'      unique:  ';
	$n = $un['r']; while(strlen($n)<10){ $n=' '.$n; }
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";
	
	$n = $ct['s']; while(strlen($n)<13){ $n=' '.$n; }
	$s = $k.'STRINGS: '.$n.'      unique:  ';
	$n = $un['s']; while(strlen($n)<10){ $n=' '.$n; }
	$s.= $n.'    ';
	while(strlen($s)<120){ $s.='.'; } $r.=$s."\r\n\r\n";

	$s = $k; while(strlen($s)<120){ $s.='.'; } $r .= "$s\r\n\r\n\r\n";

	// ================================
	HZout("\r\n\r\n".<<<EOT
/******************************************************************************************************************************/
/***  ##   # .####, ###### ###### ##         #####, ###### #####_  ####  #####, ######  ***************************************/
/***  ###### ##   #   _##  ##___  ##         ##  .# ##___  ##  .# ##`  # ##  .#   ##    ***************************************/
/***  ##   # ######  ##*   ##^^^  ##         ####*  ##^^^  #####^ ##, .# ####*    ##    ***************************************/
/***  ##   # ##   # ###### ###### ######     ##  ^# ###### ##      ####  ##  ^#   ##    ***************************************/
/******************************************************************************************************************************
EOT
);	HZout("\r\n\r\n\r\n$r");
	for($i=0;$i<strlen($rd);$i++){ HZout($sc[substr($rd,$i,1)]); }
	HZout("*/              "); 
	HZout("//  HAZEL PREPROCESSOR (c) 2017-2021 -- http://1002px.com  ");
	HZout('.............................................');
	HZout("\r\n\r\n\r\n\r\n\r\n\r\n");
}

$HZkeywords = " abstract arguments await boolean void break byte case catch char class const continue debugger default delete do double else enum eval export extends false final finally float for function goto if implements import in instanceof int interface let long native new null undefined package private protected public return short static super switch synchronized this self throw throws transient true try typeof var void volatile while with yield Array Number Boolean Date Error Object Symbol TypeError Math Window window Screen screen Function String RegExp Console console Document document Event History history HTMLCollection Location location Navigator navigator Int8Array Uint8Array Uint8ClampedArray Int16Array Uint16Array Int32Array Uint32Array Float32Array Float64Array BigInt64Array BigUint64Array Infinity isInfinity NaN isNaN JSON this parseInt parseFloat WeakMap Map global globalThis Image BeforeUnloadEvent DOMStringMap ErrorEvent GlobalEventHandlers HTMLAnchorElement HTMLAreaElement HTMLAudioElement HTMLBRElement HTMLBaseElement HTMLBaseFontElement HTMLBodyElement HTMLButtonElement HTMLCanvasElement HTMLContentElement HTMLDListElement HTMLDataElement HTMLDataListElement HTMLDialogElement HTMLDivElement HTMLDocument HTMLElement HTMLEmbedElement HTMLFieldSetElement HTMLFormControlsCollection HTMLFormElement HTMLFrameSetElement HTMLHRElement HTMLHeadElement HTMLHeadingElement HTMLHtmlElement HTMLIFrameElement HTMLInputElement HTMLIsIndexElement HTMLKeygenElement HTMLLIElement HTMLLabelElement HTMLLegendElement HTMLLinkElement HTMLMapElement HTMLMediaElement HTMLMetaElement HTMLMeterElement HTMLModElement HTMLOListElement HTMLObjectElement HTMLOptGroupElement HTMLOptionElement HTMLOptionsCollection HTMLOutputElement HTMLParagraphElement HTMLParamElement HTMLPictureElement HTMLPreElement HTMLProgressElement HTMLQuoteElement HTMLScriptElement HTMLSelectElement HTMLShadowElement HTMLSourceElement HTMLSpanElement HTMLStyleElement HTMLTableCaptionElement HTMLTableCellElement HTMLTableColElement HTMLTableDataCellElement HTMLTableElement HTMLTableHeaderCellElement HTMLTableRowElement HTMLTableSectionElement HTMLTemplateElement HTMLTextAreaElement HTMLTimeElement HTMLTitleElement HTMLTrackElement HTMLUListElement HTMLUnknownElement HTMLVideoElement HashChangeEvent History ImageData Location MessageChannel MessageEvent MessagePort Navigator NavigatorGeolocation NavigatorID NavigatorLanguage NavigatorOnLine NavigatorPlugins PageTransitionEvent Plugin PluginArray PopStateEvent PortCollection PromiseRejectionEvent RadioNodeList Transferable ValidityState Window WindowBase64 WindowEventHandlers WindowTimers ";


/******************************************************************************/

//  	HAZEL PREPROCESSOR (c) 2017-2021 -- http://1002px.com

?>
