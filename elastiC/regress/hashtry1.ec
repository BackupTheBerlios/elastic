package hashtry1;

import basic;
import string;
import hash;

// Hash manipulation

private anArray, el;

basic.print( "\nHASH !\n======\n\n" );

anArray = %[ 6,      71,
			 "Rome", "Paris" ];

anArray[2]        = 3;
anArray[2.0]      = 5;
anArray[4.2]      = 12;
anArray["Hello"]  = "World";
anArray["Goofy"]  = "Pippo";
anArray["Mickey"] = "Topolino";
anArray[4.5]      = 9;

basic.print( anArray["Mickey"], "\n" );
basic.print( anArray["Hello"],  "\n" );
basic.print( anArray[4.5],      "\n" );
basic.print( anArray["Goofy"],  "\n" );
basic.print( anArray["Hello"],  "\n" );
basic.print( anArray[2],        "\n" );
basic.print( anArray[2.0],      "\n" );
basic.print( anArray[4.2],      "\n" );
basic.print( anArray["Rome"],   "\n" );

basic.print( "keys  : ", hash.keys( anArray ),   "\n" );
basic.print( "values: ", hash.values( anArray ), "\n" );
