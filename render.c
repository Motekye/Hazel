





/* ****************************************************************** */


void HZ_render (CBUF);
int HZ_minchar (int);
void HZ_next_word (CBUF);
void HZ_next_string (CBUF);
int HZ_command_init (CBUF);
void HZ_add_match (CBUF, int);

void HZ_spill_file (CBUF);

void spill_ints (int* a, int l);
void spill_ints_arr (int** a, int* l, int c);


/* ****************************************************************** */	

/*                                                          HZ_render ()
 *
 *
*/
void HZ_render (CBUF) 
{
	int w, i;
	while(CH!=0){
	
		if (C->wh > MAX_NESTING) {
			printf ("\nError, too much nesting! Check for self or circular "
				"references.\nRefer to  -n  flag in  hazel --help  if you\n"
				"legitimately require more nesting. Current max is %d.\n\n",
				MAX_NESTING);
			return;
		}
		
		/* ............................................ */
		
		/*
		 *	Line breaks / White-space:		TODO: better minify
		*/
		if (CH=='\n') {
			PRLB = 1;
			goto HZ_render_whitespace;
		}
		if (CH==' ' || CH=='\t') {
			PRLB = 0;
			HZ_render_whitespace:
				if (MINIFY) {
					CH = ' ';
					if (HZ_minchar (PR)) { goto HZ_render_next_wsp; }
				}
				HZ_out (CH);
				goto HZ_render_next_wsp;
		}
		
		/* ............................................ */
		
		/*
		 *	Check for comment lines:
		*/
		if (PRLB==1 && match_init (&HZ_COMM, CH)) {
			w = match_index (C, &HZ_COMM);
			if (w!=-1) {
				ADV (HZ_COMM.nlen[w]-1);
				/* comment line hidden? */
				if (HZ_COMM.blen[w] == 0) {
					while (CH != '\n') {
						NEXT;
					}
					PRLB = 1;
					if (NOMINIFY) { HZ_out ('\n'); }
				/* comment line showing? */
				} else {
					i = 0;
					while (i < HZ_COMM.nlen[w]) {
						HZ_out (' ');
						i++;
					}
				}
				goto HZ_render_next_wsp;
			}
		}
		

		/* ............................................ */
		
		/*
		 *	Hazel commands:
		*/
		if (PRLB==1 && HZ_command_init (C)) {
			ADV(1);
			NEXT;
			w = CH;
			NEXT;
			switch (w) {
			
			/* Select output file... */
			case 'o':
				codebuf_output_file (C, codebuf_get_path (C), 1);
				goto HZ_render_next_wsp;
			
			/* Include Hazel file... */
			case 'i':
				add_codebuf_file (C, codebuf_get_path (C), 0);
				goto HZ_render_next_wsp;
			
			/* Include Hazel file quietly... */
			case 'q':
				add_codebuf_file (C, codebuf_get_path (C), 2);
				goto HZ_render_next_wsp;
				
			/* TODO: Hazel directive */
			case 'h':
				
				goto HZ_render_next_wsp;
			
			/* Include raw Javascript file... */
			case 'j':
				HZ_spill_file (C);
				goto HZ_render_next_wsp;
			
			/* types of shortcuts... */
			case '=': case 'v': case 's':
			case 'f': case 'b': case 'c':
			case '2':
				HZ_add_match (C, w);
				CH = '\n';
				goto HZ_render_next_wsp;
				
			/* unknown Hazel command... */
			default:
				if (NOMINIFY) {
					HZ_out ('/');
					HZ_out ('/');
					HZ_out (w);
					HZ_out (CH);
				}
				HZ_line_comment (C);
			}
			goto HZ_render_next;
		}
		
		/* ............................................ */
		
		/*
		 *	Line or block comments:
		*/
		if (MATCHc("//",2)) {
			if (NOMINIFY) { HZ_out ('/'); }
			HZ_line_comment (C);
			PRLB = 1;
			goto HZ_render_next_wsp;
		}
		if (MATCHc("/*",2)) {
			HZ_block_comment (C);
			goto HZ_render_next;
		}
		
		/* ............................................ */
		
		/*
		 *	Check for string literals:
		*/
		if (CH == '"' || CH == '\'' || CH == '`') {
			HZ_next_string (C);
			continue;
		}
		
		/* ............................................ */

		/*
		 *	Check for exact match //= macros:
		*/
		if (match_init (&HZ_EXACT, CH)) {
			w = match_index (C, &HZ_EXACT);
			if (w!=-1) {
				ADV (HZ_EXACT.nlen[w]-1);
				add_codebuf_ints (C, 
					HZ_EXACT.body[w], 
					HZ_EXACT.blen[w]);
				NEXT;
				continue;
			}
		}
		
		/* ............................................ */

		/*
		 *	Check for function match //f macros:
		*/
		if (macro_init (&HZ_FUNC, CH)) {
			w = macro_index_then (C, &HZ_FUNC, '(');
			if (w!=-1) {
				ADV (HZ_FUNC.nlen[w]-1);
				HZ_inject_func (C, w);
				goto HZ_render_next;
			}
		}
		
		/* ............................................ */

		/*
		 *	Check for block match //b macros:
		*/
		if (macro_init (&HZ_BLOCK, CH)) {
			w = macro_index_then (C, &HZ_BLOCK, '(');
			if (w!=-1) {
				ADV (HZ_BLOCK.nlen[w]-1);
				HZ_inject_block (C, w);
				goto HZ_render_next;
			}
		}
		
		/* ............................................ */

		/*
		 *	Check for pair match //2 macros:
		*/
		if (match_init (&HZ_PAIRS, CH)) {
			w = match_index_then (C, &HZ_PAIRS, '{');
			if (w!=-1) {
				ADV (HZ_PAIRS.nlen[w]-1);
				HZ_inject_pairs (C, w);
				goto HZ_render_next;
			}
		}
		
		/* ............................................ */
		
		/*
		 *	Compile words:
		*/
		if (
			(CH == '$' || CH == '_')
		||	(CH >= 'A' && CH <= 'Z')
		||	(CH >= 'a' && CH <= 'z')
		||	(CH > 128)
		) {
			HZ_next_word (C);
			continue;
		} //*/
		
		/* ............................................ */

		/*
		 *	All other characters:
		*/
		HZ_out (CH);
		
		/* ............................................ */
	
		/*
		 *	Loop...
		*/
		HZ_render_next:
			PRLB = 0;
			if (CH != ' ' && CH != '\t' && CH != '\n') { PR = CH; }
		
		HZ_render_next_wsp:
			NEXT;
		
		/* ............................................ */
		
	}

}


