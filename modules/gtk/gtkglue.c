/* gtkglue.c */

/* ------------------------------------------------------------------------
 *  Getters & setters for Boxed types
 * ------------------------------------------------------------------------ */


/* ------------------------------------------------------------------------
 *  Getters & setters for Object types
 * ------------------------------------------------------------------------ */

static EC_OBJ ec_gtk_gtk_widget_getf( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STYLE])
		res = ec_gtk_boxed2obj( (gpointer)c_obj->style, &ec_gtk_boxed[0], TRUE );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_WINDOW])
		res = ec_gtk_boxed2obj( (gpointer)c_obj->window, &ec_gdk_boxed[3], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_setf( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STYLE]) {
		GtkStyle *	c_style;
		c_style = (GtkStyle *) ec_gtk_obj2boxed( c_val );
		c_obj->style = c_style;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_WINDOW]) {
		GdkWindow *	c_window;
		c_window = (GdkWindow *) ec_gtk_obj2boxed( c_val );
		c_obj->window = c_window;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_button_getf( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CHILD])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->child );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_IN_BUTTON])
		res = EcMakeBool( (EcBool)(c_obj->in_button) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON_DOWN])
		res = EcMakeBool( (EcBool)(c_obj->button_down) );

	return res;
}
static EC_OBJ ec_gtk_gtk_button_setf( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CHILD]) {
		GtkWidget *	c_child;
		c_child = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->child = c_child;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_IN_BUTTON]) {
		EcBool	c_in_button;
		c_in_button = EC_ASBOOLVAL(c_val);
		c_obj->in_button = c_in_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON_DOWN]) {
		EcBool	c_button_down;
		c_button_down = EC_ASBOOLVAL(c_val);
		c_obj->button_down = c_button_down;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_toggle_button_getf( EC_OBJ stack, EcAny userdata )
{
	GtkToggleButton *    c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTIVE])
		res = EcMakeBool( (EcBool)(c_obj->active) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DRAW_INDICATOR])
		res = EcMakeBool( (EcBool)(c_obj->draw_indicator) );

	return res;
}
static EC_OBJ ec_gtk_gtk_toggle_button_setf( EC_OBJ stack, EcAny userdata )
{
	GtkToggleButton *    c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTIVE]) {
		EcBool	c_active;
		c_active = EC_ASBOOLVAL(c_val);
		c_obj->active = c_active;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DRAW_INDICATOR]) {
		EcBool	c_draw_indicator;
		c_draw_indicator = EC_ASBOOLVAL(c_val);
		c_obj->draw_indicator = c_draw_indicator;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_combo_getf( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ENTRY])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->entry );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPUP])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->popup );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPWIN])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->popwin );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LIST])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->list );

	return res;
}
static EC_OBJ ec_gtk_gtk_combo_setf( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ENTRY]) {
		GtkWidget *	c_entry;
		c_entry = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->entry = c_entry;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON]) {
		GtkWidget *	c_button;
		c_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->button = c_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPUP]) {
		GtkWidget *	c_popup;
		c_popup = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->popup = c_popup;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPWIN]) {
		GtkWidget *	c_popwin;
		c_popwin = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->popwin = c_popwin;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LIST]) {
		GtkWidget *	c_list;
		c_list = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->list = c_list;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_gamma_curve_getf( EC_OBJ stack, EcAny userdata )
{
	GtkGammaCurve *      c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.gamma_curve_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TABLE])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->table );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CURVE])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->curve );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA])
		res = EcMakeFloat( (EcFloat)(c_obj->gamma) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_DIALOG])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->gamma_dialog );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_TEXT])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->gamma_text );

	return res;
}
static EC_OBJ ec_gtk_gtk_gamma_curve_setf( EC_OBJ stack, EcAny userdata )
{
	GtkGammaCurve *      c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.gamma_curve_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TABLE]) {
		GtkWidget *	c_table;
		c_table = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->table = c_table;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CURVE]) {
		GtkWidget *	c_curve;
		c_curve = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->curve = c_curve;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA]) {
		EcFloat	c_gamma;
		EC_CHECKTYPE_F("gtk.gamma_curve_setf", 3, c_val, tc_fnum);
		c_gamma = EC_FNUM(c_val);
		c_obj->gamma = c_gamma;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_DIALOG]) {
		GtkWidget *	c_gamma_dialog;
		c_gamma_dialog = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->gamma_dialog = c_gamma_dialog;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_TEXT]) {
		GtkWidget *	c_gamma_text;
		c_gamma_text = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->gamma_text = c_gamma_text;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_adjustment_getf( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.adjustment_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VALUE])
		res = EcMakeFloat( (EcFloat)(c_obj->value) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LOWER])
		res = EcMakeFloat( (EcFloat)(c_obj->lower) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_UPPER])
		res = EcMakeFloat( (EcFloat)(c_obj->upper) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STEP_INCREMENT])
		res = EcMakeFloat( (EcFloat)(c_obj->step_increment) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_INCREMENT])
		res = EcMakeFloat( (EcFloat)(c_obj->page_increment) );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_SIZE])
		res = EcMakeFloat( (EcFloat)(c_obj->page_size) );

	return res;
}
static EC_OBJ ec_gtk_gtk_adjustment_setf( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.adjustment_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VALUE]) {
		EcFloat	c_value;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_value = EC_FNUM(c_val);
		c_obj->value = c_value;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LOWER]) {
		EcFloat	c_lower;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_lower = EC_FNUM(c_val);
		c_obj->lower = c_lower;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_UPPER]) {
		EcFloat	c_upper;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_upper = EC_FNUM(c_val);
		c_obj->upper = c_upper;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STEP_INCREMENT]) {
		EcFloat	c_step_increment;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_step_increment = EC_FNUM(c_val);
		c_obj->step_increment = c_step_increment;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_INCREMENT]) {
		EcFloat	c_page_increment;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_page_increment = EC_FNUM(c_val);
		c_obj->page_increment = c_page_increment;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_SIZE]) {
		EcFloat	c_page_size;
		EC_CHECKTYPE_F("gtk.adjustment_setf", 3, c_val, tc_fnum);
		c_page_size = EC_FNUM(c_val);
		c_obj->page_size = c_page_size;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_notebook_getf( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TAB_POS])
		res = ec_gtk_enum2obj( (long)c_obj->tab_pos, &ec_gtk_t_gtk_position_type );

	return res;
}
static EC_OBJ ec_gtk_gtk_notebook_setf( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TAB_POS]) {
		GtkPositionType	c_tab_pos;
		c_tab_pos = (GtkPositionType) ec_gtk_obj2enum( c_val, &ec_gtk_t_gtk_position_type );
		c_obj->tab_pos = c_tab_pos;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_text_getf( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HADJ])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->hadj );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VADJ])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->vadj );

	return res;
}
static EC_OBJ ec_gtk_gtk_text_setf( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HADJ]) {
		GtkAdjustment *	c_hadj;
		c_hadj = (GtkAdjustment *) ec_gtk_obj2object( c_val );
		c_obj->hadj = c_hadj;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VADJ]) {
		GtkAdjustment *	c_vadj;
		c_vadj = (GtkAdjustment *) ec_gtk_obj2object( c_val );
		c_obj->vadj = c_vadj;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_file_selection_getf( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DIR_LIST])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->dir_list );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_FILE_LIST])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->file_list );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_ENTRY])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->selection_entry );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_TEXT])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->selection_text );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_MAIN_VBOX])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->main_vbox );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_OK_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->ok_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->cancel_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HELP_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->help_button );

	return res;
}
static EC_OBJ ec_gtk_gtk_file_selection_setf( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DIR_LIST]) {
		GtkWidget *	c_dir_list;
		c_dir_list = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->dir_list = c_dir_list;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_FILE_LIST]) {
		GtkWidget *	c_file_list;
		c_file_list = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->file_list = c_file_list;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_ENTRY]) {
		GtkWidget *	c_selection_entry;
		c_selection_entry = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->selection_entry = c_selection_entry;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_TEXT]) {
		GtkWidget *	c_selection_text;
		c_selection_text = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->selection_text = c_selection_text;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_MAIN_VBOX]) {
		GtkWidget *	c_main_vbox;
		c_main_vbox = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->main_vbox = c_main_vbox;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_OK_BUTTON]) {
		GtkWidget *	c_ok_button;
		c_ok_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->ok_button = c_ok_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON]) {
		GtkWidget *	c_cancel_button;
		c_cancel_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->cancel_button = c_cancel_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HELP_BUTTON]) {
		GtkWidget *	c_help_button;
		c_help_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->help_button = c_help_button;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_color_selection_dialog_getf( EC_OBJ stack, EcAny userdata )
{
	GtkColorSelectionDialog * c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.color_selection_dialog_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_COLORSEL])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->colorsel );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_MAIN_VBOX])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->main_vbox );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_OK_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->ok_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_RESET_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->reset_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->cancel_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HELP_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->help_button );

	return res;
}
static EC_OBJ ec_gtk_gtk_color_selection_dialog_setf( EC_OBJ stack, EcAny userdata )
{
	GtkColorSelectionDialog * c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.color_selection_dialog_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_COLORSEL]) {
		GtkWidget *	c_colorsel;
		c_colorsel = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->colorsel = c_colorsel;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_MAIN_VBOX]) {
		GtkWidget *	c_main_vbox;
		c_main_vbox = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->main_vbox = c_main_vbox;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_OK_BUTTON]) {
		GtkWidget *	c_ok_button;
		c_ok_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->ok_button = c_ok_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_RESET_BUTTON]) {
		GtkWidget *	c_reset_button;
		c_reset_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->reset_button = c_reset_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON]) {
		GtkWidget *	c_cancel_button;
		c_cancel_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->cancel_button = c_cancel_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HELP_BUTTON]) {
		GtkWidget *	c_help_button;
		c_help_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->help_button = c_help_button;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_dialog_getf( EC_OBJ stack, EcAny userdata )
{
	GtkDialog *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.dialog_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VBOX])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->vbox );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTION_AREA])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->action_area );

	return res;
}
static EC_OBJ ec_gtk_gtk_dialog_setf( EC_OBJ stack, EcAny userdata )
{
	GtkDialog *          c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.dialog_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VBOX]) {
		GtkWidget *	c_vbox;
		c_vbox = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->vbox = c_vbox;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTION_AREA]) {
		GtkWidget *	c_action_area;
		c_action_area = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->action_area = c_action_area;
	}

	return EcTrueObject;
}
static EC_OBJ ec_gtk_gtk_input_dialog_getf( EC_OBJ stack, EcAny userdata )
{
	GtkInputDialog *     c_obj;
	EcUInt               c_sym;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.input_dialog_getf", TRUE, stack, "O&k",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CLOSE_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->close_button );
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SAVE_BUTTON])
		res = ec_gtk_object2obj( (GtkObject *)c_obj->save_button );

	return res;
}
static EC_OBJ ec_gtk_gtk_input_dialog_setf( EC_OBJ stack, EcAny userdata )
{
	GtkInputDialog *     c_obj;
	EcUInt               c_sym;
	EC_OBJ               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.input_dialog_setf", TRUE, stack, "O&kO",
	                            object_cvt, &c_obj, (EcDWord)gtk_object_get_type(), FALSE, &c_sym, &c_val );
	if (EC_ERRORP(res)) return res;

	if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CLOSE_BUTTON]) {
		GtkWidget *	c_close_button;
		c_close_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->close_button = c_close_button;
	}
	else if (c_sym == ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SAVE_BUTTON]) {
		GtkWidget *	c_save_button;
		c_save_button = (GtkWidget *) ec_gtk_obj2object( c_val );
		c_obj->save_button = c_save_button;
	}

	return EcTrueObject;
}

/* ------------------------------------------------------------------------
 *  Functions
 * ------------------------------------------------------------------------ */

