#!/bin/bash

python ../../cysec/scripts/merge-attack-layers.py
cp ../../cysec/labguide/attack/*.json ../attack
git add ../attack/*.json
git commit -m "Sync attack mapping of cysec course"
