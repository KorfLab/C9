#include <stdio.h>
#include "ik/toolbox.h"
#include "ik/sequence.h"

static char USAGE[] = "\
usage: unimeter [options] <fasta file>\n\
options:\n\
  -a <string> [GG]\n\
  -k <int>    [20]\n\
  -t <int>    [4]\n\
  -help\n\
";

static char A[16] = "GG";
static int  K     = 20;
static int  T     = 4;

int distance (const char *, const char *, int, int);


int main (int argc, char ** argv) {
	ik_pipe  pipe;
	ik_fasta fasta;
	ik_tvec  kmers;
	char kmer[64];
	char *seq, *def;
	int i, j, d, found;
	int alen, amatch; /* anchor length, match flag */
	
	if (argc == 1) ik_exit(1, USAGE);
	
	/* options */
	ik_set_program_name(argv[0]);
	ik_register_option("-help", 0);
	ik_register_option("-a", 1);
	ik_register_option("-k", 1);
	ik_register_option("-t", 1);
	ik_parse_options(&argc, argv);
	
	/* control */
	if (ik_option("-a")) strcpy(A, ik_option("-a"));
	if (ik_option("-k")) K = atoi(ik_option("-k"));
	if (ik_option("-t")) T = atoi(ik_option("-t"));
	if (ik_option("-help"))   ik_exit(1, USAGE);
	
	/* init */
	kmers = ik_tvec_new();
	alen  = strlen(A);
	
	/* Part 1: counting */
	pipe  = ik_pipe_open(argv[1], "r");
	while ((fasta = ik_fasta_read(pipe->stream)) != NULL) {
		seq = fasta->seq;
		for (i = K; i < strlen(seq) -alen; i++) {
			amatch = 1;
			for (j = 0; j < alen; j++) {
				if (A[j] == '.') continue;
				if (A[j] != seq[i+j-alen]) {
					amatch = 0;
					break;
				}
			}
			if (amatch) {
				strlcpy(kmer, seq +i -K, K+1);
				ik_tvec_push(kmers, kmer);
			}
		
		}
		ik_fasta_free(fasta);
	}
	ik_pipe_close(pipe);
	
	/* Part 2: finding similarities */
	pipe  = ik_pipe_open(argv[1], "r");
	while ((fasta = ik_fasta_read(pipe->stream)) != NULL) {
		seq = fasta->seq;
		def = fasta->def;
		for (i = 0; i < strlen(def); i++) {
			if (def[i] < 33) {
				def[i] = '\0';
				break;
			}
		}
		
		for (i = K; i < strlen(seq) -alen; i++) {
			amatch = 1;
			for (j = 0; j < alen; j++) {
				if (A[j] == '.') continue;
				if (A[j] != seq[i+j-alen]) {
					amatch = 0;
					break;
				}
			}
			
			if (amatch) {
				strlcpy(kmer, seq +i -K, K+1);
				found = 0;
				for (j = 0; j < kmers->size; j++) {
					d = distance(kmer, kmers->elem[j], K, T);
					if (d <= T) found++;
				}
				printf("%s\t%d\t%d\n", def, i - K, d);
			}
		
		}
		ik_fasta_free(fasta);
	}
	ik_pipe_close(pipe);
	

	return 0;
}

int distance (const char *s1, const char *s2, int len, int max) {
	int i, d;
	
	d = 0;
	for (i = 0; i < len; i++) {
		if (s1[i] != s2[i]) d++;
		if (d > max) break;
	}
	
	return d;
}

