package bug;

import basic;

public class Hash extends basic.Object {
	/*
	 * Hash table implementation
	 *
	 * There is a list of buckets. Every bucket has a list of
	 * pairs (key, value) with the same hash value.
	 */
	local bucket;
	local nbuckets;

	method init( ) {
		nbuckets = 530;											// BUG at >= 530
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
		if (hv >= basic.length( bucket ))
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
}

private ht = [Hash new];

ht["Hello"]  = "World";
ht["Larry"]  = "Wall";
ht["Dennis"] = "Ritchie";

private k, ks;
ks = [ht _keys];
for (k in ks) {
	basic.print( k, " -> ", ht[k], '\n' );
}
