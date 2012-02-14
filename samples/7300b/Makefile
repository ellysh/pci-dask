SUBDIRS = 7300bFile 7300bWave 7300bPgWave 7300bDbEvt 7300bDbfDO 7300bInt 7300bMWav DIDmaEvt 7300bDbfDI

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
