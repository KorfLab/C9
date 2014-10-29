#include <stdio.h>
#include "ik/toolbox.h"
#include "ik/sequence.h"


static char usage[] = "\
usage: c9 [options] <fasta file>\n\
options:\n\
  -anchor <string> [NGG]\n\
  -help\n\
";

int main (int argc, char ** argv) {
	
	/* preamble */
	if (argc == 1) {
		fprintf(stderr, "%s", usage);
		exit(1);
	}
	
	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-help", 0);
	ik_register_option("-anchor", 1);
	ik_parse_options(&argc, argv);
	
	/* control */
	
	printf("hello world\n");

	return 0;
}



