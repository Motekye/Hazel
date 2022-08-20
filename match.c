


/* ****************************************************************** */

/*                                                               Matches
 *                                                       init_matches ()
 *                                                       free_matches ()
*/
typedef struct {

	int** 		name;       /* names of each match */
	int*		nlen;       /* lengths of match names */
	int** 		body;       /* match replace */
	int*		blen;       /* lengths of each match replace */
	int			len;        /* total matches */
	int*		init;       /* init characters */
	int			ilen;       /* length of init characters */
	int 		longest;    /* longest match name */

} Matches;

void init_matches (Matches* o)
{
	o->name = malloc (sizeof (int*));
	o->nlen = malloc (sizeof (int));
	o->body = malloc (sizeof (int*));
	o->blen = malloc (sizeof (int));
	o->len = 0;
	o->init = malloc (sizeof (int));
	o->ilen = 0;
	o->longest = 0;
}

void free_matches (Matches* o)
{
	int i = 0;
	while (i < o->len) {
		free (o->name[i]);
		free (o->body[i]);
		i++;
	}
	free (o->name);
	free (o->body);
	free (o->nlen);
	free (o->blen);
	free (o->init);
}


/* ****************************************************************** */

/*                                                   init_all_matches ()
 *                                                   free_all_matches ()
 *	Defining all match types used by Hazel...
*/
Matches HZ_EXACT;
Matches HZ_WORD;
Matches HZ_STRING;
Matches HZ_COMM;
Matches HZ_PAIRS;
Matches* HZ_PAIRS_LST;
int HZ_PAIRS_CT;


void init_all_matches ()
{
	init_matches (&HZ_EXACT);
	init_matches (&HZ_WORD);
	init_matches (&HZ_STRING);
	init_matches (&HZ_COMM);
	init_matches (&HZ_PAIRS);
	HZ_PAIRS_LST = malloc (sizeof (Matches));
	HZ_PAIRS_CT = 0;
}

void free_all_matches ()
{
	int i = 0;
	free_matches (&HZ_EXACT);
	free_matches (&HZ_WORD);
	free_matches (&HZ_STRING);
	free_matches (&HZ_COMM);
	free_matches (&HZ_PAIRS);
	while (i < HZ_PAIRS_CT) {
		free_matches (&HZ_PAIRS_LST[i]);
		i++;
	}
	free (HZ_PAIRS_LST);
}


/* ****************************************************************** */

/*                                                  add_match_to_list ()
 *	
 *
*/
void add_match_to_list (
	Matches* o,
	int* name,
	int nlen,
	int* body,
	int blen
) {
	int i = 0, w = o->len;
	o->len++;
#define	ENTRY(n,t,v)	o->n = realloc (o->n, sizeof (t) * o->len); \
						o->n[w] = v
	ENTRY (name, int*, name);
	ENTRY (nlen, int, nlen);
	ENTRY (body, int**, body);
	ENTRY (blen, int*, blen);
#undef ENTRY
	if (nlen > o->longest) {
		o->longest = nlen;
	}
	while (i < o->ilen) {
		if (o->init[i] == name[0]) { return; }
		i++;
	}
	o->init = realloc (o->init, sizeof (int) * (o->ilen + 1));
	o->init[o->ilen] = name[0];
	o->ilen++;
}


/* ****************************************************************** */

/*                                                          add_match ()
 *
 *	Add a match of specified type with name and body: (//. command)
*/
void add_match (
	char type,		/* one of:  = v s c  */
	int* name,
	int nlen,
	int* body,
	int blen
) {
	int* bd;
	int bl;
	switch (type) {
	
	case '=': case 'v': case 's':
		bd = body;
		bl = blen;
		break;
	
	case 'c':
		bd = body;
		bl = 0;
		switch (body[0]) {
			case '+': bl = 1; break;
			case '-': bl = 0; break;
			case 'o':
				if (blen > 1 && body[1] == 'n') {
					bl = 1;
				} else {
					bl = 0;
				}
				break;
		}
		break;
	
	}
#define 	AM2L(n)		add_match_to_list (n, name, nlen, bd, bl)
	switch (type) {
	case '=': AM2L (&HZ_EXACT); return;
	case 'v': AM2L (&HZ_WORD); return;
	case 's': AM2L (&HZ_STRING); return;
	case 'c': AM2L (&HZ_COMM); return;
	}
#undef		AM2L
}


