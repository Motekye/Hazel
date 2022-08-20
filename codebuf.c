




/* ****************************************************************** */

/*
 *	Layered Code Input Buffers...
*/
typedef struct {

	char**		path;          /* relative path for each level */
	int*		plen;          /* lengths of relative paths */
	int** 		src;           /* content of each level */
	int*		len;           /* length of each level */
	int*		pos;           /* position in each level */
	int*		mode;          /* modes of each level */
	int			wh;            /* current level index */
	
	int			ch;            /* current character */
	int			pr;            /* previous character */
	int			pnw;           /* previous non-whitespace character */
	int			prlb;          /* previous line break? */
	
	FILE*		output;        /* output file pointer */
	int 		quiet;         /* quiet mode? */
	int 		minify;        /* minify? */
	int 		error;         /* responding with error code */

} Codebuf;


/* ****************************************************************** */


Codebuf new_codebuf (char* fname);
void free_codebuf (Codebuf* o);
void codebuf_out (Codebuf* o, int c);
void codebuf_outs (Codebuf* o, int* c, int l);
void codebuf_output_file (Codebuf* o, char* fname, int dofree);
void codebuf_set_mode (Codebuf* o, int mode);

int* read_file_as_ints (char* fname, int* len);
char* base_path (char* fname, int* len);
char* relative_path (char* path, char* fname);

void add_codebuf_file (Codebuf* o, char* fname, int mode);
char* codebuf_get_path (Codebuf* o);
void add_codebuf_ints (Codebuf* o, int* a, int l);
void append_codebuf_ints (Codebuf* o, int* a, int l);
int codebuf_next (Codebuf* o);
int codebuf_scan_next (Codebuf* o);
void codebuf_advance (Codebuf* o, int by);
int codebuf_match_char (Codebuf* o, char* m, int l);
int codebuf_match (Codebuf* o, int* m, int l);


/* ****************************************************************** */

/*                                                        new_codebuf ()
 *
 *	Create a new code buffer from file name:
*/
Codebuf new_codebuf (char* fname)
{
	Codebuf o;
	o.path = malloc (sizeof (char*));
	o.plen = malloc (sizeof (int));
	o.mode = malloc (sizeof (int));
	o.src = malloc (sizeof (int*));
	o.len = malloc (sizeof (int));
	o.pos = malloc (sizeof (int));
	o.path[0] = base_path (fname, &o.plen[0]);
	o.mode[0] = 0;
	o.src[0] = read_file_as_ints (fname, &o.len[0]);
	o.pos[0] = 0;
	o.wh = 0;
	if(o.len[0]==0){
		o.ch = '\0';
	} else {
		o.ch = o.src[0][0];
	}
	o.pr = '\0';
	o.pnw = '\0';
	o.prlb = 1;
	o.output = NULL;
	o.minify = 0;
	o.quiet = 0;
	o.error = 0;
	return o;
}


/* ****************************************************************** */

/*                                                       free_codebuf ()
 *
 *	Close output stream? and destroy code buffer:
*/
void free_codebuf (Codebuf* o)
{
	if (o->output!=NULL) { fclose (o->output); }
	while (o->wh > -1) {
		free (o->path[o->wh]);
		free (o->src[o->wh]);
		o->wh--;
	}
	free (o->path);
	free (o->plen);
	free (o->mode);
	free (o->src);
	free (o->pos);
	free (o->len);
}


/* ****************************************************************** */

/*                                                        codebuf_out ()
 *
 *	Print character to selected output:
*/
void codebuf_out (Codebuf* o, int c)
{
	/* quiet mode? don't print: */
	if (o->quiet==1) { return; }
	
	/* printing whitespace? */
	
	
	/* print to console: */
	if (o->output==NULL) {
		printf ("%c",c);
		
	/* or print to file: */
	} else {
		fputc ((char) c, o->output);
	}
}


/* ****************************************************************** */

