/* gtkglue.h */

#ifndef __GTKGLUE_H
#define __GTKGLUE_H

/* ------------------------------------------------------------------------
 *  Information for enumeration values
 * ------------------------------------------------------------------------ */

#if EC_GTK_DATASTRUCTURES
static ec_gtk_enum ec_gtk_enum_gtk_window_type[3] = {
	{ "toplevel",                   GTK_WINDOW_TOPLEVEL,           0 },
	{ "dialog",                     GTK_WINDOW_DIALOG,             0 },
	{ "popup",                      GTK_WINDOW_POPUP,              0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_state_type[5] = {
	{ "normal",                     GTK_STATE_NORMAL,              0 },
	{ "active",                     GTK_STATE_ACTIVE,              0 },
	{ "prelight",                   GTK_STATE_PRELIGHT,            0 },
	{ "selected",                   GTK_STATE_SELECTED,            0 },
	{ "insensitive",                GTK_STATE_INSENSITIVE,         0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_direction_type[6] = {
	{ "tab-forward",                GTK_DIR_TAB_FORWARD,           0 },
	{ "tab-backward",               GTK_DIR_TAB_BACKWARD,          0 },
	{ "up",                         GTK_DIR_UP,                    0 },
	{ "down",                       GTK_DIR_DOWN,                  0 },
	{ "left",                       GTK_DIR_LEFT,                  0 },
	{ "right",                      GTK_DIR_RIGHT,                 0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_shadow_type[5] = {
	{ "none",                       GTK_SHADOW_NONE,               0 },
	{ "in",                         GTK_SHADOW_IN,                 0 },
	{ "out",                        GTK_SHADOW_OUT,                0 },
	{ "etched-in",                  GTK_SHADOW_ETCHED_IN,          0 },
	{ "etched-out",                 GTK_SHADOW_ETCHED_OUT,         0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_arrow_type[4] = {
	{ "up",                         GTK_ARROW_UP,                  0 },
	{ "down",                       GTK_ARROW_DOWN,                0 },
	{ "left",                       GTK_ARROW_LEFT,                0 },
	{ "right",                      GTK_ARROW_RIGHT,               0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_pack_type[2] = {
	{ "start",                      GTK_PACK_START,                0 },
	{ "end",                        GTK_PACK_END,                  0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_policy_type[2] = {
	{ "always",                     GTK_POLICY_ALWAYS,             0 },
	{ "automatic",                  GTK_POLICY_AUTOMATIC,          0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_update_type[3] = {
	{ "continuous",                 GTK_UPDATE_CONTINUOUS,         0 },
	{ "discontinuous",              GTK_UPDATE_DISCONTINUOUS,      0 },
	{ "delayed",                    GTK_UPDATE_DELAYED,            0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_window_position[3] = {
	{ "none",                       GTK_WIN_POS_NONE,              0 },
	{ "center",                     GTK_WIN_POS_CENTER,            0 },
	{ "mouse",                      GTK_WIN_POS_MOUSE,             0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_submenu_direction[2] = {
	{ "left",                       GTK_DIRECTION_LEFT,            0 },
	{ "right",                      GTK_DIRECTION_RIGHT,           0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_submenu_placement[2] = {
	{ "top-bottom",                 GTK_TOP_BOTTOM,                0 },
	{ "left-right",                 GTK_LEFT_RIGHT,                0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_menu_factory_type[3] = {
	{ "menu",                       GTK_MENU_FACTORY_MENU,         0 },
	{ "menu-bar",                   GTK_MENU_FACTORY_MENU_BAR,     0 },
	{ "option-menu",                GTK_MENU_FACTORY_OPTION_MENU,  0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_metric_type[3] = {
	{ "pixels",                     GTK_PIXELS,                    0 },
	{ "inches",                     GTK_INCHES,                    0 },
	{ "centimeters",                GTK_CENTIMETERS,               0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_scroll_type[5] = {
	{ "none",                       GTK_SCROLL_NONE,               0 },
	{ "step-backward",              GTK_SCROLL_STEP_BACKWARD,      0 },
	{ "step-forward",               GTK_SCROLL_STEP_FORWARD,       0 },
	{ "page-backward",              GTK_SCROLL_PAGE_BACKWARD,      0 },
	{ "page-forward",               GTK_SCROLL_PAGE_FORWARD,       0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_trough_type[3] = {
	{ "none",                       GTK_TROUGH_NONE,               0 },
	{ "start",                      GTK_TROUGH_START,              0 },
	{ "end",                        GTK_TROUGH_END,                0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_position_type[4] = {
	{ "left",                       GTK_POS_LEFT,                  0 },
	{ "right",                      GTK_POS_RIGHT,                 0 },
	{ "top",                        GTK_POS_TOP,                   0 },
	{ "bottom",                     GTK_POS_BOTTOM,                0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_preview_type[2] = {
	{ "color",                      GTK_PREVIEW_COLOR,             0 },
	{ "grayscale",                  GTK_PREVIEW_GRAYSCALE,         0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_selection_mode[4] = {
	{ "single",                     GTK_SELECTION_SINGLE,          0 },
	{ "browse",                     GTK_SELECTION_BROWSE,          0 },
	{ "multiple",                   GTK_SELECTION_MULTIPLE,        0 },
	{ "extended",                   GTK_SELECTION_EXTENDED,        0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_orientation[2] = {
	{ "horizontal",                 GTK_ORIENTATION_HORIZONTAL,    0 },
	{ "vertical",                   GTK_ORIENTATION_VERTICAL,      0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_toolbar_style[3] = {
	{ "icons",                      GTK_TOOLBAR_ICONS,             0 },
	{ "text",                       GTK_TOOLBAR_TEXT,              0 },
	{ "both",                       GTK_TOOLBAR_BOTH,              0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_toolbar_child_type[5] = {
	{ "space",                      GTK_TOOLBAR_CHILD_SPACE,       0 },
	{ "button",                     GTK_TOOLBAR_CHILD_BUTTON,      0 },
	{ "toggle-button",              GTK_TOOLBAR_CHILD_TOGGLEBUTTON, 0 },
	{ "radio-button",               GTK_TOOLBAR_CHILD_RADIOBUTTON, 0 },
	{ "widget",                     GTK_TOOLBAR_CHILD_WIDGET,      0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_button_box_style[5] = {
	{ "default-style",              GTK_BUTTONBOX_DEFAULT_STYLE,   0 },
	{ "spread",                     GTK_BUTTONBOX_SPREAD,          0 },
	{ "edge",                       GTK_BUTTONBOX_EDGE,            0 },
	{ "start",                      GTK_BUTTONBOX_START,           0 },
	{ "end",                        GTK_BUTTONBOX_END,             0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_tree_view_mode[2] = {
	{ "line",                       GTK_TREE_VIEW_LINE,            0 },
	{ "item",                       GTK_TREE_VIEW_ITEM,            0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_curve_type[3] = {
	{ "linear",                     GTK_CURVE_TYPE_LINEAR,         0 },
	{ "spline",                     GTK_CURVE_TYPE_SPLINE,         0 },
	{ "free",                       GTK_CURVE_TYPE_FREE,           0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_fundamental_type[20] = {
	{ "invalid",                    GTK_TYPE_INVALID,              0 },
	{ "none",                       GTK_TYPE_NONE,                 0 },
	{ "char",                       GTK_TYPE_CHAR,                 0 },
	{ "bool",                       GTK_TYPE_BOOL,                 0 },
	{ "int",                        GTK_TYPE_INT,                  0 },
	{ "uint",                       GTK_TYPE_UINT,                 0 },
	{ "long",                       GTK_TYPE_LONG,                 0 },
	{ "ulong",                      GTK_TYPE_ULONG,                0 },
	{ "float",                      GTK_TYPE_FLOAT,                0 },
	{ "string",                     GTK_TYPE_STRING,               0 },
	{ "enum",                       GTK_TYPE_ENUM,                 0 },
	{ "flags",                      GTK_TYPE_FLAGS,                0 },
	{ "boxed",                      GTK_TYPE_BOXED,                0 },
	{ "foreign",                    GTK_TYPE_FOREIGN,              0 },
	{ "callback",                   GTK_TYPE_CALLBACK,             0 },
	{ "args",                       GTK_TYPE_ARGS,                 0 },
	{ "pointer",                    GTK_TYPE_POINTER,              0 },
	{ "signal",                     GTK_TYPE_SIGNAL,               0 },
	{ "c-callback",                 GTK_TYPE_C_CALLBACK,           0 },
	{ "object",                     GTK_TYPE_OBJECT,               0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_justification[4] = {
	{ "left",                       GTK_JUSTIFY_LEFT,              0 },
	{ "right",                      GTK_JUSTIFY_RIGHT,             0 },
	{ "center",                     GTK_JUSTIFY_CENTER,            0 },
	{ "fill",                       GTK_JUSTIFY_FILL,              0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_spin_button_update_policy[2] = {
	{ "always",                     GTK_UPDATE_ALWAYS,             0 },
	{ "if-valid",                   GTK_UPDATE_IF_VALID,           0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_cell_type[5] = {
	{ "empty",                      GTK_CELL_EMPTY,                0 },
	{ "text",                       GTK_CELL_TEXT,                 0 },
	{ "pixmap",                     GTK_CELL_PIXMAP,               0 },
	{ "pixtext",                    GTK_CELL_PIXTEXT,              0 },
	{ "widget",                     GTK_CELL_WIDGET,               0 },
};
#endif /*EC_GTK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for flags values
 * ------------------------------------------------------------------------ */

#if EC_GTK_DATASTRUCTURES
static ec_gtk_enum ec_gtk_enum_gtk_attach_options[3] = {
	{ "expand",                     GTK_EXPAND,                    0 },
	{ "shrink",                     GTK_SHRINK,                    0 },
	{ "fill",                       GTK_FILL,                      0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_signal_run_type[6] = {
	{ "first",                      GTK_RUN_FIRST,                 0 },
	{ "last",                       GTK_RUN_NO_HOOKS,              0 },
	{ "both",                       GTK_RUN_BOTH,                  0 },
	{ "no-recurse",                 GTK_RUN_NO_RECURSE,            0 },
	{ "last",                       GTK_RUN_NO_HOOKS,              0 },
	{ "last",                       GTK_RUN_NO_HOOKS,              0 },
};
static ec_gtk_enum ec_gtk_enum_gtk_widget_flags[17] = {
	{ "toplevel",                   GTK_TOPLEVEL,                  0 },
	{ "no-window",                  GTK_NO_WINDOW,                 0 },
	{ "realized",                   GTK_REALIZED,                  0 },
	{ "mapped",                     GTK_MAPPED,                    0 },
	{ "visible",                    GTK_VISIBLE,                   0 },
	{ "sensitive",                  GTK_SENSITIVE,                 0 },
	{ "parent-sensitive",           GTK_PARENT_SENSITIVE,          0 },
	{ "can-focus",                  GTK_CAN_FOCUS,                 0 },
	{ "has-focus",                  GTK_HAS_FOCUS,                 0 },
	{ "can-default",                GTK_CAN_DEFAULT,               0 },
	{ "has-default",                GTK_HAS_DEFAULT,               0 },
	{ "has-grab",                   GTK_HAS_GRAB,                  0 },
	{ "rc-style",                   GTK_RC_STYLE,                  0 },
	{ "composite-child",            GTK_COMPOSITE_CHILD,           0 },
	{ "no-reparent",                GTK_NO_REPARENT,               0 },
	{ "app-paintable",              GTK_APP_PAINTABLE,             0 },
	{ "receives-default",           GTK_RECEIVES_DEFAULT,          0 },
};
#endif /*EC_GTK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for enumerations as types
 * ------------------------------------------------------------------------ */

#if EC_GTK_DATASTRUCTURES
ec_gtk_enum_info ec_gtk_enums[] = {
	{ "GtkWindowType",                           3, ec_gtk_enum_gtk_window_type                },
	{ "GtkStateType",                            5, ec_gtk_enum_gtk_state_type                 },
	{ "GtkDirectionType",                        6, ec_gtk_enum_gtk_direction_type             },
	{ "GtkShadowType",                           5, ec_gtk_enum_gtk_shadow_type                },
	{ "GtkArrowType",                            4, ec_gtk_enum_gtk_arrow_type                 },
	{ "GtkPackType",                             2, ec_gtk_enum_gtk_pack_type                  },
	{ "GtkPolicyType",                           2, ec_gtk_enum_gtk_policy_type                },
	{ "GtkUpdateType",                           3, ec_gtk_enum_gtk_update_type                },
	{ "GtkWindowPosition",                       3, ec_gtk_enum_gtk_window_position            },
	{ "GtkSubmenuDirection",                     2, ec_gtk_enum_gtk_submenu_direction          },
	{ "GtkSubmenuPlacement",                     2, ec_gtk_enum_gtk_submenu_placement          },
	{ "GtkMenuFactoryType",                      3, ec_gtk_enum_gtk_menu_factory_type          },
	{ "GtkMetricType",                           3, ec_gtk_enum_gtk_metric_type                },
	{ "GtkScrollType",                           5, ec_gtk_enum_gtk_scroll_type                },
	{ "GtkTroughType",                           3, ec_gtk_enum_gtk_trough_type                },
	{ "GtkPositionType",                         4, ec_gtk_enum_gtk_position_type              },
	{ "GtkPreviewType",                          2, ec_gtk_enum_gtk_preview_type               },
	{ "GtkSelectionMode",                        4, ec_gtk_enum_gtk_selection_mode             },
	{ "GtkOrientation",                          2, ec_gtk_enum_gtk_orientation                },
	{ "GtkToolbarStyle",                         3, ec_gtk_enum_gtk_toolbar_style              },
	{ "GtkToolbarChildType",                     5, ec_gtk_enum_gtk_toolbar_child_type         },
	{ "GtkButtonBoxStyle",                       5, ec_gtk_enum_gtk_button_box_style           },
	{ "GtkTreeViewMode",                         2, ec_gtk_enum_gtk_tree_view_mode             },
	{ "GtkCurveType",                            3, ec_gtk_enum_gtk_curve_type                 },
	{ "GtkFundamentalType",                     20, ec_gtk_enum_gtk_fundamental_type           },
	{ "GtkJustification",                        4, ec_gtk_enum_gtk_justification              },
	{ "GtkSpinButtonUpdatePolicy",               2, ec_gtk_enum_gtk_spin_button_update_policy  },
	{ "GtkCellType",                             5, ec_gtk_enum_gtk_cell_type                  },
};

EcInt ec_gtk_nenums = 28;
#endif /* EC_GTK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for flags as types
 * ------------------------------------------------------------------------ */

#if EC_GTK_DATASTRUCTURES
ec_gtk_enum_info ec_gtk_flags[] = {
	{ "GtkAttachOptions",                        3, ec_gtk_enum_gtk_attach_options             },
	{ "GtkSignalRunType",                        6, ec_gtk_enum_gtk_signal_run_type            },
	{ "GtkWidgetFlags",                         17, ec_gtk_enum_gtk_widget_flags               },
};

EcInt ec_gtk_nflags = 3;
#endif /* EC_GTK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Symbolic information mapping for humans
 * ------------------------------------------------------------------------ */

#define ec_gtk_t_gtk_window_type                           (ec_gtk_enums[0])
#define ec_gtk_t_gtk_state_type                            (ec_gtk_enums[1])
#define ec_gtk_t_gtk_direction_type                        (ec_gtk_enums[2])
#define ec_gtk_t_gtk_shadow_type                           (ec_gtk_enums[3])
#define ec_gtk_t_gtk_arrow_type                            (ec_gtk_enums[4])
#define ec_gtk_t_gtk_pack_type                             (ec_gtk_enums[5])
#define ec_gtk_t_gtk_policy_type                           (ec_gtk_enums[6])
#define ec_gtk_t_gtk_update_type                           (ec_gtk_enums[7])
#define ec_gtk_t_gtk_attach_options                        (ec_gtk_flags[0])
#define ec_gtk_t_gtk_signal_run_type                       (ec_gtk_flags[1])
#define ec_gtk_t_gtk_window_position                       (ec_gtk_enums[8])
#define ec_gtk_t_gtk_submenu_direction                     (ec_gtk_enums[9])
#define ec_gtk_t_gtk_submenu_placement                     (ec_gtk_enums[10])
#define ec_gtk_t_gtk_menu_factory_type                     (ec_gtk_enums[11])
#define ec_gtk_t_gtk_metric_type                           (ec_gtk_enums[12])
#define ec_gtk_t_gtk_scroll_type                           (ec_gtk_enums[13])
#define ec_gtk_t_gtk_trough_type                           (ec_gtk_enums[14])
#define ec_gtk_t_gtk_position_type                         (ec_gtk_enums[15])
#define ec_gtk_t_gtk_preview_type                          (ec_gtk_enums[16])
#define ec_gtk_t_gtk_widget_flags                          (ec_gtk_flags[2])
#define ec_gtk_t_gtk_selection_mode                        (ec_gtk_enums[17])
#define ec_gtk_t_gtk_orientation                           (ec_gtk_enums[18])
#define ec_gtk_t_gtk_toolbar_style                         (ec_gtk_enums[19])
#define ec_gtk_t_gtk_toolbar_child_type                    (ec_gtk_enums[20])
#define ec_gtk_t_gtk_button_box_style                      (ec_gtk_enums[21])
#define ec_gtk_t_gtk_tree_view_mode                        (ec_gtk_enums[22])
#define ec_gtk_t_gtk_curve_type                            (ec_gtk_enums[23])
#define ec_gtk_t_gtk_fundamental_type                      (ec_gtk_enums[24])
#define ec_gtk_t_gtk_justification                         (ec_gtk_enums[25])
#define ec_gtk_t_gtk_spin_button_update_policy             (ec_gtk_enums[26])
#define ec_gtk_t_gtk_cell_type                             (ec_gtk_enums[27])

/* ------------------------------------------------------------------------
 *  Boxed types information
 * ------------------------------------------------------------------------ */

#if EC_GTK_DATASTRUCTURES
ec_gtk_boxed_info ec_gtk_boxed[] = {
	{ "GtkStyle",         (EC_GTK_BOXED_COPY)gtk_style_ref,       (EC_GTK_BOXED_DESTROY)gtk_style_unref,  0                    },
};

EcInt ec_gtk_nboxed = 1;
#endif /* EC_GTK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Boxed & object field mapping
 * ------------------------------------------------------------------------ */

#if EC_GTK_MODULE
#define EC_GTK_COMPOUNDFIELD_STYLE     0
#define EC_GTK_COMPOUNDFIELD_WINDOW    1
#define EC_GTK_COMPOUNDFIELD_CHILD     2
#define EC_GTK_COMPOUNDFIELD_IN_BUTTON 3
#define EC_GTK_COMPOUNDFIELD_BUTTON_DOWN 4
#define EC_GTK_COMPOUNDFIELD_ACTIVE    5
#define EC_GTK_COMPOUNDFIELD_DRAW_INDICATOR 6
#define EC_GTK_COMPOUNDFIELD_ENTRY     7
#define EC_GTK_COMPOUNDFIELD_BUTTON    8
#define EC_GTK_COMPOUNDFIELD_POPUP     9
#define EC_GTK_COMPOUNDFIELD_POPWIN    10
#define EC_GTK_COMPOUNDFIELD_LIST      11
#define EC_GTK_COMPOUNDFIELD_TABLE     12
#define EC_GTK_COMPOUNDFIELD_CURVE     13
#define EC_GTK_COMPOUNDFIELD_GAMMA     14
#define EC_GTK_COMPOUNDFIELD_GAMMA_DIALOG 15
#define EC_GTK_COMPOUNDFIELD_GAMMA_TEXT 16
#define EC_GTK_COMPOUNDFIELD_VALUE     17
#define EC_GTK_COMPOUNDFIELD_LOWER     18
#define EC_GTK_COMPOUNDFIELD_UPPER     19
#define EC_GTK_COMPOUNDFIELD_STEP_INCREMENT 20
#define EC_GTK_COMPOUNDFIELD_PAGE_INCREMENT 21
#define EC_GTK_COMPOUNDFIELD_PAGE_SIZE 22
#define EC_GTK_COMPOUNDFIELD_TAB_POS   23
#define EC_GTK_COMPOUNDFIELD_HADJ      24
#define EC_GTK_COMPOUNDFIELD_VADJ      25
#define EC_GTK_COMPOUNDFIELD_DIR_LIST  26
#define EC_GTK_COMPOUNDFIELD_FILE_LIST 27
#define EC_GTK_COMPOUNDFIELD_SELECTION_ENTRY 28
#define EC_GTK_COMPOUNDFIELD_SELECTION_TEXT 29
#define EC_GTK_COMPOUNDFIELD_MAIN_VBOX 30
#define EC_GTK_COMPOUNDFIELD_OK_BUTTON 31
#define EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON 32
#define EC_GTK_COMPOUNDFIELD_HELP_BUTTON 33
#define EC_GTK_COMPOUNDFIELD_COLORSEL  34
#define EC_GTK_COMPOUNDFIELD_RESET_BUTTON 35
#define EC_GTK_COMPOUNDFIELD_VBOX      36
#define EC_GTK_COMPOUNDFIELD_ACTION_AREA 37
#define EC_GTK_COMPOUNDFIELD_CLOSE_BUTTON 38
#define EC_GTK_COMPOUNDFIELD_SAVE_BUTTON 39

static EcUInt ec_gtk_compoundfield[40];
#endif /* EC_GTK_MODULE */


/* ------------------------------------------------------------------------
 *  Init. function for enumerations/flags symbols & boxed/object fields
 * ------------------------------------------------------------------------ */

#if EC_GTK_MODULE
static void ec_gtk_init_symbols( void )
{
	/* GtkWindowType                  */
	ec_gtk_enum_gtk_window_type[0].symid               = EcInternSymbol( "toplevel" );
	ec_gtk_enum_gtk_window_type[1].symid               = EcInternSymbol( "dialog" );
	ec_gtk_enum_gtk_window_type[2].symid               = EcInternSymbol( "popup" );

	/* GtkStateType                   */
	ec_gtk_enum_gtk_state_type[0].symid                = EcInternSymbol( "normal" );
	ec_gtk_enum_gtk_state_type[1].symid                = EcInternSymbol( "active" );
	ec_gtk_enum_gtk_state_type[2].symid                = EcInternSymbol( "prelight" );
	ec_gtk_enum_gtk_state_type[3].symid                = EcInternSymbol( "selected" );
	ec_gtk_enum_gtk_state_type[4].symid                = EcInternSymbol( "insensitive" );

	/* GtkDirectionType               */
	ec_gtk_enum_gtk_direction_type[0].symid            = EcInternSymbol( "tab-forward" );
	ec_gtk_enum_gtk_direction_type[1].symid            = EcInternSymbol( "tab-backward" );
	ec_gtk_enum_gtk_direction_type[2].symid            = EcInternSymbol( "up" );
	ec_gtk_enum_gtk_direction_type[3].symid            = EcInternSymbol( "down" );
	ec_gtk_enum_gtk_direction_type[4].symid            = EcInternSymbol( "left" );
	ec_gtk_enum_gtk_direction_type[5].symid            = EcInternSymbol( "right" );

	/* GtkShadowType                  */
	ec_gtk_enum_gtk_shadow_type[0].symid               = EcInternSymbol( "none" );
	ec_gtk_enum_gtk_shadow_type[1].symid               = EcInternSymbol( "in" );
	ec_gtk_enum_gtk_shadow_type[2].symid               = EcInternSymbol( "out" );
	ec_gtk_enum_gtk_shadow_type[3].symid               = EcInternSymbol( "etched-in" );
	ec_gtk_enum_gtk_shadow_type[4].symid               = EcInternSymbol( "etched-out" );

	/* GtkArrowType                   */
	ec_gtk_enum_gtk_arrow_type[0].symid                = EcInternSymbol( "up" );
	ec_gtk_enum_gtk_arrow_type[1].symid                = EcInternSymbol( "down" );
	ec_gtk_enum_gtk_arrow_type[2].symid                = EcInternSymbol( "left" );
	ec_gtk_enum_gtk_arrow_type[3].symid                = EcInternSymbol( "right" );

	/* GtkPackType                    */
	ec_gtk_enum_gtk_pack_type[0].symid                 = EcInternSymbol( "start" );
	ec_gtk_enum_gtk_pack_type[1].symid                 = EcInternSymbol( "end" );

	/* GtkPolicyType                  */
	ec_gtk_enum_gtk_policy_type[0].symid               = EcInternSymbol( "always" );
	ec_gtk_enum_gtk_policy_type[1].symid               = EcInternSymbol( "automatic" );

	/* GtkUpdateType                  */
	ec_gtk_enum_gtk_update_type[0].symid               = EcInternSymbol( "continuous" );
	ec_gtk_enum_gtk_update_type[1].symid               = EcInternSymbol( "discontinuous" );
	ec_gtk_enum_gtk_update_type[2].symid               = EcInternSymbol( "delayed" );

	/* GtkAttachOptions               */
	ec_gtk_enum_gtk_attach_options[0].symid            = EcInternSymbol( "expand" );
	ec_gtk_enum_gtk_attach_options[1].symid            = EcInternSymbol( "shrink" );
	ec_gtk_enum_gtk_attach_options[2].symid            = EcInternSymbol( "fill" );

	/* GtkSignalRunType               */
	ec_gtk_enum_gtk_signal_run_type[0].symid           = EcInternSymbol( "first" );
	ec_gtk_enum_gtk_signal_run_type[1].symid           = EcInternSymbol( "last" );
	ec_gtk_enum_gtk_signal_run_type[2].symid           = EcInternSymbol( "both" );
	ec_gtk_enum_gtk_signal_run_type[3].symid           = EcInternSymbol( "no-recurse" );
	ec_gtk_enum_gtk_signal_run_type[4].symid           = EcInternSymbol( "last" );
	ec_gtk_enum_gtk_signal_run_type[5].symid           = EcInternSymbol( "last" );

	/* GtkWindowPosition              */
	ec_gtk_enum_gtk_window_position[0].symid           = EcInternSymbol( "none" );
	ec_gtk_enum_gtk_window_position[1].symid           = EcInternSymbol( "center" );
	ec_gtk_enum_gtk_window_position[2].symid           = EcInternSymbol( "mouse" );

	/* GtkSubmenuDirection            */
	ec_gtk_enum_gtk_submenu_direction[0].symid         = EcInternSymbol( "left" );
	ec_gtk_enum_gtk_submenu_direction[1].symid         = EcInternSymbol( "right" );

	/* GtkSubmenuPlacement            */
	ec_gtk_enum_gtk_submenu_placement[0].symid         = EcInternSymbol( "top-bottom" );
	ec_gtk_enum_gtk_submenu_placement[1].symid         = EcInternSymbol( "left-right" );

	/* GtkMenuFactoryType             */
	ec_gtk_enum_gtk_menu_factory_type[0].symid         = EcInternSymbol( "menu" );
	ec_gtk_enum_gtk_menu_factory_type[1].symid         = EcInternSymbol( "menu-bar" );
	ec_gtk_enum_gtk_menu_factory_type[2].symid         = EcInternSymbol( "option-menu" );

	/* GtkMetricType                  */
	ec_gtk_enum_gtk_metric_type[0].symid               = EcInternSymbol( "pixels" );
	ec_gtk_enum_gtk_metric_type[1].symid               = EcInternSymbol( "inches" );
	ec_gtk_enum_gtk_metric_type[2].symid               = EcInternSymbol( "centimeters" );

	/* GtkScrollType                  */
	ec_gtk_enum_gtk_scroll_type[0].symid               = EcInternSymbol( "none" );
	ec_gtk_enum_gtk_scroll_type[1].symid               = EcInternSymbol( "step-backward" );
	ec_gtk_enum_gtk_scroll_type[2].symid               = EcInternSymbol( "step-forward" );
	ec_gtk_enum_gtk_scroll_type[3].symid               = EcInternSymbol( "page-backward" );
	ec_gtk_enum_gtk_scroll_type[4].symid               = EcInternSymbol( "page-forward" );

	/* GtkTroughType                  */
	ec_gtk_enum_gtk_trough_type[0].symid               = EcInternSymbol( "none" );
	ec_gtk_enum_gtk_trough_type[1].symid               = EcInternSymbol( "start" );
	ec_gtk_enum_gtk_trough_type[2].symid               = EcInternSymbol( "end" );

	/* GtkPositionType                */
	ec_gtk_enum_gtk_position_type[0].symid             = EcInternSymbol( "left" );
	ec_gtk_enum_gtk_position_type[1].symid             = EcInternSymbol( "right" );
	ec_gtk_enum_gtk_position_type[2].symid             = EcInternSymbol( "top" );
	ec_gtk_enum_gtk_position_type[3].symid             = EcInternSymbol( "bottom" );

	/* GtkPreviewType                 */
	ec_gtk_enum_gtk_preview_type[0].symid              = EcInternSymbol( "color" );
	ec_gtk_enum_gtk_preview_type[1].symid              = EcInternSymbol( "grayscale" );

	/* GtkWidgetFlags                 */
	ec_gtk_enum_gtk_widget_flags[0].symid              = EcInternSymbol( "toplevel" );
	ec_gtk_enum_gtk_widget_flags[1].symid              = EcInternSymbol( "no-window" );
	ec_gtk_enum_gtk_widget_flags[2].symid              = EcInternSymbol( "realized" );
	ec_gtk_enum_gtk_widget_flags[3].symid              = EcInternSymbol( "mapped" );
	ec_gtk_enum_gtk_widget_flags[4].symid              = EcInternSymbol( "visible" );
	ec_gtk_enum_gtk_widget_flags[5].symid              = EcInternSymbol( "sensitive" );
	ec_gtk_enum_gtk_widget_flags[6].symid              = EcInternSymbol( "parent-sensitive" );
	ec_gtk_enum_gtk_widget_flags[7].symid              = EcInternSymbol( "can-focus" );
	ec_gtk_enum_gtk_widget_flags[8].symid              = EcInternSymbol( "has-focus" );
	ec_gtk_enum_gtk_widget_flags[9].symid              = EcInternSymbol( "can-default" );
	ec_gtk_enum_gtk_widget_flags[10].symid             = EcInternSymbol( "has-default" );
	ec_gtk_enum_gtk_widget_flags[11].symid             = EcInternSymbol( "has-grab" );
	ec_gtk_enum_gtk_widget_flags[12].symid             = EcInternSymbol( "rc-style" );
	ec_gtk_enum_gtk_widget_flags[13].symid             = EcInternSymbol( "composite-child" );
	ec_gtk_enum_gtk_widget_flags[14].symid             = EcInternSymbol( "no-reparent" );
	ec_gtk_enum_gtk_widget_flags[15].symid             = EcInternSymbol( "app-paintable" );
	ec_gtk_enum_gtk_widget_flags[16].symid             = EcInternSymbol( "receives-default" );

	/* GtkSelectionMode               */
	ec_gtk_enum_gtk_selection_mode[0].symid            = EcInternSymbol( "single" );
	ec_gtk_enum_gtk_selection_mode[1].symid            = EcInternSymbol( "browse" );
	ec_gtk_enum_gtk_selection_mode[2].symid            = EcInternSymbol( "multiple" );
	ec_gtk_enum_gtk_selection_mode[3].symid            = EcInternSymbol( "extended" );

	/* GtkOrientation                 */
	ec_gtk_enum_gtk_orientation[0].symid               = EcInternSymbol( "horizontal" );
	ec_gtk_enum_gtk_orientation[1].symid               = EcInternSymbol( "vertical" );

	/* GtkToolbarStyle                */
	ec_gtk_enum_gtk_toolbar_style[0].symid             = EcInternSymbol( "icons" );
	ec_gtk_enum_gtk_toolbar_style[1].symid             = EcInternSymbol( "text" );
	ec_gtk_enum_gtk_toolbar_style[2].symid             = EcInternSymbol( "both" );

	/* GtkToolbarChildType            */
	ec_gtk_enum_gtk_toolbar_child_type[0].symid        = EcInternSymbol( "space" );
	ec_gtk_enum_gtk_toolbar_child_type[1].symid        = EcInternSymbol( "button" );
	ec_gtk_enum_gtk_toolbar_child_type[2].symid        = EcInternSymbol( "toggle-button" );
	ec_gtk_enum_gtk_toolbar_child_type[3].symid        = EcInternSymbol( "radio-button" );
	ec_gtk_enum_gtk_toolbar_child_type[4].symid        = EcInternSymbol( "widget" );

	/* GtkButtonBoxStyle              */
	ec_gtk_enum_gtk_button_box_style[0].symid          = EcInternSymbol( "default-style" );
	ec_gtk_enum_gtk_button_box_style[1].symid          = EcInternSymbol( "spread" );
	ec_gtk_enum_gtk_button_box_style[2].symid          = EcInternSymbol( "edge" );
	ec_gtk_enum_gtk_button_box_style[3].symid          = EcInternSymbol( "start" );
	ec_gtk_enum_gtk_button_box_style[4].symid          = EcInternSymbol( "end" );

	/* GtkTreeViewMode                */
	ec_gtk_enum_gtk_tree_view_mode[0].symid            = EcInternSymbol( "line" );
	ec_gtk_enum_gtk_tree_view_mode[1].symid            = EcInternSymbol( "item" );

	/* GtkCurveType                   */
	ec_gtk_enum_gtk_curve_type[0].symid                = EcInternSymbol( "linear" );
	ec_gtk_enum_gtk_curve_type[1].symid                = EcInternSymbol( "spline" );
	ec_gtk_enum_gtk_curve_type[2].symid                = EcInternSymbol( "free" );

	/* GtkFundamentalType             */
	ec_gtk_enum_gtk_fundamental_type[0].symid          = EcInternSymbol( "invalid" );
	ec_gtk_enum_gtk_fundamental_type[1].symid          = EcInternSymbol( "none" );
	ec_gtk_enum_gtk_fundamental_type[2].symid          = EcInternSymbol( "char" );
	ec_gtk_enum_gtk_fundamental_type[3].symid          = EcInternSymbol( "bool" );
	ec_gtk_enum_gtk_fundamental_type[4].symid          = EcInternSymbol( "int" );
	ec_gtk_enum_gtk_fundamental_type[5].symid          = EcInternSymbol( "uint" );
	ec_gtk_enum_gtk_fundamental_type[6].symid          = EcInternSymbol( "long" );
	ec_gtk_enum_gtk_fundamental_type[7].symid          = EcInternSymbol( "ulong" );
	ec_gtk_enum_gtk_fundamental_type[8].symid          = EcInternSymbol( "float" );
	ec_gtk_enum_gtk_fundamental_type[9].symid          = EcInternSymbol( "string" );
	ec_gtk_enum_gtk_fundamental_type[10].symid         = EcInternSymbol( "enum" );
	ec_gtk_enum_gtk_fundamental_type[11].symid         = EcInternSymbol( "flags" );
	ec_gtk_enum_gtk_fundamental_type[12].symid         = EcInternSymbol( "boxed" );
	ec_gtk_enum_gtk_fundamental_type[13].symid         = EcInternSymbol( "foreign" );
	ec_gtk_enum_gtk_fundamental_type[14].symid         = EcInternSymbol( "callback" );
	ec_gtk_enum_gtk_fundamental_type[15].symid         = EcInternSymbol( "args" );
	ec_gtk_enum_gtk_fundamental_type[16].symid         = EcInternSymbol( "pointer" );
	ec_gtk_enum_gtk_fundamental_type[17].symid         = EcInternSymbol( "signal" );
	ec_gtk_enum_gtk_fundamental_type[18].symid         = EcInternSymbol( "c-callback" );
	ec_gtk_enum_gtk_fundamental_type[19].symid         = EcInternSymbol( "object" );

	/* GtkJustification               */
	ec_gtk_enum_gtk_justification[0].symid             = EcInternSymbol( "left" );
	ec_gtk_enum_gtk_justification[1].symid             = EcInternSymbol( "right" );
	ec_gtk_enum_gtk_justification[2].symid             = EcInternSymbol( "center" );
	ec_gtk_enum_gtk_justification[3].symid             = EcInternSymbol( "fill" );

	/* GtkSpinButtonUpdatePolicy      */
	ec_gtk_enum_gtk_spin_button_update_policy[0].symid = EcInternSymbol( "always" );
	ec_gtk_enum_gtk_spin_button_update_policy[1].symid = EcInternSymbol( "if-valid" );

	/* GtkCellType                    */
	ec_gtk_enum_gtk_cell_type[0].symid                 = EcInternSymbol( "empty" );
	ec_gtk_enum_gtk_cell_type[1].symid                 = EcInternSymbol( "text" );
	ec_gtk_enum_gtk_cell_type[2].symid                 = EcInternSymbol( "pixmap" );
	ec_gtk_enum_gtk_cell_type[3].symid                 = EcInternSymbol( "pixtext" );
	ec_gtk_enum_gtk_cell_type[4].symid                 = EcInternSymbol( "widget" );


	/* Boxed/Object fields */

	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_INCREMENT] = EcInternSymbol( "page_increment" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_MAIN_VBOX] = EcInternSymbol( "main_vbox" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TABLE]   = EcInternSymbol( "table" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HELP_BUTTON] = EcInternSymbol( "help_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPWIN]  = EcInternSymbol( "popwin" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_HADJ]    = EcInternSymbol( "hadj" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STYLE]   = EcInternSymbol( "style" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_POPUP]   = EcInternSymbol( "popup" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_IN_BUTTON] = EcInternSymbol( "in_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CURVE]   = EcInternSymbol( "curve" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CLOSE_BUTTON] = EcInternSymbol( "close_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTIVE]  = EcInternSymbol( "active" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_OK_BUTTON] = EcInternSymbol( "ok_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_PAGE_SIZE] = EcInternSymbol( "page_size" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CANCEL_BUTTON] = EcInternSymbol( "cancel_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ENTRY]   = EcInternSymbol( "entry" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_RESET_BUTTON] = EcInternSymbol( "reset_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VADJ]    = EcInternSymbol( "vadj" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_ACTION_AREA] = EcInternSymbol( "action_area" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VALUE]   = EcInternSymbol( "value" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_TEXT] = EcInternSymbol( "gamma_text" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA]   = EcInternSymbol( "gamma" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_UPPER]   = EcInternSymbol( "upper" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_STEP_INCREMENT] = EcInternSymbol( "step_increment" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DIR_LIST] = EcInternSymbol( "dir_list" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_TAB_POS] = EcInternSymbol( "tab_pos" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LIST]    = EcInternSymbol( "list" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_GAMMA_DIALOG] = EcInternSymbol( "gamma_dialog" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_COLORSEL] = EcInternSymbol( "colorsel" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_WINDOW]  = EcInternSymbol( "window" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_TEXT] = EcInternSymbol( "selection_text" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_FILE_LIST] = EcInternSymbol( "file_list" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SELECTION_ENTRY] = EcInternSymbol( "selection_entry" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_VBOX]    = EcInternSymbol( "vbox" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_SAVE_BUTTON] = EcInternSymbol( "save_button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_LOWER]   = EcInternSymbol( "lower" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_DRAW_INDICATOR] = EcInternSymbol( "draw_indicator" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON_DOWN] = EcInternSymbol( "button_down" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_BUTTON]  = EcInternSymbol( "button" );
	ec_gtk_compoundfield[EC_GTK_COMPOUNDFIELD_CHILD]   = EcInternSymbol( "child" );
}
#endif /* EC_GTK_MODULE */

#endif /* __GTKGLUE_H */
