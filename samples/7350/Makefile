SUBDIRS = C7350_COS C7350_DI_DMA C7350_DI_DMA_ExtClk C7350_DI_DMA_ExtInfTrig C7350_DI_DMA_ExtReTrig C7350_DI_DMA_ExtTrig C7350_DI_DMA_File C7350_DI_DMA_SoftTrig C7350_DI_MBF_Overrun C7350_DIO_BurstHandShake C7350_DIO_HandShake C7350_DIO_POLL C7350_DO_DMA C7350_DO_DMA_ExtClk C7350_DO_DMA_ExtReTrig C7350_DO_DMA_ExtTrig C7350_DO_DMA_SoftTrig C7350_DO_MBF C7350_I2C C7350_PM_Edge C7350_PM_Level C7350_SPI

all:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE); cd ..; done

clean:
	for i in $(SUBDIRS); do cd "$$i"; $(MAKE) clean; cd ..; done
