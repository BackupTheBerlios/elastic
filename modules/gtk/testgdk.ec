package testgdk;

import basic;
import gdk;
import gtk;
import math;

private width  = 300;
private height = 200;

private function draw_spiral( w )
{
	local gdkw, gc;
	local g, alpha, r, x, y, lx, ly;
	local cx = width / 2, cy = height / 2;

	gdkw = gtk.widget_getf( w, #window );

	gc = gdk.gc_new( gdkw );
	// gtk.gc_set_foreground( gc, );

	r = width / 3.0;
	lx = -1; ly = -1;
	for (g = 0.0; g < 1800.0; g = g + 2.5)
	{
		alpha = (g / 180.0) * 3.141592;

		x = cx + math.cos( alpha ) * r;
		y = cy - math.sin( alpha ) * r;

		if (lx >= 0)
			gdk.draw_line( gdkw, gc, lx, ly, x, y );
		else
			gdk.draw_line( gdkw, gc, x, y, x, y );
		r = r - r * .006;
		lx = x;
		ly = y;
	}

	gdk.flush();
}

private w;
private vbox, btn, label;

w    = gtk.window_new( #toplevel );
vbox = gtk.vbox_new( @false, 0 );

gtk.container_add( w, vbox );

label = gtk.label_new( "   HERE   " );
btn   = gtk.button_new_with_label( "DRAW !" );
gtk.signal_connect_full( btn, "clicked",
							 function (obj)
							 {
								 basic.print( basic.sprintf( "DRAW !\n" ) );
								 draw_spiral( label );
							 }, 0 );
gtk.box_pack_start( vbox, label );
gtk.box_pack_start( vbox, btn );
gtk.widget_set_usize( label, width, height );
gtk.widget_show( label );
gtk.widget_show( btn );

gtk.window_set_title( w, "My Window !" );
gtk.widget_show( w );
gtk.widget_show( vbox );

gtk.main();
