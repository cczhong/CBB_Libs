The ASQG format describes an assembly graph. Each line is a tab-delimited record. The first field in each record describes the record type. The three types are:

1. `HT` - Header record. This record contains metadata tags for the file version (VN tag) and parameters associated with the graph (for example the minimum overlap length).
2. `VT` - Vertex records. The second field contains the vertex identifier, the third field contains the sequence. Subsequent fields contain optional tags.
3. `ED` - Edge description records. The second field describes a pair of overlapping sequences. A full description of this field is below. Subsequent fields contain optional tags.

## Tags

Tags follow the same format as [SAM](http://samtools.sourceforge.net/SAMv1.pdf)

## Example

    HT	VN:i:1	ER:f:0	OL:i:45	IN:Z:reads.fa	CN:i:1	TE:i:0
    VT	read1	GATCGATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGG
    VT	read2	CGATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGGATA
    VT	read3	ATCTAGCTAGCTAGCTAGCTAGTTAGATGCATGCATGCTAGCTGGATATT
    ED	read2 read1 0 46 50 3 49 50 0 0
    ED	read3 read2 0 47 50 2 49 50 0 0

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


