SUBDIRS = C9222_AI_DBF C9222_AI_DMA C9222_AI_DMA_ExtConvSrc C9222_AI_DMA_GatedTrigger C9222_AI_DMA_InfiniteReTrigger C9222_AI_DMA_ReTrigger C9222_AI_Polling C9222_AO_DBF C9222_AO_DMA C9222_AO_DMA_ExtConvSrc C9222_AO_DMA_InfiniteReTrigger C9222_AO_DMA_ReTrigger C9222_AO_DMA_SimuWrites C9222_AO_Polling C9222_Calibration C9222_DI_DBF C9222_DI_DMA C9222_DI_DMA_InfiniteReTrigger C9222_DI_DMA_ReTrigger C9222_DIO_Polling C9222_DO_DMA C9222_DO_DMA_InfiniteReTrigger C9222_DO_DMA_ReTrigger C9222_Encoder C9222_GPTC

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
