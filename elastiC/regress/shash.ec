/*
 * shash.ec
 *
 * Simple Hash
 */

package shash;

import basic;

public class Hash extends basic.Object {
	/*
	 * Hash table implementation
	 *
	 * There is a list of buckets. Every bucket has a list of
	 * pairs (key, value) with the same hash value.
	 */
	local bucket;
	static nbuckets = 1039;

	method init( ) {
		bucket = basic.array( nbuckets );
	}

	method _getitem( key ) {
		local hv;
		local pair;

		hv = basic.hash( key ) % nbuckets;
		if (hv < basic.length( bucket ))
		{
			for (pair in bucket[hv])
				if (pair[0] == key) return pair[1];
		}

		throw [basic.IndexError new "key " + basic.repr( key ) + " not found", key, self];
	}

	method _setitem( key, val )	{
		local hv;
		local pair;

		hv = basic.hash( key ) % nbuckets;
		if ((hv >= basic.length( bucket )) ||
			(basic.typeof( bucket[hv] ) != #array))
		{
			bucket[hv] = #[ #[ key, val ] ];
			return self;
		}

		for (pair in bucket[hv])
		{
			for (pair in bucket[hv])
				if (pair[0] == key) {
					pair[1] = val;
					return self;
				}

			local l = basic.length( bucket[hv] );
			bucket[hv][l] = #[ key, val ];
			return self;
		}
	}

	method _keys( ) {
		local keys = #[ ];

		local b, pair;
		for (b in bucket)
		{
			for (pair in b) keys[basic.length(keys)] = pair[0];
		}

		return keys;
	}

	method _values( ) {
		local values = #[ ];

		local b, pair;
		for (b in bucket)
		{
			for (pair in b) values[basic.length(values)] = pair[1];
		}

		return values;
	}
}

private ht = [Hash new];

ht["Hello"]  = "World";
ht["Larry"]  = "Wall";
ht[12.6]     = #[ 8, "Goofy", 4.4 ];
ht[16]       = 'o';
ht["Dennis"] = "Ritchie";

basic.print( ht["Larry"], '\n' );
try {
	basic.print( ht["Dennis"], '\n' );
	basic.print( ht["Lorry"], '\n' );
} catch (basic.IndexError e) {
	basic.print( "Exception: ", [e getFullMessage], '\n' );
	basic.print( "    index: ", [e getIndex], '\n' );
}
basic.print( ht["Hello"], '\n' );

private k;
for (k in [ht _keys]) {
	basic.print( k, " -> ", ht[k], '\n' );
}
