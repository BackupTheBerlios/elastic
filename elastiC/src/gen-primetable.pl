#!/usr/bin/perl -w
# ==========================================================================
# gen-primetable.pl
# ==========================================================================
# Prime table generator for elastiC hash tables.
# --------------------------------------------------------------------------
#   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
#
#   Created: 12 Nov 2000
#
#   $Id: gen-primetable.pl,v 1.1 2002/05/23 21:17:06 panta Exp $
# --------------------------------------------------------------------------
#    Copyright (C) 2000 Marco Pantaleoni. All rights reserved.
#
#  The contents of this file are subject to the elastiC License version 1.0
#  (the "elastiC License"); you may not use this file except in compliance
#  with the elastiC License. You may obtain a copy of the elastiC License at
#  http://www.elasticworld.org/LICENSE
#
#  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
#  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
#  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
#  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
#  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
#  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
#  MODIFICATIONS.
#
#  See the elastiC License for the specific language governing rights and
#  limitations under the elastiC License.
# ==========================================================================

#
# This program generates a table of primes suitable for elastiC hash
# table expansion.
# Primes are located approximately along a non linear function whose
# purpose is to space primes in a variable fashion (first primes are
# more spaced, say each one is 2.5 times the previous one, and big
# primes are less spaced, so to save space in hash table expansion)
#

use strict;
use Carp;

my $emit_header = 1;

# Approx. range for the prime table
# (10^9 should be a safe bet for the next 2 years at least, I hope :-)
my ($lo, $hi) = (0, 1000000000);
#my ($lo, $hi) = (0, 100000);

# In the stream (P[n]) of accepted primes, the condition
# P[i] / P[i-1] >= gf(P[i]) is required to hold.
#
# gf(n) is a function of the form:
#
#
# gf(x) = beta + eta * exp(gfl(x))
#
# gfl(x) = delta * x + q
#
#         gfl(p2) - gfl(p1)           p2 * gfl(p1) - p1 * gfl(p2)
# delta = -----------------   ;   q = ---------------------------
#              p2 - p1                          p2 - p1
#
# gf(0) = beta + eta * exp(q)
#
# gf(1) = beta + eta * exp(delta + q)
#

my ($beta, $eta)    = (1.3, 1.0 / 16.0);
my ($p1,    $p2)    = (1.0, 60000.0);
my ($gf_p1, $gf_p2) = (3.0, 1.2);

# External program to generate primes
my ($primestream) = ("primes");

# ========================================================================

my ($nprimes) = (0);
my ($date);

$date = `date`;
chomp($date);

my ($delta, $q);
my ($gf, $gfl);

$delta = ($gf_p2 - $gf_p1) / ($p2 - $p1);
$q     = ($p2 * $gf_p1 - $p1 * $gf_p2) / ($p2 - $p1);

#print "DELTA: $delta   Q: $q\n";

my ($prevprime, $prime);
my ($prevdiff, $prevratio);

$prevprime = 1;

if ($lo < 3) {
    $lo = 3;
}

select(STDOUT); $| = 1;
open(PS, "$primestream $lo $hi |") || die "can't run '$primestream' command: $!";
if ($emit_header) {
    emitHead();
}
my $firstprint = 1;
my $comment;

NEXTPRIME: while (<PS>) {
    chomp;
    $prime = $_;
    goto NEXTPRIME if ($prime < $lo);
    last if ($prime > $hi);

    $gfl = $delta * $prime + $q;
    $gf  = $beta + $eta * exp($gfl);

    goto NEXTPRIME if ($prime / $prevprime < $gf);

    $prevdiff  = $prime - $prevprime;
    $prevratio = $prime / $prevprime;

    #print "$gf  $prime\n";
    if ($emit_header) {
	if (not $firstprint) {
	    print sprintf(",      %s", $comment);
	    print "\n";
	}
	print sprintf("\t%10s", $prime);
    } else {
	print "$prime\n";
    }
    $comment = sprintf("/* %10s %1.4f */", $prevdiff, $prevratio);
    $nprimes++;

    $firstprint = 0;
    $prevprime = $prime;
}
print sprintf("       %s", $comment);
close(PS);
if ($emit_header) {
    emitTail();
}

sub emitHead {
    print <<"UntilTheEndOfTheString";
/* ==========================================================================
 * primes.h
 * ==========================================================================
 * Prime number table. Used for elastiC hash table expansion.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta\@elasticworld.org
 *
 *   Created: $date (Automagically generated)
 *
 *   \$Id\$
 * --------------------------------------------------------------------------
 *    Copyright (C) 2000 Marco Pantaleoni. All rights reserved.
 *
 *  The contents of this file are subject to the elastiC License version 1.0
 *  (the "elastiC License"); you may not use this file except in compliance
 *  with the elastiC License. You may obtain a copy of the elastiC License at
 *  http://www.elasticworld.org/LICENSE
 *
 *  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 *  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 *  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 *  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 *  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
 *  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 *  MODIFICATIONS.
 *
 *  See the elastiC License for the specific language governing rights and
 *  limitations under the elastiC License.
 * ==========================================================================
 */

#ifndef __PRIMES_H
#define __PRIMES_H

static const long primes[] = {
UntilTheEndOfTheString
}

sub emitTail {
    print <<"UntilTheEndOfTheString";

};

static int nprimes = $nprimes;

#endif /* __PRIMES_H */
UntilTheEndOfTheString
}