/* ****************************************************************** */

/*                                                         HZ_minchar ()
 *
 *
*/
int HZ_minchar (int c)
{
	if (c <= '#') { return 1; }
	if (c >= '%' && c <= '/') { return 1; }
	if (c >= ':' && c <= '@') { return 1; }
	if (c >= '[' && c <= '^') { return 1; }
	if (c >= '{' && c <= '~') { return 1; }
	return 0;
}


/* ****************************************************************** */

/*                                                       HZ_next_word ()
 *
 *
*/
void HZ_next_word (CBUF)
{
	int i = 0, z = WORD_STEP, w;
	int* a = malloc (sizeof (int) * z);
	while (
		(CH == '_' || CH == '$')
	||	(CH >= 'A' && CH <= 'Z')
	||	(CH >= 'a' && CH <= 'z')
	||	(CH > 128)
	) {
		a[i] = CH;
		NEXT;
		i++;
		if (i==z) {
			z += WORD_STEP;
			a = realloc (a, sizeof (int) * z);
		}
	}
	a = realloc (a, sizeof (int) * i);
	
	/*
	 *	Test word against //v macros:
	*/
	w = name_match_index (&HZ_WORD, a, i);
	if (w!=-1) {
		REW(1);
		add_codebuf_ints (C, HZ_WORD.body[w], HZ_WORD.blen[w]);
		NEXT;
		goto HZ_next_word_done;
	}
	
	/*
	 *	Spill normal word to output...
	*/
	HZ_outs (a, i);
	HZ_next_word_done:
		free (a);
}


/* ****************************************************************** */

/*                                                     HZ_next_string ()
 *	
 *	Fetch next string literal.
*/
void HZ_next_string (CBUF)
{
	int i = 0, q = CH, z = STRING_STEP;
	int* a = malloc (sizeof (int) * z);
	int* s;
	int sl;

	a[i] = CH;
	NEXT;
	i++;
	while (CH != q && CH != '\0') {
		a[i] = CH;
		if(CH=='\\') {
			NEXT;
			i++;
			a[i] = CH;
		}
		NEXT;
		i++;
		if (i>z-2) {
			z += STRING_STEP;
			a = realloc (a, sizeof (int) * z);
		}
	}
	a[i] = CH;
	NEXT;
	i++;
	a = realloc (a, sizeof (int) * i);
	
	/* string inject matches: */
	if (q == '"') {
		convert_matches (&HZ_STRING, a, i, &s, &sl);
		free (a);
		HZ_outs (s, sl);
		free (s);
		
	/* or unmodified string: */
	} else {
		HZ_outs (a, i);
		free (a);
	}
}