static EC_OBJ ec_gtk_gtk_exit( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_code = 0;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.exit", TRUE, stack, "|i",
	                            &c_code );
	if (EC_ERRORP(res)) return res;

	gtk_exit( c_code );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_main( EC_OBJ stack, EcAny userdata )
{
	gtk_main(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_main_level( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeInt( (EcInt)(gtk_main_level(  )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_main_quit( EC_OBJ stack, EcAny userdata )
{
	gtk_main_quit(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_main_iteration( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeBool( (EcBool)(gtk_main_iteration(  )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_events_pending( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeInt( (EcInt)(gtk_events_pending(  )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_timeout_add_full( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_interval;
	EC_OBJ               c_function;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.timeout_add_full", TRUE, stack, "iO",
	                            &c_interval, &c_function );
	if (EC_ERRORP(res)) return res;

	EcGCProtect( c_function );

	res = EcMakeInt( (EcInt)(gtk_timeout_add_full( c_interval, NULL, ec_gtk_callback_marshal, (gpointer)c_function, ec_gtk_callback_destroy )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_timeout_remove( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_tag;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.timeout_remove", TRUE, stack, "i",
	                            &c_tag );
	if (EC_ERRORP(res)) return res;

	gtk_timeout_remove( c_tag );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_idle_add_full( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_priority;
	EC_OBJ               c_function;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.idle_add_full", TRUE, stack, "iO",
	                            &c_priority, &c_function );
	if (EC_ERRORP(res)) return res;

	EcGCProtect( c_function );

	res = EcMakeInt( (EcInt)(gtk_idle_add_full( c_priority, NULL, ec_gtk_callback_marshal, (gpointer)c_function, ec_gtk_callback_destroy )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_idle_remove( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_tag;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.idle_remove", TRUE, stack, "i",
	                            &c_tag );
	if (EC_ERRORP(res)) return res;

	gtk_idle_remove( c_tag );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_input_remove( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_tag;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.input_remove", TRUE, stack, "i",
	                            &c_tag );
	if (EC_ERRORP(res)) return res;

	gtk_input_remove( c_tag );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_rc_parse( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_file;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.rc_parse", TRUE, stack, "s",
	                            &c_file );
	if (EC_ERRORP(res)) return res;

	gtk_rc_parse( c_file );

	return EC_NIL;
}
static EC_OBJ ec_gtk_g_mem_chunk_info( EC_OBJ stack, EcAny userdata )
{
	g_mem_chunk_info(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_object_destroy( EC_OBJ stack, EcAny userdata )
{
	GtkObject *          c_object;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.object_destroy", TRUE, stack, "O&",
	                            object_cvt, &c_object, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_object_destroy( c_object );

	return EC_NIL;
}
static EC_OBJ ec_gtk_GTK_OBJECT_DESTROYED( EC_OBJ stack, EcAny userdata )
{
	GtkObject *          c_object;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.OBJECT_DESTROYED", TRUE, stack, "O&",
	                            object_cvt, &c_object, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeBool( (EcBool)(GTK_OBJECT_DESTROYED( c_object )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_signal_connect_full( EC_OBJ stack, EcAny userdata )
{
	GtkObject *          c_object;
	EcChar *             c_name;
	EC_OBJ               c_func;
	EcBool               c_object_signal = 0;
	EcBool               c_after = 0;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.signal_connect_full", TRUE, stack, "O&sO|bb",
	                            object_cvt, &c_object, (EcDWord)gtk_object_get_type(), FALSE, &c_name, &c_func, &c_object_signal, &c_after );
	if (EC_ERRORP(res)) return res;

	EcGCProtect( c_func );

	res = EcMakeInt( (EcInt)(gtk_signal_connect_full( c_object, c_name, NULL, ec_gtk_callback_marshal, (gpointer)c_func, ec_gtk_callback_destroy, c_object_signal, c_after )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_signal_disconnect( EC_OBJ stack, EcAny userdata )
{
	GtkObject *          c_object;
	EcInt                c_id;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.signal_disconnect", TRUE, stack, "O&i",
	                            object_cvt, &c_object, (EcDWord)gtk_object_get_type(), FALSE, &c_id );
	if (EC_ERRORP(res)) return res;

	gtk_signal_disconnect( c_object, c_id );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_grab_add( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.grab_add", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_grab_add( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_grab_remove( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.grab_remove", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_grab_remove( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_GTK_WIDGET_STATE( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.WIDGET_STATE", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_enum2obj( (long)GTK_WIDGET_STATE( c_widget ), &ec_gtk_t_gtk_state_type );

	return res;
}
static EC_OBJ ec_gtk_GTK_WIDGET_FLAGS( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.WIDGET_FLAGS", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_flags2obj( (long)GTK_WIDGET_FLAGS( c_widget ), &ec_gtk_t_gtk_widget_flags );

	return res;
}
static EC_OBJ ec_gtk_GTK_WIDGET_SET_FLAGS( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkWidgetFlags       c_flags;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.WIDGET_SET_FLAGS", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, flags_cvt, &c_flags, (EcDWord)&ec_gtk_t_gtk_widget_flags, FALSE );
	if (EC_ERRORP(res)) return res;

	GTK_WIDGET_SET_FLAGS( c_widget, c_flags );

	return EC_NIL;
}
static EC_OBJ ec_gtk_GTK_WIDGET_UNSET_FLAGS( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkWidgetFlags       c_flags;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.WIDGET_UNSET_FLAGS", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, flags_cvt, &c_flags, (EcDWord)&ec_gtk_t_gtk_widget_flags, FALSE );
	if (EC_ERRORP(res)) return res;

	GTK_WIDGET_UNSET_FLAGS( c_widget, c_flags );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_destroy( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_destroy", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_destroy( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_unparent( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_unparent", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_unparent( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_show( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_show", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_show( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_hide( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_hide", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_hide( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_show_all( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_show_all", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_show_all( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_hide_all( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_hide_all", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_hide_all( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_map( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_map", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_map( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_unmap( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_unmap", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_unmap( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_realize( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_realize", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_realize( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_unrealize( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_unrealize", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_unrealize( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_event( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GdkEvent *           c_event;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_event", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, boxed_cvt, &c_event, (EcDWord)&ec_gdk_boxed[4], FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeBool( (EcBool)(gtk_widget_event( c_widget, c_event )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_activate( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_activate", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_activate( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_reparent( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkWidget *          c_new_parent;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_reparent", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_new_parent, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_reparent( c_widget, c_new_parent );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_popup( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_popup", TRUE, stack, "O&ii",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gtk_widget_popup( c_widget, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_grab_focus( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_grab_focus", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_grab_focus( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_grab_default( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_grab_default", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_grab_default( c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_name( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EcChar *             c_name;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_name", TRUE, stack, "O&s",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_name );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_name( c_widget, c_name );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_get_name( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_name", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeString( (const char *)(gtk_widget_get_name( c_widget )), 0 );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_set_state( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkStateType         c_state;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_state", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_state, (EcDWord)&ec_gtk_t_gtk_state_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_state( c_widget, c_state );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_sensitive( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EcBool               c_sensitive;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_sensitive", TRUE, stack, "O&b",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_sensitive );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_sensitive( c_widget, c_sensitive );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_parent( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkWidget *          c_parent;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_parent", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_parent, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_parent( c_widget, c_parent );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_style( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkStyle *           c_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_style", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, boxed_cvt, &c_style, (EcDWord)&ec_gtk_boxed[0], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_style( c_widget, c_style );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_uposition( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_uposition", TRUE, stack, "O&ii",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_uposition( c_widget, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_usize( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EcInt                c_height;
	EcInt                c_width;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_usize", TRUE, stack, "O&ii",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_height, &c_width );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_usize( c_widget, c_height, c_width );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_events( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GdkEventMask         c_events;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_events", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, flags_cvt, &c_events, (EcDWord)&ec_gdk_t_gdk_event_mask, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_events( c_widget, c_events );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_extension_events( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GdkEventMask         c_events;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_extension_events", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, flags_cvt, &c_events, (EcDWord)&ec_gdk_t_gdk_event_mask, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_extension_events( c_widget, c_events );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_get_toplevel( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_toplevel", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_widget_get_toplevel( c_widget ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_colormap( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_colormap", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_colormap( c_widget ), &ec_gdk_boxed[0], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_visual( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_visual", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_visual( c_widget ), &ec_gdk_boxed[1], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_style( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_style", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_style( c_widget ), &ec_gtk_boxed[0], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_events( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_events", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_flags2obj( (long)gtk_widget_get_events( c_widget ), &ec_gdk_t_gdk_event_mask );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_extension_events( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_get_extension_events", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_flags2obj( (long)gtk_widget_get_extension_events( c_widget ), &ec_gdk_t_gdk_event_mask );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_is_ancestor( EC_OBJ stack, EcAny userdata )
{
	GtkWidget *          c_widget;
	GtkWidget *          c_ancestor;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_is_ancestor", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_ancestor, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeBool( (EcBool)(gtk_widget_is_ancestor( c_widget, c_ancestor )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_push_colormap( EC_OBJ stack, EcAny userdata )
{
	GdkColormap *        c_cmap;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_push_colormap", TRUE, stack, "O&",
	                            boxed_cvt, &c_cmap, (EcDWord)&ec_gdk_boxed[0], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_push_colormap( c_cmap );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_push_visual( EC_OBJ stack, EcAny userdata )
{
	GdkVisual *          c_visual;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_push_visual", TRUE, stack, "O&",
	                            boxed_cvt, &c_visual, (EcDWord)&ec_gdk_boxed[1], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_push_visual( c_visual );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_push_style( EC_OBJ stack, EcAny userdata )
{
	GtkStyle *           c_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_push_style", TRUE, stack, "O&",
	                            boxed_cvt, &c_style, (EcDWord)&ec_gtk_boxed[0], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_push_style( c_style );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_pop_colormap( EC_OBJ stack, EcAny userdata )
{
	gtk_widget_pop_colormap(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_pop_visual( EC_OBJ stack, EcAny userdata )
{
	gtk_widget_pop_visual(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_pop_style( EC_OBJ stack, EcAny userdata )
{
	gtk_widget_pop_style(  );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_default_colormap( EC_OBJ stack, EcAny userdata )
{
	GdkColormap *        c_cmap;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_default_colormap", TRUE, stack, "O&",
	                            boxed_cvt, &c_cmap, (EcDWord)&ec_gdk_boxed[0], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_default_colormap( c_cmap );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_default_visual( EC_OBJ stack, EcAny userdata )
{
	GdkVisual *          c_visual;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_default_visual", TRUE, stack, "O&",
	                            boxed_cvt, &c_visual, (EcDWord)&ec_gdk_boxed[1], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_default_visual( c_visual );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_set_default_style( EC_OBJ stack, EcAny userdata )
{
	GtkStyle *           c_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.widget_set_default_style", TRUE, stack, "O&",
	                            boxed_cvt, &c_style, (EcDWord)&ec_gtk_boxed[0], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_widget_set_default_style( c_style );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_widget_get_default_colormap( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_default_colormap(  ), &ec_gdk_boxed[0], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_default_visual( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_default_visual(  ), &ec_gdk_boxed[1], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_widget_get_default_style( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_widget_get_default_style(  ), &ec_gtk_boxed[0], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_container_border_width( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	EcInt                c_border_width;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_border_width", TRUE, stack, "O&i",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE, &c_border_width );
	if (EC_ERRORP(res)) return res;

	gtk_container_border_width( c_container, c_border_width );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_container_add( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_add", TRUE, stack, "O&O&",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_container_add( c_container, c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_container_remove( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_remove", TRUE, stack, "O&O&",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_container_remove( c_container, c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_container_foreach_full( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	EC_OBJ               c_function;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_foreach_full", TRUE, stack, "O&O",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE, &c_function );
	if (EC_ERRORP(res)) return res;

	EcGCProtect( c_function );

	gtk_container_foreach_full( c_container, NULL, ec_gtk_callback_marshal, (gpointer)c_function, ec_gtk_callback_destroy );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_container_focus( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	GtkDirectionType     c_direction;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_focus", TRUE, stack, "O&O&",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_direction, (EcDWord)&ec_gtk_t_gtk_direction_type, FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_enum2obj( (long)gtk_container_focus( c_container, c_direction ), &ec_gtk_t_gtk_direction_type );

	return res;
}
static EC_OBJ ec_gtk_gtk_container_register_toplevel( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_register_toplevel", TRUE, stack, "O&",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_container_register_toplevel( c_container );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_container_unregister_toplevel( EC_OBJ stack, EcAny userdata )
{
	GtkContainer *       c_container;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.container_unregister_toplevel", TRUE, stack, "O&",
	                            object_cvt, &c_container, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_container_unregister_toplevel( c_container );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_fixed_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_fixed_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_fixed_put( EC_OBJ stack, EcAny userdata )
{
	GtkFixed *           c_fixed;
	GtkWidget *          c_widget;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.fixed_put", TRUE, stack, "O&O&ii",
	                            object_cvt, &c_fixed, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gtk_fixed_put( c_fixed, c_widget, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_fixed_move( EC_OBJ stack, EcAny userdata )
{
	GtkFixed *           c_fixed;
	GtkWidget *          c_widget;
	EcInt                c_x;
	EcInt                c_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.fixed_move", TRUE, stack, "O&O&ii",
	                            object_cvt, &c_fixed, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_x, &c_y );
	if (EC_ERRORP(res)) return res;

	gtk_fixed_move( c_fixed, c_widget, c_x, c_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_tree_item_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tree_item_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_tree_item_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tree_item_set_subtree( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	GtkWidget *          c_subtree;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_set_subtree", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_subtree, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_set_subtree( c_tree_item, c_subtree );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_remove_subtree( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_remove_subtree", TRUE, stack, "O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_remove_subtree( c_tree_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_select( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_select", TRUE, stack, "O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_select( c_tree_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_deselect( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_deselect", TRUE, stack, "O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_deselect( c_tree_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_expand( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_expand", TRUE, stack, "O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_expand( c_tree_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_item_collapse( EC_OBJ stack, EcAny userdata )
{
	GtkTreeItem *        c_tree_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_item_collapse", TRUE, stack, "O&",
	                            object_cvt, &c_tree_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_item_collapse( c_tree_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_tree_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tree_append( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_append", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_append( c_tree, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_prepend( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_prepend", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_prepend( c_tree, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_insert( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_insert", TRUE, stack, "O&O&i",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_tree_insert( c_tree, c_child, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_remove_item( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_remove_item", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_remove_item( c_tree, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_clear_items( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	EcInt                c_start;
	EcInt                c_end;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_clear_items", TRUE, stack, "O&ii",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, &c_start, &c_end );
	if (EC_ERRORP(res)) return res;

	gtk_tree_clear_items( c_tree, c_start, c_end );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_select_item( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	EcInt                c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_select_item", TRUE, stack, "O&i",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, &c_item );
	if (EC_ERRORP(res)) return res;

	gtk_tree_select_item( c_tree, c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_unselect_item( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	EcInt                c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_unselect_item", TRUE, stack, "O&i",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, &c_item );
	if (EC_ERRORP(res)) return res;

	gtk_tree_unselect_item( c_tree, c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_select_child( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_select_child", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_select_child( c_tree, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_unselect_child( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_unselect_child", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_unselect_child( c_tree, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_child_position( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_child_position", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_tree_child_position( c_tree, c_child )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tree_set_selection_mode( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkSelectionMode     c_mode;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_set_selection_mode", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_mode, (EcDWord)&ec_gtk_t_gtk_selection_mode, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_set_selection_mode( c_tree, c_mode );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_set_view_mode( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	GtkTreeViewMode      c_mode;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_set_view_mode", TRUE, stack, "O&O&",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_mode, (EcDWord)&ec_gtk_t_gtk_tree_view_mode, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tree_set_view_mode( c_tree, c_mode );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tree_set_view_lines( EC_OBJ stack, EcAny userdata )
{
	GtkTree *            c_tree;
	EcBool               c_flag;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tree_set_view_lines", TRUE, stack, "O&b",
	                            object_cvt, &c_tree, (EcDWord)gtk_object_get_type(), FALSE, &c_flag );
	if (EC_ERRORP(res)) return res;

	gtk_tree_set_view_lines( c_tree, c_flag );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_new( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_columns;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_new", TRUE, stack, "i",
	                            &c_columns );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_clist_new( c_columns ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_clist_set_selection_mode( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	GtkSelectionMode     c_mode;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_selection_mode", TRUE, stack, "O&O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_mode, (EcDWord)&ec_gtk_t_gtk_selection_mode, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_selection_mode( c_clist, c_mode );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_freeze( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_freeze", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_freeze( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_thaw( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_thaw", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_thaw( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_titles_show( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_titles_show", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_titles_show( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_titles_hide( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_titles_hide", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_titles_hide( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_title_active( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_title_active", TRUE, stack, "O&i",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_title_active( c_clist, c_column );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_title_passive( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_title_passive", TRUE, stack, "O&i",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_title_passive( c_clist, c_column );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_titles_active( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_titles_active", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_titles_active( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_column_titles_passive( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_column_titles_passive", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_column_titles_passive( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_column_title( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	EcChar *             c_title;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_column_title", TRUE, stack, "O&is",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column, &c_title );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_column_title( c_clist, c_column, c_title );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_column_widget( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	GtkWidget *          c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_column_widget", TRUE, stack, "O&iO&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_column_widget( c_clist, c_column, c_widget );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_column_justification( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	GtkJustification     c_justification;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_column_justification", TRUE, stack, "O&iO&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column, enum_cvt, &c_justification, (EcDWord)&ec_gtk_t_gtk_justification, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_column_justification( c_clist, c_column, c_justification );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_column_width( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_column;
	EcInt                c_width;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_column_width", TRUE, stack, "O&ii",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_column, &c_width );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_column_width( c_clist, c_column, c_width );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_row_height( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_row_height", TRUE, stack, "O&i",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_height );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_row_height( c_clist, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_moveto( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EcInt                c_column;
	EcFloat              c_row_align;
	EcFloat              c_column_align;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_moveto", TRUE, stack, "O&iiff",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_column, &c_row_align, &c_column_align );
	if (EC_ERRORP(res)) return res;

	gtk_clist_moveto( c_clist, c_row, c_column, c_row_align, c_column_align );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_row_is_visible( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_row_is_visible", TRUE, stack, "O&i",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row );
	if (EC_ERRORP(res)) return res;

	gtk_clist_row_is_visible( c_clist, c_row );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_text( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EcInt                c_column;
	EcChar *             c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_text", TRUE, stack, "O&iis",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_column, &c_text );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_text( c_clist, c_row, c_column, c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_foreground( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	GdkColor *           c_color;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_foreground", TRUE, stack, "O&iO&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, boxed_cvt, &c_color, (EcDWord)&ec_gdk_boxed[5], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_foreground( c_clist, c_row, c_color );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_background( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	GdkColor *           c_color;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_background", TRUE, stack, "O&iO&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, boxed_cvt, &c_color, (EcDWord)&ec_gdk_boxed[5], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_background( c_clist, c_row, c_color );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_set_shift( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EcInt                c_column;
	EcInt                c_vertical;
	EcInt                c_horizontal;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_set_shift", TRUE, stack, "O&iiii",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_column, &c_vertical, &c_horizontal );
	if (EC_ERRORP(res)) return res;

	gtk_clist_set_shift( c_clist, c_row, c_column, c_vertical, c_horizontal );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_select_row( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EcInt                c_column;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_select_row", TRUE, stack, "O&ii",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_column );
	if (EC_ERRORP(res)) return res;

	gtk_clist_select_row( c_clist, c_row, c_column );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_unselect_row( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EcInt                c_row;
	EcInt                c_column;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_unselect_row", TRUE, stack, "O&ii",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_column );
	if (EC_ERRORP(res)) return res;

	gtk_clist_unselect_row( c_clist, c_row, c_column );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_clist_clear( EC_OBJ stack, EcAny userdata )
{
	GtkCList *           c_clist;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.clist_clear", TRUE, stack, "O&",
	                            object_cvt, &c_clist, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_clist_clear( c_clist );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_event_box_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_event_box_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_window_new( EC_OBJ stack, EcAny userdata )
{
	GtkWindowType        c_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_new", TRUE, stack, "O&",
	                            enum_cvt, &c_type, (EcDWord)&ec_gtk_t_gtk_window_type, FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_window_new( c_type ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_window_set_title( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EcChar *             c_title;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_title", TRUE, stack, "O&s",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, &c_title );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_title( c_window, c_title );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_set_wmclass( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EcChar *             c_wmclass_class;
	EcChar *             c_wmclass_name;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_wmclass", TRUE, stack, "O&ss",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, &c_wmclass_class, &c_wmclass_name );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_wmclass( c_window, c_wmclass_class, c_wmclass_name );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_set_focus( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	GtkWidget *          c_focus;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_focus", TRUE, stack, "O&O&",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_focus, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_focus( c_window, c_focus );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_set_default( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	GtkWidget *          c_default;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_default", TRUE, stack, "O&O&",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_default, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_default( c_window, c_default );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_set_policy( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EcBool               c_allow_shrink;
	EcBool               c_allow_grow;
	EcBool               c_auto_shrink;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_policy", TRUE, stack, "O&bbb",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, &c_allow_shrink, &c_allow_grow, &c_auto_shrink );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_policy( c_window, c_allow_shrink, c_allow_grow, c_auto_shrink );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_position( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	GtkWindowPosition    c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_position", TRUE, stack, "O&O&",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_position, (EcDWord)&ec_gtk_t_gtk_window_position, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_window_position( c_window, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_activate_focus( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_activate_focus", TRUE, stack, "O&",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_window_activate_focus( c_window )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_window_activate_default( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_activate_default", TRUE, stack, "O&",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_window_activate_default( c_window )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_window_set_default_size( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_default_size", TRUE, stack, "O&ii",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_default_size( c_window, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_window_set_modal( EC_OBJ stack, EcAny userdata )
{
	GtkWindow *          c_window;
	EcBool               c_modal;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.window_set_modal", TRUE, stack, "O&b",
	                            object_cvt, &c_window, (EcDWord)gtk_object_get_type(), FALSE, &c_modal );
	if (EC_ERRORP(res)) return res;

	gtk_window_set_modal( c_window, c_modal );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_handle_box_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_handle_box_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_box_pack_start( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EcBool               c_expand = TRUE;
	EcBool               c_fill = TRUE;
	EcInt                c_padding = 0;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_pack_start", TRUE, stack, "O&O&|bbi",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_expand, &c_fill, &c_padding );
	if (EC_ERRORP(res)) return res;

	gtk_box_pack_start( c_box, c_child, c_expand, c_fill, c_padding );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_pack_end( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EcBool               c_expand = TRUE;
	EcBool               c_fill = TRUE;
	EcInt                c_padding = 0;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_pack_end", TRUE, stack, "O&O&|bbi",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_expand, &c_fill, &c_padding );
	if (EC_ERRORP(res)) return res;

	gtk_box_pack_end( c_box, c_child, c_expand, c_fill, c_padding );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_pack_start_defaults( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_pack_start_defaults", TRUE, stack, "O&O&",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_box_pack_start_defaults( c_box, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_pack_end_defaults( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_pack_end_defaults", TRUE, stack, "O&O&",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_box_pack_end_defaults( c_box, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_set_homogeneous( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	EcBool               c_homogenous;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_set_homogeneous", TRUE, stack, "O&b",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, &c_homogenous );
	if (EC_ERRORP(res)) return res;

	gtk_box_set_homogeneous( c_box, c_homogenous );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_set_spacing( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_set_spacing", TRUE, stack, "O&i",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_box_set_spacing( c_box, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_reorder_child( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EcInt                c_pos;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_reorder_child", TRUE, stack, "O&O&i",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_pos );
	if (EC_ERRORP(res)) return res;

	gtk_box_reorder_child( c_box, c_child, c_pos );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_box_set_child_packing( EC_OBJ stack, EcAny userdata )
{
	GtkBox *             c_box;
	GtkWidget *          c_child;
	EcBool               c_expand;
	EcBool               c_fill;
	EcInt                c_padding;
	GtkPackType          c_pack_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.box_set_child_packing", TRUE, stack, "O&O&bbiO&",
	                            object_cvt, &c_box, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_expand, &c_fill, &c_padding, enum_cvt, &c_pack_type, (EcDWord)&ec_gtk_t_gtk_pack_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_box_set_child_packing( c_box, c_child, c_expand, c_fill, c_padding, c_pack_type );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_set_child_size_default( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_min_width;
	EcInt                c_min_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_child_size_default", TRUE, stack, "ii",
	                            &c_min_width, &c_min_height );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_child_size_default( c_min_width, c_min_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_set_child_ipadding_default( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_ipad_x;
	EcInt                c_ipad_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_child_ipadding_default", TRUE, stack, "ii",
	                            &c_ipad_x, &c_ipad_y );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_child_ipadding_default( c_ipad_x, c_ipad_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_get_spacing( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_get_spacing", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_button_box_get_spacing( c_widget )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_button_box_get_layout( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_get_layout", TRUE, stack, "O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_enum2obj( (long)gtk_button_box_get_layout( c_widget ), &ec_gtk_t_gtk_button_box_style );

	return res;
}
static EC_OBJ ec_gtk_gtk_button_box_set_spacing( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_spacing", TRUE, stack, "O&i",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_spacing( c_widget, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_set_layout( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	GtkButtonBoxStyle    c_layout_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_layout", TRUE, stack, "O&O&",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_layout_style, (EcDWord)&ec_gtk_t_gtk_button_box_style, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_layout( c_widget, c_layout_style );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_set_child_size( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	EcInt                c_min_width;
	EcInt                c_min_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_child_size", TRUE, stack, "O&ii",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_min_width, &c_min_height );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_child_size( c_widget, c_min_width, c_min_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_box_set_child_ipadding( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBox *       c_widget;
	EcInt                c_ipad_x;
	EcInt                c_ipad_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_box_set_child_ipadding", TRUE, stack, "O&ii",
	                            object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_ipad_x, &c_ipad_y );
	if (EC_ERRORP(res)) return res;

	gtk_button_box_set_child_ipadding( c_widget, c_ipad_x, c_ipad_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_hbutton_box_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hbutton_box_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_hbutton_box_get_spacing_default( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeInt( (EcInt)(gtk_hbutton_box_get_spacing_default(  )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_hbutton_box_get_layout_default( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_enum2obj( (long)gtk_hbutton_box_get_layout_default(  ), &ec_gtk_t_gtk_button_box_style );

	return res;
}
static EC_OBJ ec_gtk_gtk_hbutton_box_set_spacing_default( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.hbutton_box_set_spacing_default", TRUE, stack, "i",
	                            &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_hbutton_box_set_spacing_default( c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_hbutton_box_set_layout_default( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBoxStyle    c_layout;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.hbutton_box_set_layout_default", TRUE, stack, "O&",
	                            enum_cvt, &c_layout, (EcDWord)&ec_gtk_t_gtk_button_box_style, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_hbutton_box_set_layout_default( c_layout );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_vbutton_box_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vbutton_box_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vbutton_box_get_spacing_default( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = EcMakeInt( (EcInt)(gtk_vbutton_box_get_spacing_default(  )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vbutton_box_get_layout_default( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_enum2obj( (long)gtk_vbutton_box_get_layout_default(  ), &ec_gtk_t_gtk_button_box_style );

	return res;
}
static EC_OBJ ec_gtk_gtk_vbutton_box_set_spacing_default( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.vbutton_box_set_spacing_default", TRUE, stack, "i",
	                            &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_vbutton_box_set_spacing_default( c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_vbutton_box_set_layout_default( EC_OBJ stack, EcAny userdata )
{
	GtkButtonBoxStyle    c_layout;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.vbutton_box_set_layout_default", TRUE, stack, "O&",
	                            enum_cvt, &c_layout, (EcDWord)&ec_gtk_t_gtk_button_box_style, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_vbutton_box_set_layout_default( c_layout );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_new( EC_OBJ stack, EcAny userdata )
{
	GtkOrientation       c_orientation;
	GtkToolbarStyle      c_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_new", TRUE, stack, "O&O&",
	                            enum_cvt, &c_orientation, (EcDWord)&ec_gtk_t_gtk_orientation, FALSE, enum_cvt, &c_style, (EcDWord)&ec_gtk_t_gtk_toolbar_style, FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_toolbar_new( c_orientation, c_style ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_toolbar_append_space( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_append_space", TRUE, stack, "O&",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_append_space( c_toolbar );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_prepend_space( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_prepend_space", TRUE, stack, "O&",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_prepend_space( c_toolbar );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_insert_space( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_insert_space", TRUE, stack, "O&i",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_insert_space( c_toolbar, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_append_widget( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_Toolbar;
	GtkWidget *          c_Widget;
	EcChar *             c_tooltip_text;
	EcChar *             c_tooltip_private_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_append_widget", TRUE, stack, "O&O&ss",
	                            object_cvt, &c_Toolbar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_Widget, (EcDWord)gtk_object_get_type(), FALSE, &c_tooltip_text, &c_tooltip_private_text );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_append_widget( c_Toolbar, c_Widget, c_tooltip_text, c_tooltip_private_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_prepend_widget( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_Toolbar;
	GtkWidget *          c_Widget;
	EcChar *             c_tooltip_text;
	EcChar *             c_tooltip_private_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_prepend_widget", TRUE, stack, "O&O&ss",
	                            object_cvt, &c_Toolbar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_Widget, (EcDWord)gtk_object_get_type(), FALSE, &c_tooltip_text, &c_tooltip_private_text );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_prepend_widget( c_Toolbar, c_Widget, c_tooltip_text, c_tooltip_private_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_insert_widget( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	GtkWidget *          c_widget;
	EcChar *             c_tooltip_text;
	EcChar *             c_tooltip_private_text;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_insert_widget", TRUE, stack, "O&O&ssi",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_tooltip_text, &c_tooltip_private_text, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_insert_widget( c_toolbar, c_widget, c_tooltip_text, c_tooltip_private_text, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_set_orientation( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	GtkOrientation       c_orientation;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_set_orientation", TRUE, stack, "O&O&",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_orientation, (EcDWord)&ec_gtk_t_gtk_orientation, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_set_orientation( c_toolbar, c_orientation );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_set_style( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	GtkToolbarStyle      c_style;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_set_style", TRUE, stack, "O&O&",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_style, (EcDWord)&ec_gtk_t_gtk_toolbar_style, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_set_style( c_toolbar, c_style );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_set_space_size( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	EcInt                c_space_size;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_set_space_size", TRUE, stack, "O&i",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, &c_space_size );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_set_space_size( c_toolbar, c_space_size );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toolbar_set_tooltips( EC_OBJ stack, EcAny userdata )
{
	GtkToolbar *         c_toolbar;
	EcBool               c_enable;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toolbar_set_tooltips", TRUE, stack, "O&b",
	                            object_cvt, &c_toolbar, (EcDWord)gtk_object_get_type(), FALSE, &c_enable );
	if (EC_ERRORP(res)) return res;

	gtk_toolbar_set_tooltips( c_toolbar, c_enable );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_new( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_rows;
	EcInt                c_columns;
	EcBool               c_homogenous;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_new", TRUE, stack, "iib",
	                            &c_rows, &c_columns, &c_homogenous );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_table_new( c_rows, c_columns, c_homogenous ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_table_attach( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	GtkWidget *          c_child;
	EcInt                c_left_attach;
	EcInt                c_right_attach;
	EcInt                c_top_attach;
	EcInt                c_bottom_attach;
	GtkAttachOptions     c_xoptions = GTK_EXPAND|GTK_FILL;
	GtkAttachOptions     c_yoptions = GTK_EXPAND|GTK_FILL;
	EcInt                c_xpadding = 0;
	EcInt                c_ypadding = 0;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_attach", TRUE, stack, "O&O&iiii|O&O&ii",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_left_attach, &c_right_attach, &c_top_attach, &c_bottom_attach, flags_cvt, &c_xoptions, (EcDWord)&ec_gtk_t_gtk_attach_options, FALSE, flags_cvt, &c_yoptions, (EcDWord)&ec_gtk_t_gtk_attach_options, FALSE, &c_xpadding, &c_ypadding );
	if (EC_ERRORP(res)) return res;

	gtk_table_attach( c_table, c_child, c_left_attach, c_right_attach, c_top_attach, c_bottom_attach, c_xoptions, c_yoptions, c_xpadding, c_ypadding );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_attach_defaults( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	GtkWidget *          c_child;
	EcInt                c_left_attach;
	EcInt                c_right_attach;
	EcInt                c_top_attach;
	EcInt                c_bottom_attach;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_attach_defaults", TRUE, stack, "O&O&iiii",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_left_attach, &c_right_attach, &c_top_attach, &c_bottom_attach );
	if (EC_ERRORP(res)) return res;

	gtk_table_attach_defaults( c_table, c_child, c_left_attach, c_right_attach, c_top_attach, c_bottom_attach );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_set_row_spacing( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	EcInt                c_row;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_set_row_spacing", TRUE, stack, "O&ii",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, &c_row, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_table_set_row_spacing( c_table, c_row, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_set_col_spacing( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	EcInt                c_column;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_set_col_spacing", TRUE, stack, "O&ii",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, &c_column, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_table_set_col_spacing( c_table, c_column, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_set_row_spacings( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_set_row_spacings", TRUE, stack, "O&i",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_table_set_row_spacings( c_table, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_table_set_col_spacings( EC_OBJ stack, EcAny userdata )
{
	GtkTable *           c_table;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.table_set_col_spacings", TRUE, stack, "O&i",
	                            object_cvt, &c_table, (EcDWord)gtk_object_get_type(), FALSE, &c_spacing );
	if (EC_ERRORP(res)) return res;

	gtk_table_set_col_spacings( c_table, c_spacing );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_button_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_button_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_button_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_button_pressed( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_pressed", TRUE, stack, "O&",
	                            object_cvt, &c_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_pressed( c_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_released( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_released", TRUE, stack, "O&",
	                            object_cvt, &c_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_released( c_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_clicked( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_clicked", TRUE, stack, "O&",
	                            object_cvt, &c_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_clicked( c_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_enter( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_enter", TRUE, stack, "O&",
	                            object_cvt, &c_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_enter( c_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_button_leave( EC_OBJ stack, EcAny userdata )
{
	GtkButton *          c_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.button_leave", TRUE, stack, "O&",
	                            object_cvt, &c_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_button_leave( c_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toggle_button_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_toggle_button_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_toggle_button_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_toggle_button_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_toggle_button_set_mode( EC_OBJ stack, EcAny userdata )
{
	GtkToggleButton *    c_toggle_button;
	EcBool               c_draw_indicator;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_set_mode", TRUE, stack, "O&b",
	                            object_cvt, &c_toggle_button, (EcDWord)gtk_object_get_type(), FALSE, &c_draw_indicator );
	if (EC_ERRORP(res)) return res;

	gtk_toggle_button_set_mode( c_toggle_button, c_draw_indicator );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toggle_button_set_state( EC_OBJ stack, EcAny userdata )
{
	GtkToggleButton *    c_toggle_button;
	EcBool               c_state;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_set_state", TRUE, stack, "O&b",
	                            object_cvt, &c_toggle_button, (EcDWord)gtk_object_get_type(), FALSE, &c_state );
	if (EC_ERRORP(res)) return res;

	gtk_toggle_button_set_state( c_toggle_button, c_state );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_toggle_button_toggled( EC_OBJ stack, EcAny userdata )
{
	GtkToggleButton *    c_toggle_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.toggle_button_toggled", TRUE, stack, "O&",
	                            object_cvt, &c_toggle_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_toggle_button_toggled( c_toggle_button );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_check_button_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_check_button_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_check_button_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.check_button_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_check_button_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_radio_button_new_from_widget( EC_OBJ stack, EcAny userdata )
{
	GtkRadioButton *     c_group = NULL;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.radio_button_new_from_widget", TRUE, stack, "O&",
	                            object_cvt, &c_group, (EcDWord)gtk_object_get_type(), TRUE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_radio_button_new_from_widget( c_group ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_radio_button_new_with_label_from_widget( EC_OBJ stack, EcAny userdata )
{
	GtkRadioButton *     c_group = NULL;
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.radio_button_new_with_label_from_widget", TRUE, stack, "O&s",
	                            object_cvt, &c_group, (EcDWord)gtk_object_get_type(), TRUE, &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_radio_button_new_with_label_from_widget( c_group, c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_misc_set_alignment( EC_OBJ stack, EcAny userdata )
{
	GtkMisc *            c_misc;
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.misc_set_alignment", TRUE, stack, "O&ff",
	                            object_cvt, &c_misc, (EcDWord)gtk_object_get_type(), FALSE, &c_xalign, &c_yalign );
	if (EC_ERRORP(res)) return res;

	gtk_misc_set_alignment( c_misc, c_xalign, c_yalign );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_misc_set_padding( EC_OBJ stack, EcAny userdata )
{
	GtkMisc *            c_misc;
	EcInt                c_xpad;
	EcInt                c_ypad;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.misc_set_padding", TRUE, stack, "O&ii",
	                            object_cvt, &c_misc, (EcDWord)gtk_object_get_type(), FALSE, &c_xpad, &c_ypad );
	if (EC_ERRORP(res)) return res;

	gtk_misc_set_padding( c_misc, c_xpad, c_ypad );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_arrow_new( EC_OBJ stack, EcAny userdata )
{
	GtkArrowType         c_arrow_type;
	GtkShadowType        c_shadow_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.arrow_new", TRUE, stack, "O&O&",
	                            enum_cvt, &c_arrow_type, (EcDWord)&ec_gtk_t_gtk_arrow_type, FALSE, enum_cvt, &c_shadow_type, (EcDWord)&ec_gtk_t_gtk_shadow_type, FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_arrow_new( c_arrow_type, c_shadow_type ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_arrow_set( EC_OBJ stack, EcAny userdata )
{
	GtkArrow *           c_arrow;
	GtkArrowType         c_arrow_type;
	GtkShadowType        c_shadow_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.arrow_set", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_arrow, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_arrow_type, (EcDWord)&ec_gtk_t_gtk_arrow_type, FALSE, enum_cvt, &c_shadow_type, (EcDWord)&ec_gtk_t_gtk_shadow_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_arrow_set( c_arrow, c_arrow_type, c_shadow_type );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_label_new( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_str;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.label_new", TRUE, stack, "s",
	                            &c_str );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_label_new( c_str ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_label_set( EC_OBJ stack, EcAny userdata )
{
	GtkLabel *           c_label;
	EcChar *             c_str;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.label_set", TRUE, stack, "O&s",
	                            object_cvt, &c_label, (EcDWord)gtk_object_get_type(), FALSE, &c_str );
	if (EC_ERRORP(res)) return res;

	gtk_label_set( c_label, c_str );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tips_query_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_tips_query_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tips_query_start_query( EC_OBJ stack, EcAny userdata )
{
	GtkTipsQuery *       c_tips_query;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tips_query_start_query", TRUE, stack, "O&",
	                            object_cvt, &c_tips_query, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tips_query_start_query( c_tips_query );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tips_query_stop_query( EC_OBJ stack, EcAny userdata )
{
	GtkTipsQuery *       c_tips_query;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tips_query_stop_query", TRUE, stack, "O&",
	                            object_cvt, &c_tips_query, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tips_query_stop_query( c_tips_query );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tips_query_set_caller( EC_OBJ stack, EcAny userdata )
{
	GtkTipsQuery *       c_tips_query;
	GtkWidget *          c_caller;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tips_query_set_caller", TRUE, stack, "O&O&",
	                            object_cvt, &c_tips_query, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_caller, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tips_query_set_caller( c_tips_query, c_caller );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tips_query_set_labels( EC_OBJ stack, EcAny userdata )
{
	GtkTipsQuery *       c_tips_query;
	EcChar *             c_label_inactive;
	EcChar *             c_label_no_tip;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tips_query_set_labels", TRUE, stack, "O&ss",
	                            object_cvt, &c_tips_query, (EcDWord)gtk_object_get_type(), FALSE, &c_label_inactive, &c_label_no_tip );
	if (EC_ERRORP(res)) return res;

	gtk_tips_query_set_labels( c_tips_query, c_label_inactive, c_label_no_tip );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_vbox_new( EC_OBJ stack, EcAny userdata )
{
	EcBool               c_homogenous;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.vbox_new", TRUE, stack, "bi",
	                            &c_homogenous, &c_spacing );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vbox_new( c_homogenous, c_spacing ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_hbox_new( EC_OBJ stack, EcAny userdata )
{
	EcBool               c_homogenous;
	EcInt                c_spacing;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.hbox_new", TRUE, stack, "bi",
	                            &c_homogenous, &c_spacing );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hbox_new( c_homogenous, c_spacing ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_item_select( EC_OBJ stack, EcAny userdata )
{
	GtkItem *            c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.item_select", TRUE, stack, "O&",
	                            object_cvt, &c_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_item_select( c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_item_deselect( EC_OBJ stack, EcAny userdata )
{
	GtkItem *            c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.item_deselect", TRUE, stack, "O&",
	                            object_cvt, &c_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_item_deselect( c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_item_toggle( EC_OBJ stack, EcAny userdata )
{
	GtkItem *            c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.item_toggle", TRUE, stack, "O&",
	                            object_cvt, &c_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_item_toggle( c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_combo_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_combo_set_value_in_list( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	EcBool               c_val;
	EcBool               c_ok_if_empty;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_set_value_in_list", TRUE, stack, "O&bb",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE, &c_val, &c_ok_if_empty );
	if (EC_ERRORP(res)) return res;

	gtk_combo_set_value_in_list( c_combo, c_val, c_ok_if_empty );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_set_use_arrows( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	EcBool               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_set_use_arrows", TRUE, stack, "O&b",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE, &c_val );
	if (EC_ERRORP(res)) return res;

	gtk_combo_set_use_arrows( c_combo, c_val );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_set_use_arrows_always( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	EcBool               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_set_use_arrows_always", TRUE, stack, "O&b",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE, &c_val );
	if (EC_ERRORP(res)) return res;

	gtk_combo_set_use_arrows_always( c_combo, c_val );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_set_case_sensitive( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	EcBool               c_val;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_set_case_sensitive", TRUE, stack, "O&b",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE, &c_val );
	if (EC_ERRORP(res)) return res;

	gtk_combo_set_case_sensitive( c_combo, c_val );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_set_item_string( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	GtkItem *            c_item;
	EcChar *             c_item_value;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_set_item_string", TRUE, stack, "O&O&s",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_item, (EcDWord)gtk_object_get_type(), FALSE, &c_item_value );
	if (EC_ERRORP(res)) return res;

	gtk_combo_set_item_string( c_combo, c_item, c_item_value );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_combo_disable_activate( EC_OBJ stack, EcAny userdata )
{
	GtkCombo *           c_combo;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.combo_disable_activate", TRUE, stack, "O&",
	                            object_cvt, &c_combo, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_combo_disable_activate( c_combo );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_statusbar_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_statusbar_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_statusbar_push( EC_OBJ stack, EcAny userdata )
{
	GtkStatusbar *       c_statusbar;
	EcInt                c_context_id;
	EcChar *             c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.statusbar_push", TRUE, stack, "O&is",
	                            object_cvt, &c_statusbar, (EcDWord)gtk_object_get_type(), FALSE, &c_context_id, &c_text );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_statusbar_push( c_statusbar, c_context_id, c_text )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_statusbar_pop( EC_OBJ stack, EcAny userdata )
{
	GtkStatusbar *       c_statusbar;
	EcInt                c_context_id;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.statusbar_pop", TRUE, stack, "O&i",
	                            object_cvt, &c_statusbar, (EcDWord)gtk_object_get_type(), FALSE, &c_context_id );
	if (EC_ERRORP(res)) return res;

	gtk_statusbar_pop( c_statusbar, c_context_id );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_statusbar_get_context_id( EC_OBJ stack, EcAny userdata )
{
	GtkStatusbar *       c_statusbar;
	EcChar *             c_context_description;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.statusbar_get_context_id", TRUE, stack, "O&s",
	                            object_cvt, &c_statusbar, (EcDWord)gtk_object_get_type(), FALSE, &c_context_description );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_statusbar_get_context_id( c_statusbar, c_context_description )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_statusbar_remove( EC_OBJ stack, EcAny userdata )
{
	GtkStatusbar *       c_statusbar;
	EcInt                c_context_id;
	EcInt                c_message_id;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.statusbar_remove", TRUE, stack, "O&ii",
	                            object_cvt, &c_statusbar, (EcDWord)gtk_object_get_type(), FALSE, &c_context_id, &c_message_id );
	if (EC_ERRORP(res)) return res;

	gtk_statusbar_remove( c_statusbar, c_context_id, c_message_id );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_gamma_curve_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_gamma_curve_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_hseparator_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hseparator_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vseparator_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vseparator_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_frame_new( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label = NULL;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.frame_new", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_frame_new( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_frame_set_label( EC_OBJ stack, EcAny userdata )
{
	GtkFrame *           c_frame;
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.frame_set_label", TRUE, stack, "O&s",
	                            object_cvt, &c_frame, (EcDWord)gtk_object_get_type(), FALSE, &c_label );
	if (EC_ERRORP(res)) return res;

	gtk_frame_set_label( c_frame, c_label );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_frame_set_label_align( EC_OBJ stack, EcAny userdata )
{
	GtkFrame *           c_frame;
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.frame_set_label_align", TRUE, stack, "O&ff",
	                            object_cvt, &c_frame, (EcDWord)gtk_object_get_type(), FALSE, &c_xalign, &c_yalign );
	if (EC_ERRORP(res)) return res;

	gtk_frame_set_label_align( c_frame, c_xalign, c_yalign );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_frame_set_shadow_type( EC_OBJ stack, EcAny userdata )
{
	GtkFrame *           c_frame;
	GtkShadowType        c_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.frame_set_shadow_type", TRUE, stack, "O&O&",
	                            object_cvt, &c_frame, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_type, (EcDWord)&ec_gtk_t_gtk_shadow_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_frame_set_shadow_type( c_frame, c_type );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_aspect_frame_new( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EcFloat              c_ratio;
	EcInt                c_obey_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.aspect_frame_new", TRUE, stack, "sfffi",
	                            &c_label, &c_xalign, &c_yalign, &c_ratio, &c_obey_child );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_aspect_frame_new( c_label, c_xalign, c_yalign, c_ratio, c_obey_child ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_aspect_frame_set( EC_OBJ stack, EcAny userdata )
{
	GtkAspectFrame *     c_aspect_frame;
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EcFloat              c_ratio;
	EcInt                c_obey_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.aspect_frame_set", TRUE, stack, "O&fffi",
	                            object_cvt, &c_aspect_frame, (EcDWord)gtk_object_get_type(), FALSE, &c_xalign, &c_yalign, &c_ratio, &c_obey_child );
	if (EC_ERRORP(res)) return res;

	gtk_aspect_frame_set( c_aspect_frame, c_xalign, c_yalign, c_ratio, c_obey_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_progress_bar_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_progress_bar_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_progress_bar_update( EC_OBJ stack, EcAny userdata )
{
	GtkProgressBar *     c_progress_bar;
	EcFloat              c_percentage;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.progress_bar_update", TRUE, stack, "O&f",
	                            object_cvt, &c_progress_bar, (EcDWord)gtk_object_get_type(), FALSE, &c_percentage );
	if (EC_ERRORP(res)) return res;

	gtk_progress_bar_update( c_progress_bar, c_percentage );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tooltips_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_tooltips_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_tooltips_enable( EC_OBJ stack, EcAny userdata )
{
	GtkTooltips *        c_tooltips;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tooltips_enable", TRUE, stack, "O&",
	                            object_cvt, &c_tooltips, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tooltips_enable( c_tooltips );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tooltips_disable( EC_OBJ stack, EcAny userdata )
{
	GtkTooltips *        c_tooltips;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tooltips_disable", TRUE, stack, "O&",
	                            object_cvt, &c_tooltips, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tooltips_disable( c_tooltips );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tooltips_set_delay( EC_OBJ stack, EcAny userdata )
{
	GtkTooltips *        c_tooltips;
	EcInt                c_delay;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tooltips_set_delay", TRUE, stack, "O&i",
	                            object_cvt, &c_tooltips, (EcDWord)gtk_object_get_type(), FALSE, &c_delay );
	if (EC_ERRORP(res)) return res;

	gtk_tooltips_set_delay( c_tooltips, c_delay );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tooltips_set_tip( EC_OBJ stack, EcAny userdata )
{
	GtkTooltips *        c_tooltips;
	GtkWidget *          c_widget;
	EcChar *             c_tip_text = NULL;
	EcChar *             c_tip_private;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tooltips_set_tip", TRUE, stack, "O&O&ss",
	                            object_cvt, &c_tooltips, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_widget, (EcDWord)gtk_object_get_type(), FALSE, &c_tip_text, &c_tip_private );
	if (EC_ERRORP(res)) return res;

	gtk_tooltips_set_tip( c_tooltips, c_widget, c_tip_text, c_tip_private );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_tooltips_set_colors( EC_OBJ stack, EcAny userdata )
{
	GtkTooltips *        c_tooltips;
	GdkColor *           c_background;
	GdkColor *           c_foreground;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.tooltips_set_colors", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_tooltips, (EcDWord)gtk_object_get_type(), FALSE, boxed_cvt, &c_background, (EcDWord)&ec_gdk_boxed[5], FALSE, boxed_cvt, &c_foreground, (EcDWord)&ec_gdk_boxed[5], FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_tooltips_set_colors( c_tooltips, c_background, c_foreground );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_shell_append( EC_OBJ stack, EcAny userdata )
{
	GtkMenuShell *       c_menu_shell;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_shell_append", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_shell, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_shell_append( c_menu_shell, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_shell_prepend( EC_OBJ stack, EcAny userdata )
{
	GtkMenuShell *       c_menu_shell;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_shell_prepend", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_shell, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_shell_prepend( c_menu_shell, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_shell_insert( EC_OBJ stack, EcAny userdata )
{
	GtkMenuShell *       c_menu_shell;
	GtkWidget *          c_child;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_shell_insert", TRUE, stack, "O&O&i",
	                            object_cvt, &c_menu_shell, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_menu_shell_insert( c_menu_shell, c_child, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_shell_deactivate( EC_OBJ stack, EcAny userdata )
{
	GtkMenuShell *       c_menu_shell;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_shell_deactivate", TRUE, stack, "O&",
	                            object_cvt, &c_menu_shell, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_shell_deactivate( c_menu_shell );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_bar_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_bar_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_bar_append( EC_OBJ stack, EcAny userdata )
{
	GtkMenuBar *         c_menu_bar;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_bar_append", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_bar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_bar_append( c_menu_bar, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_bar_prepend( EC_OBJ stack, EcAny userdata )
{
	GtkMenuBar *         c_menu_bar;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_bar_prepend", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_bar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_bar_prepend( c_menu_bar, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_bar_insert( EC_OBJ stack, EcAny userdata )
{
	GtkMenuBar *         c_menu_bar;
	GtkWidget *          c_child;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_bar_insert", TRUE, stack, "O&O&i",
	                            object_cvt, &c_menu_bar, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_menu_bar_insert( c_menu_bar, c_child, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_append( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_append", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_append( c_menu, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_prepend( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_prepend", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_prepend( c_menu, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_insert( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	GtkWidget *          c_child;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_insert", TRUE, stack, "O&O&i",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_menu_insert( c_menu, c_child, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_popdown( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_popdown", TRUE, stack, "O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_popdown( c_menu );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_get_active( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_get_active", TRUE, stack, "O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_get_active( c_menu ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_set_active( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	EcInt                c_index;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_set_active", TRUE, stack, "O&i",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE, &c_index );
	if (EC_ERRORP(res)) return res;

	gtk_menu_set_active( c_menu, c_index );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_get_attach_widget( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_get_attach_widget", TRUE, stack, "O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_get_attach_widget( c_menu ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_detach( EC_OBJ stack, EcAny userdata )
{
	GtkMenu *            c_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_detach", TRUE, stack, "O&",
	                            object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_detach( c_menu );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_item_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_item_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_menu_item_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_menu_item_set_submenu( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	GtkWidget *          c_submenu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_set_submenu", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_submenu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_set_submenu( c_menu_item, c_submenu );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_remove_submenu( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_remove_submenu", TRUE, stack, "O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_remove_submenu( c_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_set_placement( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	GtkSubmenuPlacement  c_placement;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_set_placement", TRUE, stack, "O&O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_placement, (EcDWord)&ec_gtk_t_gtk_submenu_placement, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_set_placement( c_menu_item, c_placement );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_configure( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EcBool               c_show_toggle_indicator;
	EcBool               c_show_submenu_indicator;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_configure", TRUE, stack, "O&bb",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE, &c_show_toggle_indicator, &c_show_submenu_indicator );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_configure( c_menu_item, c_show_toggle_indicator, c_show_submenu_indicator );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_select( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_select", TRUE, stack, "O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_select( c_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_deselect( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_deselect", TRUE, stack, "O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_deselect( c_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_activate( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_activate", TRUE, stack, "O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_activate( c_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_menu_item_right_justify( EC_OBJ stack, EcAny userdata )
{
	GtkMenuItem *        c_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.menu_item_right_justify", TRUE, stack, "O&",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_menu_item_right_justify( c_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_check_menu_item_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_check_menu_item_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_check_menu_item_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.check_menu_item_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_check_menu_item_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_check_menu_item_set_state( EC_OBJ stack, EcAny userdata )
{
	GtkCheckMenuItem *   c_check_menu_item;
	EcBool               c_state;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.check_menu_item_set_state", TRUE, stack, "O&b",
	                            object_cvt, &c_check_menu_item, (EcDWord)gtk_object_get_type(), FALSE, &c_state );
	if (EC_ERRORP(res)) return res;

	gtk_check_menu_item_set_state( c_check_menu_item, c_state );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_check_menu_item_set_show_toggle( EC_OBJ stack, EcAny userdata )
{
	GtkCheckMenuItem *   c_menu_item;
	EcBool               c_always;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.check_menu_item_set_show_toggle", TRUE, stack, "O&b",
	                            object_cvt, &c_menu_item, (EcDWord)gtk_object_get_type(), FALSE, &c_always );
	if (EC_ERRORP(res)) return res;

	gtk_check_menu_item_set_show_toggle( c_menu_item, c_always );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_check_menu_item_toggled( EC_OBJ stack, EcAny userdata )
{
	GtkCheckMenuItem *   c_check_menu_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.check_menu_item_toggled", TRUE, stack, "O&",
	                            object_cvt, &c_check_menu_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_check_menu_item_toggled( c_check_menu_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_option_menu_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_option_menu_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_option_menu_get_menu( EC_OBJ stack, EcAny userdata )
{
	GtkOptionMenu *      c_option_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.option_menu_get_menu", TRUE, stack, "O&",
	                            object_cvt, &c_option_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_option_menu_get_menu( c_option_menu ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_option_menu_set_menu( EC_OBJ stack, EcAny userdata )
{
	GtkOptionMenu *      c_option_menu;
	GtkWidget *          c_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.option_menu_set_menu", TRUE, stack, "O&O&",
	                            object_cvt, &c_option_menu, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_option_menu_set_menu( c_option_menu, c_menu );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_option_menu_remove_menu( EC_OBJ stack, EcAny userdata )
{
	GtkOptionMenu *      c_option_menu;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.option_menu_remove_menu", TRUE, stack, "O&",
	                            object_cvt, &c_option_menu, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_option_menu_remove_menu( c_option_menu );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_option_menu_set_history( EC_OBJ stack, EcAny userdata )
{
	GtkOptionMenu *      c_option_menu;
	EcInt                c_index;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.option_menu_set_history", TRUE, stack, "O&i",
	                            object_cvt, &c_option_menu, (EcDWord)gtk_object_get_type(), FALSE, &c_index );
	if (EC_ERRORP(res)) return res;

	gtk_option_menu_set_history( c_option_menu, c_index );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_adjustment_new( EC_OBJ stack, EcAny userdata )
{
	EcFloat              c_value;
	EcFloat              c_lower;
	EcFloat              c_upper;
	EcFloat              c_step_increment;
	EcFloat              c_page_increment;
	EcFloat              c_page_size;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.adjustment_new", TRUE, stack, "ffffff",
	                            &c_value, &c_lower, &c_upper, &c_step_increment, &c_page_increment, &c_page_size );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_adjustment_new( c_value, c_lower, c_upper, c_step_increment, c_page_increment, c_page_size ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_adjustment_set_value( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EcFloat              c_value;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.adjustment_set_value", TRUE, stack, "O&f",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE, &c_value );
	if (EC_ERRORP(res)) return res;

	gtk_adjustment_set_value( c_adjustment, c_value );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_viewport_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_hadjustment;
	GtkAdjustment *      c_vadjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_new", TRUE, stack, "O&O&",
	                            object_cvt, &c_hadjustment, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_vadjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_viewport_new( c_hadjustment, c_vadjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_viewport_get_hadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkViewport *        c_viewport;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_get_hadjustment", TRUE, stack, "O&",
	                            object_cvt, &c_viewport, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_viewport_get_hadjustment( c_viewport ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_viewport_get_vadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkViewport *        c_viewport;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_get_vadjustment", TRUE, stack, "O&",
	                            object_cvt, &c_viewport, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_viewport_get_vadjustment( c_viewport ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_viewport_set_hadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkViewport *        c_viewport;
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_set_hadjustment", TRUE, stack, "O&O&",
	                            object_cvt, &c_viewport, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_viewport_set_hadjustment( c_viewport, c_adjustment );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_viewport_set_vadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkViewport *        c_viewport;
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_set_vadjustment", TRUE, stack, "O&O&",
	                            object_cvt, &c_viewport, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_viewport_set_vadjustment( c_viewport, c_adjustment );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_viewport_set_shadow_type( EC_OBJ stack, EcAny userdata )
{
	GtkViewport *        c_viewport;
	GtkShadowType        c_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.viewport_set_shadow_type", TRUE, stack, "O&O&",
	                            object_cvt, &c_viewport, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_type, (EcDWord)&ec_gtk_t_gtk_shadow_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_viewport_set_shadow_type( c_viewport, c_type );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_scrolled_window_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_hadjustment = NULL;
	GtkAdjustment *      c_vadjustment = NULL;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scrolled_window_new", TRUE, stack, "|O&O&",
	                            object_cvt, &c_hadjustment, (EcDWord)gtk_object_get_type(), TRUE, object_cvt, &c_vadjustment, (EcDWord)gtk_object_get_type(), TRUE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_scrolled_window_new( c_hadjustment, c_vadjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_scrolled_window_get_hadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkScrolledWindow *  c_scrolled_window;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scrolled_window_get_hadjustment", TRUE, stack, "O&",
	                            object_cvt, &c_scrolled_window, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_scrolled_window_get_hadjustment( c_scrolled_window ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_scrolled_window_get_vadjustment( EC_OBJ stack, EcAny userdata )
{
	GtkScrolledWindow *  c_scrolled_window;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scrolled_window_get_vadjustment", TRUE, stack, "O&",
	                            object_cvt, &c_scrolled_window, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_scrolled_window_get_vadjustment( c_scrolled_window ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_scrolled_window_set_policy( EC_OBJ stack, EcAny userdata )
{
	GtkScrolledWindow *  c_scrolled_window;
	GtkPolicyType        c_hscrollbar_policy;
	GtkPolicyType        c_vscrollbar_policy;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scrolled_window_set_policy", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_scrolled_window, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_hscrollbar_policy, (EcDWord)&ec_gtk_t_gtk_policy_type, FALSE, enum_cvt, &c_vscrollbar_policy, (EcDWord)&ec_gtk_t_gtk_policy_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_scrolled_window_set_policy( c_scrolled_window, c_hscrollbar_policy, c_vscrollbar_policy );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_item_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_list_item_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_list_item_new_with_label( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_item_new_with_label", TRUE, stack, "s",
	                            &c_label );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_list_item_new_with_label( c_label ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_list_item_select( EC_OBJ stack, EcAny userdata )
{
	GtkListItem *        c_list_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_item_select", TRUE, stack, "O&",
	                            object_cvt, &c_list_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_list_item_select( c_list_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_item_deselect( EC_OBJ stack, EcAny userdata )
{
	GtkListItem *        c_list_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_item_deselect", TRUE, stack, "O&",
	                            object_cvt, &c_list_item, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_list_item_deselect( c_list_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_list_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_list_clear_items( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	EcInt                c_start;
	EcInt                c_end;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_clear_items", TRUE, stack, "O&ii",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, &c_start, &c_end );
	if (EC_ERRORP(res)) return res;

	gtk_list_clear_items( c_list, c_start, c_end );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_select_item( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	EcInt                c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_select_item", TRUE, stack, "O&i",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, &c_item );
	if (EC_ERRORP(res)) return res;

	gtk_list_select_item( c_list, c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_unselect_item( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	EcInt                c_item;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_unselect_item", TRUE, stack, "O&i",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, &c_item );
	if (EC_ERRORP(res)) return res;

	gtk_list_unselect_item( c_list, c_item );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_select_child( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_select_child", TRUE, stack, "O&O&",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_list_select_child( c_list, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_unselect_child( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_unselect_child", TRUE, stack, "O&O&",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_list_unselect_child( c_list, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_list_child_position( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_child_position", TRUE, stack, "O&O&",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_list_child_position( c_list, c_child )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_list_set_selection_mode( EC_OBJ stack, EcAny userdata )
{
	GtkList *            c_list;
	GtkSelectionMode     c_mode;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.list_set_selection_mode", TRUE, stack, "O&O&",
	                            object_cvt, &c_list, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_mode, (EcDWord)&ec_gtk_t_gtk_selection_mode, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_list_set_selection_mode( c_list, c_mode );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_notebook_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_notebook_append_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_append_page", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_append_page( c_notebook, c_child, c_tab_label );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_append_page_menu( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	GtkWidget *          c_menu_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_append_page_menu", TRUE, stack, "O&O&O&O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_menu_label, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_append_page_menu( c_notebook, c_child, c_tab_label, c_menu_label );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_prepend_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_prepend_page", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_prepend_page( c_notebook, c_child, c_tab_label );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_prepend_page_menu( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	GtkWidget *          c_menu_label;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_prepend_page_menu", TRUE, stack, "O&O&O&O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_menu_label, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_prepend_page_menu( c_notebook, c_child, c_tab_label, c_menu_label );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_insert_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_insert_page", TRUE, stack, "O&O&O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_insert_page( c_notebook, c_child, c_tab_label, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_insert_page_menu( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkWidget *          c_child;
	GtkWidget *          c_tab_label;
	GtkWidget *          c_menu_label;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_insert_page_menu", TRUE, stack, "O&O&O&O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_tab_label, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_menu_label, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_insert_page_menu( c_notebook, c_child, c_tab_label, c_menu_label, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_remove_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcInt                c_page_num;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_remove_page", TRUE, stack, "O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_page_num );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_remove_page( c_notebook, c_page_num );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_current_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_current_page", TRUE, stack, "O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_notebook_current_page( c_notebook )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_notebook_set_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcInt                c_page_num;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_page", TRUE, stack, "O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_page_num );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_page( c_notebook, c_page_num );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_next_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_next_page", TRUE, stack, "O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_next_page( c_notebook );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_prev_page( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_prev_page", TRUE, stack, "O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_prev_page( c_notebook );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_set_tab_pos( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	GtkPositionType      c_pos;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_tab_pos", TRUE, stack, "O&O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_pos, (EcDWord)&ec_gtk_t_gtk_position_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_tab_pos( c_notebook, c_pos );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_set_show_tabs( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcBool               c_show_tabs;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_show_tabs", TRUE, stack, "O&b",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_show_tabs );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_show_tabs( c_notebook, c_show_tabs );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_set_show_border( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcBool               c_show_border;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_show_border", TRUE, stack, "O&b",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_show_border );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_show_border( c_notebook, c_show_border );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_set_scrollable( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcInt                c_scrollable;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_scrollable", TRUE, stack, "O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_scrollable );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_scrollable( c_notebook, c_scrollable );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_set_tab_border( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EcInt                c_border_width;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_set_tab_border", TRUE, stack, "O&i",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE, &c_border_width );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_set_tab_border( c_notebook, c_border_width );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_popup_enable( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_popup_enable", TRUE, stack, "O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_popup_enable( c_notebook );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_notebook_popup_disable( EC_OBJ stack, EcAny userdata )
{
	GtkNotebook *        c_notebook;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.notebook_popup_disable", TRUE, stack, "O&",
	                            object_cvt, &c_notebook, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_notebook_popup_disable( c_notebook );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_entry_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_entry_new_with_max_length( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_max;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_new_with_max_length", TRUE, stack, "i",
	                            &c_max );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_entry_new_with_max_length( c_max ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_entry_set_text( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcChar *             c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_set_text", TRUE, stack, "O&s",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_text );
	if (EC_ERRORP(res)) return res;

	gtk_entry_set_text( c_entry, c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_append_text( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcChar *             c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_append_text", TRUE, stack, "O&s",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_text );
	if (EC_ERRORP(res)) return res;

	gtk_entry_append_text( c_entry, c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_prepend_text( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcChar *             c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_prepend_text", TRUE, stack, "O&s",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_text );
	if (EC_ERRORP(res)) return res;

	gtk_entry_prepend_text( c_entry, c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_set_position( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcInt                c_position;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_set_position", TRUE, stack, "O&i",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_position );
	if (EC_ERRORP(res)) return res;

	gtk_entry_set_position( c_entry, c_position );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_get_text( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_get_text", TRUE, stack, "O&",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeString( (const char *)(gtk_entry_get_text( c_entry )), 0 );

	return res;
}
static EC_OBJ ec_gtk_gtk_entry_select_region( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcInt                c_start;
	EcInt                c_end;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_select_region", TRUE, stack, "O&ii",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_start, &c_end );
	if (EC_ERRORP(res)) return res;

	gtk_entry_select_region( c_entry, c_start, c_end );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_set_visibility( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcBool               c_visible;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_set_visibility", TRUE, stack, "O&b",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_visible );
	if (EC_ERRORP(res)) return res;

	gtk_entry_set_visibility( c_entry, c_visible );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_entry_set_editable( EC_OBJ stack, EcAny userdata )
{
	GtkEntry *           c_entry;
	EcBool               c_editable;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.entry_set_editable", TRUE, stack, "O&b",
	                            object_cvt, &c_entry, (EcDWord)gtk_object_get_type(), FALSE, &c_editable );
	if (EC_ERRORP(res)) return res;

	gtk_entry_set_editable( c_entry, c_editable );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_spin_button_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EcFloat              c_climb_rate;
	EcInt                c_digits;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_new", TRUE, stack, "O&fi",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE, &c_climb_rate, &c_digits );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_spin_button_new( c_adjustment, c_climb_rate, c_digits ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_spin_button_set_adjustment( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_set_adjustment", TRUE, stack, "O&O&",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_spin_button_set_adjustment( c_spin_button, c_adjustment );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_spin_button_get_adjustment( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_get_adjustment", TRUE, stack, "O&",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_spin_button_get_adjustment( c_spin_button ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_spin_button_set_digits( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	EcInt                c_digits;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_set_digits", TRUE, stack, "O&i",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE, &c_digits );
	if (EC_ERRORP(res)) return res;

	gtk_spin_button_set_digits( c_spin_button, c_digits );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_spin_button_get_value_as_float( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_get_value_as_float", TRUE, stack, "O&",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeFloat( (EcFloat)(gtk_spin_button_get_value_as_float( c_spin_button )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_spin_button_get_value_as_int( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_get_value_as_int", TRUE, stack, "O&",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_spin_button_get_value_as_int( c_spin_button )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_spin_button_set_value( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	EcFloat              c_value;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_set_value", TRUE, stack, "O&f",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE, &c_value );
	if (EC_ERRORP(res)) return res;

	gtk_spin_button_set_value( c_spin_button, c_value );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_spin_button_set_update_policy( EC_OBJ stack, EcAny userdata )
{
	GtkSpinButton *      c_spin_button;
	GtkSpinButtonUpdatePolicy c_policy;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.spin_button_set_update_policy", TRUE, stack, "O&O&",
	                            object_cvt, &c_spin_button, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_policy, (EcDWord)&ec_gtk_t_gtk_spin_button_update_policy, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_spin_button_set_update_policy( c_spin_button, c_policy );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_hadj = NULL;
	GtkAdjustment *      c_vadj = NULL;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_new", TRUE, stack, "O&O&",
	                            object_cvt, &c_hadj, (EcDWord)gtk_object_get_type(), TRUE, object_cvt, &c_vadj, (EcDWord)gtk_object_get_type(), TRUE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_text_new( c_hadj, c_vadj ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_text_set_editable( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EcBool               c_editable;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_set_editable", TRUE, stack, "O&b",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, &c_editable );
	if (EC_ERRORP(res)) return res;

	gtk_text_set_editable( c_text, c_editable );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_set_adjustments( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	GtkAdjustment *      c_hadj;
	GtkAdjustment *      c_vadj;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_set_adjustments", TRUE, stack, "O&O&O&",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_hadj, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_vadj, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_text_set_adjustments( c_text, c_hadj, c_vadj );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_set_point( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EcInt                c_index;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_set_point", TRUE, stack, "O&i",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, &c_index );
	if (EC_ERRORP(res)) return res;

	gtk_text_set_point( c_text, c_index );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_get_point( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_get_point", TRUE, stack, "O&",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_text_get_point( c_text )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_text_get_length( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_get_length", TRUE, stack, "O&",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_text_get_length( c_text )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_text_freeze( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_freeze", TRUE, stack, "O&",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_text_freeze( c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_thaw( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_thaw", TRUE, stack, "O&",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_text_thaw( c_text );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_insert( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	GdkFont *            c_font = NULL;
	GdkColor *           c_fore = NULL;
	GdkColor *           c_back = NULL;
	EcChar *             c_chars;
	EcInt                c_length;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_insert", TRUE, stack, "O&O&O&O&si",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, boxed_cvt, &c_font, (EcDWord)&ec_gdk_boxed[2], TRUE, boxed_cvt, &c_fore, (EcDWord)&ec_gdk_boxed[5], TRUE, boxed_cvt, &c_back, (EcDWord)&ec_gdk_boxed[5], TRUE, &c_chars, &c_length );
	if (EC_ERRORP(res)) return res;

	gtk_text_insert( c_text, c_font, c_fore, c_back, c_chars, c_length );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_backward_delete( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EcInt                c_nchars;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_backward_delete", TRUE, stack, "O&i",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, &c_nchars );
	if (EC_ERRORP(res)) return res;

	gtk_text_backward_delete( c_text, c_nchars );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_text_forward_delete( EC_OBJ stack, EcAny userdata )
{
	GtkText *            c_text;
	EcInt                c_nchars;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.text_forward_delete", TRUE, stack, "O&i",
	                            object_cvt, &c_text, (EcDWord)gtk_object_get_type(), FALSE, &c_nchars );
	if (EC_ERRORP(res)) return res;

	gtk_text_forward_delete( c_text, c_nchars );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_alignment_new( EC_OBJ stack, EcAny userdata )
{
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EcFloat              c_xscale;
	EcFloat              c_yscale;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.alignment_new", TRUE, stack, "ffff",
	                            &c_xalign, &c_yalign, &c_xscale, &c_yscale );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_alignment_new( c_xalign, c_yalign, c_xscale, c_yscale ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_alignment_set( EC_OBJ stack, EcAny userdata )
{
	GtkAlignment *       c_alignment;
	EcFloat              c_xalign;
	EcFloat              c_yalign;
	EcFloat              c_xscale;
	EcFloat              c_yscale;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.alignment_set", TRUE, stack, "O&ffff",
	                            object_cvt, &c_alignment, (EcDWord)gtk_object_get_type(), FALSE, &c_xalign, &c_yalign, &c_xscale, &c_yscale );
	if (EC_ERRORP(res)) return res;

	gtk_alignment_set( c_alignment, c_xalign, c_yalign, c_xscale, c_yscale );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_drawing_area_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_drawing_area_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_drawing_area_size( EC_OBJ stack, EcAny userdata )
{
	GtkDrawingArea *     c_darea;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.drawing_area_size", TRUE, stack, "O&ii",
	                            object_cvt, &c_darea, (EcDWord)gtk_object_get_type(), FALSE, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gtk_drawing_area_size( c_darea, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_curve_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_curve_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_curve_reset( EC_OBJ stack, EcAny userdata )
{
	GtkCurve *           c_curve;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.curve_reset", TRUE, stack, "O&",
	                            object_cvt, &c_curve, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_curve_reset( c_curve );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_curve_set_gamma( EC_OBJ stack, EcAny userdata )
{
	GtkCurve *           c_curve;
	EcFloat              c_gamma;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.curve_set_gamma", TRUE, stack, "O&f",
	                            object_cvt, &c_curve, (EcDWord)gtk_object_get_type(), FALSE, &c_gamma );
	if (EC_ERRORP(res)) return res;

	gtk_curve_set_gamma( c_curve, c_gamma );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_curve_set_range( EC_OBJ stack, EcAny userdata )
{
	GtkCurve *           c_curve;
	EcFloat              c_min_x;
	EcFloat              c_max_x;
	EcFloat              c_min_y;
	EcFloat              c_max_y;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.curve_set_range", TRUE, stack, "O&ffff",
	                            object_cvt, &c_curve, (EcDWord)gtk_object_get_type(), FALSE, &c_min_x, &c_max_x, &c_min_y, &c_max_y );
	if (EC_ERRORP(res)) return res;

	gtk_curve_set_range( c_curve, c_min_x, c_max_x, c_min_y, c_max_y );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_curve_set_curve_type( EC_OBJ stack, EcAny userdata )
{
	GtkCurve *           c_curve;
	GtkCurveType         c_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.curve_set_curve_type", TRUE, stack, "O&O&",
	                            object_cvt, &c_curve, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_type, (EcDWord)&ec_gtk_t_gtk_curve_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_curve_set_curve_type( c_curve, c_type );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_new( EC_OBJ stack, EcAny userdata )
{
	GtkPreviewType       c_type;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_new", TRUE, stack, "O&",
	                            enum_cvt, &c_type, (EcDWord)&ec_gtk_t_gtk_preview_type, FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_preview_new( c_type ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_preview_size( EC_OBJ stack, EcAny userdata )
{
	GtkPreview *         c_preview;
	EcInt                c_width;
	EcInt                c_height;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_size", TRUE, stack, "O&ii",
	                            object_cvt, &c_preview, (EcDWord)gtk_object_get_type(), FALSE, &c_width, &c_height );
	if (EC_ERRORP(res)) return res;

	gtk_preview_size( c_preview, c_width, c_height );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_set_expand( EC_OBJ stack, EcAny userdata )
{
	GtkPreview *         c_preview;
	EcBool               c_expand;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_set_expand", TRUE, stack, "O&b",
	                            object_cvt, &c_preview, (EcDWord)gtk_object_get_type(), FALSE, &c_expand );
	if (EC_ERRORP(res)) return res;

	gtk_preview_set_expand( c_preview, c_expand );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_set_color_cube( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_nred_shades;
	EcInt                c_ngreen_shades;
	EcInt                c_nblue_shades;
	EcInt                c_ngray_shades;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_set_color_cube", TRUE, stack, "iiii",
	                            &c_nred_shades, &c_ngreen_shades, &c_nblue_shades, &c_ngray_shades );
	if (EC_ERRORP(res)) return res;

	gtk_preview_set_color_cube( c_nred_shades, c_ngreen_shades, c_nblue_shades, c_ngray_shades );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_set_install_cmap( EC_OBJ stack, EcAny userdata )
{
	EcBool               c_install_cmap;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_set_install_cmap", TRUE, stack, "b",
	                            &c_install_cmap );
	if (EC_ERRORP(res)) return res;

	gtk_preview_set_install_cmap( c_install_cmap );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_set_reserved( EC_OBJ stack, EcAny userdata )
{
	EcInt                c_nreserved;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.preview_set_reserved", TRUE, stack, "i",
	                            &c_nreserved );
	if (EC_ERRORP(res)) return res;

	gtk_preview_set_reserved( c_nreserved );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_preview_get_visual( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_preview_get_visual(  ), &ec_gdk_boxed[1], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_preview_get_cmap( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_boxed2obj( (gpointer)gtk_preview_get_cmap(  ), &ec_gdk_boxed[0], TRUE );

	return res;
}
static EC_OBJ ec_gtk_gtk_file_selection_new( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_title;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_new", TRUE, stack, "s",
	                            &c_title );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_file_selection_new( c_title ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_file_selection_set_filename( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_filesel;
	EcChar *             c_filename;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_set_filename", TRUE, stack, "O&s",
	                            object_cvt, &c_filesel, (EcDWord)gtk_object_get_type(), FALSE, &c_filename );
	if (EC_ERRORP(res)) return res;

	gtk_file_selection_set_filename( c_filesel, c_filename );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_file_selection_get_filename( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_filesel;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_get_filename", TRUE, stack, "O&",
	                            object_cvt, &c_filesel, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeString( (const char *)(gtk_file_selection_get_filename( c_filesel )), 0 );

	return res;
}
static EC_OBJ ec_gtk_gtk_file_selection_show_fileop_buttons( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_filesel;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_show_fileop_buttons", TRUE, stack, "O&",
	                            object_cvt, &c_filesel, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_file_selection_show_fileop_buttons( c_filesel );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_file_selection_hide_fileop_buttons( EC_OBJ stack, EcAny userdata )
{
	GtkFileSelection *   c_filesel;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.file_selection_hide_fileop_buttons", TRUE, stack, "O&",
	                            object_cvt, &c_filesel, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_file_selection_hide_fileop_buttons( c_filesel );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_color_selection_dialog_new( EC_OBJ stack, EcAny userdata )
{
	EcChar *             c_title;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.color_selection_dialog_new", TRUE, stack, "s",
	                            &c_title );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_color_selection_dialog_new( c_title ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_color_selection_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_color_selection_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_color_selection_set_update_policy( EC_OBJ stack, EcAny userdata )
{
	GtkColorSelection *  c_colorsel;
	GtkUpdateType        c_policy;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.color_selection_set_update_policy", TRUE, stack, "O&O&",
	                            object_cvt, &c_colorsel, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_policy, (EcDWord)&ec_gtk_t_gtk_update_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_color_selection_set_update_policy( c_colorsel, c_policy );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_color_selection_set_opacity( EC_OBJ stack, EcAny userdata )
{
	GtkColorSelection *  c_colorsel;
	EcBool               c_use_opacity;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.color_selection_set_opacity", TRUE, stack, "O&b",
	                            object_cvt, &c_colorsel, (EcDWord)gtk_object_get_type(), FALSE, &c_use_opacity );
	if (EC_ERRORP(res)) return res;

	gtk_color_selection_set_opacity( c_colorsel, c_use_opacity );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_range_get_adjustment( EC_OBJ stack, EcAny userdata )
{
	GtkRange *           c_range;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.range_get_adjustment", TRUE, stack, "O&",
	                            object_cvt, &c_range, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_range_get_adjustment( c_range ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_range_set_update_policy( EC_OBJ stack, EcAny userdata )
{
	GtkRange *           c_range;
	GtkUpdateType        c_policy;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.range_set_update_policy", TRUE, stack, "O&O&",
	                            object_cvt, &c_range, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_policy, (EcDWord)&ec_gtk_t_gtk_update_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_range_set_update_policy( c_range, c_policy );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_range_set_adjustment( EC_OBJ stack, EcAny userdata )
{
	GtkRange *           c_range;
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.range_set_adjustment", TRUE, stack, "O&O&",
	                            object_cvt, &c_range, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_range_set_adjustment( c_range, c_adjustment );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_scale_set_digits( EC_OBJ stack, EcAny userdata )
{
	GtkScale *           c_scale;
	EcInt                c_digits;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scale_set_digits", TRUE, stack, "O&i",
	                            object_cvt, &c_scale, (EcDWord)gtk_object_get_type(), FALSE, &c_digits );
	if (EC_ERRORP(res)) return res;

	gtk_scale_set_digits( c_scale, c_digits );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_scale_set_draw_value( EC_OBJ stack, EcAny userdata )
{
	GtkScale *           c_scale;
	EcBool               c_draw_value;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scale_set_draw_value", TRUE, stack, "O&b",
	                            object_cvt, &c_scale, (EcDWord)gtk_object_get_type(), FALSE, &c_draw_value );
	if (EC_ERRORP(res)) return res;

	gtk_scale_set_draw_value( c_scale, c_draw_value );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_scale_set_value_pos( EC_OBJ stack, EcAny userdata )
{
	GtkScale *           c_scale;
	GtkPositionType      c_pos;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scale_set_value_pos", TRUE, stack, "O&O&",
	                            object_cvt, &c_scale, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_pos, (EcDWord)&ec_gtk_t_gtk_position_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_scale_set_value_pos( c_scale, c_pos );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_scale_value_width( EC_OBJ stack, EcAny userdata )
{
	GtkScale *           c_scale;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scale_value_width", TRUE, stack, "O&",
	                            object_cvt, &c_scale, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = EcMakeInt( (EcInt)(gtk_scale_value_width( c_scale )) );

	return res;
}
static EC_OBJ ec_gtk_gtk_scale_draw_value( EC_OBJ stack, EcAny userdata )
{
	GtkScale *           c_scale;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.scale_draw_value", TRUE, stack, "O&",
	                            object_cvt, &c_scale, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_scale_draw_value( c_scale );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_hscale_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.hscale_new", TRUE, stack, "O&",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hscale_new( c_adjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vscale_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.vscale_new", TRUE, stack, "O&",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vscale_new( c_adjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_hscrollbar_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.hscrollbar_new", TRUE, stack, "O&",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hscrollbar_new( c_adjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vscrollbar_new( EC_OBJ stack, EcAny userdata )
{
	GtkAdjustment *      c_adjustment;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.vscrollbar_new", TRUE, stack, "O&",
	                            object_cvt, &c_adjustment, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vscrollbar_new( c_adjustment ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_ruler_set_metric( EC_OBJ stack, EcAny userdata )
{
	GtkRuler *           c_ruler;
	GtkMetricType        c_metric;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.ruler_set_metric", TRUE, stack, "O&O&",
	                            object_cvt, &c_ruler, (EcDWord)gtk_object_get_type(), FALSE, enum_cvt, &c_metric, (EcDWord)&ec_gtk_t_gtk_metric_type, FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_ruler_set_metric( c_ruler, c_metric );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_ruler_set_range( EC_OBJ stack, EcAny userdata )
{
	GtkRuler *           c_ruler;
	EcFloat              c_lower;
	EcFloat              c_upper;
	EcFloat              c_position;
	EcFloat              c_max_size;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.ruler_set_range", TRUE, stack, "O&ffff",
	                            object_cvt, &c_ruler, (EcDWord)gtk_object_get_type(), FALSE, &c_lower, &c_upper, &c_position, &c_max_size );
	if (EC_ERRORP(res)) return res;

	gtk_ruler_set_range( c_ruler, c_lower, c_upper, c_position, c_max_size );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_hruler_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hruler_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vruler_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vruler_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_dialog_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_dialog_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_input_dialog_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_input_dialog_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_paned_add1( EC_OBJ stack, EcAny userdata )
{
	GtkPaned *           c_paned;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.paned_add1", TRUE, stack, "O&O&",
	                            object_cvt, &c_paned, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_paned_add1( c_paned, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_paned_add2( EC_OBJ stack, EcAny userdata )
{
	GtkPaned *           c_paned;
	GtkWidget *          c_child;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.paned_add2", TRUE, stack, "O&O&",
	                            object_cvt, &c_paned, (EcDWord)gtk_object_get_type(), FALSE, object_cvt, &c_child, (EcDWord)gtk_object_get_type(), FALSE );
	if (EC_ERRORP(res)) return res;

	gtk_paned_add2( c_paned, c_child );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_paned_handle_size( EC_OBJ stack, EcAny userdata )
{
	GtkPaned *           c_paned;
	EcInt                c_size;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.paned_handle_size", TRUE, stack, "O&i",
	                            object_cvt, &c_paned, (EcDWord)gtk_object_get_type(), FALSE, &c_size );
	if (EC_ERRORP(res)) return res;

	gtk_paned_handle_size( c_paned, c_size );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_paned_gutter_size( EC_OBJ stack, EcAny userdata )
{
	GtkPaned *           c_paned;
	EcInt                c_size;
	EC_OBJ               res;

	res = EcParseStackFunction( "gtk.paned_gutter_size", TRUE, stack, "O&i",
	                            object_cvt, &c_paned, (EcDWord)gtk_object_get_type(), FALSE, &c_size );
	if (EC_ERRORP(res)) return res;

	gtk_paned_gutter_size( c_paned, c_size );

	return EC_NIL;
}
static EC_OBJ ec_gtk_gtk_hpaned_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_hpaned_new(  ) );

	return res;
}
static EC_OBJ ec_gtk_gtk_vpaned_new( EC_OBJ stack, EcAny userdata )
{
	EC_OBJ               res;

	res = ec_gtk_object2obj( (GtkObject *)gtk_vpaned_new(  ) );

	return res;
}

/* ------------------------------------------------------------------------
 *  Intialization function
 * ------------------------------------------------------------------------ */

static void ec_gtk_register_functions( void )
{
	EcAddPrimitive( "gtk.exit",                                       ec_gtk_gtk_exit                                    );
	EcAddPrimitive( "gtk.main",                                       ec_gtk_gtk_main                                    );
	EcAddPrimitive( "gtk.main_level",                                 ec_gtk_gtk_main_level                              );
	EcAddPrimitive( "gtk.main_quit",                                  ec_gtk_gtk_main_quit                               );
	EcAddPrimitive( "gtk.main_iteration",                             ec_gtk_gtk_main_iteration                          );
	EcAddPrimitive( "gtk.events_pending",                             ec_gtk_gtk_events_pending                          );
	EcAddPrimitive( "gtk.timeout_add_full",                           ec_gtk_gtk_timeout_add_full                        );
	EcAddPrimitive( "gtk.timeout_remove",                             ec_gtk_gtk_timeout_remove                          );
	EcAddPrimitive( "gtk.idle_add_full",                              ec_gtk_gtk_idle_add_full                           );
	EcAddPrimitive( "gtk.idle_remove",                                ec_gtk_gtk_idle_remove                             );
	EcAddPrimitive( "gtk.input_remove",                               ec_gtk_gtk_input_remove                            );
	EcAddPrimitive( "gtk.rc_parse",                                   ec_gtk_gtk_rc_parse                                );
	EcAddPrimitive( "gtk.g_mem_chunk_info",                           ec_gtk_g_mem_chunk_info                            );
	EcAddPrimitive( "gtk.object_destroy",                             ec_gtk_gtk_object_destroy                          );
	EcAddPrimitive( "gtk.OBJECT_DESTROYED",                           ec_gtk_GTK_OBJECT_DESTROYED                        );
	EcAddPrimitive( "gtk.signal_connect_full",                        ec_gtk_gtk_signal_connect_full                     );
	EcAddPrimitive( "gtk.signal_disconnect",                          ec_gtk_gtk_signal_disconnect                       );
	EcAddPrimitive( "gtk.widget_getf",                                ec_gtk_gtk_widget_getf                             );
	EcAddPrimitive( "gtk.widget_setf",                                ec_gtk_gtk_widget_setf                             );
	EcAddPrimitive( "gtk.grab_add",                                   ec_gtk_gtk_grab_add                                );
	EcAddPrimitive( "gtk.grab_remove",                                ec_gtk_gtk_grab_remove                             );
	EcAddPrimitive( "gtk.WIDGET_STATE",                               ec_gtk_GTK_WIDGET_STATE                            );
	EcAddPrimitive( "gtk.WIDGET_FLAGS",                               ec_gtk_GTK_WIDGET_FLAGS                            );
	EcAddPrimitive( "gtk.WIDGET_SET_FLAGS",                           ec_gtk_GTK_WIDGET_SET_FLAGS                        );
	EcAddPrimitive( "gtk.WIDGET_UNSET_FLAGS",                         ec_gtk_GTK_WIDGET_UNSET_FLAGS                      );
	EcAddPrimitive( "gtk.widget_destroy",                             ec_gtk_gtk_widget_destroy                          );
	EcAddPrimitive( "gtk.widget_unparent",                            ec_gtk_gtk_widget_unparent                         );
	EcAddPrimitive( "gtk.widget_show",                                ec_gtk_gtk_widget_show                             );
	EcAddPrimitive( "gtk.widget_hide",                                ec_gtk_gtk_widget_hide                             );
	EcAddPrimitive( "gtk.widget_show_all",                            ec_gtk_gtk_widget_show_all                         );
	EcAddPrimitive( "gtk.widget_hide_all",                            ec_gtk_gtk_widget_hide_all                         );
	EcAddPrimitive( "gtk.widget_map",                                 ec_gtk_gtk_widget_map                              );
	EcAddPrimitive( "gtk.widget_unmap",                               ec_gtk_gtk_widget_unmap                            );
	EcAddPrimitive( "gtk.widget_realize",                             ec_gtk_gtk_widget_realize                          );
	EcAddPrimitive( "gtk.widget_unrealize",                           ec_gtk_gtk_widget_unrealize                        );
	EcAddPrimitive( "gtk.widget_event",                               ec_gtk_gtk_widget_event                            );
	EcAddPrimitive( "gtk.widget_activate",                            ec_gtk_gtk_widget_activate                         );
	EcAddPrimitive( "gtk.widget_reparent",                            ec_gtk_gtk_widget_reparent                         );
	EcAddPrimitive( "gtk.widget_popup",                               ec_gtk_gtk_widget_popup                            );
	EcAddPrimitive( "gtk.widget_grab_focus",                          ec_gtk_gtk_widget_grab_focus                       );
	EcAddPrimitive( "gtk.widget_grab_default",                        ec_gtk_gtk_widget_grab_default                     );
	EcAddPrimitive( "gtk.widget_set_name",                            ec_gtk_gtk_widget_set_name                         );
	EcAddPrimitive( "gtk.widget_get_name",                            ec_gtk_gtk_widget_get_name                         );
	EcAddPrimitive( "gtk.widget_set_state",                           ec_gtk_gtk_widget_set_state                        );
	EcAddPrimitive( "gtk.widget_set_sensitive",                       ec_gtk_gtk_widget_set_sensitive                    );
	EcAddPrimitive( "gtk.widget_set_parent",                          ec_gtk_gtk_widget_set_parent                       );
	EcAddPrimitive( "gtk.widget_set_style",                           ec_gtk_gtk_widget_set_style                        );
	EcAddPrimitive( "gtk.widget_set_uposition",                       ec_gtk_gtk_widget_set_uposition                    );
	EcAddPrimitive( "gtk.widget_set_usize",                           ec_gtk_gtk_widget_set_usize                        );
	EcAddPrimitive( "gtk.widget_set_events",                          ec_gtk_gtk_widget_set_events                       );
	EcAddPrimitive( "gtk.widget_set_extension_events",                ec_gtk_gtk_widget_set_extension_events             );
	EcAddPrimitive( "gtk.widget_get_toplevel",                        ec_gtk_gtk_widget_get_toplevel                     );
	EcAddPrimitive( "gtk.widget_get_colormap",                        ec_gtk_gtk_widget_get_colormap                     );
	EcAddPrimitive( "gtk.widget_get_visual",                          ec_gtk_gtk_widget_get_visual                       );
	EcAddPrimitive( "gtk.widget_get_style",                           ec_gtk_gtk_widget_get_style                        );
	EcAddPrimitive( "gtk.widget_get_events",                          ec_gtk_gtk_widget_get_events                       );
	EcAddPrimitive( "gtk.widget_get_extension_events",                ec_gtk_gtk_widget_get_extension_events             );
	EcAddPrimitive( "gtk.widget_is_ancestor",                         ec_gtk_gtk_widget_is_ancestor                      );
	EcAddPrimitive( "gtk.widget_push_colormap",                       ec_gtk_gtk_widget_push_colormap                    );
	EcAddPrimitive( "gtk.widget_push_visual",                         ec_gtk_gtk_widget_push_visual                      );
	EcAddPrimitive( "gtk.widget_push_style",                          ec_gtk_gtk_widget_push_style                       );
	EcAddPrimitive( "gtk.widget_pop_colormap",                        ec_gtk_gtk_widget_pop_colormap                     );
	EcAddPrimitive( "gtk.widget_pop_visual",                          ec_gtk_gtk_widget_pop_visual                       );
	EcAddPrimitive( "gtk.widget_pop_style",                           ec_gtk_gtk_widget_pop_style                        );
	EcAddPrimitive( "gtk.widget_set_default_colormap",                ec_gtk_gtk_widget_set_default_colormap             );
	EcAddPrimitive( "gtk.widget_set_default_visual",                  ec_gtk_gtk_widget_set_default_visual               );
	EcAddPrimitive( "gtk.widget_set_default_style",                   ec_gtk_gtk_widget_set_default_style                );
	EcAddPrimitive( "gtk.widget_get_default_colormap",                ec_gtk_gtk_widget_get_default_colormap             );
	EcAddPrimitive( "gtk.widget_get_default_visual",                  ec_gtk_gtk_widget_get_default_visual               );
	EcAddPrimitive( "gtk.widget_get_default_style",                   ec_gtk_gtk_widget_get_default_style                );
	EcAddPrimitive( "gtk.container_border_width",                     ec_gtk_gtk_container_border_width                  );
	EcAddPrimitive( "gtk.container_add",                              ec_gtk_gtk_container_add                           );
	EcAddPrimitive( "gtk.container_remove",                           ec_gtk_gtk_container_remove                        );
	EcAddPrimitive( "gtk.container_foreach_full",                     ec_gtk_gtk_container_foreach_full                  );
	EcAddPrimitive( "gtk.container_focus",                            ec_gtk_gtk_container_focus                         );
	EcAddPrimitive( "gtk.container_register_toplevel",                ec_gtk_gtk_container_register_toplevel             );
	EcAddPrimitive( "gtk.container_unregister_toplevel",              ec_gtk_gtk_container_unregister_toplevel           );
	EcAddPrimitive( "gtk.fixed_new",                                  ec_gtk_gtk_fixed_new                               );
	EcAddPrimitive( "gtk.fixed_put",                                  ec_gtk_gtk_fixed_put                               );
	EcAddPrimitive( "gtk.fixed_move",                                 ec_gtk_gtk_fixed_move                              );
	EcAddPrimitive( "gtk.tree_item_new",                              ec_gtk_gtk_tree_item_new                           );
	EcAddPrimitive( "gtk.tree_item_new_with_label",                   ec_gtk_gtk_tree_item_new_with_label                );
	EcAddPrimitive( "gtk.tree_item_set_subtree",                      ec_gtk_gtk_tree_item_set_subtree                   );
	EcAddPrimitive( "gtk.tree_item_remove_subtree",                   ec_gtk_gtk_tree_item_remove_subtree                );
	EcAddPrimitive( "gtk.tree_item_select",                           ec_gtk_gtk_tree_item_select                        );
	EcAddPrimitive( "gtk.tree_item_deselect",                         ec_gtk_gtk_tree_item_deselect                      );
	EcAddPrimitive( "gtk.tree_item_expand",                           ec_gtk_gtk_tree_item_expand                        );
	EcAddPrimitive( "gtk.tree_item_collapse",                         ec_gtk_gtk_tree_item_collapse                      );
	EcAddPrimitive( "gtk.tree_new",                                   ec_gtk_gtk_tree_new                                );
	EcAddPrimitive( "gtk.tree_append",                                ec_gtk_gtk_tree_append                             );
	EcAddPrimitive( "gtk.tree_prepend",                               ec_gtk_gtk_tree_prepend                            );
	EcAddPrimitive( "gtk.tree_insert",                                ec_gtk_gtk_tree_insert                             );
	EcAddPrimitive( "gtk.tree_remove_item",                           ec_gtk_gtk_tree_remove_item                        );
	EcAddPrimitive( "gtk.tree_clear_items",                           ec_gtk_gtk_tree_clear_items                        );
	EcAddPrimitive( "gtk.tree_select_item",                           ec_gtk_gtk_tree_select_item                        );
	EcAddPrimitive( "gtk.tree_unselect_item",                         ec_gtk_gtk_tree_unselect_item                      );
	EcAddPrimitive( "gtk.tree_select_child",                          ec_gtk_gtk_tree_select_child                       );
	EcAddPrimitive( "gtk.tree_unselect_child",                        ec_gtk_gtk_tree_unselect_child                     );
	EcAddPrimitive( "gtk.tree_child_position",                        ec_gtk_gtk_tree_child_position                     );
	EcAddPrimitive( "gtk.tree_set_selection_mode",                    ec_gtk_gtk_tree_set_selection_mode                 );
	EcAddPrimitive( "gtk.tree_set_view_mode",                         ec_gtk_gtk_tree_set_view_mode                      );
	EcAddPrimitive( "gtk.tree_set_view_lines",                        ec_gtk_gtk_tree_set_view_lines                     );
	EcAddPrimitive( "gtk.clist_new",                                  ec_gtk_gtk_clist_new                               );
	EcAddPrimitive( "gtk.clist_set_selection_mode",                   ec_gtk_gtk_clist_set_selection_mode                );
	EcAddPrimitive( "gtk.clist_freeze",                               ec_gtk_gtk_clist_freeze                            );
	EcAddPrimitive( "gtk.clist_thaw",                                 ec_gtk_gtk_clist_thaw                              );
	EcAddPrimitive( "gtk.clist_column_titles_show",                   ec_gtk_gtk_clist_column_titles_show                );
	EcAddPrimitive( "gtk.clist_column_titles_hide",                   ec_gtk_gtk_clist_column_titles_hide                );
	EcAddPrimitive( "gtk.clist_column_title_active",                  ec_gtk_gtk_clist_column_title_active               );
	EcAddPrimitive( "gtk.clist_column_title_passive",                 ec_gtk_gtk_clist_column_title_passive              );
	EcAddPrimitive( "gtk.clist_column_titles_active",                 ec_gtk_gtk_clist_column_titles_active              );
	EcAddPrimitive( "gtk.clist_column_titles_passive",                ec_gtk_gtk_clist_column_titles_passive             );
	EcAddPrimitive( "gtk.clist_set_column_title",                     ec_gtk_gtk_clist_set_column_title                  );
	EcAddPrimitive( "gtk.clist_set_column_widget",                    ec_gtk_gtk_clist_set_column_widget                 );
	EcAddPrimitive( "gtk.clist_set_column_justification",             ec_gtk_gtk_clist_set_column_justification          );
	EcAddPrimitive( "gtk.clist_set_column_width",                     ec_gtk_gtk_clist_set_column_width                  );
	EcAddPrimitive( "gtk.clist_set_row_height",                       ec_gtk_gtk_clist_set_row_height                    );
	EcAddPrimitive( "gtk.clist_moveto",                               ec_gtk_gtk_clist_moveto                            );
	EcAddPrimitive( "gtk.clist_row_is_visible",                       ec_gtk_gtk_clist_row_is_visible                    );
	EcAddPrimitive( "gtk.clist_set_text",                             ec_gtk_gtk_clist_set_text                          );
	EcAddPrimitive( "gtk.clist_set_foreground",                       ec_gtk_gtk_clist_set_foreground                    );
	EcAddPrimitive( "gtk.clist_set_background",                       ec_gtk_gtk_clist_set_background                    );
	EcAddPrimitive( "gtk.clist_set_shift",                            ec_gtk_gtk_clist_set_shift                         );
	EcAddPrimitive( "gtk.clist_select_row",                           ec_gtk_gtk_clist_select_row                        );
	EcAddPrimitive( "gtk.clist_unselect_row",                         ec_gtk_gtk_clist_unselect_row                      );
	EcAddPrimitive( "gtk.clist_clear",                                ec_gtk_gtk_clist_clear                             );
	EcAddPrimitive( "gtk.event_box_new",                              ec_gtk_gtk_event_box_new                           );
	EcAddPrimitive( "gtk.window_new",                                 ec_gtk_gtk_window_new                              );
	EcAddPrimitive( "gtk.window_set_title",                           ec_gtk_gtk_window_set_title                        );
	EcAddPrimitive( "gtk.window_set_wmclass",                         ec_gtk_gtk_window_set_wmclass                      );
	EcAddPrimitive( "gtk.window_set_focus",                           ec_gtk_gtk_window_set_focus                        );
	EcAddPrimitive( "gtk.window_set_default",                         ec_gtk_gtk_window_set_default                      );
	EcAddPrimitive( "gtk.window_set_policy",                          ec_gtk_gtk_window_set_policy                       );
	EcAddPrimitive( "gtk.window_position",                            ec_gtk_gtk_window_position                         );
	EcAddPrimitive( "gtk.window_activate_focus",                      ec_gtk_gtk_window_activate_focus                   );
	EcAddPrimitive( "gtk.window_activate_default",                    ec_gtk_gtk_window_activate_default                 );
	EcAddPrimitive( "gtk.window_set_default_size",                    ec_gtk_gtk_window_set_default_size                 );
	EcAddPrimitive( "gtk.window_set_modal",                           ec_gtk_gtk_window_set_modal                        );
	EcAddPrimitive( "gtk.handle_box_new",                             ec_gtk_gtk_handle_box_new                          );
	EcAddPrimitive( "gtk.box_pack_start",                             ec_gtk_gtk_box_pack_start                          );
	EcAddPrimitive( "gtk.box_pack_end",                               ec_gtk_gtk_box_pack_end                            );
	EcAddPrimitive( "gtk.box_pack_start_defaults",                    ec_gtk_gtk_box_pack_start_defaults                 );
	EcAddPrimitive( "gtk.box_pack_end_defaults",                      ec_gtk_gtk_box_pack_end_defaults                   );
	EcAddPrimitive( "gtk.box_set_homogeneous",                        ec_gtk_gtk_box_set_homogeneous                     );
	EcAddPrimitive( "gtk.box_set_spacing",                            ec_gtk_gtk_box_set_spacing                         );
	EcAddPrimitive( "gtk.box_reorder_child",                          ec_gtk_gtk_box_reorder_child                       );
	EcAddPrimitive( "gtk.box_set_child_packing",                      ec_gtk_gtk_box_set_child_packing                   );
	EcAddPrimitive( "gtk.button_box_set_child_size_default",          ec_gtk_gtk_button_box_set_child_size_default       );
	EcAddPrimitive( "gtk.button_box_set_child_ipadding_default",      ec_gtk_gtk_button_box_set_child_ipadding_default   );
	EcAddPrimitive( "gtk.button_box_get_spacing",                     ec_gtk_gtk_button_box_get_spacing                  );
	EcAddPrimitive( "gtk.button_box_get_layout",                      ec_gtk_gtk_button_box_get_layout                   );
	EcAddPrimitive( "gtk.button_box_set_spacing",                     ec_gtk_gtk_button_box_set_spacing                  );
	EcAddPrimitive( "gtk.button_box_set_layout",                      ec_gtk_gtk_button_box_set_layout                   );
	EcAddPrimitive( "gtk.button_box_set_child_size",                  ec_gtk_gtk_button_box_set_child_size               );
	EcAddPrimitive( "gtk.button_box_set_child_ipadding",              ec_gtk_gtk_button_box_set_child_ipadding           );
	EcAddPrimitive( "gtk.hbutton_box_new",                            ec_gtk_gtk_hbutton_box_new                         );
	EcAddPrimitive( "gtk.hbutton_box_get_spacing_default",            ec_gtk_gtk_hbutton_box_get_spacing_default         );
	EcAddPrimitive( "gtk.hbutton_box_get_layout_default",             ec_gtk_gtk_hbutton_box_get_layout_default          );
	EcAddPrimitive( "gtk.hbutton_box_set_spacing_default",            ec_gtk_gtk_hbutton_box_set_spacing_default         );
	EcAddPrimitive( "gtk.hbutton_box_set_layout_default",             ec_gtk_gtk_hbutton_box_set_layout_default          );
	EcAddPrimitive( "gtk.vbutton_box_new",                            ec_gtk_gtk_vbutton_box_new                         );
	EcAddPrimitive( "gtk.vbutton_box_get_spacing_default",            ec_gtk_gtk_vbutton_box_get_spacing_default         );
	EcAddPrimitive( "gtk.vbutton_box_get_layout_default",             ec_gtk_gtk_vbutton_box_get_layout_default          );
	EcAddPrimitive( "gtk.vbutton_box_set_spacing_default",            ec_gtk_gtk_vbutton_box_set_spacing_default         );
	EcAddPrimitive( "gtk.vbutton_box_set_layout_default",             ec_gtk_gtk_vbutton_box_set_layout_default          );
	EcAddPrimitive( "gtk.toolbar_new",                                ec_gtk_gtk_toolbar_new                             );
	EcAddPrimitive( "gtk.toolbar_append_space",                       ec_gtk_gtk_toolbar_append_space                    );
	EcAddPrimitive( "gtk.toolbar_prepend_space",                      ec_gtk_gtk_toolbar_prepend_space                   );
	EcAddPrimitive( "gtk.toolbar_insert_space",                       ec_gtk_gtk_toolbar_insert_space                    );
	EcAddPrimitive( "gtk.toolbar_append_widget",                      ec_gtk_gtk_toolbar_append_widget                   );
	EcAddPrimitive( "gtk.toolbar_prepend_widget",                     ec_gtk_gtk_toolbar_prepend_widget                  );
	EcAddPrimitive( "gtk.toolbar_insert_widget",                      ec_gtk_gtk_toolbar_insert_widget                   );
	EcAddPrimitive( "gtk.toolbar_set_orientation",                    ec_gtk_gtk_toolbar_set_orientation                 );
	EcAddPrimitive( "gtk.toolbar_set_style",                          ec_gtk_gtk_toolbar_set_style                       );
	EcAddPrimitive( "gtk.toolbar_set_space_size",                     ec_gtk_gtk_toolbar_set_space_size                  );
	EcAddPrimitive( "gtk.toolbar_set_tooltips",                       ec_gtk_gtk_toolbar_set_tooltips                    );
	EcAddPrimitive( "gtk.table_new",                                  ec_gtk_gtk_table_new                               );
	EcAddPrimitive( "gtk.table_attach",                               ec_gtk_gtk_table_attach                            );
	EcAddPrimitive( "gtk.table_attach_defaults",                      ec_gtk_gtk_table_attach_defaults                   );
	EcAddPrimitive( "gtk.table_set_row_spacing",                      ec_gtk_gtk_table_set_row_spacing                   );
	EcAddPrimitive( "gtk.table_set_col_spacing",                      ec_gtk_gtk_table_set_col_spacing                   );
	EcAddPrimitive( "gtk.table_set_row_spacings",                     ec_gtk_gtk_table_set_row_spacings                  );
	EcAddPrimitive( "gtk.table_set_col_spacings",                     ec_gtk_gtk_table_set_col_spacings                  );
	EcAddPrimitive( "gtk.button_getf",                                ec_gtk_gtk_button_getf                             );
	EcAddPrimitive( "gtk.button_setf",                                ec_gtk_gtk_button_setf                             );
	EcAddPrimitive( "gtk.button_new",                                 ec_gtk_gtk_button_new                              );
	EcAddPrimitive( "gtk.button_new_with_label",                      ec_gtk_gtk_button_new_with_label                   );
	EcAddPrimitive( "gtk.button_pressed",                             ec_gtk_gtk_button_pressed                          );
	EcAddPrimitive( "gtk.button_released",                            ec_gtk_gtk_button_released                         );
	EcAddPrimitive( "gtk.button_clicked",                             ec_gtk_gtk_button_clicked                          );
	EcAddPrimitive( "gtk.button_enter",                               ec_gtk_gtk_button_enter                            );
	EcAddPrimitive( "gtk.button_leave",                               ec_gtk_gtk_button_leave                            );
	EcAddPrimitive( "gtk.toggle_button_getf",                         ec_gtk_gtk_toggle_button_getf                      );
	EcAddPrimitive( "gtk.toggle_button_setf",                         ec_gtk_gtk_toggle_button_setf                      );
	EcAddPrimitive( "gtk.toggle_button_new",                          ec_gtk_gtk_toggle_button_new                       );
	EcAddPrimitive( "gtk.toggle_button_new_with_label",               ec_gtk_gtk_toggle_button_new_with_label            );
	EcAddPrimitive( "gtk.toggle_button_set_mode",                     ec_gtk_gtk_toggle_button_set_mode                  );
	EcAddPrimitive( "gtk.toggle_button_set_state",                    ec_gtk_gtk_toggle_button_set_state                 );
	EcAddPrimitive( "gtk.toggle_button_toggled",                      ec_gtk_gtk_toggle_button_toggled                   );
	EcAddPrimitive( "gtk.check_button_new",                           ec_gtk_gtk_check_button_new                        );
	EcAddPrimitive( "gtk.check_button_new_with_label",                ec_gtk_gtk_check_button_new_with_label             );
	EcAddPrimitive( "gtk.radio_button_new_from_widget",               ec_gtk_gtk_radio_button_new_from_widget            );
	EcAddPrimitive( "gtk.radio_button_new_with_label_from_widget",    ec_gtk_gtk_radio_button_new_with_label_from_widget );
	EcAddPrimitive( "gtk.misc_set_alignment",                         ec_gtk_gtk_misc_set_alignment                      );
	EcAddPrimitive( "gtk.misc_set_padding",                           ec_gtk_gtk_misc_set_padding                        );
	EcAddPrimitive( "gtk.arrow_new",                                  ec_gtk_gtk_arrow_new                               );
	EcAddPrimitive( "gtk.arrow_set",                                  ec_gtk_gtk_arrow_set                               );
	EcAddPrimitive( "gtk.label_new",                                  ec_gtk_gtk_label_new                               );
	EcAddPrimitive( "gtk.label_set",                                  ec_gtk_gtk_label_set                               );
	EcAddPrimitive( "gtk.tips_query_new",                             ec_gtk_gtk_tips_query_new                          );
	EcAddPrimitive( "gtk.tips_query_start_query",                     ec_gtk_gtk_tips_query_start_query                  );
	EcAddPrimitive( "gtk.tips_query_stop_query",                      ec_gtk_gtk_tips_query_stop_query                   );
	EcAddPrimitive( "gtk.tips_query_set_caller",                      ec_gtk_gtk_tips_query_set_caller                   );
	EcAddPrimitive( "gtk.tips_query_set_labels",                      ec_gtk_gtk_tips_query_set_labels                   );
	EcAddPrimitive( "gtk.vbox_new",                                   ec_gtk_gtk_vbox_new                                );
	EcAddPrimitive( "gtk.hbox_new",                                   ec_gtk_gtk_hbox_new                                );
	EcAddPrimitive( "gtk.item_select",                                ec_gtk_gtk_item_select                             );
	EcAddPrimitive( "gtk.item_deselect",                              ec_gtk_gtk_item_deselect                           );
	EcAddPrimitive( "gtk.item_toggle",                                ec_gtk_gtk_item_toggle                             );
	EcAddPrimitive( "gtk.combo_getf",                                 ec_gtk_gtk_combo_getf                              );
	EcAddPrimitive( "gtk.combo_setf",                                 ec_gtk_gtk_combo_setf                              );
	EcAddPrimitive( "gtk.combo_new",                                  ec_gtk_gtk_combo_new                               );
	EcAddPrimitive( "gtk.combo_set_value_in_list",                    ec_gtk_gtk_combo_set_value_in_list                 );
	EcAddPrimitive( "gtk.combo_set_use_arrows",                       ec_gtk_gtk_combo_set_use_arrows                    );
	EcAddPrimitive( "gtk.combo_set_use_arrows_always",                ec_gtk_gtk_combo_set_use_arrows_always             );
	EcAddPrimitive( "gtk.combo_set_case_sensitive",                   ec_gtk_gtk_combo_set_case_sensitive                );
	EcAddPrimitive( "gtk.combo_set_item_string",                      ec_gtk_gtk_combo_set_item_string                   );
	EcAddPrimitive( "gtk.combo_disable_activate",                     ec_gtk_gtk_combo_disable_activate                  );
	EcAddPrimitive( "gtk.statusbar_new",                              ec_gtk_gtk_statusbar_new                           );
	EcAddPrimitive( "gtk.statusbar_push",                             ec_gtk_gtk_statusbar_push                          );
	EcAddPrimitive( "gtk.statusbar_pop",                              ec_gtk_gtk_statusbar_pop                           );
	EcAddPrimitive( "gtk.statusbar_get_context_id",                   ec_gtk_gtk_statusbar_get_context_id                );
	EcAddPrimitive( "gtk.statusbar_remove",                           ec_gtk_gtk_statusbar_remove                        );
	EcAddPrimitive( "gtk.gamma_curve_getf",                           ec_gtk_gtk_gamma_curve_getf                        );
	EcAddPrimitive( "gtk.gamma_curve_setf",                           ec_gtk_gtk_gamma_curve_setf                        );
	EcAddPrimitive( "gtk.gamma_curve_new",                            ec_gtk_gtk_gamma_curve_new                         );
	EcAddPrimitive( "gtk.hseparator_new",                             ec_gtk_gtk_hseparator_new                          );
	EcAddPrimitive( "gtk.vseparator_new",                             ec_gtk_gtk_vseparator_new                          );
	EcAddPrimitive( "gtk.frame_new",                                  ec_gtk_gtk_frame_new                               );
	EcAddPrimitive( "gtk.frame_set_label",                            ec_gtk_gtk_frame_set_label                         );
	EcAddPrimitive( "gtk.frame_set_label_align",                      ec_gtk_gtk_frame_set_label_align                   );
	EcAddPrimitive( "gtk.frame_set_shadow_type",                      ec_gtk_gtk_frame_set_shadow_type                   );
	EcAddPrimitive( "gtk.aspect_frame_new",                           ec_gtk_gtk_aspect_frame_new                        );
	EcAddPrimitive( "gtk.aspect_frame_set",                           ec_gtk_gtk_aspect_frame_set                        );
	EcAddPrimitive( "gtk.progress_bar_new",                           ec_gtk_gtk_progress_bar_new                        );
	EcAddPrimitive( "gtk.progress_bar_update",                        ec_gtk_gtk_progress_bar_update                     );
	EcAddPrimitive( "gtk.tooltips_new",                               ec_gtk_gtk_tooltips_new                            );
	EcAddPrimitive( "gtk.tooltips_enable",                            ec_gtk_gtk_tooltips_enable                         );
	EcAddPrimitive( "gtk.tooltips_disable",                           ec_gtk_gtk_tooltips_disable                        );
	EcAddPrimitive( "gtk.tooltips_set_delay",                         ec_gtk_gtk_tooltips_set_delay                      );
	EcAddPrimitive( "gtk.tooltips_set_tip",                           ec_gtk_gtk_tooltips_set_tip                        );
	EcAddPrimitive( "gtk.tooltips_set_colors",                        ec_gtk_gtk_tooltips_set_colors                     );
	EcAddPrimitive( "gtk.menu_shell_append",                          ec_gtk_gtk_menu_shell_append                       );
	EcAddPrimitive( "gtk.menu_shell_prepend",                         ec_gtk_gtk_menu_shell_prepend                      );
	EcAddPrimitive( "gtk.menu_shell_insert",                          ec_gtk_gtk_menu_shell_insert                       );
	EcAddPrimitive( "gtk.menu_shell_deactivate",                      ec_gtk_gtk_menu_shell_deactivate                   );
	EcAddPrimitive( "gtk.menu_bar_new",                               ec_gtk_gtk_menu_bar_new                            );
	EcAddPrimitive( "gtk.menu_bar_append",                            ec_gtk_gtk_menu_bar_append                         );
	EcAddPrimitive( "gtk.menu_bar_prepend",                           ec_gtk_gtk_menu_bar_prepend                        );
	EcAddPrimitive( "gtk.menu_bar_insert",                            ec_gtk_gtk_menu_bar_insert                         );
	EcAddPrimitive( "gtk.menu_new",                                   ec_gtk_gtk_menu_new                                );
	EcAddPrimitive( "gtk.menu_append",                                ec_gtk_gtk_menu_append                             );
	EcAddPrimitive( "gtk.menu_prepend",                               ec_gtk_gtk_menu_prepend                            );
	EcAddPrimitive( "gtk.menu_insert",                                ec_gtk_gtk_menu_insert                             );
	EcAddPrimitive( "gtk.menu_popdown",                               ec_gtk_gtk_menu_popdown                            );
	EcAddPrimitive( "gtk.menu_get_active",                            ec_gtk_gtk_menu_get_active                         );
	EcAddPrimitive( "gtk.menu_set_active",                            ec_gtk_gtk_menu_set_active                         );
	EcAddPrimitive( "gtk.menu_get_attach_widget",                     ec_gtk_gtk_menu_get_attach_widget                  );
	EcAddPrimitive( "gtk.menu_detach",                                ec_gtk_gtk_menu_detach                             );
	EcAddPrimitive( "gtk.menu_item_new",                              ec_gtk_gtk_menu_item_new                           );
	EcAddPrimitive( "gtk.menu_item_new_with_label",                   ec_gtk_gtk_menu_item_new_with_label                );
	EcAddPrimitive( "gtk.menu_item_set_submenu",                      ec_gtk_gtk_menu_item_set_submenu                   );
	EcAddPrimitive( "gtk.menu_item_remove_submenu",                   ec_gtk_gtk_menu_item_remove_submenu                );
	EcAddPrimitive( "gtk.menu_item_set_placement",                    ec_gtk_gtk_menu_item_set_placement                 );
	EcAddPrimitive( "gtk.menu_item_configure",                        ec_gtk_gtk_menu_item_configure                     );
	EcAddPrimitive( "gtk.menu_item_select",                           ec_gtk_gtk_menu_item_select                        );
	EcAddPrimitive( "gtk.menu_item_deselect",                         ec_gtk_gtk_menu_item_deselect                      );
	EcAddPrimitive( "gtk.menu_item_activate",                         ec_gtk_gtk_menu_item_activate                      );
	EcAddPrimitive( "gtk.menu_item_right_justify",                    ec_gtk_gtk_menu_item_right_justify                 );
	EcAddPrimitive( "gtk.check_menu_item_new",                        ec_gtk_gtk_check_menu_item_new                     );
	EcAddPrimitive( "gtk.check_menu_item_new_with_label",             ec_gtk_gtk_check_menu_item_new_with_label          );
	EcAddPrimitive( "gtk.check_menu_item_set_state",                  ec_gtk_gtk_check_menu_item_set_state               );
	EcAddPrimitive( "gtk.check_menu_item_set_show_toggle",            ec_gtk_gtk_check_menu_item_set_show_toggle         );
	EcAddPrimitive( "gtk.check_menu_item_toggled",                    ec_gtk_gtk_check_menu_item_toggled                 );
	EcAddPrimitive( "gtk.option_menu_new",                            ec_gtk_gtk_option_menu_new                         );
	EcAddPrimitive( "gtk.option_menu_get_menu",                       ec_gtk_gtk_option_menu_get_menu                    );
	EcAddPrimitive( "gtk.option_menu_set_menu",                       ec_gtk_gtk_option_menu_set_menu                    );
	EcAddPrimitive( "gtk.option_menu_remove_menu",                    ec_gtk_gtk_option_menu_remove_menu                 );
	EcAddPrimitive( "gtk.option_menu_set_history",                    ec_gtk_gtk_option_menu_set_history                 );
	EcAddPrimitive( "gtk.adjustment_getf",                            ec_gtk_gtk_adjustment_getf                         );
	EcAddPrimitive( "gtk.adjustment_setf",                            ec_gtk_gtk_adjustment_setf                         );
	EcAddPrimitive( "gtk.adjustment_new",                             ec_gtk_gtk_adjustment_new                          );
	EcAddPrimitive( "gtk.adjustment_set_value",                       ec_gtk_gtk_adjustment_set_value                    );
	EcAddPrimitive( "gtk.viewport_new",                               ec_gtk_gtk_viewport_new                            );
	EcAddPrimitive( "gtk.viewport_get_hadjustment",                   ec_gtk_gtk_viewport_get_hadjustment                );
	EcAddPrimitive( "gtk.viewport_get_vadjustment",                   ec_gtk_gtk_viewport_get_vadjustment                );
	EcAddPrimitive( "gtk.viewport_set_hadjustment",                   ec_gtk_gtk_viewport_set_hadjustment                );
	EcAddPrimitive( "gtk.viewport_set_vadjustment",                   ec_gtk_gtk_viewport_set_vadjustment                );
	EcAddPrimitive( "gtk.viewport_set_shadow_type",                   ec_gtk_gtk_viewport_set_shadow_type                );
	EcAddPrimitive( "gtk.scrolled_window_new",                        ec_gtk_gtk_scrolled_window_new                     );
	EcAddPrimitive( "gtk.scrolled_window_get_hadjustment",            ec_gtk_gtk_scrolled_window_get_hadjustment         );
	EcAddPrimitive( "gtk.scrolled_window_get_vadjustment",            ec_gtk_gtk_scrolled_window_get_vadjustment         );
	EcAddPrimitive( "gtk.scrolled_window_set_policy",                 ec_gtk_gtk_scrolled_window_set_policy              );
	EcAddPrimitive( "gtk.list_item_new",                              ec_gtk_gtk_list_item_new                           );
	EcAddPrimitive( "gtk.list_item_new_with_label",                   ec_gtk_gtk_list_item_new_with_label                );
	EcAddPrimitive( "gtk.list_item_select",                           ec_gtk_gtk_list_item_select                        );
	EcAddPrimitive( "gtk.list_item_deselect",                         ec_gtk_gtk_list_item_deselect                      );
	EcAddPrimitive( "gtk.list_new",                                   ec_gtk_gtk_list_new                                );
	EcAddPrimitive( "gtk.list_clear_items",                           ec_gtk_gtk_list_clear_items                        );
	EcAddPrimitive( "gtk.list_select_item",                           ec_gtk_gtk_list_select_item                        );
	EcAddPrimitive( "gtk.list_unselect_item",                         ec_gtk_gtk_list_unselect_item                      );
	EcAddPrimitive( "gtk.list_select_child",                          ec_gtk_gtk_list_select_child                       );
	EcAddPrimitive( "gtk.list_unselect_child",                        ec_gtk_gtk_list_unselect_child                     );
	EcAddPrimitive( "gtk.list_child_position",                        ec_gtk_gtk_list_child_position                     );
	EcAddPrimitive( "gtk.list_set_selection_mode",                    ec_gtk_gtk_list_set_selection_mode                 );
	EcAddPrimitive( "gtk.notebook_getf",                              ec_gtk_gtk_notebook_getf                           );
	EcAddPrimitive( "gtk.notebook_setf",                              ec_gtk_gtk_notebook_setf                           );
	EcAddPrimitive( "gtk.notebook_new",                               ec_gtk_gtk_notebook_new                            );
	EcAddPrimitive( "gtk.notebook_append_page",                       ec_gtk_gtk_notebook_append_page                    );
	EcAddPrimitive( "gtk.notebook_append_page_menu",                  ec_gtk_gtk_notebook_append_page_menu               );
	EcAddPrimitive( "gtk.notebook_prepend_page",                      ec_gtk_gtk_notebook_prepend_page                   );
	EcAddPrimitive( "gtk.notebook_prepend_page_menu",                 ec_gtk_gtk_notebook_prepend_page_menu              );
	EcAddPrimitive( "gtk.notebook_insert_page",                       ec_gtk_gtk_notebook_insert_page                    );
	EcAddPrimitive( "gtk.notebook_insert_page_menu",                  ec_gtk_gtk_notebook_insert_page_menu               );
	EcAddPrimitive( "gtk.notebook_remove_page",                       ec_gtk_gtk_notebook_remove_page                    );
	EcAddPrimitive( "gtk.notebook_current_page",                      ec_gtk_gtk_notebook_current_page                   );
	EcAddPrimitive( "gtk.notebook_set_page",                          ec_gtk_gtk_notebook_set_page                       );
	EcAddPrimitive( "gtk.notebook_next_page",                         ec_gtk_gtk_notebook_next_page                      );
	EcAddPrimitive( "gtk.notebook_prev_page",                         ec_gtk_gtk_notebook_prev_page                      );
	EcAddPrimitive( "gtk.notebook_set_tab_pos",                       ec_gtk_gtk_notebook_set_tab_pos                    );
	EcAddPrimitive( "gtk.notebook_set_show_tabs",                     ec_gtk_gtk_notebook_set_show_tabs                  );
	EcAddPrimitive( "gtk.notebook_set_show_border",                   ec_gtk_gtk_notebook_set_show_border                );
	EcAddPrimitive( "gtk.notebook_set_scrollable",                    ec_gtk_gtk_notebook_set_scrollable                 );
	EcAddPrimitive( "gtk.notebook_set_tab_border",                    ec_gtk_gtk_notebook_set_tab_border                 );
	EcAddPrimitive( "gtk.notebook_popup_enable",                      ec_gtk_gtk_notebook_popup_enable                   );
	EcAddPrimitive( "gtk.notebook_popup_disable",                     ec_gtk_gtk_notebook_popup_disable                  );
	EcAddPrimitive( "gtk.entry_new",                                  ec_gtk_gtk_entry_new                               );
	EcAddPrimitive( "gtk.entry_new_with_max_length",                  ec_gtk_gtk_entry_new_with_max_length               );
	EcAddPrimitive( "gtk.entry_set_text",                             ec_gtk_gtk_entry_set_text                          );
	EcAddPrimitive( "gtk.entry_append_text",                          ec_gtk_gtk_entry_append_text                       );
	EcAddPrimitive( "gtk.entry_prepend_text",                         ec_gtk_gtk_entry_prepend_text                      );
	EcAddPrimitive( "gtk.entry_set_position",                         ec_gtk_gtk_entry_set_position                      );
	EcAddPrimitive( "gtk.entry_get_text",                             ec_gtk_gtk_entry_get_text                          );
	EcAddPrimitive( "gtk.entry_select_region",                        ec_gtk_gtk_entry_select_region                     );
	EcAddPrimitive( "gtk.entry_set_visibility",                       ec_gtk_gtk_entry_set_visibility                    );
	EcAddPrimitive( "gtk.entry_set_editable",                         ec_gtk_gtk_entry_set_editable                      );
	EcAddPrimitive( "gtk.spin_button_new",                            ec_gtk_gtk_spin_button_new                         );
	EcAddPrimitive( "gtk.spin_button_set_adjustment",                 ec_gtk_gtk_spin_button_set_adjustment              );
	EcAddPrimitive( "gtk.spin_button_get_adjustment",                 ec_gtk_gtk_spin_button_get_adjustment              );
	EcAddPrimitive( "gtk.spin_button_set_digits",                     ec_gtk_gtk_spin_button_set_digits                  );
	EcAddPrimitive( "gtk.spin_button_get_value_as_float",             ec_gtk_gtk_spin_button_get_value_as_float          );
	EcAddPrimitive( "gtk.spin_button_get_value_as_int",               ec_gtk_gtk_spin_button_get_value_as_int            );
	EcAddPrimitive( "gtk.spin_button_set_value",                      ec_gtk_gtk_spin_button_set_value                   );
	EcAddPrimitive( "gtk.spin_button_set_update_policy",              ec_gtk_gtk_spin_button_set_update_policy           );
	EcAddPrimitive( "gtk.text_getf",                                  ec_gtk_gtk_text_getf                               );
	EcAddPrimitive( "gtk.text_setf",                                  ec_gtk_gtk_text_setf                               );
	EcAddPrimitive( "gtk.text_new",                                   ec_gtk_gtk_text_new                                );
	EcAddPrimitive( "gtk.text_set_editable",                          ec_gtk_gtk_text_set_editable                       );
	EcAddPrimitive( "gtk.text_set_adjustments",                       ec_gtk_gtk_text_set_adjustments                    );
	EcAddPrimitive( "gtk.text_set_point",                             ec_gtk_gtk_text_set_point                          );
	EcAddPrimitive( "gtk.text_get_point",                             ec_gtk_gtk_text_get_point                          );
	EcAddPrimitive( "gtk.text_get_length",                            ec_gtk_gtk_text_get_length                         );
	EcAddPrimitive( "gtk.text_freeze",                                ec_gtk_gtk_text_freeze                             );
	EcAddPrimitive( "gtk.text_thaw",                                  ec_gtk_gtk_text_thaw                               );
	EcAddPrimitive( "gtk.text_insert",                                ec_gtk_gtk_text_insert                             );
	EcAddPrimitive( "gtk.text_backward_delete",                       ec_gtk_gtk_text_backward_delete                    );
	EcAddPrimitive( "gtk.text_forward_delete",                        ec_gtk_gtk_text_forward_delete                     );
	EcAddPrimitive( "gtk.alignment_new",                              ec_gtk_gtk_alignment_new                           );
	EcAddPrimitive( "gtk.alignment_set",                              ec_gtk_gtk_alignment_set                           );
	EcAddPrimitive( "gtk.drawing_area_new",                           ec_gtk_gtk_drawing_area_new                        );
	EcAddPrimitive( "gtk.drawing_area_size",                          ec_gtk_gtk_drawing_area_size                       );
	EcAddPrimitive( "gtk.curve_new",                                  ec_gtk_gtk_curve_new                               );
	EcAddPrimitive( "gtk.curve_reset",                                ec_gtk_gtk_curve_reset                             );
	EcAddPrimitive( "gtk.curve_set_gamma",                            ec_gtk_gtk_curve_set_gamma                         );
	EcAddPrimitive( "gtk.curve_set_range",                            ec_gtk_gtk_curve_set_range                         );
	EcAddPrimitive( "gtk.curve_set_curve_type",                       ec_gtk_gtk_curve_set_curve_type                    );
	EcAddPrimitive( "gtk.preview_new",                                ec_gtk_gtk_preview_new                             );
	EcAddPrimitive( "gtk.preview_size",                               ec_gtk_gtk_preview_size                            );
	EcAddPrimitive( "gtk.preview_set_expand",                         ec_gtk_gtk_preview_set_expand                      );
	EcAddPrimitive( "gtk.preview_set_color_cube",                     ec_gtk_gtk_preview_set_color_cube                  );
	EcAddPrimitive( "gtk.preview_set_install_cmap",                   ec_gtk_gtk_preview_set_install_cmap                );
	EcAddPrimitive( "gtk.preview_set_reserved",                       ec_gtk_gtk_preview_set_reserved                    );
	EcAddPrimitive( "gtk.preview_get_visual",                         ec_gtk_gtk_preview_get_visual                      );
	EcAddPrimitive( "gtk.preview_get_cmap",                           ec_gtk_gtk_preview_get_cmap                        );
	EcAddPrimitive( "gtk.file_selection_getf",                        ec_gtk_gtk_file_selection_getf                     );
	EcAddPrimitive( "gtk.file_selection_setf",                        ec_gtk_gtk_file_selection_setf                     );
	EcAddPrimitive( "gtk.file_selection_new",                         ec_gtk_gtk_file_selection_new                      );
	EcAddPrimitive( "gtk.file_selection_set_filename",                ec_gtk_gtk_file_selection_set_filename             );
	EcAddPrimitive( "gtk.file_selection_get_filename",                ec_gtk_gtk_file_selection_get_filename             );
	EcAddPrimitive( "gtk.file_selection_show_fileop_buttons",         ec_gtk_gtk_file_selection_show_fileop_buttons      );
	EcAddPrimitive( "gtk.file_selection_hide_fileop_buttons",         ec_gtk_gtk_file_selection_hide_fileop_buttons      );
	EcAddPrimitive( "gtk.color_selection_dialog_getf",                ec_gtk_gtk_color_selection_dialog_getf             );
	EcAddPrimitive( "gtk.color_selection_dialog_setf",                ec_gtk_gtk_color_selection_dialog_setf             );
	EcAddPrimitive( "gtk.color_selection_dialog_new",                 ec_gtk_gtk_color_selection_dialog_new              );
	EcAddPrimitive( "gtk.color_selection_new",                        ec_gtk_gtk_color_selection_new                     );
	EcAddPrimitive( "gtk.color_selection_set_update_policy",          ec_gtk_gtk_color_selection_set_update_policy       );
	EcAddPrimitive( "gtk.color_selection_set_opacity",                ec_gtk_gtk_color_selection_set_opacity             );
	EcAddPrimitive( "gtk.range_get_adjustment",                       ec_gtk_gtk_range_get_adjustment                    );
	EcAddPrimitive( "gtk.range_set_update_policy",                    ec_gtk_gtk_range_set_update_policy                 );
	EcAddPrimitive( "gtk.range_set_adjustment",                       ec_gtk_gtk_range_set_adjustment                    );
	EcAddPrimitive( "gtk.scale_set_digits",                           ec_gtk_gtk_scale_set_digits                        );
	EcAddPrimitive( "gtk.scale_set_draw_value",                       ec_gtk_gtk_scale_set_draw_value                    );
	EcAddPrimitive( "gtk.scale_set_value_pos",                        ec_gtk_gtk_scale_set_value_pos                     );
	EcAddPrimitive( "gtk.scale_value_width",                          ec_gtk_gtk_scale_value_width                       );
	EcAddPrimitive( "gtk.scale_draw_value",                           ec_gtk_gtk_scale_draw_value                        );
	EcAddPrimitive( "gtk.hscale_new",                                 ec_gtk_gtk_hscale_new                              );
	EcAddPrimitive( "gtk.vscale_new",                                 ec_gtk_gtk_vscale_new                              );
	EcAddPrimitive( "gtk.hscrollbar_new",                             ec_gtk_gtk_hscrollbar_new                          );
	EcAddPrimitive( "gtk.vscrollbar_new",                             ec_gtk_gtk_vscrollbar_new                          );
	EcAddPrimitive( "gtk.ruler_set_metric",                           ec_gtk_gtk_ruler_set_metric                        );
	EcAddPrimitive( "gtk.ruler_set_range",                            ec_gtk_gtk_ruler_set_range                         );
	EcAddPrimitive( "gtk.hruler_new",                                 ec_gtk_gtk_hruler_new                              );
	EcAddPrimitive( "gtk.vruler_new",                                 ec_gtk_gtk_vruler_new                              );
	EcAddPrimitive( "gtk.dialog_getf",                                ec_gtk_gtk_dialog_getf                             );
	EcAddPrimitive( "gtk.dialog_setf",                                ec_gtk_gtk_dialog_setf                             );
	EcAddPrimitive( "gtk.dialog_new",                                 ec_gtk_gtk_dialog_new                              );
	EcAddPrimitive( "gtk.input_dialog_getf",                          ec_gtk_gtk_input_dialog_getf                       );
	EcAddPrimitive( "gtk.input_dialog_setf",                          ec_gtk_gtk_input_dialog_setf                       );
	EcAddPrimitive( "gtk.input_dialog_new",                           ec_gtk_gtk_input_dialog_new                        );
	EcAddPrimitive( "gtk.paned_add1",                                 ec_gtk_gtk_paned_add1                              );
	EcAddPrimitive( "gtk.paned_add2",                                 ec_gtk_gtk_paned_add2                              );
	EcAddPrimitive( "gtk.paned_handle_size",                          ec_gtk_gtk_paned_handle_size                       );
	EcAddPrimitive( "gtk.paned_gutter_size",                          ec_gtk_gtk_paned_gutter_size                       );
	EcAddPrimitive( "gtk.hpaned_new",                                 ec_gtk_gtk_hpaned_new                              );
	EcAddPrimitive( "gtk.vpaned_new",                                 ec_gtk_gtk_vpaned_new                              );
}


