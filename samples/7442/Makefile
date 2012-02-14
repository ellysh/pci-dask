SUBDIRS = C7442DbInt C7442TTL_Line C7442TTL_Port C7442dio CWdtOvflow

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
