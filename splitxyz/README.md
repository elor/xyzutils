# splitxyz
## Description

splitxyz is a program to pick from and split xyz files and optionally write the output to one or more files.

## Installation

    ./configure
    make
    make install

## Options

* -b, --begin: First step to extract
* -e, --end: Last step to extract
* -s, --step: Step size
* -o, --out: output file name. See examples below

## Input sources

Input files are read from the command line. If none are specified, input is written from STDIN.

Please note that the input xyz files aren't validated. Only empty last lines are stripped from the xyz file, but otherwise they're just echoed.

## Examples

Split a single file into multiple files named out001.xyz, out002.xyz and so on.

    splitxyz -o out###.xyz in.xyz

Extract everything after the 100th step

    splitxyz -b 101 in.xyz

Pick only the 53th step from a file and output it to the command line

    splitxyz -b 53 -e 53 in.xyz

Write every 10th step into a file

    splitxyz -s 10 -o out.xyz in.xyz

Write every 1th step from a set of files, each containing e.g. 3 steps, with a little help from shell filename completion

    splitxyz -s 10 -o out.xyz in*.xyz

Reading via pipe

    cat in.xyz | splitxyz -o out###.xyz

Yes, we can!

    cat in.xyz | splitxyz -s 4 | splitxyz -b 8 -s 15 | splitxyz -s 16 | splitxyz -b 23 -e 42 -o out##.xyz
