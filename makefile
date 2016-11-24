################################
#    Makefile for Smile OS     #
################################

# The general compiling commands #
CC = gcc

TGT = tgt/*

TARGET = ImageAnalyser.bin Bitmap.bin

.PHONY : build

build : clean $(TARGET)

clean :
	 rm -f $(TARGET) $(TGT)

rmfiles :
	 rm -f $(TGT)

#---------------------------------------------------------------#
ImageAnalyser.bin : src/imageAnalyser.c src/imageAnalyser.h
	$(CC) $< -o $@

Bitmap.bin : src/bitmap.c src/bitmap.h
	$(CC) $< -o $@
