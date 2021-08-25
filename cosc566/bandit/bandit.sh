#!/bin/bash
if [ ! -z $2 ]; then
    ssh bandit$1@bandit.labs.overthewire.org -p 2220 -i $2
else
    ssh bandit$1@bandit.labs.overthewire.org -p 2220
fi