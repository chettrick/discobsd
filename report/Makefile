# To generate report automatically, run:
# $ ls *.ms | entr -s make
#

REPORT=	Porting_the_Unix_Kernel-CSC490-Christopher_Hettrick
BIB=	bib.refer

SRCS=	${REPORT}.ms

${REPORT}.pdf: ${REPORT}.ms
	refer -p ${BIB} ${REPORT}.ms |\
	pdfroff -mspdf - |\
	pdftk - cat 9 6 7 8 10-end output $@

clean:
	rm ${REPORT}.pdf

.PHONY: clean
