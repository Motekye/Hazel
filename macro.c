



/* ****************************************************************** */


void HZ_split_params (int*, int, int***, int**, int*, int*);


/* ****************************************************************** */

/*                                                                Macros
 *                                                        init_macros ()
 *                                                        free_macros ()
*/
typedef struct {

	int** 		name;       /* names of each macro */
	int*		nlen;       /* lengths of macro names */
	int*** 		body;       /* macro segments */
	int**		blen;       /* lengths of each segment */
	int*		bct;        /* segment counts */
	int*		argc;       /* argument counts */
	int*		inst;       /* instance counts */
	int			len;        /* total macros */
	int*		init;       /* init characters */
	int			ilen;       /* length of init characters */
	int 		longest;    /* longest macro name */

} Macros;

void init_macros (Macros* o)
{
	o->name = malloc (sizeof (int*));
	o->nlen = malloc (sizeof (int));
	o->body = malloc (sizeof (int**));
	o->blen = malloc (sizeof (int*));
	o->bct = malloc (sizeof (int));
	o->argc = malloc (sizeof (int));
	o->inst = malloc (sizeof (int));
	o->len = 0;
	o->init = malloc (sizeof (int));
	o->ilen = 0;
	o->longest = 0;
}

void free_macros (Macros* o)
{
	int i = 0, j;
	while (i < o->len) {
		free (o->name[i]);
		j = 0;
		while (j < o->bct[i]) {
			free (o->body[i][j]);
			j++;
		}
		free (o->blen[i]);
		free (o->body[i]);
		i++;
	}
	free (o->name);
	free (o->body);
	free (o->nlen);
	free (o->blen);
	free (o->bct);
	free (o->argc);
	free (o->inst);
	free (o->init);
}


/* ****************************************************************** */

/*                                                    init_all_macros ()
 *                                                    free_all_macros ()
 *	Defining all macro types used by Hazel...
*/
Macros HZ_FUNC;
Macros HZ_BLOCK;


void init_all_macros ()
{
	init_macros (&HZ_FUNC);
	init_macros (&HZ_BLOCK);
}

void free_all_macros ()
{
	free_macros (&HZ_FUNC);
	free_macros (&HZ_BLOCK);
}


/* ****************************************************************** */

