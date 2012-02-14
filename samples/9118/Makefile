SUBDIRS = 9118AboutTrg 9118DbfAboutTrg 9118DbfPostTrg 9118DbfPreTrg 9118PostTrg 9118burstdma 9118DbfFile 9118dio 9118file p9118ai p9118ao

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
