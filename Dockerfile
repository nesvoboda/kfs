from ubuntu:latest

RUN apt-get update

RUN apt-get install -y build-essential bison flex libgmp3-dev libmpc-dev \
libmpfr-dev texinfo libisl-dev binutils

RUN apt-get install -y git

RUN mkdir src

RUN cd src

RUN git clone --depth 1 git://sourceware.org/git/binutils-gdb.git

RUN git clone --depth 1 git://gcc.gnu.org/git/gcc.git gcc

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

/binutils-gdb/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror