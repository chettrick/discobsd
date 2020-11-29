# To generate report automatically, run:
# $ ls *.ms | entr -s make
#

REPORT=	Porting_the_Unix_Kernel-CSC490-Christopher_Hettrick

SRCS=	${REPORT}.ms

${REPORT}.pdf: ${REPORT}.ms
	pdfroff -mspdf ${REPORT}.ms > $@

clean:
	rm ${REPORT}.pdf

.PHONY: clean
