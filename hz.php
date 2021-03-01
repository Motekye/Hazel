<?php

	// 	Hazel glob or entire folder including samples and def.hz
	
	// .............................................................

	include_once('hazelr.php');		// use reporting version
	include_once('hazel-samples.php');	// sample function hooks
	include_once('csssh.php');		// CSS Short-hand filter
	
	header("Content-type: text/javascript\r\n");

	$QUERY = urldecode($_SERVER['QUERY_STRING']);

	// start request with ! to disable minify, start with ? for reporting:
	if(substr($QUERY,0,1)=='!'){ $QUERY=substr($QUERY,1); $m=false; } else { $m=true; }
	if(substr($QUERY,0,1)=='@'){ $QUERY=substr($QUERY,1); $r=true; } else { $r=false; }

	// load all hazel files in foler. output, file, bytes, print size:
	$g = glob($QUERY); $o=''; $f=''; $n=0; $p='';

	// only proceed if files found:
	$c = count($g); if($c==0){ echo "// Nothing found for... $QUERY"; exit; }

	// check for definitions file for general includes:
	if(file_exists('def.hz')){ hazel(file_get_contents('def.hz'),false); }

	// load each file found and render with Hazel:
	for($i=0;$i<$c;$i++){			// minify, report?
		$f = hazel(file_get_contents($g[$i]),$m,$r); 
		$n+=strlen($f); $o.=$f; } 

	// print full output showing bytes...
	$z = "( $c ) hazel file"; if($c!=1){ $z.='s'; }
	if($m){ $p=" -- minified to: "; } else { $p=" -- total size: "; }
	if($n!=0){ $p.="[ $n bytes ]"; }
	echo "// Hazel preprocessor -- loading $z$p...\r\n\r\n$o\r\n\r\n";
	exit;

	// .............................................................

	//  Written February 28th, 2021 for Hazel - http://1002px.com

?>
