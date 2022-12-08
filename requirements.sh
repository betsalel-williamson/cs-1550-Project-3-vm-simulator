#!/usr/bin/env sh

# This is all mac based
# Tested on Mac OS 13.0.1 (22A400)

trap "kill 0" SIGINT

usage_string="Usage: ./requirements.sh

Description: Tests that requirements are all installed"

set -e

### Start check requirements

program="cmake"
have=`${program} --version | cut -d ' ' -f 2 | head -n1`
want="3.25.1"

if ! [  "${want}" = "`echo "${have}\n${want}" | sort -V | head -n1`" ];
then
    echo "Install ${program} version ${want} or greater"
    exit 1
fi

program="xcrun"
have=`${program} --version | cut -d ' ' -f 3 | head -n1`
want="61"

if ! [  "${want}" = "`echo "${have}\n${want}" | sort -V | head -n1`" ];
then
    echo "Install ${program} version ${want} or greater"
    echo "Run: xcode-select --install"
    exit 1
fi

### End check requirements