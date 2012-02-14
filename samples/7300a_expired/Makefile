SUBDIRS=7300aFile 7300aWave 7300aPgWave 7300aDbEvt 7300aInt 7300aMWav

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
