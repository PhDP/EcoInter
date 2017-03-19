TEX = pdflatex
BIB = bibtex
PYC = python
PDF = ms_netflix.pdf

all: $(PDF)

ms_netflix.pdf: ms_netflix.tex
	$(TEX) ms_netflix.tex && $(BIB) ms_netflix.aux && $(TEX) ms_netflix.tex && $(TEX) ms_netflix.tex

clean:
	rm *.aux *.log *.bbl *.out *.blg
