# Mumit Khan's howto, updated and turned into a shell script

# download (using wget), build and install gcc-2.95.2 as a
# cross-compiler for win32 using msvc.

# Allin Cottrell, January 2000.  MINIMALLY TESTED.

# local dirs -- pick these to suit
SRCDIR="/usr/src/cross-tools"
CROSSDIR="/usr/local/cross-tools"

# versions of stuff
GCC_VERSION="gcc-2.95.2"
LATEST="bin-msvcrt-1999-12-21.tar.gz"

# ftp sites
KHAN="ftp://ftp.xraylith.wisc.edu/pub/khan/gnu-win32/"
#CYGNUS="ftp://sourceware.cygnus.com/pub/cygwin/latest/src-by-top-dir"
# I can't get all files with wget from the cygnus site (why not?).
# Use Norwegian mirror instead
CYGNUS="ftp://ftp.ntnu.no/pub/cygwin/cygwin-b20/src-by-top-dir"
GCCPAK="ftp://ftp.gnu.org/pub/gnu/gcc/${GCC_VERSION}.tar.gz"
PATCHSITE="${KHAN}/cygb20/${GCC_VERSION}/patches"
GCCPATCH="${PATCHSITE}/${GCC_VERSION}-x86-win32-patches.tar.gz"

function get_out {
	echo "Failed to get required file from ftp site"
	exit 1
}	

# make required dirs
mkdir -p ${SRCDIR}
mkdir -p ${CROSSDIR}

# get the various packages
cd /tmp
if ! wget ${KHAN}/mingw32/runtime/${LATEST} ; then get_out ; fi
cd ${CROSSDIR}
tar zxvf /tmp/${LATEST}

# this will create a directory tree starting with i386-mingw32msvc

# 2. GNU Binutils: This is tricky. Currently, if you don't have Cygwin b20.1
#    source tree on your disk, you'll need to get the broken up pieces from
#    Cygwin ftp server.

#   ftp site: ftp://sourceware.cygnus.com/pub/cygwin/latest/src-by-top-dir

#   and get the following files:

mkdir ${SRCDIR}/binutils
cd ${SRCDIR}/binutils
cd /tmp
for f in bfd.tar.bz2 gas.tar.bz2 ld.tar.bz2 toplevel.tar.bz2 \
   binutils.tar.bz2 gprof.tar.bz2 libiberty.tar.bz2 \
   config.tar.bz2 include.tar.bz2 opcodes.tar.bz2 \
   etc.tar.bz2 intl.tar.bz2 texinfo.tar.bz2
do
   if ! wget ${CYGNUS}/$f ; then get_out ; fi
   cd ${SRCDIR}/binutils && tar xvfy /tmp/$f
   cd /tmp
done

# 3. GCC: Download sources and unpack
cd /tmp
if ! wget ${GCCPAK} ; then get_out ; fi
cd ${SRCDIR}
tar xvfz /tmp/${GCC_VERSION}.tar.gz

#   Now patch time. 
if ! wget ${GCCPATCH} ; then get_out ; fi
tar xvfz ${GCC_VERSION}-x86-win32-patches.tar.gz ${GCC_VERSION}-x86-win32-diff
mv ${GCC_VERSION}-x86-win32-diff /tmp
cd ${GCC_VERSION}
patch -p1 -s < /tmp/${GCC_VERSION}-x86-win32-diff

#============ Building CROSS Mingw32 tools

#Step 1: build the entire binary utilities and install before you start with
#  gcc

cd ${SRCDIR}/binutils

# Do we need to apply the patch binutils-2.9.4.diff
# before building the binutils??  Let's suppose so.
if ! wget ${KHAN}mingw32/gcc-2.95.2/patches/binutils-2.9.4.diff 
  then get_out 
fi
patch -p1 -s < binutils-2.9.4.diff

mkdir CROSS
cd CROSS
../src/configure --prefix=${CROSSDIR} --target=i386-mingw32msvc -v
make > make.log 2>&1
make install > install.log 2>&1

#After you're done, add it to path.
  
export PATH=$PATH:${CROSSDIR}/bin

# All the binaries for i386-mingw32 target are prefixed with 
# i386-mingw32msvc

#The next step, building gcc compiler itself, requires that you have
#the cross binutils in your PATH.

#============ Building CROSS gcc-2.95.2 for Mingw32

#Now configure, build and install gcc.

cd ${SRCDIR}/${GCC_VERSION}
mkdir CROSS
cd CROSS
../configure --prefix=${CROSSDIR} --target=i386-mingw32msvc -v
cd gcc ; make installdirs ; cd ..
make > make.log 2>&1
make install > install.log 2>&1


