EC  = ec
ECC = ecc

ECFLAGS  =
ECCFLAGS =

%.ecc: %.ec
	$(ECC) $(ECCFLAGS) $<

%.dvi: %.tex
	latex $<

all: IDLModel.ecc api-specification-proposal.dvi

IDLModel.ecc: IDLModel.ec util.ecc


clean:
	@rm -f *.ecc *.dvi *.aux *.log

