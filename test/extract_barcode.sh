#!/bin/sh

READ_STRUCTURE=5C3X18U6C9M1X
READ_1=test/data/imran_puck_11_humangbm_R1.fastq.gz
READ_2=test/data/imran_puck_11_humangbm_R2.fastq.gz

./bin/extract_barcode --read-structure 5C3X18U6C9M1X $READ_1 $READ_2

