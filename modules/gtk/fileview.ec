package fileview;

from basic import *;
import file;

import gdk;
import gtk;

private selectFile, fillText;

private function initGui()
{
	local top  = gtk.window_new( #toplevel );

	gtk.window_set_default_size( top, 500, 400 );
	gtk.window_set_title( top, "File View" );
	gtk.signal_connect_full( top, "destroy",
							 function (obj) { gtk.main_quit(); }, 0 );

	local vbox = gtk.vbox_new( @false, 0 );
	local btnbox  = gtk.hbox_new( @false, 0 );
	local textbox = gtk.hbox_new( @false, 0 );

	gtk.container_add( top,  vbox );

	local chooseBtn = gtk.button_new_with_label( "Select..." );
	local text      = gtk.text_new( @nil, @nil );
	local vsb       = gtk.vscrollbar_new( gtk.text_getf( text, #vadj ) );
	gtk.signal_connect_full( chooseBtn,
							 "clicked",
							 function (obj)
							 {
								 local sel = selectFile( "Choose a file" );
								 if (sel) fillText( text, sel );
							 }, 0 );

	gtk.box_pack_start( vbox, btnbox, @false );
	gtk.box_pack_start( vbox, textbox );
	gtk.box_pack_start( btnbox,  chooseBtn );
	gtk.box_pack_start( textbox, text );
	gtk.box_pack_start( textbox, vsb, @false, @false, 0 );
	gtk.widget_show( chooseBtn );
	gtk.widget_show( text );
	gtk.widget_show( vsb );

	gtk.widget_show( btnbox );
	gtk.widget_show( textbox );
	gtk.widget_show( vbox );
	gtk.widget_show( top );

	gtk.main();
}

private function selectFile( title )
{
	local fs = gtk.file_selection_new( title );
	local selected = @nil;

	gtk.signal_connect_full( gtk.file_selection_getf( fs, #ok_button ),
							 "clicked",
							 function (obj)
							 {
								 selected = gtk.file_selection_get_filename( fs );
								 gtk.widget_destroy( fs );
								 gtk.main_quit();
							 }, 0 );
	gtk.signal_connect_full( gtk.file_selection_getf( fs, #cancel_button ),
							 "clicked",
							 function (obj)
							 {
								 selected = @nil;
								 gtk.widget_destroy( fs );
								 gtk.main_quit();
							 }, 0 );
	gtk.window_set_modal( fs, @true );
	gtk.widget_show( fs );
	gtk.main();
	return selected;
}

private function fillText( widget, filename )
{
	local line;

	local fh = file.open( filename, "r" );
	gtk.text_freeze( widget );
	while ((line = file.gets( fh )))
		gtk.text_insert( widget,
						 @nil, @nil, @nil,
						 line, length( line ) );
	gtk.text_thaw( widget );
	file.close( fh );
}

initGui();