/*                                                       codebuf_outs ()
 *
 *	Print character array to selected output:
*/
void codebuf_outs (Codebuf* o, int* c, int l)
{
	int i;
	
	/* quiet mode? don't print: */
	if (o->quiet==1) { return; }
	
	/* printing whitespace? */
	
	
	/* print to console: */
	if (o->output==NULL) {
		i = 0;
		while (i < l) {
			printf ("%c",c[i]);
			i++;
		}
		
	/* or print to file: */
	} else {
		i = 0;
		while (i < l) {
			fputc ((char) c[i], o->output);
			i++;
		}
	}
	
	/* last character printed: */
	o->pr = c[i-1];
	
}


/* ****************************************************************** */

/*                                                codebuf_output_file ()
 *
 *	Select the file to output to or . to print to console.
*/
void codebuf_output_file (Codebuf* o, char* fname, int dofree)
{
	if (o->output!=NULL) { fclose (o->output); }
	
	if (fname[0]=='\0' || fname[0]==' ' || fname[0]=='\t') {
		return;
	}
	
	o->output = fopen (fname, "wb");
	
	if (dofree) { free (fname); }
}


/* ****************************************************************** */

/*                                                   codebuf_set_mode ()
 *
 *	Mode flags for codebuf level...
 *	External communication with modules:
*/
void codebuf_set_mode (Codebuf* o, int mode)
{
	if (mode & 1) { o->minify = 1; } /* <- can't disable! */
	if (mode & 2) { o->quiet = 1; } else { o->quiet = 0; }
	
}


/* ****************************************************************** */

/*                                                  read_file_as_ints ()
 *
 *	Load a file as int* array with length.
 *	Normalize line breaks to \n
 *
 *	TODO: support for unicode.
*/

int* read_file_as_ints (char* fname, int* len)
{
	FILE* f = fopen (fname, "rb");
	char* not_found = "/* File Not Found: _ */\n";
	int i = 0, j, l = 0, c, p = '\0';
	int sz = FILE_STEP;
	int* o;
	if (f==NULL) {
		while (fname[l]!='\0') {
			l++;
		}
		o = malloc (sizeof (int) * (l + 23));
		i = 0;
		while (i < 19) {
			o[i] = not_found[i];
			i++;
		}
		j = 0;
		while (j < l) {
			o[i] = fname[j];
			i++;
			j++;
		}
		j = 20;
		while (j < 24) {
			o[i] = not_found[j];
			i++;
			j++;
		}
		len[0] = i;
		return o;
	}
	o = malloc (sizeof(int) * sz);
	c = (int) getc (f);
	while (!feof (f)){
		if (c < 1){ break; }
		if (c!='\n' || p!='\r') {
			o[i] = c;
		}
		p = c;
		i++;
		if(i > sz-2){
			sz += FILE_STEP;
			o = realloc (o, sizeof(int) * sz);
		}
		c = (int) getc (f);
	}
	o = realloc (o, sizeof(int) * i);
	len[0] = i;
	fclose (f);
	return o;
}


/* ****************************************************************** */

/*                                                          base_path ()
 *
 *	Get base path from filename:
*/
char* base_path (char* fname, int* len)
{
	int i = 0, j = 0;
	char* o;
	while (fname[i]!='\0') {
		i++;
	}
	i--;
	while (i > -1 && fname[i]!='/') {
		i--;
	}
	i++;
	o = malloc (sizeof (char) * i + 1);
	while (j < i) {
		o[j] = fname[j];
		j++;
	}
	o[j] = '\0';
	len[0] = i + 1;
	return o;
}


/* ****************************************************************** */

