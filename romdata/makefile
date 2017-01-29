ASSETS=\
	GSLOT000.CHR\
	DSLOT000.DAT

all: ROMDATA.BIN

.SUFFIXES: .bmp .CHR
.bmp.CHR:
	vgs2bmp $< $@

.SUFFIXES: .wav .PCM
.wav.PCM:
	vgs2wav $< $@

.SUFFIXES: .mml .BGM
.mml.BGM:
	vgs2mml $< $@

ROMDATA.BIN: $(ASSETS)
	vgs2rom ./ ROMDATA.BIN

