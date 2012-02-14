SUBDIRS = AI_Polling AI_ScanChannels AO_PollUpdate AO_SimuUpdate DIO_Polling Calibration AI_ContDma AI_ContDmaExtD AI_ContScans AI_ContScansExtD AI_ContDbfDma CTR_PulseGenration CTR_QuadratureDecoder

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
