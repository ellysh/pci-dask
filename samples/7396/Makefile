SUBDIRS = 7396dio 7396Int1 7396Int2 7396COSi 7396DbEvt

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
