#SUBDIR+=	000_blinky
SUBDIR+=	001_reg
SUBDIR+=	002_led
SUBDIR+=	003_cmsis

all: targets

targets:
	@echo "Targets are:" 1>&2
	@echo "    build, clean" 1>&2

build:
	@for dir in ${SUBDIR}; do \
		${MAKE} -C $${dir} build; \
	done

clean:
	@for dir in ${SUBDIR}; do \
		${MAKE} -C $${dir} clean; \
	done

.PHONY: all build clean targets
