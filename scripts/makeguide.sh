#!/bin/bash

cd ../../cysec/labguide
pandoc *.md -o pack/labguide.pdf --from markdown --template resources/eisvogel.tex  -V lang=de --listings
#pdf-engine xelatex