/* ****************************************************************** */


/* ****************************************************************** */


/*	                   D E T E C T   M A T C H E S                    */


/* ****************************************************************** */


/* ****************************************************************** */

/*                                                         match_init ()
 *
 *	Check if current character is initial for matches from set:
*/
int match_init (Matches* o, int c)
{
	int i = 0;
	while (i < o->ilen) {
		if (o->init[i] == c) { return 1; }
		i++;
	}
	return 0;
}


/* ****************************************************************** */

/*                                                        match_index ()
 *
 *	Get index of exact match at position or -1:
*/
int match_index (Codebuf* o, Matches* m)
{
	int i = m->len - 1;
	while (i > -1) {
		if (codebuf_match (o, m->name[i], m->nlen[i])) {
			return i;
		}
		i--;
	}
	return -1;
}


/* ****************************************************************** */

/*                                                   match_index_then ()
 *
 *	Get index of exact match, whitespace, then char at position; or -1:
*/
int match_index_then (Codebuf* o, Matches* m, char t)
{
	int i = m->len - 1, j, c;
	while (i > -1) {
		if (codebuf_match (o, m->name[i], m->nlen[i])) {
			/* check for opening ( bracket */
			j = o->pos[o->wh] + m->nlen[i];
			c = o->src[o->wh][j];
			while (c==' ' || c=='\t' || c=='\n') {
				j++;
				c = o->src[o->wh][j];
			}
			if (c==t) { return i; }
		}
		i--;
	}
	return -1;
}


/* ****************************************************************** */

/*                                                   text_match_index ()
 *
 *	Get index of exact match in arbitrary text or -1:
*/
int text_match_index (Matches* m, int* s, int p)
{
	int w = m->len - 1, i, j;
	while (w > -1) {
		j = 0;
		i = p;
		while (j < m->nlen[w]) {
			if (m->name[w][j] != s[i]) { break; }
			i++;
			j++;
		}
		if (j == m->nlen[w]) { return w; }
		w--;
	}
	return -1;
}


/* ****************************************************************** */

/*                                                   name_match_index ()
 *
 *	Get index of full name match in set or -1:
*/
int name_match_index (Matches* o, int* a, int l)
{
	int i = 0, j, y = 0;
	if (l > o->longest) { return -1; }
	while (i < o->ilen) {
		if (o->init[i] == a[0]) { y = 1; }
		i++;
	}
	if (y==0) { return -1; }
	i = o->len - 1;
	while (i > -1) {
		j = 0;
		while (j < l && j < o->nlen[i]) {
			if (o->name[i][j] != a[j]) { break; }
			j++;
		}
		if (j==l && j == o->nlen[i]) {
			return i;
		}
		i--;
	}
	return -1;
}


/* ****************************************************************** */

/*                                                    convert_matches ()
 *
 *	Convert all unconditional matches found in a body of text.
*/
void convert_matches (
	Matches* o, 
	int* src, 
	int srcl, 
	int** out, 
	int* outl
) {
	int i = 0, j, w, z = STRING_STEP;
	out[0] = malloc (sizeof (int) * z);
	outl[0] = 0;
	while (i < srcl) {
		/* match found? */
		if (match_init (o, src[i])) {
			w = text_match_index (o, src, i);
			if (w != -1) {
				i += o->nlen[w];
				j = 0;
				while (j < o->blen[w]) {
					out[0][outl[0]] = o->body[w][j];
					j++;
					outl[0]++;
					if (outl[0] >= z) {
						z += STRING_STEP;
						out[0] = realloc (out[0], sizeof (int) * z);
					}
				}
				continue;
			}
		}
		/* regular characters: */
		out[0][outl[0]] = src[i];
		i++;
		outl[0]++;
		if (outl[0] >= z) {
			z += STRING_STEP;
			out[0] = realloc (out[0], sizeof (int) * z);
		}
	}
}


