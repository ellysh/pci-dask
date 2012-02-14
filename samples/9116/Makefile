SUBDIRS = 9116DbfDelayTrg 9116DbfFile 9116DbfPostTrg 9116DelayTrg 9116File 9116PostTrg 9116SoftTrg 9116MidTrg 9116dio p9116ai

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
