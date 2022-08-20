/****************************************************************************/
/***  .####, #####,  ####_ ##   #  ******************************************/
/***  ##   # ##  .# ##  __ ##   #  ******************************************/
/***  ###### ####*  ##  ^# `##_#`  ******************************************/
/***  ##   # ##  ^#  ####*  `##`   ******************************************/
/****************************************************************************
 *
 *	Handles arguments when invoking Overload on the command line.
 *	
 *	Interprets flags and paths, invokes help and populates environment
 *	variables for executing Overload.
 *
 ***
 *** DEFINES:
 ***	variables:              source_path, output_mode
 ***	functions:              process_argv
 ***
 *** USES:
 ***	from pairs.c:           break_by, ctkey, free_charpp
 ***	from help.c:            display_help
 ***
*/

char* source_path;
char* output_path;
int output_mode = 0;

static char*** flag_choice;
static int* flag_ct;

#define ARGV_FLAG_CHOICES		5

#define ARGV_FLAG(n,m,x)		flag_choice[n] = break_by ("|", m); flag_ct[n] = x


/* ************************************************************************ */


/*
 *	Process all arguments passed to Overload...
*/

int process_argv (int argc, char *argv[]) 
{
	int z = 1, i = 1, j, n;

	/*
	 *	Open help if no arguments are provided.
	*/
	if (argc == 1) {
		display_help ();
		return 1;
	}

	/*
	 *	Create structures for flag options and populate.
	*/
	flag_choice = malloc (sizeof (char**) * ARGV_FLAG_CHOICES);
	flag_ct = malloc (sizeof (int) * ARGV_FLAG_CHOICES);

	ARGV_FLAG (0, "-h|--help|--man", 3);
	ARGV_FLAG (1, "-o|--out|--output", 3);
	ARGV_FLAG (2, "-m|--min|--minify", 3);
	ARGV_FLAG (3, "-q|--quiet", 2);
	ARGV_FLAG (4, "-n|--nest|--max-nest", 3);
	
	/*
	 *	Checking all arguments...
	*/
	while (i < argc) {
	
		/*
		 *	Get file name:
		*/
		if (argv[i][0] != '-') {
			source_path = argv[i];
			z = 0;
		} else {
	
			/*
			 *	Arguments that begin with '-' for match in choices.
			*/
			j = 0;
			while (j < ARGV_FLAG_CHOICES) {
				if (key_ct (argv[i], flag_choice[j], flag_ct[j]) != 0) {
					switch (j) {

						/*
						 *	Display help:
						*/
						case 0:
							z = 1;
							display_help (); /* (help.c) */
							goto process_argv_finished;
							
						/*
						 *	Specify output path:
						*/
						case 1:
							i++;
							if (i == argc) {
								puts ("Must specify a file for output!");
								z = 1;
								goto process_argv_finished;
							} else {
								output_path = argv[i];
							}
							break;

						/*
						 *	Minify, Quiet:
						*/
						case 2: output_mode += 1; break;
						case 3: output_mode += 2; break;

						/*
						 *	TODO: Change max nest level:
						*/
						case 4:
							i++;
							if (i == argc) {
								puts ("Must specify max nesting level!");
								z = 1;
								goto process_argv_finished;
							} else {
								n = 0;
								j = 0;
								while (argv[i][j] != '\0') {
									n *= 10;
									n += argv[i][j] - '0';
									j++;
								}
								MAX_NESTING = n;
							}
							break;
							
						/*
						 *	Unrecognized argument
						*/
						default: goto process_argv_unrecognized;
						
					}
				}
				j++;
			}
			
		}
		i++;
	}
	goto process_argv_finished;

	/*
	 *	Malformed or unrecognized argument flag. 
	*/
	process_argv_unrecognized:
		printf ("Unrecognized argument flag: %s\n", argv[i]);
		puts ("Try --help for more information.");

	/*
	 *	Free argument option flag choices.
	*/
	process_argv_finished:
		i = 0;
		while (i < ARGV_FLAG_CHOICES) {
			free_charpp (flag_choice[i], flag_ct[i]-1);
			i++;
		}
		free (flag_ct);
		free (flag_choice);
	
	/*
	 *	Return 0 (ok) or 1 (error) state.
	*/
	return z;
}


/* ************************************************************************ */


#undef ARGV_FLAG_CHOICES
#undef ARGV_FLAGS


/* ************************************************************************ */


