package Re;

import basic;
import string;
import array;
import re;

public class Match extends basic.Object
{
	local d_match;
	local d_len;
	local d_re;
	local d_string;

	method init( match_data, rx, str )
	{
		d_match  = match_data;
		d_len    = re.number( d_match );
		d_re     = rx;
		d_string = str;
	}

	method number()
	{
		return d_len;
	}

	method matched()
	{
		return (d_len > 0);
	}

	method group( group )
	{
		return re.group( d_match, group );
	}

	method range( group )
	{
		return re.range( d_match, group );
	}

	method regexp()
	{
		return d_re;
	}

	method string()
	{
		return d_string;
	}

	method _getitem( key )
	{
		return re.group( d_match, key );
	}

	method _keys( )
	{
		local keys = #[];
		local i;

		for (i = 0; i < d_len + 1; i++)
			array.push( keys, i );
		return keys;
	}

	method _values( )
	{
		local values = #[];
		local i;

		for (i = 0; i < d_len + 1; i++)
			array.push( values, re.group( d_match, i ) );
		return values;
	}
}

public class Regexp extends basic.Object
{
	local d_regexp;
	local d_pattern;
	local d_flags;

	// Constructors

	method init( pattern, flags = @nil, study = @false )
	{
		d_pattern = pattern;
		d_flags   = flags;
		if (study != @nil)
			d_regexp = re.compile(pattern, flags, study);
		else
			d_regexp = re.compile(pattern, flags);
	}

	class method init: pattern
	{
		return [self new pattern, @nil, @nil];
	}

	class method init: pattern with: flags
	{
		return [self new pattern, flags, @nil];
	}

	class method init: pattern with: flags study: study
	{
		return [self new pattern, flags, study];
	}

	// Methods

	method pattern()
	{
		return d_pattern;
	}

	method search(str, pos = 0, endpos = @nil)
	{
		if ((endpos != @nil) && (endpos >= 0))
			str = string.sub(str, 0, endpos);
		local match_i = re.match(d_regexp, str, pos);

		if (! match_i) return match_i;
		return [Match new match_i, self, str];
	}

	method split(str, maxsplit = @nil)
	{
		local m;
		local start, len;
		local m_s, m_e;
		local has_sub;

		local left_s, match_s, rem_s;
		local res = #[];

		local nsplit = 0;

		has_sub = re.info(d_regexp, #capture-count) > 0 ? @true : @false;

		len = string.length(str);
		start = 0;
		while (start < len)
		{
			if ((maxsplit != @nil) &&
				(nsplit >= maxsplit))
			{
				array.push(res, string.sub(str, start));
				break;
			}

			m = [self search str, start];
			nsplit++;

			if ((! m) || (! [m matched]))
			{
				array.push(res, string.sub(str, start));
				break;
			}

			if (has_sub)
			{
				[m_s, m_e] = [m range 1];

				[left_s, match_s, rem_s] = [ string.range(str, 0, m_s-1),
										     string.range(str, m_s, m_e),
										     string.sub(str, m_e+1) ];
				array.push(res, left_s);
				array.push(res, match_s);
				start = m_e + 1;
			} else
			{
				[m_s, m_e] = [m range 0];

				[left_s, match_s, rem_s] = [ string.range(str, 0, m_s-1),
										     string.range(str, m_s, m_e),
										     string.sub(str, m_e+1) ];
				array.push(res, left_s);
				start = m_e + 1;
			}
		}

		return res;
	}
}

private rx = [Regexp init:"new"];
private res = [rx search "This is a brave new world !"];

basic.printf("Res keys   : %s\n", [res _keys]);
basic.printf("Res match 0: %s\n", res[0]);

res = [rx split "This is a brave new world !", 1];
basic.printf("Split 1 res: : %s\n", res);
