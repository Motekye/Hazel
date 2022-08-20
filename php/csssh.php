<?php



//			CSSSH :: CSS shorthand...



function  csssh  ($s,$b=false){ $o=''; $a = $GLOBALS['CSSSH'];
	if($b){ $w=1; $v=0; } else { $w=0; $v=1; } // <<< translate backwards?
	for($i=0;$i<strlen($s);$i++){ $c=substr($s,$i,1);
	for($j=0;$j<count($a);$j+=2){ $l=strlen($a[$j+$w]);
	if(substr($s,$i,$l)==$a[$j+$w]){ $c=$a[$j+$v]; $i+=($l-1); break; } } 
	$o.=$c; } return $o; }

$CSSSH = array(

	// ////  ALL CSS SHORTHAND ALIAS -> RULE PAIRS  //// //

		'^;',	'px;',		'^',	'px ',
		'~;',	'em;',		'~',	'em ',
		
		'%-',	'% - ',		'%+',	'% + ',
	
		'!;',	' !important;',

	// //////////////// BOX DIMENSIONS ///////////////// //

		'NONE;',        'display:none;',
		'BLOCK;',       'display:block;',
		'INLINE;',      'display:inline;',
		'FN;',          'float:none;',
		'FR;',          'float:right;',
		'CR;',          'clear:right;',
		'FL;',          'float:left;',
		'CL;',          'clear:left;',
		'CB;',          'clear:both;',
		'CN;',		'clear:none;',
		'PSR;',         'position:relative;',
		'PSA;',         'position:absolute;',
		'PSF;',         'position:fixed;',
		'Z:',           'z-index:',
		'T(',           'top:calc(',
		'L(',           'left:calc(',
		'R(',           'right:calc(',
		'B(',           'bottom:calc(',
		'T:',           'top:',
		'L:',           'left:',
		'R:',           'right:',
		'B:',           'bottom:',
		'W(',           'width:calc(',
		'W:',           'width:',
		'W1;',          'width:100%;',
		'H(',           'height:calc(',
		'H:',           'height:',
		'H1;',          'height:100%;',
		'OH;',          'overflow:hidden;',
		'OV;',          'overflow:visible;',
		'OH;',          'overflow:hidden;',
		'OS;',          'overflow:scroll;',
		'OX:',          'overflow-x:',
		'OY:',          'overflow-y:',
		'O:',		'opacity:',

	// //////////////// BOX STYLES ///////////////// //

		'P:',           'padding:',
		'PT:',          'padding-top:',
		'PL:',          'padding-left:',
		'PR:',          'padding-right:',
		'PB:',          'padding-bottom:',
		'M:',           'margin:',
		'MT:',          'margin-top:',
		'ML:',          'margin-left:',
		'MR:',          'margin-right:',
		'MB:',          'margin-bottom:',
		'S#',		'solid #',
		'D#',		'dotted #',
		'B1S#',         'border:1px solid #',
		'B2S#',         'border:2px solid #',
		'B1D#',         'border:1px dotted #',
		'BD:',          'border:',
		'BRD:',         'border-radius:',
		'BT:',          'border-top:',
		'BL:',          'border-left:',
		'BR:',          'border-right:',
		'BB:',          'border-bottom:',
		'BW:',          'border-width:',
		'BC:',          'border-color:',
		'BGP:',         'background-position:',
		'BGI:',         'background-image:',
		'BGNR;',        'background-repeat:no-repeat;',
		'BGRX;',        'background-repeat:repeat-x;',
		'BGRY;',        'background-repeat:repeat-y;',
		'TRANS;',       'background-color:transparent;',
		'BG:',          'background:',
		'IMG(',         'background-image:url(',
		'B#',           'background-color:#',
		'BGC:',         'background-color:',
		'BGI:',         'background-image:',

	// //////////////// FONT STYLES ///////////////// //

		'C#',           'color:#',
		'C:',           'color:',
		'TS:',          'text-shadow:',
		'TS1#',         'text-shadow:1px 1px #',
		'TS2#',         'text-shadow:2px 2px #',
		'TS12#',        'text-shadow:1px 2px #',
		'TI:',          'text-indent:',
		'TT:',          'text-transform:',
		'UPPER;',       'text-transform:uppercase;',
		'LOWER;',       'text-transform:lowercase;',
		'CAPS;',        'text-transform:capitalize;',
		'FF:',          'font-family:',
		'LH:',          'line-height:',
		'LS:',          'letter-spacing:',
		'WS:',          'word-spacing:',
		'LST:',         'list-style:',
		'LST(',         'list-style:url(',
		'MONO;',   	'font-family:monospace;',
		'SERIF;',       'font-family:serif;',
		'SANS;',        'font-family:sans-serif;',
		'FS:',          'font-size:',
		'BOLD;',        'font-weight:bold;',
		'THIN;',        'font-weight:100;',
		'FW:',          'font-weight:',
		'TDN;',		'text-decoration:none;',
		'ULINE;',       'text-decoration:underline;',
		'UCASE;',       'text-transform:uppercase;',
		'LCASE;',       'text-transform:lowercase;',
		'ITALIC;',      'font-style:italic;',
		'FSN;',         'font-style:normal;',
		'STRIKE;',      'text-decoration:line-through;',
		'LEFT;',        'text-align:left;',
		'RIGHT;',       'text-align:right;',
		'CENTER;',      'text-align:center;',
		'JUSTIFY;',     'text-align:justify;',
		'NOWRAP;',	'white-space:nowrap;',
		'PRE;',		'white-space:pre;',

	// //////////////// GENERIC PROPRIETARY ///////////////// //

		'CPOINTER;',	'cursor:pointer;',
		'CTEXT;',	'cursor:text;',
		'CDEFAULT;',	'cursor:default;'

);	// ..................................................



/******************************************************************************/

//  	CSSSH (c) 2017 -- http://1002px.com


?>
