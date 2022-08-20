/****************************************************************************/
/***  ##   # .####, ###### ###### ##      ***********************************/
/***  ###### ##   #   _##  ##___  ##      ***********************************/
/***  ##   # ######  ##*   ##^^^  ##      ***********************************/
/***  ##   # ##   # ###### ###### ######  ***********************************/
/****************************************************************************
 *
 *
 *
*/


/* ************************************************************************ */


#define		HAZEL_LOGO_LARGE		"\n" \
		"  ##   # .####, ###### ###### ##    \n" \
		"  ###### ##   #   _##  ##___  ##    \n" \
		"  ##   # ######  ##*   ##^^^  ##       Preprocessor\n" \
		"  ##   # ##   # ###### ###### ######   (c) 2017 - 2022\n"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pairs.c"


/* ************************************************************************ */


#define		MAX_PATH		128
#define		FILE_STEP		128


#include "codebuf.c"


int MAX_NESTING = 1024;

#define 	MAX_NAME_SIZE 	128
#define		MAX_ARGUMENTS	32
#define 	WORD_STEP		24
#define		STRING_STEP		32
#define		SOURCE_STEP		128
#define		MACRO_STEP		64
#define		TEMPLATE_STEP	64
#define 	BRACKET_MAX		128
#define		MAX_DIGITS		24

#define 	CBUF			Codebuf* C

#define		MINIFY			C->minify==1
#define		NOMINIFY		C->minify==0

#define		PRLB			C->prlb
#define		CH				C->ch
#define		PR				C->pr
#define		PNW				C->pnw

#define		HZ_out(n)		codebuf_out (C,n)
#define		HZ_outs(n,l)	codebuf_outs (C,n,l)
#define		NEXT			C->ch = codebuf_next (C)
#define		ADV(n)			codebuf_advance (C,n)
#define		REW(n)			C->pos[C->wh] -= n
#define		MATCH(n,l)		codebuf_match (C,n,l)
#define		MATCHc(n,l)		codebuf_match_char (C,n,l)
#define		MATCHwtc(n,l)	codebuf_match_wsp_then_char (C,n,l)


#include "literals.c"
#include "match.c"
#include "macro.c"
#include "render.c"


/*
 *	Handles settings and CLI arguments.
*/
#include "help.c"
#include "argv.c"


/* ************************************************************************ */


/*
 *	Initialize Hazel based on arguments provided...
*/
int main ( int argc, char** argv ) 
{
	/*
	 *	Pass arguments to this function from "argv.c"
	 *	to populate the environment and handle params.
	 *	Returns 0 if there is a valid request to process.
	*/
	if (process_argv (argc, argv) == 1) { return 0; }

	/*
	 *	Open file for rendering and prepare...
	*/
	init_all_matches ();
	init_all_macros ();
	Codebuf C = new_codebuf (source_path);
	codebuf_set_mode (&C, output_mode);
	if (output_path != NULL) {
		codebuf_output_file (&C, output_path, 0);
	}

	/*
	 *	Begin rendering file...
	*/
	HZ_render (&C);

	/*
	 *	Finish rendering and cleanup...
	*/
	free_codebuf (&C);
	free_all_matches ();
	free_all_macros ();
	return 0;
}


