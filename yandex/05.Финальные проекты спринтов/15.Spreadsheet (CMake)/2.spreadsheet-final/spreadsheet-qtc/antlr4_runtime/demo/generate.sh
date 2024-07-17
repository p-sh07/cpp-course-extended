#!/bin/bash
set -o errexit

# Created 2016, Mike Lischke (public domain)

# 1) Running from jar. Use the given jar (or replace it by another one you built or downloaded) for generation.
LOCATION=antlr-4.13.1-complete.jar

java -jar $LOCATION -Dlanguage=Cpp -listener -visitor -o generated/ Formula.g4
