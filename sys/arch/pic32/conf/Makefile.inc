BOARD?=		PINGUINO_MICRO
KERNDIR=	${TOPSRC}/sys/arch/pic32/compile/${BOARD}
KERN=		unix

installflash:
	sudo pic32prog ${KERNDIR}/${KERN}.hex

.PHONY: installflash
