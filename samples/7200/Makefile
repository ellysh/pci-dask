SUBDIRS = 7200File 7200DbfFile 7200Wav 7200DbfWav 7200HdSk 7200ETrig

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
