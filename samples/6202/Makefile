SUBDIRS = 6202_ao 6202_ao_dma 6202_ao_dma_dbf 6202_dio 6202_Encoder 6202_GPTC 6202_PWM

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
