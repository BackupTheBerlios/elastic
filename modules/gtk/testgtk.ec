package testgtk;

import basic;
import gdk;
import gtk;

private w, vbox, btn = #[];

w    = gtk.window_new( #toplevel );
vbox = gtk.vbox_new( @false, 0 );

gtk.container_add( w, vbox );

for (local i = 0; i < 10; i++)
{
	btn[i] = gtk.button_new_with_label( basic.sprintf( "Button %d", i ) );

	/*
	 * A little explanation on callbacks in elastiC gtk...
	 * A callback can be a primitive function, a compiled one, or
	 * a two element array, where slot 0 is a function and slot 1 is a parameter to
	 * pass back on signal delivery (client data).
	 * The callback will receive as first argument the signalled object, and as second
	 * argument the user parameter passed to connect (client data). This allows to
	 * use always the same function, differentiating on client data.
	 *
	 * Said that, let's see an example where the connect callback parameter is in array
	 * form, with an anonymous function (closure) used as callback, and an index
	 * (button number) as client data.
	 */
	gtk.signal_connect_full( btn[i],						// Connect to
							 "clicked",						// on this signal
							 #[ function (obj, num)			// and with this callback
								{
									basic.print( basic.sprintf( "Button %d clicked\n", num ) );
								},
								i ], 0 );

	gtk.box_pack_start( vbox, btn[i] );
	gtk.widget_show( btn[i] );
}

/*
 * Another callback. A simple anonymous function this time (no array, no client data).
 * Note that now the function receives only one argument (the signalled object).
 */
gtk.signal_connect_full( w, "destroy",
						 function (obj)
						 {
							 basic.print( "Bye bye\n" );
							 gtk.main_quit();
						 }, 0 );

gtk.window_set_title( w, "My Window !" );

gtk.widget_show( w );
gtk.widget_show( vbox );
basic.print( gtk.widget_get_name( w ), "\n" );
gtk.main();
