#SUBDIR+=	000_blinky
SUBDIR+=	001_reg

all: targets

targets:
	@echo "Targets are: build, clean" 1>&2

build:
	@for dir in ${SUBDIR}; do \
		${MAKE} -C $${dir} build; \
	done

clean:
	@for dir in ${SUBDIR}; do \
		${MAKE} -C $${dir} clean; \
	done

.PHONY: all build clean targets
