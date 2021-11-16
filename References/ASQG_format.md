The eASQG format describes an assembly graph. Each line is a tab-delimited record. The first field in each record describes the record type. The four types are:

1. `HT` - Header record. This record contains metadata tags for the file version (VN tag) and parameters associated with the graph (for example the minimum overlap length).
2. `VT` - Vertex records. The second field contains the vertex identifier (integer ID), the third field contains the sequence. Subsequent fields contain optional tags.
3. `ED` - Edge description records. The second field describes a pair of overlapping sequences. A full description of this field is below. Subsequent fields contain optional tags.
4. `PM` - Point polymorphism records. The second field describes the type of the bio sequence ("N" for nucleotide; "P" for peptide). Subsequent fields describe vertext identifier, the polymophic site location (0-based), and the frequencies (ASCII-33 converted integers) of each letter in the corresponding alphabet.
 
## Tags

Tags follow the same format as [SAM](http://samtools.sourceforge.net/SAMv1.pdf)

## Example

    HT	VN:i:1	ER:f:0	OL:i:45	IN:Z:reads.fa	CN:i:1	TE:i:0
    VT	0	GATCGATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGG
    VT	1	CGATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGGATA
    VT	2	ATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGGATATT
    ED	1 0 0 46 50 3 49 50 0 0
    ED	2 1 0 47 50 2 49 50 0 0
    PM	N	2	5	+@(Q

## Edge descriptions

The second field of ED records describe an overlap between a pair of sequences. This field contains 10 elements which are:

1. sequence 1 name
2. sequence 2 name
3. sequence 1 overlap start (0 based)
4. sequence 1 overlap end (inclusive)
5. sequence 1 length
6. sequence 2 overlap start (0 based)
7. sequence 2 overlap end (inclusive)
8. sequence 2 length
9. sequence 2 orientation (1 for reversed with respect to sequence 1)
10. number of differences in overlap  (0 for perfect overlaps, which is the default).

## Polymorphism descriptions

1. The second field describes the bio sequence type. (In the example, "N" indicates a nucleotide sequence)
2. The third field describes the vertex ID.
3. The fourth field describes the index of the polymorphic site (0-based index).
4. The fifth field describes the frequencies of the lexicographically-ordered alphabet. (In the example, nucleotide has alphabet size of 4, i.e. "A", "C", "G", "T". The first symbol "+" has an ASCII code of 43. The ASCII-33 code is 43-33=10, indicating the frequency of "A" is 10%. Similarly, "@" indicates the frequency of "C" is 64-33=31%.)

## beASQG format (binary format)

1. `HT`: "number of VT records" (64-bit); "number of ED records" (64-bit); "number of PM records" (64-bit)
2. `VT`: "vertex ID" (32-bit); "sequence length" (32-bit); "actual sequence" (8-bit* [sequence length])
3. `ED`: "vertex ID source" (32-bit); "vertex ID target" (32-bit); "source overlap begin" (32-bit); "source overlap end" (32-bit); "source length" (32-bit); "target overlap begin" (32-bit); "target overlap end" (32-bit); "target length" (32-bit); "orientation" (1-bit); "number of differences in overlap" (32-bit)
4. `PM`: "bio sequence type" (8-bit); "vertex ID" (32-bit); "site position" (32-bit); "frequency" (8-bit * [alphabet size])
