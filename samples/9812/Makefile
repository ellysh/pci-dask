SUBDIRS = 9812DbfDelayTrg 9812DbfMidTrg 9812DbfPostTrg 9812DbfPreTrg 9812DelayTrg 9812PostTrg 9812PreTrg 9812SoftTrg 9812MidTrg 9812DbfFile 9812File

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
