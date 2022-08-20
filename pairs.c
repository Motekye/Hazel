/****************************************************************************/
/***  #####_ .####, ###### #####, .####_  ***********************************/
/***  ##  .# ##   #   ##   ##  .# ##__ ^  ***********************************/
/***  #####^ ######   ##   ####*  __^^##  ***********************************/
/***  ##     ##   # ###### ##  ^# *####   ***********************************/
/****************************************************************************
 *
 *	
 *
 ***
 *** DEFINES:
 ***	functions:              str_eq, key_ct, key_index, key_index_ints, 
 ***							delim_ct, break_by, break_by_ct, 
 ***							free_charpp
*/


/* ************************************************************************ */


/*                                                                   str_eq ()
 *
 *	Check if two null terminated char* arrays match each-other...
*/
int str_eq (char* a, char* b)
{
	int i = 0;
	while (a[i] != '\0' && b[i] != '\0')
	{
		if (a[i] != b[i]) { return 0; }
		i++;
	}
	if (a[i] == '\0' && b[i] == '\0') { return 1; }
	return 0;
}




/* ************************************************************************ */


/*                                                                   key_ct ()
 *
 *	Count occurences of a full key in a list produced by break_by...
*/
int key_ct (
	char* m,			/* match string */
	char** a,			/* array to search */
	int l				/* length of array */
) {
	int i = 0, j, c = 0;
	while (i < l)
	{
		j = 0;
		while (m[j] == a[i][j])
		{
			if (m[j] == '\0')
			{
				c++;
				break;
			}
			j++;
		}
		i++;
	}
	return c;
}


/* ************************************************************************ */


/*                                                                key_index ()
 *
 *	Get index of first matching key found or -1 for none.
*/
int key_index (
	char* m,			/* match string */
	char** a,			/* array to search */
	int l				/* length of array */
) {
	int i = 0, j;
	while (i < l)
	{
		j = 0;
		while (m[j] == a[i][j])
		{
			if (m[j] == '\0') { return i; }
			j++;
		}
		i++;
	}
	return -1;
}


/* ************************************************************************ */


/*                                                           key_index_ints ()
 *
 *	Get index of first matching key found or -1 for none.
*/
int key_index_ints (
	int* m,				/* match string as ints */
	char** a,			/* array to search */
	int l				/* length of array */
) {
	int i = 0, j;
	while (i < l)
	{
		j = 0;
		while (m[j] == a[i][j])
		{
			if (m[j] == '\0') { return i; }
			j++;
		}
		i++;
	}
	return -1;
}


/* ************************************************************************ */


/*                                                                 delim_ct ()
 *
 *	Get list of locations of each substring and original length...
 *
 *	OUTPUT:		[ top, >, <, >, <... srclen ]
 *
*/
int* delim_ct (
	char* d,			/* delimiter string */
	char* s				/* source string */
) {

	int i = 0,			/* current pointer */
		j = 0,			/* secondary pointer */
		n = 0,			/* pointer in temp array */
		l = 0;			/* length of source string */
		
	int* t;				/* temporary delimiter array */
	int* o;				/* final output array */
	
	/*
	 *	Empty source string or delimiter...
	*/
	if (d[0] == '\0')
	{ 
		o = (int*) malloc (sizeof (int) * 2);
		o[0] = 0;
		o[1] = 1;		
		return o;
	}
	if (s[0] == '\0')
	{ 
		o = (int*) malloc (sizeof (int) * 2);
		o[0] = 0;
		o[1] = 0;		
		return o;
	}
	
	/*
	 *	Make temporary int* array length of whole string * 2.
	*/
	while (s[l] != '\0')
	{
		l++;
	}
	t = (int*) malloc (sizeof (int) * l * 2);
	
	/*
	 *	Measure each instance of delimiter into temporary array...
	*/
	while (s[i] != '\0')
	{
		if (s[i] == d[0])
		{
			/*
			 *	First char matches delimiter, check the rest.
			*/
			j = 1;
			while (d[j] != '\0' && s[i+j] != '\0' && s[i+j] == d[j])
			{
				j++;
			}
			/*
			 *	Should reach end of delimiter.
			 *	Place position and stop position in temp array.
			*/
			if (d[j] == '\0')
			{
				t[n] = i;
				t[n + 1] = i + j;
				n += 2;
				i = i + j - 1;
			}
		}
		i++;
	}
	/*
	 *	Create final return array of proper size.
	 *	Open with top index, close with source length...
	*/
	o = (int*) malloc (sizeof (int) * (n + 2));
	i = 0;
	o[0] = n+1;
	while (i < n)
	{
		o[i + 1] = t[i];
		i++;
	}
	o[i + 1] = l;
	free (t);
	/*
	 *	Done.
	*/
	return o;
}


/* ************************************************************************ */


/*                                                              break_by_ct ()
 *
 *	Break string by delimiter using output from delim_ct...
*/
char** break_by_ct (
	int* a,				/* delim_ct output */
	char* s				/* source string */
) {
	int i = 0, 			/* iterator in coords */
		j = 0,			/* iterator in output array */
		k = 0,			/* position in particle */
		n = 0,			/* position in source string */
		c = 0;			/* total particle count */
	char** o;			/* output list */

	/*
	 *	Return 1 length array of null on invalid input.
	*/
	if (a[0] == 0 || a[a[0]] == 0)
	{
		o = malloc (sizeof (char*));
		o[0] = malloc (sizeof (char));
		o[0][0] = '\0';
		return o;
	}
	
	/*
	 *	Allocate, then fill particles...
	*/
	c = (a[0] >> 1) + 1;
	o = malloc (sizeof (char*) * c);
	i = 1;
	while (1)
	{
		o[j] = malloc (sizeof (char) * (a[i] - n + 1));
		while (n < a[i])
		{
			o[j][k] = s[n];
			n++;
			k++;
		}
		o[j][k] = '\0';
		if (i >= a[0]) { break; }
		n = a[i+1];
		k = 0;
		j++;
		i += 2;
	}

	/*
	 *	Done.
	*/
	return o;
}


/* ************************************************************************ */


/*                                                                 break_by ()
 *
 *	Break string by delimiter using raw input string for delimiter...
*/
char** break_by (
	char* d,			/* delimiter string */
	char* s				/* source string */
) {
	int* a = delim_ct (d, s);
	char** o = break_by_ct (a, s);
	free (a);
	return o;
}


/* ************************************************************************ */


/*                                                              free_charpp ()
 *
 *	Free complete char* array...
*/
void free_charpp (
	char** a, 			/* source array of char* to free */
	int t				/* top member of array */
) {
	int i = 0;
	while (i <= t)
	{
		free (a[i]);
		i++;
	}
	free (a);
}


/* ************************************************************************ */



