/* gdkglue.c */

/* ------------------------------------------------------------------------
 *  Getters & setters for Boxed types
 * ------------------------------------------------------------------------ */

static EC_OBJ ec_gdk_gdk_color_getf( EC_OBJ stack, EcAny userdata )
{
	GdkColor *           c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.color_getf", TRUE, stack, "O&k",
	                            boxed_cvt, &c_obj, (EcDWord)&ec_gdk_boxed[5], FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_RED])
		res = EcMakeInt( (EcInt)(c_obj->red) );
	else if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_GREEN])
		res = EcMakeInt( (EcInt)(c_obj->green) );
	else if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_BLUE])
		res = EcMakeInt( (EcInt)(c_obj->blue) );

	return res;
}
static EC_OBJ ec_gdk_gdk_color_setf( EC_OBJ stack, EcAny userdata )
{
	GdkColor *           c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.color_setf", TRUE, stack, "O&kO",
	                            boxed_cvt, &c_obj, (EcDWord)&ec_gdk_boxed[5], FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_RED]) {
		EcInt	c_red;
		EC_CHECKTYPE_F("gdk.color_setf", 3, c_val, tc_inum);
		c_red = EC_INUM(c_val);
		c_obj->red = c_red;
	}
	else if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_GREEN]) {
		EcInt	c_green;
		EC_CHECKTYPE_F("gdk.color_setf", 3, c_val, tc_inum);
		c_green = EC_INUM(c_val);
		c_obj->green = c_green;
	}
	else if (c_sym == ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_BLUE]) {
		EcInt	c_blue;
		EC_CHECKTYPE_F("gdk.color_setf", 3, c_val, tc_inum);
		c_blue = EC_INUM(c_val);
		c_obj->blue = c_blue;
	}

	return EcTrueObject;
}

/* ------------------------------------------------------------------------
 *  Getters & setters for Object types
 * ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------
 *  Functions
 * ------------------------------------------------------------------------ */

