#!/bin/bash
head -n 4 logo.ppm > header.txt
tail -n +5  logo.ppm > body.bin
openssl enc -aes-128-ecb -nosalt -pass pass:"FNORD" -in body.bin -out body.ecb.bin
openssl enc -aes-128-cbc -nosalt -pass pass:"FNORD" -in body.bin -out body.cbc.bin
cat header.txt body.cbc.bin > logo.cbc.ppm
cat header.txt body.cbc.bin > logo.cbc.ppm
