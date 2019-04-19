#!/bin/bash

python ../../cysec/scripts/merge-attack-layers.py
cp ../../cysec/labguide/attack/cysec.json ../attack
git add ../attack/cysec.json
git commit -m "Update attack mapping of cysec course"