/*                                                      relative_path ()
 *
 *	Combine path with relative path:
 *	(Cannot navigate up past first script's directory)
*/
char* relative_path (char* path, char* fname)
{
	int i, j, l, pn = 0, px, fn = 0, fs = 0, up = 0;
	char* o;
	while (path[pn]!='\0') {
		pn++;
	}
	while (fname[fn]!='\0') {
		fn++;
	}
	while (fs < fn - 3
		&& fname[fs] == '.'
		&& fname[fs+1] == '.'
		&& fname[fs+2] == '/'
	) {
		up++;
		fs += 3;
	}
	px = pn;
	if(up > 0) {
		px--;
		while (px > 0 && up > 0) {
			px--;
			if (path[px] == '/') { up--; }
		}
		px++;
		if (px < 0) { px = 0; }
	}
	l = px + (fn - fs) + 1;
	o = malloc (sizeof (char) * l);
	i = 0;
	j = 0;
	while (i < px) {
		o[j] = path[i];
		i++;
		j++;
	}
	i = fs;
	while (i < fn) {
		o[j] = fname[i];
		i++;
		j++;
	}
	o[j] = '\0';
	return o;
}


/* ****************************************************************** */

/*                                                   codebuf_get_path ()
 *
 *	Get path up to line break: (char*)
*/
char* codebuf_get_path (Codebuf* o)
{
	char* p = malloc (sizeof (char) * MAX_PATH);
	int i = 0, c = codebuf_next (o);
	while (c=='\t' || c== ' ') {
		codebuf_next (o);
	}
	while (c!='\n' && c!= '\0' && c!= ' ' && c!= '\t') {
		p[i] = (char) c;
		i++;
		c = codebuf_next (o);
	}
	p[i] = '\0';
	i++;
	p = realloc (p, sizeof (char) * i);
	return p;
}


/* ****************************************************************** */

/*                                                   add_codebuf_file ()
 *
 *	Add level to code buffer from file:
 *	(Operates with path relative to current file)
*/
void add_codebuf_file (Codebuf* o, char* fname, int mode)
{
	char* f = relative_path (o->path[o->wh], fname);
	o->wh++;
	o->path = realloc (o->path, sizeof (char*) * (o->wh + 1));
	o->plen = realloc (o->plen, sizeof (int) * (o->wh + 1));
	o->path[o->wh] = base_path (f, &o->plen[o->wh]);
	o->mode = realloc (o->mode, sizeof (int) * (o->wh + 1));
	o->mode[o->wh] = mode;
	codebuf_set_mode (o, mode);
	o->src = realloc (o->src, sizeof (int*) * (o->wh + 1));
	o->len = realloc (o->len, sizeof (int) * (o->wh + 1));
	o->src[o->wh] = read_file_as_ints (f, &o->len[o->wh]);
	o->pos = realloc (o->pos, sizeof (int) * (o->wh + 1));
	o->pos[o->wh] = -1;
	free (fname);
	free (f);
}


/* ****************************************************************** */

/*                                                   add_codebuf_ints ()
 *
 *	Add level to code buffer from int*:
*/
void add_codebuf_ints (Codebuf* o, int* a, int l)
{
	int i = 0;
	o->wh++;
	o->path = realloc (o->path, sizeof (char*) * (o->wh + 1));
	o->path[o->wh] = malloc (sizeof (int) * o->plen[o->wh-1]);
	o->plen = realloc (o->plen, sizeof (int) * (o->wh + 1));
	while (i < o->plen[o->wh-1]) {
		o->path[o->wh][i] = o->path[o->wh-1][i];
		i++;
	}
	o->plen[o->wh] = o->plen[o->wh-1];
	o->mode = realloc (o->mode, sizeof (int) * (o->wh + 1));
	o->mode[o->wh] = o->mode[o->wh-1];
	o->src = realloc (o->src, sizeof (int*) * (o->wh + 1));
	o->src[o->wh] = malloc (sizeof (int) * l);
	i = 0;
	while (i < l) {
		o->src[o->wh][i] = a[i];
		i++;
	}
	o->len = realloc (o->len, sizeof (int) * (o->wh + 1));
	o->len[o->wh] = l;
	o->pos = realloc (o->pos, sizeof (int) * (o->wh + 1));
	o->pos[o->wh] = -1;
}



/* ****************************************************************** */

