#!/bin/sh

FASTQ=test/data/puck_7_mbrain.fastq.gz
SAMPLE=tmp/pcr_dup

./bin/pcr_duplicates --sample $SAMPLE $FASTQ

