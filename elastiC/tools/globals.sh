#!/bin/sh

nm --dynamic $1 | awk -- '{ if (($1 != "U") && (toupper($2) == $2)) { print $3; } }' | sort
