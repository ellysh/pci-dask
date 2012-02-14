SUBDIRS = C9223_AI_DBF C9223_AI_DMA C9223_AI_DMA_ExtConvSrc C9223_AI_DMA_GatedTrigger C9223_AI_DMA_InfiniteReTrigger C9223_AI_DMA_ReTrigger C9223_AI_Polling C9223_AO_DBF C9223_AO_DMA C9223_AO_DMA_ExtConvSrc C9223_AO_DMA_InfiniteReTrigger C9223_AO_DMA_ReTrigger C9223_AO_DMA_SimuWrites C9223_AO_Polling C9223_Calibration C9223_DI_DBF C9223_DI_DMA C9223_DI_DMA_InfiniteReTrigger C9223_DI_DMA_ReTrigger C9223_DIO_Polling C9223_DO_DMA C9223_DO_DMA_InfiniteReTrigger C9223_DO_DMA_ReTrigger C9223_Encoder C9223_GPTC

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
