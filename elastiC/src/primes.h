/* ==========================================================================
 * primes.h
 * ==========================================================================
 * Prime number table. Used for elastiC hash table expansion.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: Sun Nov 12 18:17:19 CET 2000 (Automagically generated)
 *
 *   $Id: primes.h,v 1.1 2002/05/23 21:17:33 panta Exp $
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
	         3,      /*          2 3.0000 */
	        11,      /*          8 3.6667 */
	        29,      /*         18 2.6364 */
	        79,      /*         50 2.7241 */
	       211,      /*        132 2.6709 */
	       541,      /*        330 2.5640 */
	      1361,      /*        820 2.5157 */
	      3319,      /*       1958 2.4386 */
	      7639,      /*       4320 2.3016 */
	     15887,      /*       8248 2.0797 */
	     29009,      /*      13122 1.8260 */
	     46687,      /*      17678 1.6094 */
	     68261,      /*      21574 1.4621 */
	     93871,      /*      25610 1.3752 */
	    124819,      /*      30948 1.3297 */
	    163433,      /*      38614 1.3094 */
	    212827,      /*      49394 1.3022 */
	    276763,      /*      63936 1.3004 */
	    359837,      /*      83074 1.3002 */
	    467813,      /*     107976 1.3001 */
	    608161,      /*     140348 1.3000 */
	    790613,      /*     182452 1.3000 */
	   1027799,      /*     237186 1.3000 */
	   1336141,      /*     308342 1.3000 */
	   1736989,      /*     400848 1.3000 */
	   2258111,      /*     521122 1.3000 */
	   2935547,      /*     677436 1.3000 */
	   3816233,      /*     880686 1.3000 */
	   4961107,      /*    1144874 1.3000 */
	   6449447,      /*    1488340 1.3000 */
	   8384287,      /*    1934840 1.3000 */
	  10899587,      /*    2515300 1.3000 */
	  14169509,      /*    3269922 1.3000 */
	  18420373,      /*    4250864 1.3000 */
	  23946491,      /*    5526118 1.3000 */
	  31130441,      /*    7183950 1.3000 */
	  40469599,      /*    9339158 1.3000 */
	  52610479,      /*   12140880 1.3000 */
	  68393623,      /*   15783144 1.3000 */
	  88911749,      /*   20518126 1.3000 */
	 115585291,      /*   26673542 1.3000 */
	 150260879,      /*   34675588 1.3000 */
	 195339173,      /*   45078294 1.3000 */
	 253940959,      /*   58601786 1.3000 */
	 330123259,      /*   76182300 1.3000 */
	 429160267,      /*   99037008 1.3000 */
	 557908357,      /*  128748090 1.3000 */
	 725280869,      /*  167372512 1.3000 */
	 942865201       /*  217584332 1.3000 */
};

static int nprimes = 49;

#endif /* __PRIMES_H */
