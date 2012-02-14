SUBDIRS = C7444TTL_Line C7444TTL_Port C7444do CWdtOvflow

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