/*                                                append_codebuf_ints ()
 *
 *	Append to current level from int*:
*/
void append_codebuf_ints (Codebuf* o, int* a, int l)
{
	int j = o->len[o->wh];
	int n = j + l;
	int i = 0;
	o->src[o->wh] = realloc (o->src[o->wh], sizeof (int*) * n);
	o->len[o->wh] = n;
	while (i < l) {
		o->src[o->wh][j] = a[i];
		i++;
		j++;
	}
}


/* ****************************************************************** */

/*                                                       codebuf_next ()
 *
 *	Fetch next character in code buffer:
 *	Advance up levels as needed.
 *	Automatically free levels at the end.
*/
int codebuf_next (Codebuf* o) 
{
	o->pos[o->wh]++;
	while (o->pos[o->wh] >= o->len[o->wh]) {
		if (o->wh == 0) {
			return '\0';
		}
		free (o->path[o->wh]);
		free (o->src[o->wh]);
		o->wh--;
		codebuf_set_mode (o, o->mode[o->wh]);
		o->pos[o->wh]++;
	}
	return o->src[o->wh][o->pos[o->wh]];
}


/* ****************************************************************** */

/*                                                    codebuf_advance ()
 *
 *	Advance without fetching characters:
*/
void codebuf_advance (Codebuf* o, int by) 
{
	int i = 0;
	while (i < by) {
		o->pos[o->wh]++;
		while (o->pos[o->wh] >= o->len[o->wh]) {
			if (o->wh == 0) {
				return;
			}
			free (o->path[o->wh]);
			free (o->src[o->wh]);
			o->wh--;
			o->pos[o->wh]++;
			codebuf_set_mode (o, o->mode[o->wh]);
		}
		i++;
	}
}


/* ****************************************************************** */

/*                                                      codebuf_match ()
 *
 *	Test for a match at this position: (int*)
*/
int codebuf_match (Codebuf* o, int* m, int l)
{
	int i = 0, w = o->wh, j = o->pos[w];
	int* s = o->src[w];
	while (i < l) {
		if (m[i] != s[j]) { return 0; }
		i++;
		j++;
		while (j==o->len[w]) {  
			if (w==0) { return 0; }
			w--;
			s = o->src[w];
			j = o->pos[w] + 1;
		}
	}
	return 1;
}


/* ****************************************************************** */

/*                                                 codebuf_match_char ()
 *
 *	Test for a match at this position: (char*)
*/
int codebuf_match_char (Codebuf* o, char* m, int l)
{
	int i = 0, w, j;
	int* s;
	if (o->ch=='\0') { return 0; }
	w = o->wh;
	j = o->pos[w];
	s = o->src[w];
	if (j>=o->len[w]) { return 0; }
	while (i < l) {
		if (m[i] != s[j]) { return 0; }
		i++;
		j++;
		while (j==o->len[w]) {  
			if (w==0) { return 0; }
			w--;
			s = o->src[w];
			j = o->pos[w] + 1;
		}
	}
	return 1;
} 


/* ****************************************************************** */

/*                                        codebuf_match_wsp_then_char ()
 *
 *	Test for a match at this position: (char*)
 *	After any amount of whitespace or line breaks.
 *	Returns length of match to advance or 0 for none...
*/
int codebuf_match_wsp_then_char (Codebuf* o, char* m, int l)
{
	int i = 0, w, j, d = 0;
	int* s;
	if (o->ch=='\0') { return 0; }
	w = o->wh;
	j = o->pos[w];
	s = o->src[w];
	while (j<o->len[w] && (
		s[j] == ' ' || s[j] == '\t' || s[j] == '\n' 
	)) {
		j++;
		d++;
	}
	if (j>=o->len[w]) { return 0; }
	while (i < l) {
		if (m[i] != s[j]) { return 0; }
		i++;
		j++;
		d++;
		while (j==o->len[w]) {  
			if (w==0) { return 0; }
			w--;
			s = o->src[w];
			j = o->pos[w] + 1;
		}
	}
	return d;
} 


/* ****************************************************************** */