/*                                                  add_macro_to_list ()
 *	
 *
*/
void add_macro_to_list (
	Macros* o,
	int* name,
	int nlen,
	int** body,
	int* blen,
	int bct,
	int argc
) {
	int i = 0, w = o->len;
	o->len++;
#define	ENTRY(n,t,v)	o->n = realloc (o->n, sizeof (t) * o->len); \
						o->n[w] = v
	ENTRY (name, int*, name);
	ENTRY (nlen, int, nlen);
	ENTRY (body, int**, body);
	ENTRY (blen, int*, blen);
	ENTRY (bct, int, bct);
	ENTRY (argc, int, argc);
	ENTRY (inst, int, 0);
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

/*                                                          add_macro ()
 *
 *	Add a macro of specified type with name and body: (//. command)
*/
void add_macro (
	char type,		/* one of:  f b  */
	int* name,
	int nlen,
	int* body,
	int blen
) {
	int** bd;
	int* bl;
	int bc;
	int ac = 0;
	switch (type) {
	
	case 'f': case 'b':
		HZ_split_params (body, blen, &bd, &bl, &bc, &ac);
		free (body);
		break;
		
	}
#define 	AM2L(n)		add_macro_to_list (n, name, nlen, bd, bl, bc, ac)
	switch (type) {
	case 'f': AM2L (&HZ_FUNC); return;
	case 'b': AM2L (&HZ_BLOCK); return;
	}
#undef		AM2L
}


/* ****************************************************************** */


/* ****************************************************************** */


/*	                  D E T E C T   M A C R O S                       */


/* ****************************************************************** */


/* ****************************************************************** */

/*                                                         macro_init ()
 *
 *	Check if current character is initial for macro from set:
*/
int macro_init (Macros* o, int c)
{
	int i = 0;
	while (i < o->ilen) {
		if (o->init[i] == c) { return 1; }
		i++;
	}
	return 0;
}


/* ****************************************************************** */

/*                                                   macro_index_then ()
 *
 *	Get index of exact match, whitespace, then char at position; or -1:
*/
int macro_index_then (Codebuf* o, Macros* m, char t)
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


/* ****************************************************************** */


/*	                 M A C R O   P A R A M E T E R S                  */


/* ****************************************************************** */


/* ****************************************************************** */

/*                                                             HZ_Params
 *
 *	Parameters object for each instance of a function or block wrapper.
*/
typedef struct {

	int**		args;       /* all arguments provided */
	int*		alen;		/* length of each argument */
	int*		used;       /* argument has been used? */
	int			ct;         /* argument count provided */
	int*		blk;        /* main block */
	int			blen;       /* length of main block */
	int*		els;        /* else block */
	int			elen;       /* length of else block */
	
} HZ_Params;


/* ****************************************************************** */

/*                                                     HZ_free_params ()
 *
 *	
*/
void HZ_free_params (HZ_Params* o)
{
	int i = 0;
	while (i < o->ct) {
		free (o->args[i]);
		i++;
	}
	free (o->args);
	free (o->alen);
	free (o->used);
	if (o->blen != -1) { free (o->blk); }
	if (o->elen != -1) { free (o->els); }
}


/* ****************************************************************** */

/*                                                    HZ_split_params ()
 *	
 *	Split a template into alternating template data and parameters.
 *	(Done on macro creation, not injection)
*/
void HZ_split_params (
	int* src, 
	int slen, 
	int*** bd, 
	int** bl, 
	int* bc,
	int* ac
) {
	int i = 0, j, k, sc = 1, z;
	int stfy, argn, argv = 0;
	int* s;
	/* count segments */
	while (i < slen) {
		if (src[i] == '#') { sc += 2; }
		i++;
	}
	bd[0] = malloc (sizeof (int*) * sc);
	bl[0] = malloc (sizeof (int) * sc);
	bc[0] = sc;
	/* each segment and parameter */
	i = 0;
	j = 0;
	while (i < sc) {
		/* segment... */
		z = TEMPLATE_STEP;
		s = malloc (sizeof (int) * z);
		k = 0;
		while (j < slen) {
			if (src[j] == '#') { break; }
			s[k] = src[j];
			j++;
			k++;
			if (k==z) {
				z += TEMPLATE_STEP;
				s = realloc (s, sizeof (int) * z);
			}
		}
		s = realloc (s, sizeof (int) * k);
		bd[0][i] = s;
		bl[0][i] = k;
		i++;
		if (i==sc) { break; }
		/* parameter... */
		s = malloc (sizeof (int) * 12);
		stfy = 0;
		argn = 0;
		s[0] = '#';
		j++;
		k = 1;
		/* untrimmed? */
		if (j < slen && src[j]=='_') {
			s[k] = '_';
			j++;
			k++;
		}
		/* stringify? */
		if (j < slen && (src[j]=='"' || src[j]=='\'')) {
			stfy = src[j];
			s[k] = src[j];
			j++;
			k++;
		}
		/* decimal sequence? */
		while (j < slen && src[j] >= '0' && src[j] <= '9') {
			argn = 1;
			argv *= 10;
			argv += (src[j] - '0');
			s[k] = src[j];
			j++;
			k++;
		}
		/* remember highest argument for count */
		if (argn == 1) {
			argv++;
			if(argv > ac[0]) {
				ac[0] = argv;
			}
		}
		/* or letter? */
		if (j < slen && argn == 0) {
			s[k] = src[j];
			j++;
			k++;
		}
		/* stringify end? */
		if (j < slen && stfy!=0) {
			if (src[j]==stfy) {
				s[k] = stfy;
				j++;
				k++;
			}
		}
		s = realloc (s, sizeof (int) * k);
		bd[0][i] = s;
		bl[0][i] = k;
		i++;
	}
}


/* ****************************************************************** */

/*                                                    HZ_render_param ()
 *
 *	Renders a single # parameter in a template to a value.
*/
int HZ_render_param (
	int mode,
	HZ_Params* o, 
	Macros* m,
	int w,
	int s,
	int** par,
	int* plen
) {
	int i = 1, stfy = 0, trim = 1, argn = 0, n = 0, j, k;
	int* p = NULL;
	int l = 0, fr = 0;
	int* b = m->body[w][s];
	int bl = m->blen[w][s];
	/* untrimmed? */
	if (b[i]=='_') {
		trim = 0;
		i++;
	}
	/* stringify? */
	if (b[i]=='"' || b[i]=='\'') {
		stfy = b[i];
		i++;
	}
	switch (mode) {
	/* ................................ */
	
	/* first round parameters... */
	case 0:
		/* decimal? */
		while (i < bl && b[i] >= '0' && b[i] <= '9') {
			argn = 1;
			n *= 10;
			n += b[i] - '0';
			i++;
		}
		if (argn != 0) {
			if (n >= o->ct) {
				goto HZ_render_param_unused;
			} else {
				p = o->args[n];
				l = o->alen[n];
				o->used[n] = 1;
			}
		/* or letter? */
		} else {
			switch (b[i]) {
			
			/* closure and else block */
			case 'B':
				p = o->blk;
				l = o->blen;
				break;
			case 'E':
				p = o->els;
				l = o->elen;
				break;
				
			/* instance number */
			case 'I':
				p = HZ_num_to_ints(m->inst[w]+1, &l);
				fr = 1;
				break;
			case 'i':
				p = HZ_num_to_ints(m->inst[w], &l);
				fr = 1;
				break;
				
			/* provided argument count */
			case 'C':
				p = HZ_num_to_ints(o->ct, &l);
				fr = 1;
				break;
			case 'c':
				p = HZ_num_to_ints(o->ct-1, &l);
				fr = 1;
				break;
			
			/* defined parameter count */
			case 'P':
				p = HZ_num_to_ints(m->argc[w], &l);
				fr = 1;
				break;
			case 'p':
				p = HZ_num_to_ints(m->argc[w]-1, &l);
				fr = 1;
				break;
				
			default: goto HZ_render_param_unused;
			}
			i++;
		}
		break;
	/* ................................ */
	
	/* second round parameters... */
	case 1:
		switch (b[i]) {
		
		/* next parameter from start */
		case 'N':
			j = 0;
			while (j < o->ct) {
				if (o->used[j] == 0) {
					p = o->args[j];
					l = o->alen[j];
					o->used[j] = 1;
					break;
				}
				j++;
			}
			break;
			
		/* next parameter from end */
		case 'X':
			j = o->ct - 1;
			while (j > -1) {
				if (o->used[j] == 0) {
					p = o->args[j];
					l = o->alen[j];
					o->used[j] = 1;
					break;
				}
				j--;
			}
			break;
			
		default: goto HZ_render_param_unused;
		}
		i++;
		break;
	/* ................................ */
	
	/* third round parameters... */
	case 2:
		switch (b[i]) {
		
		/* all unused parameters */
		case 'R':
			j = 0;
			while (j < o->ct) {
				if (o->used[j] == 0) {
					l += o->alen[j] + 1;
				}
				j++;
			}
			l--;
			p = malloc (sizeof (int) * l);
			j = 0;
			n = 0;
			while (j < o->ct) {
				if (o->used[j] == 0) {
					k = 0;
					while (k < o->alen[j]) {
						p[n] = o->args[j][k];
						k++;
						n++;
					}
					if (n < l) {
						p[n] = ',';
						n++;
					}
				}
				j++;
			}
			fr = 1;
			break;
			
		/* all parameters period */
		case 'A':
			j = 0;
			while (j < o->ct) {
				l += o->alen[j] + 1;
				j++;
			}
			l--;
			p = malloc (sizeof (int) * l);
			j = 0;
			n = 0;
			while (j < o->ct) {
				k = 0;
				while (k < o->alen[j]) {
					p[n] = o->args[j][k];
					k++;
					n++;
				}
				if (n < l) {
					p[n] = ',';
					n++;
				}
				j++;
			}
			fr = 1;
			break;
			
		default: goto HZ_render_param_unused;
		}
		i++;
		break;
	/* ................................ */
		
	}
	/* stringify end? */
	if (stfy!=0 && b[i]==stfy) {
		i++;
	}
	/* return -1 length for untouched parameters */
	if(p==NULL){
		HZ_render_param_unused:
			plen[0] = -1;
			return 0;
	}
	/* copy with modifications, returns if complete */
	HZ_stringify_trim (stfy, trim, p, l, &par[0], &plen[0]);
	if (fr==1) { free (p); }
	return 1;
}


/* ****************************************************************** */

/*                                                   HZ_render_params ()
 *
 *	Render all parameters in a template in order of precedence.
*/
void HZ_render_params (
	HZ_Params* o, 
	Macros* m,
	int w,
	int*** par,
	int** plen
) {
	int i = 0, s, z = m->bct[w] >> 1;
	/* set all injects as unvisited */
	int* x = malloc (sizeof (int) * z);
	while (i < z) {
		x[i] = 0;
		i++;
	}
	/* set all numbered arguments as unused */
	o->used = malloc (sizeof (int) * o->ct);
	i = 0;
	while (i < o->ct) {
		o->used[i] = 0;
		i++;
	}
	
#define RPARAM(y)	HZ_render_param (y,o,m,w,s,&par[0][i],&plen[0][i])
	/* first comb static values and arguments... */
	i = 0;
	s = 1;
	while (i < z) {
		x[i] = RPARAM(0);
		i++;
		s += 2;
	}
	/* next comb #N #X injects... */
	i = 0;
	s = 1;
	while (i < z) {
		if (x[i] == 0) {
			x[i] = RPARAM(1);
		}
		i++;
		s += 2;
	}
	/* then comb #R #A injects... */
	i = 0;
	s = 1;
	while (i < z) {
		if (x[i] == 0) {
			x[i] = RPARAM(2);
		}
		i++;
		s += 2;
	}
#undef RPARAM

	free (x);
}


/* ****************************************************************** */


/* ****************************************************************** */


/*	                    R E N D E R   M A C R O S                     */


/* ****************************************************************** */


/* ****************************************************************** */

/*                                                 HZ_render_template ()
 *
 *	Populates out and olen with finished template.
*/
void HZ_render_template (
	HZ_Params* o, 
	Macros* m,
	int w,
	int** out,
	int* olen
) {
	int l = 0, s = 0, i, j = 0, z = TEMPLATE_STEP, x = m->bct[w] >> 1;
	int** p = malloc (sizeof (int*) * x);
	int* pl = malloc (sizeof (int) * x);
	out[0] = malloc (sizeof (int) * z);
	
	/* render parameter values ahead of time */
	HZ_render_params (o, m, w, &p, &pl);
	
	/* alternate segments and parameters... */
	while (s < m->bct[w]) {
		i = 0;
		while (i < m->blen[w][s]) {
			out[0][l] = m->body[w][s][i];
			l++;
			i++;
			if (l==z) {
				z += TEMPLATE_STEP;
				out[0] = realloc (out[0], sizeof (int) * z);
			}
		}
		s++;
		if (s==m->bct[w]) { break; }
		i = 0;
		while (i < pl[j]) {
			out[0][l] = p[j][i];
			l++;
			i++;
			if (l==z) {
				z += TEMPLATE_STEP;
				out[0] = realloc (out[0], sizeof (int) * z);
			}
		}
		j++;
		s++;
	}
	out[0] = realloc (out[0], sizeof (int) * l);
	olen[0] = l;
	/* discard particles */
	i = 0;
	while (i < x) {
		if (pl[i] != -1) {
			free (p[i]);
		}
		i++;
	}
	free (p);
	free (pl);
	/* adjust instance count */
	m->inst[w]++;
}


/* ****************************************************************** */

/*                                                     HZ_func_params ()
 *
 *	Fetches parameters in brackets.
*/
void HZ_func_params (HZ_Params* o, int* str, int* del)
{
	int d = 0, i = 0, j;
	o->args = malloc (sizeof (int*) * del[0]);
	o->alen = malloc (sizeof (int) * del[0]);
	o->ct = del[0];
	while (d < del[0]) {
		o->alen[d] = del[d+1] - i;
		o->args[d] = malloc (sizeof (int) * o->alen[d]);
		j = 0;
		while (i < del[d+1]) {
			o->args[d][j] = str[i];
			i++;
			j++;
		}
		i++;
		d++;
	}
	o->blen = -1;
	o->elen = -1;
}


/* ****************************************************************** */

/*                                                     HZ_inject_func ()
 *
 *	Render //f function wrapper macro.
 *	(Expects to be on last character of name)
*/
void HZ_inject_func (CBUF, int w)
{
	HZ_Params o;
	int* str; 
	int* del;
	int len;
	NEXT;
	
	/* check for (params) and fetch: */
	while (CH==' ' || CH=='\t' || CH=='\n') { NEXT; }
	if (CH != '(') { return; }
	HZ_end_bracket (C, &str, &del, ',');
	HZ_func_params (&o, str, del);
	free (str);
	free (del);

	/* render template... */
	HZ_render_template (&o, &HZ_FUNC, w, &str, &len);
	add_codebuf_ints (C, str, len);
	free (str);
	HZ_free_params (&o);
}


/* ****************************************************************** */

/*                                                    HZ_inject_block ()
 *
 *	Render //b block wrapper macro.
 *	(Expects to be on last character of name)
*/
void HZ_inject_block (CBUF, int w)
{
	HZ_Params o;
	int* str; 
	int* del;
	int len, d;
	NEXT;
	
	/* check for (params) and fetch: */
	while (CH==' ' || CH=='\t' || CH=='\n') { NEXT; }
	if (CH != '(') { return; }
	HZ_end_bracket (C, &str, &del, ';');
	HZ_func_params (&o, str, del);
	free (str);
	free (del);
	NEXT;
	
	/* check for initial #B block or no: */
	d = MATCHwtc("{",1);
	if (d==0) { REW(1); goto HZ_inject_block_halt; }
	while (d>1) { NEXT; d--; }
	HZ_end_bracket (C, &str, &del, -1);
	o.blk = str;
	o.blen = del[1];
	free (del);
	NEXT;
	
	/* check for added #E else block or no: */
	d = MATCHwtc("else",4);
	if (d==0) { REW(1); goto HZ_inject_block_inject; }
	while (d>0) { NEXT; d--; }
	d = MATCHwtc("{",1);
	if (d==0) { REW(1); goto HZ_inject_block_inject; }
	while (d>1) { NEXT; d--; }
	HZ_end_bracket (C, &str, &del, -1);
	o.els = str;
	o.elen = del[1];
	free (del);
	
	/* render template... */
	HZ_inject_block_inject:
		HZ_render_template (&o, &HZ_BLOCK, w, &str, &len);
		add_codebuf_ints (C, str, len);
		free (str);

	HZ_inject_block_halt:
		HZ_free_params (&o);
}


/* ****************************************************************** */





