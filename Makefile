########################################################
# Project: 2 SHO
# File: Makefile
# Date: 22.11.2023
# Author: Martin Mores xmores02
#
# Description: Make script
########################################################

CC=g++
FILES=main.cpp
FILES_FOR_PACK=main.cpp IMS.pdf Makefile 
CFLAGS= -std=c++17 -Wall -Wextra
SIMLIB_PATH = -I simlib/src
PRJ=run

PACK_NAME=02_xmores02

.PHONY: all clean build pack clean_pack
all: build

pack: $(FILES_FOR_PACK) clean_pack
	zip $(PACK_NAME).zip $(FILES_FOR_PACK)

build: clean $(PRJ)

$(PRJ):
	$(CC) $(CFLAGS) $(SIMLIB_PATH) -o $@ $(FILES) -lsimlib -lm

clean:
	rm -f $(PRJ)

clean_pack:
	rm -f $(PACK_NAME).zip

