EXTRA_DIST = LICENSE autogen.sh aclocal-flags \
	README.original ACKNOWLEDGEMENTS.original 		\
	ChangeLog.original NEW_FEATURES.original 		\
	NO_WARRANTY.original					\
	mac-byacc.r mpw-config.in mpw-make.sed			\
	byacc.texi						\
	manpage

#SUBDIRS = . sample-elastic

bin_PROGRAMS = byacc-ec

byacc_ec_SOURCES = \
	closure.c	\
	defs.h		\
	error.c		\
	lalr.c		\
	lr0.c		\
	main.c		\
	mkpar.c		\
	output.c	\
	reader.c	\
	skeleton.c	\
	symtab.c	\
	verbose.c	\
	warshall.c

dist-hook:
	mkdir -p $(distdir)/test
	cp -a $(srcdir)/test/* $(distdir)/test
	mkdir -p $(distdir)/sample-elastic
	cp -a $(srcdir)/sample-elastic/* $(distdir)/sample-elastic
#mkdir -p $(distdir)/build
#cp -a $(srcdir)/build/* $(distdir)/build

deb:
	dpkg-buildpackage -rfakeroot -b -uc
