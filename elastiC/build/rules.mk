$(OBJDIR)%.ecc: %.ec .depend
	ELASTICPATH="$(ELASTICPATH):$(top_builddir)/src/modules-dynamic/re:$(top_builddir)/src/modules:$(top_builddir)/modules" $(ECC) -o $@ $<

%.html: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2html_ $<

%.txt: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2txt_ $<

%.tex: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2latex_sgml $<

%.info: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2info_sgml $<

%.man: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2man_pod $< >$@

%.sgml: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2sgml_ $<

%.pod: %.sdf
	$(SDF) $(SDFFLAGS) +sdf2pod_ $<

%.1: %.pod
	pod2man --section=1 --center="elastiC Reference Documentation" --date="28 Sep 2001" --release="Version $(VERSION)" $< >$@

%.3: %.pod
	pod2man --section=3 --center="elastiC Reference Documentation" --date="28 Sep 2001" --release="Version $(VERSION)" $< >$@

SUFFIXES = .ec .ecc .sdf .html .txt .tex .info .man .sgml .pod .1 .3
