#!/usr/bin/perl
use strict;
use warnings 'FATAL' => 'all';
use FAlite;
use Inline C => 'DATA';

die "usage: $0 <fasta file> <K> <anchor> <T>\n" unless @ARGV == 4;
my ($FILE, $K, $A, $T) = @ARGV;

# Part 1: counting
my %count;
my $fh;
if ($FILE =~ /\.gz$/) {open($fh, "gunzip -c $FILE |") or die}
else                  {open($fh, $FILE) or die}
my $fasta = new FAlite($fh);
while (my $entry = $fasta->nextEntry) {
	my $seq = uc $entry->seq;
	while ($seq =~ m/$A/g) {
		$count{substr($seq, pos($seq) -$K, $K)}++;
	}
}
close $fh;

# Part 2: finding similarities
if ($FILE =~ /\.gz$/) {open($fh, "gunzip -c $FILE |") or die}
else                  {open($fh, $FILE) or die}
$fasta = new FAlite($fh);
while (my $entry = $fasta->nextEntry) {
	my $seq = uc $entry->seq;
	my ($def) = $entry->def =~ /^>(\S+)/;
	while ($seq =~ m/$A/g) {
		my $pos = pos($seq) - $K;
		my $s1 = substr($seq, $pos, $K);
		my $found = 0;
		foreach my $s2 (keys %count) {
			my $d = distance($s1, $s2, $K, $T);
			if ($d <= $T) {$found++}
		}
		print "$def\t$pos\t$found\n";
	}
}
close $fh;

__END__
__C__

int distance (const char *s1, const char *s2, int length, int max) {
	int i, d;
	
	d = 0;
	for (i = 0; i < length; i++) {
		if (s1[i] != s2[i]) d++;
		if (d > max) break;
	}
	
	return d;
}

