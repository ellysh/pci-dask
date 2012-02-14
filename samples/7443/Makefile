SUBDIRS = C7443DbInt C7443TTL_Line C7443TTL_Port C7443di

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
