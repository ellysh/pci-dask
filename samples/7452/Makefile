SUBDIRS = 7452dio 7452Int 7452DbEvt WdtOvflow

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
