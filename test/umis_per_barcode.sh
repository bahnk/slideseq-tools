#!/bin/sh

BAM=test/data/210611_imran_puck_11_humangbm.up_matched.bam

./bin/umis_per_barcode --threshold 3 $BAM test/test_umis_per_barcode.bam

