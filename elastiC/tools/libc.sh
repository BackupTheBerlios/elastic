#!/bin/sh

nm --dynamic $1 | awk -- '{ if ($1 == "U") { gsub(/@@(.+)/,"",$2 ); print $2; } }' | sort
