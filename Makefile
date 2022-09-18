# nourdinebah@gmail.com

CC=g++
STD=-std=c++14
OPTIM=-O3
FLAGS=-w -pedantic -fopenmp
INC=-I include
LIBS=-lz -lrt -lpthread
OPTS=-DNDEBUG
OPTS+=-DSEQAN_HAS_ZLIB=1
OPTS+=-DSEQAN_ENABLE_DEBUG=0
OPTS+=-DSEQAN_ENABLE_TESTING=0
ARGS=$(STD) $(OPTIM) $(FLAGS) $(INC) $(LIBS) $(OPTS)

all: all_bin

###############################################################################
## OBJECT FILES ###############################################################

obj/utils.o:
	$(CC) lib/utils.cpp $(ARGS) -c -o obj/utils.o

obj/read_structure.o:
	$(CC) lib/read_structure.cpp $(ARGS) -c -o obj/read_structure.o

obj/read1.o:
	$(CC) lib/read1.cpp $(ARGS) -c -o obj/read1.o

obj/read2.o:
	$(CC) lib/read2.cpp $(ARGS) -c -o obj/read2.o

obj/transcript.o:
	$(CC) lib/transcript.cpp $(ARGS) -c -o obj/transcript.o

obj/record.o:
	$(CC) lib/record.cpp $(ARGS) -c -o obj/record.o

obj/mapping.o:
	$(CC) lib/mapping.cpp $(ARGS) -c -o obj/mapping.o

obj/mappings.o: obj/mapping.o
	$(CC) lib/mappings.cpp $(ARGS) -c -o obj/mappings.o

obj/molecule.o: obj/record.o
	$(CC) lib/molecule.cpp $(ARGS) -c -o obj/molecule.o

obj/molecules.o: obj/molecule.o
	$(CC) lib/molecules.cpp $(ARGS) -c -o obj/molecules.o

obj/gene.o:
	$(CC) lib/gene.cpp $(ARGS) -c -o obj/gene.o

obj/count.o: obj/gene.o
	$(CC) lib/count.cpp $(ARGS) -c -o obj/count.o

obj/counts.o: obj/gene.o
	$(CC) lib/counts.cpp $(ARGS) -c -o obj/counts.o

obj/counter.o: obj/gene.o obj/counts.o
	$(CC) lib/counter.cpp $(ARGS) -c -o obj/counter.o

###############################################################################
## TESTS ######################################################################

test/utils: obj/utils.o
	$(CC) test/utils.cpp $(ARGS) -o test/utils obj/utils.o

test/read_structure: obj/read_structure.o
	$(CC) obj/read_structure.o test/read_structure.cpp $(ARGS) \
		-o test/read_structure
		
test/read1: obj/utils.o obj/read_structure.o obj/read1.o
	$(CC) obj/utils.o obj/read_structure.o obj/read1.o test/read1.cpp \
		$(ARGS) -o test/read1

test/read2: obj/read2.o
	$(CC) obj/read2.o test/read2.cpp $(ARGS) -o test/read2

test/transcript: obj/utils.o
test/transcript: obj/read_structure.o obj/read1.o obj/read2.o obj/transcript.o
	$(CC) obj/utils.o \
		obj/read_structure.o \
		obj/read1.o \
		obj/read2.o \
		obj/transcript.o \
		test/transcript.cpp $(ARGS) -o test/transcript

test/record: obj/record.o
	$(CC) obj/record.o test/record.cpp $(ARGS) -o test/record

test/mapping: obj/mapping.o
	$(CC) obj/mapping.o test/mapping.cpp $(ARGS) -o test/mapping

test/mappings: obj/mapping.o obj/mappings.o
	$(CC) obj/mapping.o obj/mappings.o test/mappings.cpp \
		$(ARGS) -o test/mappings

test/molecule: obj/record.o obj/mapping.o obj/mappings.o obj/molecule.o
	$(CC) obj/record.o \
		obj/mapping.o \
		obj/mappings.o \
		obj/molecule.o \
		test/molecule.cpp $(ARGS) -o test/molecule

test/molecules: obj/record.o obj/mapping.o obj/mappings.o obj/molecule.o
	$(CC) obj/record.o \
		obj/mapping.o \
		obj/mappings.o \
		obj/molecule.o \
		obj/molecules.o \
		test/molecules.cpp $(ARGS) -o test/molecules

