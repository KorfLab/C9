#include <stdio.h>
#include "ik/toolbox.h"
#include "ik/sequence.h"

static char USAGE[] = "\
usage: c9 [options] <fasta file>\n\
options:\n\
  -anchor <string> [GG]\n\
  -length <int> [20]\n\
  -help\n\
";

static char ANCHOR[16] = "GG";
static int  LENGTH     = 20;


int main (int argc, char ** argv) {
	ik_pipe  pipe;
	ik_fasta fasta;
	ik_tvec  seqs, defs;
	char id[256], word[64];
	char *seq, *def;
	int i, j;
	
	if (argc == 1) ik_exit(1, USAGE);
	
	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-help", 0);
	ik_register_option("-anchor", 1);
	ik_register_option("-length", 1);
	ik_parse_options(&argc, argv);
	
	/* control */
	if (ik_option("-anchor")) strcpy(ANCHOR, ik_option("-anchor"));
	if (ik_option("-length")) LENGTH = atoi(ik_option("-length"));
	if (ik_option("-help"))   ik_exit(1, USAGE);
	
	/* read fasta files into memory */
	seqs = ik_tvec_new();
	defs = ik_tvec_new();
	pipe = ik_pipe_open(argv[1], "r");
	while ((fasta = ik_fasta_read(pipe->stream)) != NULL) {
		
		/* convert sequence */
		seq = malloc(strlen(fasta->seq) + 1);
		strcpy(seq, fasta->seq);
		for (i = 0; i < strlen(seq); i++) {
			switch (seq[i]) {
				case 'A': case 'a': seq[i] = 'A'; break;
				case 'C': case 'c': seq[i] = 'C'; break;
				case 'G': case 'g': seq[i] = 'G'; break;
				case 'T': case 't': seq[i] = 'T'; break;
				default:            seq[i] = 'N'; break;
			}
		}
				
		/* abbreviate fasta def line */
		strcpy(id, fasta->def);
		for (i = 0; i < strlen(id); i++) {
			if (id[i] < 33) {
				id[i] = '\0';
				break;
			}
		}
		
		/* save & clean up */
		ik_tvec_push(seqs, seq);
		ik_tvec_push(defs, id);
		ik_fasta_free(fasta);
	}
	ik_pipe_close(pipe);
	
	/* main loop */
	for (i = 0; i < seqs->size; i++) {
		seq = seqs->elem[i];
		def = defs->elem[i];
		
		/* find anchors */
		for (j = LENGTH; j < strlen(seq); j++) {
			if (seq[j+1] == ANCHOR[0] && seq[j+2] == ANCHOR[1]) {
				strlcpy(word, seq +j-20, 24);
				printf("%s\n", word);
			}
		}
	}

	return 0;
}