/* ****************************************************************** */

/*                                                       HZ_get_pairs ()
 *
 *	Collect pairs for //2 pair matches macro.
*/
void HZ_get_pairs (CBUF, int* name, int nlen)
{
	Matches o;
	int* bd;
	int* n;
	int nl;
	int* b;
	int bl;
	int z;

	/* skip to next line: */
	while (CH != '\n') { 
		if (NOMINIFY) { HZ_out (CH); }
		NEXT; 
	}
	if (NOMINIFY) { HZ_out ('\n'); }
	NEXT;
	
	/* invalid without block comment start: */
	if (!MATCHc("/*",2)) {
		free (name);
		return;
	}
	if (NOMINIFY) { 
		HZ_out ('/'); 
		HZ_out ('*'); 
	}
	ADV (1);
	NEXT;
	
	/* create pairs reference and expand pair match list: */
	bd = malloc (sizeof (int));
	add_match_to_list (&HZ_PAIRS, name, nlen, bd, HZ_PAIRS_CT);
	init_matches (&o);
	HZ_PAIRS_LST = realloc (HZ_PAIRS_LST, 
		sizeof (Matches) * (HZ_PAIRS_CT + 1));

	/* for each line of pair matches... */
	while (CH != '\0') {
	
		/* skip leading space, check for end or empty line: */
		while (CH == ' ' || CH == '\t') { 
			if (NOMINIFY) { HZ_out (CH); }
			NEXT; 
		}
		if (MATCHc("*/",2)) {
			ADV (1);
			if (NOMINIFY) { 
				HZ_out ('*'); 
				HZ_out ('/'); 
			}
			break;
		}
		if (CH == '\n') {
			if (NOMINIFY) { HZ_out (CH); }
			NEXT;
			continue;
		}
		
		/* get match name: */
		z = WORD_STEP;
		n = malloc (sizeof (int) * z);
		nl = 0;
		while (CH != ' ' && CH != '\t' && CH != '\n') {
			n[nl] = CH;
			nl++;
			if (nl == z) {
				z += WORD_STEP;
				n = realloc (n, sizeof (int) * z);
			}
			if (NOMINIFY) { HZ_out (CH); }
			NEXT;
		}
		n = realloc (n, sizeof (int) * nl);
		
		/* skip space: */
		while (CH == ' ' || CH == '\t') { 
			if (NOMINIFY) { HZ_out (CH); }
			NEXT; 
		}
		
		/* get match body: */
		z = WORD_STEP;
		b = malloc (sizeof (int) * z);
		bl = 0;
		while (CH != ' ' && CH != '\t' && CH != '\n') {
			b[bl] = CH;
			bl++;
			if (bl == z) {
				z += WORD_STEP;
				b = realloc (b, sizeof (int) * z);
			}
			if (NOMINIFY) { HZ_out (CH); }
			NEXT;
		}
		b = realloc (b, sizeof (int) * bl);
		
		/* record match: */
		add_match_to_list (&o, n, nl, b, bl);
	}
	
	/* save pair match list: */
	HZ_PAIRS_LST[HZ_PAIRS_CT] = o;
	HZ_PAIRS_CT++;
}


/* ****************************************************************** */

/*                                                    HZ_inject_pairs ()
 *
 *	Render //2 pair matches macro.
 *	(Expects to be on last character of name)
*/
void HZ_inject_pairs (CBUF, int w)
{
	int* str; 
	int* del;
	int* s;
	int sl;
	
	NEXT;
	while (CH==' ' || CH=='\t' || CH=='\n') { NEXT; }
	
	HZ_end_bracket (C, &str, &del, -1);
	convert_matches (&HZ_PAIRS_LST[w], str, del[1], &s, &sl);
	add_codebuf_ints (C, s, sl);
	
	free (del);
	free (str);
	free (s);
}


/* ****************************************************************** */