/* ****************************************************************** */

/*                                                    HZ_command_init ()
 *
 *		Test for hazel command init:
*/
int HZ_command_init (CBUF)
{
	int w = C->wh, p = C->pos[w];
	int* s = C->src[w];
	/* check clearance */
	if (C->len[w] < p + 4) { return 0; }
	/* check for // or ## initial */
	if (s[p] != '/' && s[p] != '#') { return 0; }
	if (s[p] != s[p + 1]) { return 0; }
	/* check for valid char, then space or tab */
	if (s[p + 2] < 33) { return 0; }
	if (s[p + 3] != ' ' && s[p + 3] != '\t') { return 0; }
	return 1;
}


/* ****************************************************************** */

/*                                                       HZ_add_match ()
 *
 *
*/
void HZ_add_match (CBUF, int kind) 
{
	int* name = malloc (sizeof (int) * MAX_NAME_SIZE);
	int* body = malloc (sizeof (int) * MACRO_STEP);
	int nl = 0, bl = 0, z = MACRO_STEP;
	PRLB = 1;
	NEXT;

	/* print command? skip whitespace: */
	if (NOMINIFY) {
		HZ_out ('/');
		HZ_out ('/');
		HZ_out (kind);
		HZ_out (' ');
	}
	while (CH=='\t' || CH==' ') {
		if (NOMINIFY) { HZ_out (CH); }
		NEXT;
	}
	
	/* no name error: */
	if (CH=='\n' || CH=='\0') {
		goto HZ_add_match_illegal;
	}
	
	/* get match name: */
	while (CH!='\n' && CH!='\0' && CH!='\t' && CH!=' ') {
		name[nl] = CH;
		if (NOMINIFY) { HZ_out (CH); }
		NEXT;
		nl++;
	}
	name = realloc (name, sizeof (int) * nl);
	
	/* skip whitespace: */
	while (CH=='\t' || CH==' ') {
		if (NOMINIFY) { HZ_out (CH); }
		NEXT;
	}
	
	/* pair matches: */
	if (kind == '2') {
		free (body);
		HZ_get_pairs (C, name, nl);
		return;
	}

	/* get match body... */
	while (CH!='\0') {
		body[bl] = CH;
		if (NOMINIFY) { HZ_out (CH); }
		NEXT;
		bl++;
		if (bl == z) {
			z += MACRO_STEP;
			body = realloc (body, sizeof (int) * z);
		}
		if (CH=='\n') {
			/* check for multi-line body: */
			if (MATCHc("\n//_",4) || MATCHc("\n##_",4)) {
				body[bl] = ' ';
				bl++;
				if (NOMINIFY) {
					HZ_out ('\n');
					HZ_out ('/');
					HZ_out ('/');
					HZ_out ('_');
				}
				ADV (3);
				NEXT;
				while (CH=='\t' || CH==' ') {
					if (NOMINIFY) { HZ_out (CH); }
					NEXT;
				}
			} else {
				if (NOMINIFY) { HZ_out ('\n'); }
				break;
			}
		}
	}
	
	/* create new macro with name and body: */
	body = realloc (body, sizeof (int) * bl);
	switch (kind) {
	case '=': case 'v': case 's': case 'c':
		add_match (kind, name, nl, body, bl);
		break;
	case 'f': case 'b':
		add_macro (kind, name, nl, body, bl);
		break;
	}
	return;
		
	/* there was an error. No action: */
	HZ_add_match_illegal:
		free (name);
		free (body);
}


/* ****************************************************************** */

/*                                                      HZ_spill_file ()
 *
 *		Spill file out directly with no buffering...	TODO: minified
*/
void HZ_spill_file (CBUF)
{
	char* fname = codebuf_get_path (C);
	char* f = relative_path (C->path[C->wh], fname);
	int l, i = 0;
	int* a = read_file_as_ints (f, &l);
	while (i < l) {
		HZ_out (a[i]);
		i++;
	}
	free (fname);
	free (f);
	free (a);
}


/* ****************************************************************** */

/*                                                         spill_ints ()
 *                                                     spill_ints_arr ()
 *	Spill int* to the console for debugging...
*/
void spill_ints (int* a, int l)
{
	int i = 0;
	while (i < l) {
		printf ("%c", a[i]);
		i++;
	}
	printf ("\n");
}

void spill_ints_arr (int** a, int* l, int c)
{
	int i = 0, j;
	while (i < c) {
		j = 0;
		while (j < l[i]) {
			printf ("%c", a[i][j]);
			j++;
		}
		printf ("\n");
		i++;
	}
	printf ("\n");
}


/* ****************************************************************** */

















