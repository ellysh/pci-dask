SUBDIRS = 9111dbf 9111dio 9111file 9111sptg a9111file p9111ai p9111ao a9111postTrg

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
