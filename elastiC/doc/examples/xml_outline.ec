package xml_outline;

import string;
import xml;
import stream;
import filestream;
import hash;
import sys;
from basic import print, sprintf;

private level = 0;

private function indent(lev)
{
	local i;
	for (i = 0; i < lev; i++)
		print("  ");
}

private function startElement(parser, name, attrs)
	"startElement - Called for XML opening tags."
{
	local k;

	indent(level);
	print("enter \"", name, "\"\n");
	for (k in hash.keys(attrs))
	{
		indent(level);
		print("  attr[%s]: %s\n" % {k, attrs[k]});
	}
	level++;
}

private function endElement(parser, name)
	"endElement - Called for XML closing tags."
{
	level--;
	indent(level);
	print("exit \"", name, "\"\n");
}

private function characterData(parser, str)
	"characterData - Called for XML text."
{
	indent(level);
	print("data: ", str, "\n");
}

private function main()
	"Entry point."
{
	local parser;
	local line;
	local eof;

	parser = xml.parser_create();

	xml.set_element_handler(parser, startElement, endElement, characterData);

	local fh = filestream.open( sys.args[0], "r" );
	while (line = stream.gets(fh))
	{
		eof = stream.eof(fh);

		xml.parse(parser, line, eof);
	}
	stream.close( fh );
}

main();
