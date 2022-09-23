#!/bin/sh

BAM=test/data/imran.gene_tags.bam
BAM=test/data/25-06.gene_tags.bam
BAM=test/data/sample1.tagged.bam
SAMPLE=tmp/pos_dup

./bin/position_duplicates --sample $SAMPLE $BAM

