SUBDIRS = ai_polling ao_polling dio_line dio_port calibration ai_dma_dbf ai_dma_extd ai_dma_scans gptc

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
