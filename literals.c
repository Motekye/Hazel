




/* ****************************************************************** */


void HZ_end_bracket (CBUF, int** str, int** del, int delim);
void HZ_stringify_trim (int q, int t, int* s, int sl, int** o, int* ol);
int* HZ_num_to_ints (int n, int* l);
void HZ_line_comment (CBUF);
void HZ_block_comment (CBUF);


/* ****************************************************************** */

/*                                                     HZ_end_bracket ()
 *
 *	Finds the matching end bracket, 
 *	locating delimiters in top level content.
 *	Wise to the syntax of Javascript strings and regular expressions.
 *
 *	del -> { segment count, start 1, start 2, start 3... end position }
*/
void HZ_end_bracket (CBUF, int** str, int** del, int delim)
{
	int* lvl = malloc (sizeof (int) * BRACKET_MAX);
	int dp = 0, dc = 1, i = 0, l = SOURCE_STEP, p;
	switch (CH) {
	case '(': lvl[0] = ')'; break;
	case '[': lvl[0] = ']'; break;
	case '{': lvl[0] = '}'; break;
	default: lvl[0] = '\0';
	}
	str[0] = malloc (sizeof (int) * l);
	del[0] = malloc (sizeof (int) * MAX_ARGUMENTS);
	del[0][0] = 0;
	if (lvl[0]=='\0') {
		goto HZ_end_bracket_finish;
	}
	while (dp > -1 && CH != '\0') {
	
		/* NEXT CHARACTER */
		p = CH;
		NEXT;
		
		/* BRACKET DEPTH */
		if (CH==lvl[dp]) {
			dp--;
			if (dp==-1){ break; }
		} else {
			switch (CH) {
			case '(': dp++; lvl[dp] = ')'; break;
			case '[': dp++; lvl[dp] = ']'; break;
			case '{': dp++; lvl[dp] = '}'; break;
			}
		}
		
		/* TOP LEVEL DELIMITERS */
		if (dp==0 && CH==delim) {
			del[0][dc] = i;
			dc++;
		}
		
		/* RECORD CHARACTER */
		str[0][i] = CH;
		i++;
		if (i>l-2) {
			l += SOURCE_STEP;
			str[0] = realloc (str[0], sizeof (int) * l);
		}
		
		/* STRING LITERALS */
		switch (CH) {
		case '"': case '\'': case '`':
			p = CH;
			NEXT;
			while (CH != p && CH != '\0') {
				str[0][i] = CH;
				i++;
				if (CH=='\\') {
					NEXT;
					str[0][i] = CH;
					i++;
				}
				NEXT;
				if (i>l-2) {
					l += SOURCE_STEP;
					str[0] = realloc (str[0], sizeof (int) * l);
				}
			}
			str[0][i] = CH;
			i++;
		}	
		
		/* TODO: REGULAR EXPRESSIONS */
		
	}
	/* RESIZE VALUES APPROPRIATELY */
	HZ_end_bracket_finish:
		str[0] = realloc (str[0], sizeof (int) * i);
		del[0][dc] = i;
		del[0][0] = dc;
		del[0] = realloc (del[0], sizeof (int) * (dc + 2));
		free (lvl);
}


/* ****************************************************************** */

/*                                                  HZ_stringify_trim ()
 *
 *	Function of #N or #_N or #"N" or #_"N" etc...
*/
void HZ_stringify_trim (int q, int t, int* s, int sl, int** o, int* ol)
{
	int i = 0, x = sl - 1, l = 0, z = STRING_STEP;
	o[0] = malloc (sizeof (int*) * z);
	/* left / right trim */
	if (t == 1) {
		while (i < sl && (s[i]==' ' || s[i]=='\t' || s[i]=='\n')) {
			i++;
		}
		while (x > -1 && (s[x]==' ' || s[x]=='\t' || s[x]=='\n')) {
			x--;
		}
	}
	x++;
	/* un-modified? */
	if (q==0) {
		while (i < x) {
			o[0][l] = s[i];
			i++;
			l++;
			if (l==z) {
				z += STRING_STEP;
				o[0] = realloc (o[0], sizeof (int*) * z);
			}
		}
	}
	/* stringify? */
	else {
		o[0][l] = q;
		l++;
		while (i < x) {
			if (s[i]=='\n'){  
				o[0][l] = '\\';
				l++;
				o[0][l] = 'n';
			} else if (s[i]=='\t'){  
				o[0][l] = '\\';
				l++;
				o[0][l] = 't';
			} else {
				if (s[i]=='\\' || s[i]==q){  
					o[0][l] = '\\';
					l++;
				}
				o[0][l] = s[i];
			}
			i++;
			l++;
			if (l>z-2) {
				z += STRING_STEP;
				o[0] = realloc (o[0], sizeof (int*) * z);
			}
		}
		o[0][l] = q;
		l++;
	}
	/* done... */
	o[0] = realloc (o[0], sizeof (int*) * l);
	ol[0] = l;
} 


/* ****************************************************************** */

/*                                                     HZ_num_to_ints ()
 *
 *	Convert integer to int* array of characters...
*/
int* HZ_num_to_ints (int n, int* l)
{
	int i = 0, j = 0;
	int* r = malloc (sizeof (int) * MAX_DIGITS);
	int* o;
	/* less than 0... */
	if (n < 0) {
		n *= -1;
		while (n > 0) {
			r[i] = (n % 10) + '0';
			n /= 10;
			i++;
		}
		r[i] = '-';
		i++;
	/* 0 or greater... */
	} else {
		while (n > 0) {
			r[i] = (n % 10) + '0';
			n /= 10;
			i++;
		}
		if (i==0) {
			r[0] = '0';
			i++;
		}
	}
	/* copy reversed number right way around... */
	o = malloc (sizeof (int) * i);
	l[0] = i;
	i--;
	while (i > -1) {
		o[j] = r[i];
		j++;
		i--;
	}
	free (r);
	return o;
}


/* ****************************************************************** */

/*                                                    HZ_line_comment ()
 *
 *
*/
void HZ_line_comment (CBUF) 
{
	NEXT;
	if (MINIFY) {
		while (CH) {
			if (CH=='\n') { return; }
			NEXT;
		}
	} else {
		while (CH) {
			if (NOMINIFY) { HZ_out (CH); }
			if (CH=='\n') { return; }
			NEXT;
		}
	}
}


/* ****************************************************************** */

/*                                                   HZ_block_comment ()
 *
 *
*/
void HZ_block_comment (CBUF) 
{
	NEXT;
	if (MINIFY) {
		while (CH) {
			if (MATCHc("*/",2)) {
				ADV(1);
				return; 
			}
			NEXT;
		}
	} else {
		HZ_out ('/');
		while (CH) {
			HZ_out (CH);
			if (MATCHc("*/",2)) {
				NEXT;
				HZ_out (CH);
				return; 
			}
			NEXT;
		}
	}
}


/* ****************************************************************** */




