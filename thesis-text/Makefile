all: clean

create : 
	pdflatex -synctex=1 -interaction=nonstopmode "thesis".tex
	bibtex thesis.aux
	pdflatex -synctex=1 -interaction=nonstopmode "thesis".tex
	pdflatex -synctex=1 -interaction=nonstopmode "thesis".tex


clean: create
	rm -rf *.aux  *.nav  *.vrb  *.log *.lof *.lot  *.out *.snm *.toc *.gz *.mtc* *.maf *.fls *.tmp *.bbl *.blg  *.mtc  *.bcf *.xml *.fdb_latexmk 