test/gene: obj/gene.o
	$(CC) obj/gene.o test/gene.cpp $(ARGS) -o test/gene

test/count: obj/gene.o obj/count.o
	$(CC) obj/gene.o obj/count.o test/count.cpp $(ARGS) -o test/count

test/counts: obj/gene.o obj/counts.o
	$(CC) obj/gene.o obj/counts.o test/counts.cpp $(ARGS) -o test/counts

test/gff: obj/gene.o
	$(CC) obj/gene.o test/gff.cpp $(ARGS) -o test/gff

test/counter: obj/gene.o obj/counts.o
	$(CC) obj/gene.o \
		obj/counts.o \
		obj/counter.o \
		test/counter.cpp $(ARGS) -o test/counter

###############################################################################
## SOME RULES #################################################################

utils: obj/utils.o test/utils
read_structure: obj/read_structure.o test/read_structure
read1: obj/read1.o test/read1
read2: obj/read2.o test/read2
transcript: obj/transcript.o test/transcript
record: obj/record.o test/record
mapping: obj/mapping.o test/mapping
mappings: obj/mappings.o test/mappings
molecule: obj/molecule.o test/molecule
molecules: obj/molecules.o test/molecules
gene: obj/gene.o test/gene
cnt: obj/count.o test/count
counts: obj/counts.o test/counts
gff: obj/gene.o test/gff
counter: obj/counter.o test/counter

###############################################################################
## PROGRAMS ###################################################################

extract_barcode: obj/utils.o
extract_barcode: obj/read_structure.o obj/read1.o obj/read2.o obj/transcript.o
	$(CC) obj/utils.o \
		obj/read_structure.o \
		obj/read1.o \
		obj/read2.o \
		obj/transcript.o \
		src/extract_barcode.cpp \
		$(ARGS) -o bin/extract_barcode

tag_bam:
	$(CC) src/tag_bam.cpp $(ARGS) -o bin/tag_bam

add_match: obj/utils.o
	$(CC) obj/utils.o src/add_match.cpp $(ARGS) -o bin/add_match

umis_per_barcode:
	$(CC) src/umis_per_barcode.cpp $(ARGS) -o bin/umis_per_barcode

select: obj/utils.o
select: obj/record.o obj/mapping.o obj/mappings.o obj/molecule.o obj/molecules.o
	$(CC) obj/utils.o \
		obj/record.o \
		obj/mapping.o \
		obj/mappings.o \
		obj/molecule.o \
		obj/molecules.o \
		src/select.cpp $(ARGS) -o bin/select

count_bin: obj/utils.o obj/gene.o obj/counts.o obj/counter.o
	$(CC) obj/utils.o \
		obj/gene.o \
		obj/counts.o \
		obj/counter.o \
		src/count.cpp \
		$(ARGS) -o bin/count

all_bin: extract_barcode tag_bam add_match umis_per_barcode select count_bin
	@echo "Done"

###############################################################################
## CLEANING ###################################################################

clean_obj:
	rm -fv obj/utils.o
	rm -fv obj/read_structure.o
	rm -fv obj/read1.o
	rm -fv obj/read2.o
	rm -fv obj/transcript.o
	rm -fv obj/record.o
	rm -fv obj/mapping.o
	rm -fv obj/mappings.o
	rm -fv obj/molecule.o
	rm -fv obj/molecules.o
	rm -fv obj/gene.o
	rm -fv obj/count.o
	rm -fv obj/counts.o
	rm -fv obj/counter.o

clean_test:
	rm -fv test/utils
	rm -fv test/read_structure
	rm -fv test/read1
	rm -fv test/read2
	rm -fv test/transcript
	rm -fv test/record
	rm -fv test/mapping
	rm -fv test/mappings
	rm -fv test/molecule
	rm -fv test/molecules
	rm -fv test/gene
	rm -fv test/count
	rm -fv test/counts
	rm -fv test/gff
	rm -fv test/counter

clean_tmp:
	rm -fv features.tsv barcodes.tsv matrix.mtx
	rm -fv test.bam

clean_bin:
	rm -fv bin/extract_barcode
	rm -fv bin/tag_bam
	rm -fv bin/add_match
	rm -fv bin/umis_per_barcode
	rm -fv bin/select
	rm -fv bin/count

