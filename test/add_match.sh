#!/bin/sh

BAM=test/data/210611_imran_puck_11_humangbm.umi_threshold.bam
CSV=test/data/imran_puck_11_humangbm.ordered.map.matching.csv

./bin/add_match $CSV $BAM test/test_add_match.bam