static EC_OBJ ec_gdk_gdk_gc_new( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_new", TRUE, stack, "O&",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gdk_gc_new( c_window ), &ec_gdk_boxed[6], TRUE );

	return res;
}
static EC_OBJ ec_gdk_gdk_gc_set_foreground( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkColor *           c_color;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_foreground", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, boxed_cvt, &c_color, (EcDWord)&ec_gdk_boxed[5], FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_foreground( c_gc, c_color );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_background( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkColor *           c_color;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_background", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, boxed_cvt, &c_color, (EcDWord)&ec_gdk_boxed[5], FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_background( c_gc, c_color );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_font( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkFont *            c_font;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_font", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, boxed_cvt, &c_font, (EcDWord)&ec_gdk_boxed[2], FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_font( c_gc, c_font );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_function( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkFunction          c_function;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_function", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, enum_cvt, &c_function, (EcDWord)&ec_gdk_t_gdk_function, FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_function( c_gc, c_function );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_fill( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkFill              c_fill;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_fill", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, enum_cvt, &c_fill, (EcDWord)&ec_gdk_t_gdk_fill, FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_fill( c_gc, c_fill );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_ts_origin( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_ts_origin", TRUE, stack, "O&ii",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_ts_origin( c_gc, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_clip_origin( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_clip_origin", TRUE, stack, "O&ii",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_clip_origin( c_gc, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_subwindow( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	GdkSubwindowMode     c_mode;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_subwindow", TRUE, stack, "O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, enum_cvt, &c_mode, (EcDWord)&ec_gdk_t_gdk_subwindow_mode, FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_subwindow( c_gc, c_mode );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_exposures( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	EcBool               c_exposures;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_exposures", TRUE, stack, "O&b",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_exposures );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_exposures( c_gc, c_exposures );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_gc_set_line_attributes( EC_OBJ stack, EcAny userdata )
{
	GdkGC *              c_gc;
	EcInt                c_line_width;
	GdkLineStyle         c_line_style;
	GdkCapStyle          c_cap_style;
	GdkJoinStyle         c_join_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.gc_set_line_attributes", TRUE, stack, "O&iO&O&O&",
	                            boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_line_width, enum_cvt, &c_line_style, (EcDWord)&ec_gdk_t_gdk_line_style, FALSE, enum_cvt, &c_cap_style, (EcDWord)&ec_gdk_t_gdk_cap_style, FALSE, enum_cvt, &c_join_style, (EcDWord)&ec_gdk_t_gdk_join_style, FALSE );
	if (EC_ERRORP(res)) return res;

	gdk_gc_set_line_attributes( c_gc, c_line_width, c_line_style, c_cap_style, c_join_style );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_draw_line( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	GdkGC *              c_gc;
	EcInt                c_x1;
	EcInt                c_y1;
	EcInt                c_x2;
	EcInt                c_y2;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.draw_line", TRUE, stack, "O&O&iiii",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_x1, &c_y1, &c_x2, &c_y2 );
	if (EC_ERRORP(res)) return res;

	gdk_draw_line( c_window, c_gc, c_x1, c_y1, c_x2, c_y2 );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_draw_rectangle( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	GdkGC *              c_gc;
	EcBool               c_filled;
	EcInt                c_x;
	EcInt                c_y;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.draw_rectangle", TRUE, stack, "O&O&biiii",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_filled, &c_x, &c_y, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gdk_draw_rectangle( c_window, c_gc, c_filled, c_x, c_y, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_draw_arc( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	GdkGC *              c_gc;
	EcBool               c_filled;
	EcInt                c_x;
	EcInt                c_y;
	EcInt                c_width;
	EcInt                c_height;
	EcInt                c_angle1;
	EcInt                c_angle2;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.draw_arc", TRUE, stack, "O&O&biiiiii",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_filled, &c_x, &c_y, &c_width, &c_height, &c_angle1, &c_angle2 );
	if (EC_ERRORP(res)) return res;

	gdk_draw_arc( c_window, c_gc, c_filled, c_x, c_y, c_width, c_height, c_angle1, c_angle2 );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_draw_string( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	GdkFont *            c_font;
	GdkGC *              c_gc;
	EcInt                c_x;
	EcInt                c_y;
	EcChar *             c_string;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.draw_string", TRUE, stack, "O&O&O&iis",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, boxed_cvt, &c_font, (EcDWord)&ec_gdk_boxed[2], FALSE, boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, &c_x, &c_y, &c_string );
	if (EC_ERRORP(res)) return res;

	gdk_draw_string( c_window, c_font, c_gc, c_x, c_y, c_string );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_window_clear_area( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	EcInt                c_x;
	EcInt                c_y;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.window_clear_area", TRUE, stack, "O&iiii",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, &c_x, &c_y, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gdk_window_clear_area( c_window, c_x, c_y, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_window_clear_area_e( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window;
	EcInt                c_x;
	EcInt                c_y;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.window_clear_area_e", TRUE, stack, "O&iiii",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], FALSE, &c_x, &c_y, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gdk_window_clear_area_e( c_window, c_x, c_y, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_pixmap_new( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_window = NULL;
	EcInt                c_width;
	EcInt                c_height;
	EcInt                c_depth = -1;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.pixmap_new", TRUE, stack, "O&ii|i",
	                            boxed_cvt, &c_window, (EcDWord)&ec_gdk_boxed[3], TRUE, &c_width, &c_height, &c_depth );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gdk_pixmap_new( c_window, c_width, c_height, c_depth ), &ec_gdk_boxed[3], TRUE );

	return res;
}
static EC_OBJ ec_gdk_gdk_draw_pixmap( EC_OBJ stack, EcAny userdata )
{
	GdkWindow *          c_drawable;
	GdkGC *              c_gc;
	GdkWindow *          c_src;
	EcInt                c_xsrc;
	EcInt                c_ysrc;
	EcInt                c_xdest;
	EcInt                c_ydest;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.draw_pixmap", TRUE, stack, "O&O&O&iiiiii",
	                            boxed_cvt, &c_drawable, (EcDWord)&ec_gdk_boxed[3], FALSE, boxed_cvt, &c_gc, (EcDWord)&ec_gdk_boxed[6], FALSE, boxed_cvt, &c_src, (EcDWord)&ec_gdk_boxed[3], FALSE, &c_xsrc, &c_ysrc, &c_xdest, &c_ydest, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gdk_draw_pixmap( c_drawable, c_gc, c_src, c_xsrc, c_ysrc, c_xdest, c_ydest, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gdk_gdk_set_locale( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeString( (const char *)(gdk_set_locale(  )), 0 );

	return res;
}
static EC_OBJ ec_gdk_gdk_font_load( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_name;
	EC_OBJ               res;

	res = EcParseStackFunction( "gdk.font_load", TRUE, stack, "s",
	                            &c_name );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gdk_font_load( c_name ), &ec_gdk_boxed[2], TRUE );

	return res;
}
static EC_OBJ ec_gdk_gdk_flush( EC_OBJ stack, EcAny userdata )
{
	gdk_flush(  );

	return EC_NIL;
}

/* ------------------------------------------------------------------------
 *  Intialization function
 * ------------------------------------------------------------------------ */

static void ec_gdk_register_functions( void )
{
	EcAddPrimitive( "gdk.color_getf",                                 ec_gdk_gdk_color_getf                              );
	EcAddPrimitive( "gdk.color_setf",                                 ec_gdk_gdk_color_setf                              );
	EcAddPrimitive( "gdk.gc_new",                                     ec_gdk_gdk_gc_new                                  );
	EcAddPrimitive( "gdk.gc_set_foreground",                          ec_gdk_gdk_gc_set_foreground                       );
	EcAddPrimitive( "gdk.gc_set_background",                          ec_gdk_gdk_gc_set_background                       );
	EcAddPrimitive( "gdk.gc_set_font",                                ec_gdk_gdk_gc_set_font                             );
	EcAddPrimitive( "gdk.gc_set_function",                            ec_gdk_gdk_gc_set_function                         );
	EcAddPrimitive( "gdk.gc_set_fill",                                ec_gdk_gdk_gc_set_fill                             );
	EcAddPrimitive( "gdk.gc_set_ts_origin",                           ec_gdk_gdk_gc_set_ts_origin                        );
	EcAddPrimitive( "gdk.gc_set_clip_origin",                         ec_gdk_gdk_gc_set_clip_origin                      );
	EcAddPrimitive( "gdk.gc_set_subwindow",                           ec_gdk_gdk_gc_set_subwindow                        );
	EcAddPrimitive( "gdk.gc_set_exposures",                           ec_gdk_gdk_gc_set_exposures                        );
	EcAddPrimitive( "gdk.gc_set_line_attributes",                     ec_gdk_gdk_gc_set_line_attributes                  );
	EcAddPrimitive( "gdk.draw_line",                                  ec_gdk_gdk_draw_line                               );
	EcAddPrimitive( "gdk.draw_rectangle",                             ec_gdk_gdk_draw_rectangle                          );
	EcAddPrimitive( "gdk.draw_arc",                                   ec_gdk_gdk_draw_arc                                );
	EcAddPrimitive( "gdk.draw_string",                                ec_gdk_gdk_draw_string                             );
	EcAddPrimitive( "gdk.window_clear_area",                          ec_gdk_gdk_window_clear_area                       );
	EcAddPrimitive( "gdk.window_clear_area_e",                        ec_gdk_gdk_window_clear_area_e                     );
	EcAddPrimitive( "gdk.pixmap_new",                                 ec_gdk_gdk_pixmap_new                              );
	EcAddPrimitive( "gdk.draw_pixmap",                                ec_gdk_gdk_draw_pixmap                             );
	EcAddPrimitive( "gdk.set_locale",                                 ec_gdk_gdk_set_locale                              );
	EcAddPrimitive( "gdk.font_load",                                  ec_gdk_gdk_font_load                               );
	EcAddPrimitive( "gdk.flush",                                      ec_gdk_gdk_flush                                   );
}


