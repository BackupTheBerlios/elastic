#!/bin/sh

nm -g $1 | awk -- '{ if (($2 == "D") || ($2 == "B")) print; }'
