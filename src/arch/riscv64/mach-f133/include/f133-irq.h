#ifndef __F133_IRQ_H__
#define __F133_IRQ_H__

#ifdef __cplusplus
extern "C" {
#endif

#define F133_IRQ_UART0				(18)
#define F133_IRQ_UART1				(19)
#define F133_IRQ_UART2				(20)
#define F133_IRQ_UART3				(21)
#define F133_IRQ_UART4				(22)
#define F133_IRQ_UART5				(23)
#define F133_IRQ_TWI0				(25)
#define F133_IRQ_TWI1				(26)
#define F133_IRQ_TWI2				(27)
#define F133_IRQ_TWI3				(28)
#define F133_IRQ_SPI0				(31)
#define F133_IRQ_SPI1				(32)
#define F133_IRQ_PWM				(34)
#define F133_IRQ_IR_TX				(35)
#define F133_IRQ_LEDC				(36)
#define F133_IRQ_OWA				(39)
#define F133_IRQ_DMIC				(40)
#define F133_IRQ_AUDIO_CODEC		(41)
#define F133_IRQ_I2S0				(42)
#define F133_IRQ_I2S1				(43)
#define F133_IRQ_I2S2				(44)
#define F133_IRQ_USB0_DEVICE		(45)
#define F133_IRQ_USB0_EHCI			(46)
#define F133_IRQ_USB0_OHCI			(47)
#define F133_IRQ_USB1_EHCI			(49)
#define F133_IRQ_USB1_OHCI			(50)
#define F133_IRQ_SMHC0				(56)
#define F133_IRQ_SMHC1				(57)
#define F133_IRQ_SMHC2				(58)
#define F133_IRQ_MSI				(59)
#define F133_IRQ_EMAC				(62)
#define F133_IRQ_ECCU_FERR			(64)
#define F133_IRQ_AHB_TIMEOUT		(65)
#define F133_IRQ_DMAC_NS			(66)
#define F133_IRQ_CE_NS				(68)
#define F133_IRQ_SPINLOCK			(70)
#define F133_IRQ_HSTIME0			(71)
#define F133_IRQ_HSTIME1			(72)
#define F133_IRQ_GPADC				(73)
#define F133_IRQ_THS				(74)
#define F133_IRQ_TIMER0				(75)
#define F133_IRQ_TIMER1				(76)
#define F133_IRQ_LRADC				(77)
#define F133_IRQ_TPADC				(78)
#define F133_IRQ_WATCHDOG			(79)
#define F133_IRQ_IOMMU				(80)
#define F133_IRQ_VE					(82)
#define F133_IRQ_GPIOB_NS			(85)
#define F133_IRQ_GPIOC_NS			(87)
#define F133_IRQ_GPIOD_NS			(89)
#define F133_IRQ_GPIOE_NS			(91)
#define F133_IRQ_GPIOF_NS			(93)
#define F133_IRQ_GPIOG_NS			(95)
#define F133_IRQ_DE					(103)
#define F133_IRQ_DI					(104)
#define F133_IRQ_G2D				(105)
#define F133_IRQ_LCD				(106)
#define F133_IRQ_TV					(107)
#define F133_IRQ_DSI				(108)
#define F133_IRQ_HDMI				(109)
#define F133_IRQ_TVE				(110)
#define F133_IRQ_CSI_DMA0			(111)
#define F133_IRQ_CSI_DMA1			(112)
#define F133_IRQ_CSI_PARSER0		(116)
#define F133_IRQ_CSI_TOP_PKT		(122)
#define F133_IRQ_TVD				(123)
#define F133_IRQ_DSP_DFE			(136)
#define F133_IRQ_DSP_PFE			(137)
#define F133_IRQ_DSP_WDG			(138)
#define F133_IRQ_DSP_MBOX_RISCV_W	(140)
#define F133_IRQ_DSP_TZMA			(141)
#define F133_IRQ_DMAC_IRQ_DSP_NS	(142)
#define F133_IRQ_RISCV_MBOX_RISCV	(144)
#define F133_IRQ_RISCV_MBOX_DSP		(145)
#define F133_IRQ_RISCV_WDG			(147)
#define F133_IRQ_IRRX				(167)
#define F133_IRQ_C0_CTI0			(176)
#define F133_IRQ_C0_CTI1			(177)
#define F133_IRQ_C0_COMMTX0			(180)
#define F133_IRQ_C0_COMMTX1			(181)
#define F133_IRQ_C0_COMMRX0			(184)
#define F133_IRQ_C0_COMMRX1			(185)
#define F133_IRQ_C0_PMU0			(188)
#define F133_IRQ_C0_PMU1			(189)
#define F133_IRQ_C0_AXI_ERROR		(192)
#define F133_IRQ_AXI_WR_IRQ			(194)
#define F133_IRQ_AXI_RD_IRQ			(195)
#define F133_IRQ_DBGWRUPREQ_OUT0	(196)
#define F133_IRQ_DBGWRUPREQ_OUT1	(197)

#define F133_IRQ_GPIOB2				(256 + 2)
#define F133_IRQ_GPIOB3				(256 + 3)
#define F133_IRQ_GPIOB4				(256 + 4)
#define F133_IRQ_GPIOB5				(256 + 5)
#define F133_IRQ_GPIOB6				(256 + 6)
#define F133_IRQ_GPIOB7				(256 + 7)

#define F133_IRQ_GPIOC2				(288 + 2)
#define F133_IRQ_GPIOC3				(288 + 3)
#define F133_IRQ_GPIOC4				(288 + 4)
#define F133_IRQ_GPIOC5				(288 + 5)
#define F133_IRQ_GPIOC6				(288 + 6)
#define F133_IRQ_GPIOC7				(288 + 7)

#define F133_IRQ_GPIOD0				(320 + 0)
#define F133_IRQ_GPIOD1				(320 + 1)
#define F133_IRQ_GPIOD2				(320 + 2)
#define F133_IRQ_GPIOD3				(320 + 3)
#define F133_IRQ_GPIOD4				(320 + 4)
#define F133_IRQ_GPIOD5				(320 + 5)
#define F133_IRQ_GPIOD6				(320 + 6)
#define F133_IRQ_GPIOD7				(320 + 7)
#define F133_IRQ_GPIOD8				(320 + 8)
#define F133_IRQ_GPIOD9				(320 + 9)
#define F133_IRQ_GPIOD10			(320 + 10)
#define F133_IRQ_GPIOD11			(320 + 11)
#define F133_IRQ_GPIOD12			(320 + 12)
#define F133_IRQ_GPIOD13			(320 + 13)
#define F133_IRQ_GPIOD14			(320 + 14)
#define F133_IRQ_GPIOD15			(320 + 15)
#define F133_IRQ_GPIOD16			(320 + 16)
#define F133_IRQ_GPIOD17			(320 + 17)
#define F133_IRQ_GPIOD18			(320 + 18)
#define F133_IRQ_GPIOD19			(320 + 19)
#define F133_IRQ_GPIOD20			(320 + 20)
#define F133_IRQ_GPIOD21			(320 + 21)
#define F133_IRQ_GPIOD22			(320 + 22)

#define F133_IRQ_GPIOE0				(352 + 0)
#define F133_IRQ_GPIOE1				(352 + 1)
#define F133_IRQ_GPIOE2				(352 + 2)
#define F133_IRQ_GPIOE3				(352 + 3)
#define F133_IRQ_GPIOE4				(352 + 4)
#define F133_IRQ_GPIOE5				(352 + 5)
#define F133_IRQ_GPIOE6				(352 + 6)
#define F133_IRQ_GPIOE7				(352 + 7)
#define F133_IRQ_GPIOE8				(352 + 8)
#define F133_IRQ_GPIOE9				(352 + 9)
#define F133_IRQ_GPIOE10			(352 + 10)
#define F133_IRQ_GPIOE11			(352 + 11)
#define F133_IRQ_GPIOE12			(352 + 12)
#define F133_IRQ_GPIOE13			(352 + 13)

#define F133_IRQ_GPIOF0				(384 + 0)
#define F133_IRQ_GPIOF1				(384 + 1)
#define F133_IRQ_GPIOF2				(384 + 2)
#define F133_IRQ_GPIOF3				(384 + 3)
#define F133_IRQ_GPIOF4				(384 + 4)
#define F133_IRQ_GPIOF5				(384 + 5)
#define F133_IRQ_GPIOF6				(384 + 6)

#define F133_IRQ_GPIOG0				(416 + 0)
#define F133_IRQ_GPIOG1				(416 + 1)
#define F133_IRQ_GPIOG2				(416 + 2)
#define F133_IRQ_GPIOG3				(416 + 3)
#define F133_IRQ_GPIOG4				(416 + 4)
#define F133_IRQ_GPIOG5				(416 + 5)
#define F133_IRQ_GPIOG6				(416 + 6)
#define F133_IRQ_GPIOG7				(416 + 7)
#define F133_IRQ_GPIOG8				(416 + 8)
#define F133_IRQ_GPIOG9				(416 + 9)
#define F133_IRQ_GPIOG10			(416 + 10)
#define F133_IRQ_GPIOG11			(416 + 11)
#define F133_IRQ_GPIOG12			(416 + 12)
#define F133_IRQ_GPIOG13			(416 + 13)
#define F133_IRQ_GPIOG14			(416 + 14)
#define F133_IRQ_GPIOG15			(416 + 15)

#ifdef __cplusplus
}
#endif

#endif /* __F133_IRQ_H__ */
