/* gdkglue.h */

#ifndef __GDKGLUE_H
#define __GDKGLUE_H

/* ------------------------------------------------------------------------
 *  Information for enumeration values
 * ------------------------------------------------------------------------ */

#if EC_GDK_DATASTRUCTURES
static ec_gtk_enum ec_gdk_enum_gdk_window_type[6] = {
	{ "root",                       GDK_WINDOW_ROOT,               0 },
	{ "toplevel",                   GDK_WINDOW_TOPLEVEL,           0 },
	{ "child",                      GDK_WINDOW_CHILD,              0 },
	{ "dialog",                     GDK_WINDOW_DIALOG,             0 },
	{ "temp",                       GDK_WINDOW_TEMP,               0 },
	{ "pixmap",                     GDK_WINDOW_PIXMAP,             0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_window_class[2] = {
	{ "input-output",               GDK_INPUT_OUTPUT,              0 },
	{ "input-only",                 GDK_INPUT_ONLY,                0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_image_type[3] = {
	{ "normal",                     GDK_IMAGE_NORMAL,              0 },
	{ "shared",                     GDK_IMAGE_SHARED,              0 },
	{ "fastest",                    GDK_IMAGE_FASTEST,             0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_visual_type[6] = {
	{ "static-gray",                GDK_VISUAL_STATIC_GRAY,        0 },
	{ "grayscale",                  GDK_VISUAL_GRAYSCALE,          0 },
	{ "static-color",               GDK_VISUAL_STATIC_COLOR,       0 },
	{ "pseudo-color",               GDK_VISUAL_PSEUDO_COLOR,       0 },
	{ "true-color",                 GDK_VISUAL_TRUE_COLOR,         0 },
	{ "direct-color",               GDK_VISUAL_DIRECT_COLOR,       0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_function[3] = {
	{ "copy",                       GDK_COPY,                      0 },
	{ "invert",                     GDK_INVERT,                    0 },
	{ "xor",                        GDK_XOR,                       0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_fill[4] = {
	{ "solid",                      GDK_SOLID,                     0 },
	{ "tiled",                      GDK_TILED,                     0 },
	{ "stippled",                   GDK_STIPPLED,                  0 },
	{ "opaque-stippled",            GDK_OPAQUE_STIPPLED,           0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_line_style[3] = {
	{ "solid",                      GDK_LINE_SOLID,                0 },
	{ "on-off-dash",                GDK_LINE_ON_OFF_DASH,          0 },
	{ "double-dash",                GDK_LINE_DOUBLE_DASH,          0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_cap_style[4] = {
	{ "not-last",                   GDK_CAP_NOT_LAST,              0 },
	{ "butt",                       GDK_CAP_BUTT,                  0 },
	{ "round",                      GDK_CAP_ROUND,                 0 },
	{ "projecting",                 GDK_CAP_PROJECTING,            0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_join_style[3] = {
	{ "miter",                      GDK_JOIN_MITER,                0 },
	{ "round",                      GDK_JOIN_ROUND,                0 },
	{ "bevel",                      GDK_JOIN_BEVEL,                0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_cursor_type[1] = {
	{ "cursor",                     GDK_LAST_CURSOR,               0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_event_type[32] = {
	{ "nothing",                    GDK_NOTHING,                   0 },
	{ "delete",                     GDK_DELETE,                    0 },
	{ "destroy",                    GDK_DESTROY,                   0 },
	{ "expose",                     GDK_EXPOSE,                    0 },
	{ "motion-notify",              GDK_MOTION_NOTIFY,             0 },
	{ "button-press",               GDK_BUTTON_PRESS,              0 },
	{ "button2-press",              GDK_2BUTTON_PRESS,             0 },
	{ "button3-press",              GDK_3BUTTON_PRESS,             0 },
	{ "button-release",             GDK_BUTTON_RELEASE,            0 },
	{ "key-press",                  GDK_KEY_PRESS,                 0 },
	{ "key-release",                GDK_KEY_RELEASE,               0 },
	{ "enter-notify",               GDK_ENTER_NOTIFY,              0 },
	{ "leave-notify",               GDK_LEAVE_NOTIFY,              0 },
	{ "focus-change",               GDK_FOCUS_CHANGE,              0 },
	{ "configure",                  GDK_CONFIGURE,                 0 },
	{ "map",                        GDK_MAP,                       0 },
	{ "unmap",                      GDK_UNMAP,                     0 },
	{ "property-notify",            GDK_PROPERTY_NOTIFY,           0 },
	{ "selection-clear",            GDK_SELECTION_CLEAR,           0 },
	{ "selection-request",          GDK_SELECTION_REQUEST,         0 },
	{ "selection-notify",           GDK_SELECTION_NOTIFY,          0 },
	{ "proximity-in",               GDK_PROXIMITY_IN,              0 },
	{ "proximity-out",              GDK_PROXIMITY_OUT,             0 },
	{ "drag-enter",                 GDK_DRAG_ENTER,                0 },
	{ "drag-leave",                 GDK_DRAG_LEAVE,                0 },
	{ "drag-motion",                GDK_DRAG_MOTION,               0 },
	{ "drag-status",                GDK_DRAG_STATUS,               0 },
	{ "drop-start",                 GDK_DROP_START,                0 },
	{ "drop-finished",              GDK_DROP_FINISHED,             0 },
	{ "client-event",               GDK_CLIENT_EVENT,              0 },
	{ "visibility-notify",          GDK_VISIBILITY_NOTIFY,         0 },
	{ "no-expose",                  GDK_NO_EXPOSE,                 0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_notify_type[6] = {
	{ "ancestor",                   GDK_NOTIFY_ANCESTOR,           0 },
	{ "virtual",                    GDK_NOTIFY_VIRTUAL,            0 },
	{ "inferior",                   GDK_NOTIFY_INFERIOR,           0 },
	{ "nonlinear",                  GDK_NOTIFY_NONLINEAR,          0 },
	{ "nonlinear-virtual",          GDK_NOTIFY_NONLINEAR_VIRTUAL,  0 },
	{ "unknown",                    GDK_NOTIFY_UNKNOWN,            0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_subwindow_mode[2] = {
	{ "clip-by-children",           GDK_CLIP_BY_CHILDREN,          0 },
	{ "include-inferiors",          GDK_INCLUDE_INFERIORS,         0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_status[5] = {
	{ "ok",                         GDK_OK,                        0 },
	{ "error",                      GDK_ERROR,                     0 },
	{ "error-param",                GDK_ERROR_PARAM,               0 },
	{ "error-file",                 GDK_ERROR_FILE,                0 },
	{ "error-mem",                  GDK_ERROR_MEM,                 0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_byte_order[2] = {
	{ "lsb-first",                  GDK_LSB_FIRST,                 0 },
	{ "msb-first",                  GDK_MSB_FIRST,                 0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_selection[2] = {
	{ "primary",                    GDK_SELECTION_PRIMARY,         0 },
	{ "secondary",                  GDK_SELECTION_SECONDARY,       0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_property_state[2] = {
	{ "new-value",                  GDK_PROPERTY_NEW_VALUE,        0 },
	{ "delete",                     GDK_PROPERTY_DELETE,           0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_prop_mode[3] = {
	{ "replace",                    GDK_PROP_MODE_REPLACE,         0 },
	{ "prepend",                    GDK_PROP_MODE_PREPEND,         0 },
	{ "append",                     GDK_PROP_MODE_APPEND,          0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_overlap_type[3] = {
	{ "in",                         GDK_OVERLAP_RECTANGLE_IN,      0 },
	{ "out",                        GDK_OVERLAP_RECTANGLE_OUT,     0 },
	{ "part",                       GDK_OVERLAP_RECTANGLE_PART,    0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_visibility_state[3] = {
	{ "unobscured",                 GDK_VISIBILITY_UNOBSCURED,     0 },
	{ "partial",                    GDK_VISIBILITY_PARTIAL,        0 },
	{ "fully-obscured",             GDK_VISIBILITY_FULLY_OBSCURED, 0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_input_source[4] = {
	{ "mouse",                      GDK_SOURCE_MOUSE,              0 },
	{ "pen",                        GDK_SOURCE_PEN,                0 },
	{ "eraser",                     GDK_SOURCE_ERASER,             0 },
	{ "cursor",                     GDK_SOURCE_CURSOR,             0 },
};
#endif /*EC_GDK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for flags values
 * ------------------------------------------------------------------------ */

#if EC_GDK_DATASTRUCTURES
static ec_gtk_enum ec_gdk_enum_gdk_window_attributes_type[6] = {
	{ "title",                      GDK_WA_TITLE,                  0 },
	{ "x",                          GDK_WA_X,                      0 },
	{ "y",                          GDK_WA_Y,                      0 },
	{ "cursor",                     GDK_WA_CURSOR,                 0 },
	{ "colormap",                   GDK_WA_COLORMAP,               0 },
	{ "visual",                     GDK_WA_VISUAL,                 0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_window_hints[3] = {
	{ "pos",                        GDK_HINT_POS,                  0 },
	{ "min-size",                   GDK_HINT_MIN_SIZE,             0 },
	{ "max-size",                   GDK_HINT_MAX_SIZE,             0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_event_mask[16] = {
	{ "exposure-mask",              GDK_EXPOSURE_MASK,             0 },
	{ "pointer-motion-mask",        GDK_POINTER_MOTION_MASK,       0 },
	{ "pointer-motion-hint-mask",   GDK_POINTER_MOTION_HINT_MASK,  0 },
	{ "button-motion-mask",         GDK_BUTTON_MOTION_MASK,        0 },
	{ "button1-motion-mask",        GDK_BUTTON1_MOTION_MASK,       0 },
	{ "button2-motion-mask",        GDK_BUTTON2_MOTION_MASK,       0 },
	{ "button3-motion-mask",        GDK_BUTTON3_MOTION_MASK,       0 },
	{ "button-press-mask",          GDK_BUTTON_PRESS_MASK,         0 },
	{ "button-release-mask",        GDK_BUTTON_RELEASE_MASK,       0 },
	{ "key-press-mask",             GDK_KEY_PRESS_MASK,            0 },
	{ "key-release-mask",           GDK_KEY_RELEASE_MASK,          0 },
	{ "enter-notify-mask",          GDK_ENTER_NOTIFY_MASK,         0 },
	{ "leave-notify-mask",          GDK_LEAVE_NOTIFY_MASK,         0 },
	{ "focus-change-mask",          GDK_FOCUS_CHANGE_MASK,         0 },
	{ "structure-mask",             GDK_STRUCTURE_MASK,            0 },
	{ "all-events-mask",            GDK_ALL_EVENTS_MASK,           0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_modifier_type[13] = {
	{ "shift-mask",                 GDK_SHIFT_MASK,                0 },
	{ "lock-mask",                  GDK_LOCK_MASK,                 0 },
	{ "control-mask",               GDK_CONTROL_MASK,              0 },
	{ "mod1-mask",                  GDK_MOD1_MASK,                 0 },
	{ "mod2-mask",                  GDK_MOD2_MASK,                 0 },
	{ "mod3-mask",                  GDK_MOD3_MASK,                 0 },
	{ "mod4-mask",                  GDK_MOD4_MASK,                 0 },
	{ "mod5-mask",                  GDK_MOD5_MASK,                 0 },
	{ "button1-mask",               GDK_BUTTON1_MASK,              0 },
	{ "button2-mask",               GDK_BUTTON2_MASK,              0 },
	{ "button3-mask",               GDK_BUTTON3_MASK,              0 },
	{ "button4-mask",               GDK_BUTTON4_MASK,              0 },
	{ "button5-mask",               GDK_BUTTON5_MASK,              0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_input_condition[3] = {
	{ "read",                       GDK_INPUT_READ,                0 },
	{ "write",                      GDK_INPUT_WRITE,               0 },
	{ "exception",                  GDK_INPUT_EXCEPTION,           0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_gcvalues_mask[18] = {
	{ "foreground",                 GDK_GC_FOREGROUND,             0 },
	{ "background",                 GDK_GC_BACKGROUND,             0 },
	{ "font",                       GDK_GC_FONT,                   0 },
	{ "function",                   GDK_GC_FUNCTION,               0 },
	{ "fill",                       GDK_GC_FILL,                   0 },
	{ "tile",                       GDK_GC_TILE,                   0 },
	{ "stipple",                    GDK_GC_STIPPLE,                0 },
	{ "clip-mask",                  GDK_GC_CLIP_MASK,              0 },
	{ "subwindow",                  GDK_GC_SUBWINDOW,              0 },
	{ "ts-x-origin",                GDK_GC_TS_X_ORIGIN,            0 },
	{ "ts-y-origin",                GDK_GC_TS_Y_ORIGIN,            0 },
	{ "clip-x-origin",              GDK_GC_CLIP_X_ORIGIN,          0 },
	{ "clip-y-origin",              GDK_GC_CLIP_Y_ORIGIN,          0 },
	{ "exposures",                  GDK_GC_EXPOSURES,              0 },
	{ "line-width",                 GDK_GC_LINE_WIDTH,             0 },
	{ "line-style",                 GDK_GC_LINE_STYLE,             0 },
	{ "cap-style",                  GDK_GC_CAP_STYLE,              0 },
	{ "join-style",                 GDK_GC_JOIN_STYLE,             0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_wmdecoration[7] = {
	{ "all",                        GDK_DECOR_ALL,                 0 },
	{ "border",                     GDK_DECOR_BORDER,              0 },
	{ "resizeh",                    GDK_DECOR_RESIZEH,             0 },
	{ "title",                      GDK_DECOR_TITLE,               0 },
	{ "menu",                       GDK_DECOR_MENU,                0 },
	{ "minimize",                   GDK_DECOR_MINIMIZE,            0 },
	{ "maximize",                   GDK_DECOR_MAXIMIZE,            0 },
};
static ec_gtk_enum ec_gdk_enum_gdk_wmfunction[6] = {
	{ "all",                        GDK_FUNC_ALL,                  0 },
	{ "resize",                     GDK_FUNC_RESIZE,               0 },
	{ "move",                       GDK_FUNC_MOVE,                 0 },
	{ "minimize",                   GDK_FUNC_MINIMIZE,             0 },
	{ "maximize",                   GDK_FUNC_MAXIMIZE,             0 },
	{ "close",                      GDK_FUNC_CLOSE,                0 },
};
#endif /*EC_GDK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for enumerations as types
 * ------------------------------------------------------------------------ */

#if EC_GDK_DATASTRUCTURES
ec_gtk_enum_info ec_gdk_enums[] = {
	{ "GdkWindowType",                           6, ec_gdk_enum_gdk_window_type                },
	{ "GdkWindowClass",                          2, ec_gdk_enum_gdk_window_class               },
	{ "GdkImageType",                            3, ec_gdk_enum_gdk_image_type                 },
	{ "GdkVisualType",                           6, ec_gdk_enum_gdk_visual_type                },
	{ "GdkFunction",                             3, ec_gdk_enum_gdk_function                   },
	{ "GdkFill",                                 4, ec_gdk_enum_gdk_fill                       },
	{ "GdkLineStyle",                            3, ec_gdk_enum_gdk_line_style                 },
	{ "GdkCapStyle",                             4, ec_gdk_enum_gdk_cap_style                  },
	{ "GdkJoinStyle",                            3, ec_gdk_enum_gdk_join_style                 },
	{ "GdkCursorType",                           1, ec_gdk_enum_gdk_cursor_type                },
	{ "GdkEventType",                           32, ec_gdk_enum_gdk_event_type                 },
	{ "GdkNotifyType",                           6, ec_gdk_enum_gdk_notify_type                },
	{ "GdkSubwindowMode",                        2, ec_gdk_enum_gdk_subwindow_mode             },
	{ "GdkStatus",                               5, ec_gdk_enum_gdk_status                     },
	{ "GdkByteOrder",                            2, ec_gdk_enum_gdk_byte_order                 },
	{ "GdkSelection",                            2, ec_gdk_enum_gdk_selection                  },
	{ "GdkPropertyState",                        2, ec_gdk_enum_gdk_property_state             },
	{ "GdkPropMode",                             3, ec_gdk_enum_gdk_prop_mode                  },
	{ "GdkOverlapType",                          3, ec_gdk_enum_gdk_overlap_type               },
	{ "GdkVisibilityState",                      3, ec_gdk_enum_gdk_visibility_state           },
	{ "GdkInputSource",                          4, ec_gdk_enum_gdk_input_source               },
};

EcInt ec_gdk_nenums = 21;
#endif /* EC_GDK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Information for flags as types
 * ------------------------------------------------------------------------ */

#if EC_GDK_DATASTRUCTURES
ec_gtk_enum_info ec_gdk_flags[] = {
	{ "GdkWindowAttributesType",                 6, ec_gdk_enum_gdk_window_attributes_type     },
	{ "GdkWindowHints",                          3, ec_gdk_enum_gdk_window_hints               },
	{ "GdkEventMask",                           16, ec_gdk_enum_gdk_event_mask                 },
	{ "GdkModifierType",                        13, ec_gdk_enum_gdk_modifier_type              },
	{ "GdkInputCondition",                       3, ec_gdk_enum_gdk_input_condition            },
	{ "GdkGCValuesMask",                        18, ec_gdk_enum_gdk_gcvalues_mask              },
	{ "GdkWMDecoration",                         7, ec_gdk_enum_gdk_wmdecoration               },
	{ "GdkWMFunction",                           6, ec_gdk_enum_gdk_wmfunction                 },
};

EcInt ec_gdk_nflags = 8;
#endif /* EC_GDK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Symbolic information mapping for humans
 * ------------------------------------------------------------------------ */

#define ec_gdk_t_gdk_window_type                           (ec_gdk_enums[0])
#define ec_gdk_t_gdk_window_class                          (ec_gdk_enums[1])
#define ec_gdk_t_gdk_image_type                            (ec_gdk_enums[2])
#define ec_gdk_t_gdk_visual_type                           (ec_gdk_enums[3])
#define ec_gdk_t_gdk_window_attributes_type                (ec_gdk_flags[0])
#define ec_gdk_t_gdk_window_hints                          (ec_gdk_flags[1])
#define ec_gdk_t_gdk_function                              (ec_gdk_enums[4])
#define ec_gdk_t_gdk_fill                                  (ec_gdk_enums[5])
#define ec_gdk_t_gdk_line_style                            (ec_gdk_enums[6])
#define ec_gdk_t_gdk_cap_style                             (ec_gdk_enums[7])
#define ec_gdk_t_gdk_join_style                            (ec_gdk_enums[8])
#define ec_gdk_t_gdk_cursor_type                           (ec_gdk_enums[9])
#define ec_gdk_t_gdk_event_type                            (ec_gdk_enums[10])
#define ec_gdk_t_gdk_event_mask                            (ec_gdk_flags[2])
#define ec_gdk_t_gdk_notify_type                           (ec_gdk_enums[11])
#define ec_gdk_t_gdk_modifier_type                         (ec_gdk_flags[3])
#define ec_gdk_t_gdk_subwindow_mode                        (ec_gdk_enums[12])
#define ec_gdk_t_gdk_input_condition                       (ec_gdk_flags[4])
#define ec_gdk_t_gdk_status                                (ec_gdk_enums[13])
#define ec_gdk_t_gdk_byte_order                            (ec_gdk_enums[14])
#define ec_gdk_t_gdk_gcvalues_mask                         (ec_gdk_flags[5])
#define ec_gdk_t_gdk_selection                             (ec_gdk_enums[15])
#define ec_gdk_t_gdk_property_state                        (ec_gdk_enums[16])
#define ec_gdk_t_gdk_prop_mode                             (ec_gdk_enums[17])
#define ec_gdk_t_gdk_overlap_type                          (ec_gdk_enums[18])
#define ec_gdk_t_gdk_wmdecoration                          (ec_gdk_flags[6])
#define ec_gdk_t_gdk_wmfunction                            (ec_gdk_flags[7])
#define ec_gdk_t_gdk_visibility_state                      (ec_gdk_enums[19])
#define ec_gdk_t_gdk_input_source                          (ec_gdk_enums[20])

/* ------------------------------------------------------------------------
 *  Boxed types information
 * ------------------------------------------------------------------------ */

#if EC_GDK_DATASTRUCTURES
ec_gtk_boxed_info ec_gdk_boxed[] = {
	{ "GdkColormap",      (EC_GTK_BOXED_COPY)gdk_colormap_ref,    (EC_GTK_BOXED_DESTROY)gdk_colormap_unref,0                    },
	{ "GdkVisual",        (EC_GTK_BOXED_COPY)gdk_visual_ref,      (EC_GTK_BOXED_DESTROY)gdk_visual_unref, 0                    },
	{ "GdkFont",          (EC_GTK_BOXED_COPY)gdk_font_ref,        (EC_GTK_BOXED_DESTROY)gdk_font_unref,   0                    },
	{ "GdkWindow",        (EC_GTK_BOXED_COPY)gdk_window_ref,      (EC_GTK_BOXED_DESTROY)gdk_window_unref, 0                    },
	{ "GdkEvent",         (EC_GTK_BOXED_COPY)gdk_event_copy,      (EC_GTK_BOXED_DESTROY)gdk_event_free,   sizeof(GdkEvent)     },
	{ "GdkColor",         (EC_GTK_BOXED_COPY)gdk_color_copy,      (EC_GTK_BOXED_DESTROY)gdk_color_free,   sizeof(GdkColor)     },
	{ "GdkGC",            (EC_GTK_BOXED_COPY)gdk_gc_ref,          (EC_GTK_BOXED_DESTROY)gdk_gc_unref,     0                    },
};

EcInt ec_gdk_nboxed = 7;
#endif /* EC_GDK_DATASTRUCTURES */

/* ------------------------------------------------------------------------
 *  Boxed & object field mapping
 * ------------------------------------------------------------------------ */

#if EC_GDK_MODULE
#define EC_GDK_COMPOUNDFIELD_RED       0
#define EC_GDK_COMPOUNDFIELD_GREEN     1
#define EC_GDK_COMPOUNDFIELD_BLUE      2

static EcUInt ec_gdk_compoundfield[3];
#endif /* EC_GDK_MODULE */


/* ------------------------------------------------------------------------
 *  Init. function for enumerations/flags symbols & boxed/object fields
 * ------------------------------------------------------------------------ */

#if EC_GDK_MODULE
static void ec_gdk_init_symbols( void )
{
	/* GdkWindowType                  */
	ec_gdk_enum_gdk_window_type[0].symid               = EcInternSymbol( "root" );
	ec_gdk_enum_gdk_window_type[1].symid               = EcInternSymbol( "toplevel" );
	ec_gdk_enum_gdk_window_type[2].symid               = EcInternSymbol( "child" );
	ec_gdk_enum_gdk_window_type[3].symid               = EcInternSymbol( "dialog" );
	ec_gdk_enum_gdk_window_type[4].symid               = EcInternSymbol( "temp" );
	ec_gdk_enum_gdk_window_type[5].symid               = EcInternSymbol( "pixmap" );

	/* GdkWindowClass                 */
	ec_gdk_enum_gdk_window_class[0].symid              = EcInternSymbol( "input-output" );
	ec_gdk_enum_gdk_window_class[1].symid              = EcInternSymbol( "input-only" );

	/* GdkImageType                   */
	ec_gdk_enum_gdk_image_type[0].symid                = EcInternSymbol( "normal" );
	ec_gdk_enum_gdk_image_type[1].symid                = EcInternSymbol( "shared" );
	ec_gdk_enum_gdk_image_type[2].symid                = EcInternSymbol( "fastest" );

	/* GdkVisualType                  */
	ec_gdk_enum_gdk_visual_type[0].symid               = EcInternSymbol( "static-gray" );
	ec_gdk_enum_gdk_visual_type[1].symid               = EcInternSymbol( "grayscale" );
	ec_gdk_enum_gdk_visual_type[2].symid               = EcInternSymbol( "static-color" );
	ec_gdk_enum_gdk_visual_type[3].symid               = EcInternSymbol( "pseudo-color" );
	ec_gdk_enum_gdk_visual_type[4].symid               = EcInternSymbol( "true-color" );
	ec_gdk_enum_gdk_visual_type[5].symid               = EcInternSymbol( "direct-color" );

	/* GdkWindowAttributesType        */
	ec_gdk_enum_gdk_window_attributes_type[0].symid    = EcInternSymbol( "title" );
	ec_gdk_enum_gdk_window_attributes_type[1].symid    = EcInternSymbol( "x" );
	ec_gdk_enum_gdk_window_attributes_type[2].symid    = EcInternSymbol( "y" );
	ec_gdk_enum_gdk_window_attributes_type[3].symid    = EcInternSymbol( "cursor" );
	ec_gdk_enum_gdk_window_attributes_type[4].symid    = EcInternSymbol( "colormap" );
	ec_gdk_enum_gdk_window_attributes_type[5].symid    = EcInternSymbol( "visual" );

	/* GdkWindowHints                 */
	ec_gdk_enum_gdk_window_hints[0].symid              = EcInternSymbol( "pos" );
	ec_gdk_enum_gdk_window_hints[1].symid              = EcInternSymbol( "min-size" );
	ec_gdk_enum_gdk_window_hints[2].symid              = EcInternSymbol( "max-size" );

	/* GdkFunction                    */
	ec_gdk_enum_gdk_function[0].symid                  = EcInternSymbol( "copy" );
	ec_gdk_enum_gdk_function[1].symid                  = EcInternSymbol( "invert" );
	ec_gdk_enum_gdk_function[2].symid                  = EcInternSymbol( "xor" );

	/* GdkFill                        */
	ec_gdk_enum_gdk_fill[0].symid                      = EcInternSymbol( "solid" );
	ec_gdk_enum_gdk_fill[1].symid                      = EcInternSymbol( "tiled" );
	ec_gdk_enum_gdk_fill[2].symid                      = EcInternSymbol( "stippled" );
	ec_gdk_enum_gdk_fill[3].symid                      = EcInternSymbol( "opaque-stippled" );

	/* GdkLineStyle                   */
	ec_gdk_enum_gdk_line_style[0].symid                = EcInternSymbol( "solid" );
	ec_gdk_enum_gdk_line_style[1].symid                = EcInternSymbol( "on-off-dash" );
	ec_gdk_enum_gdk_line_style[2].symid                = EcInternSymbol( "double-dash" );

	/* GdkCapStyle                    */
	ec_gdk_enum_gdk_cap_style[0].symid                 = EcInternSymbol( "not-last" );
	ec_gdk_enum_gdk_cap_style[1].symid                 = EcInternSymbol( "butt" );
	ec_gdk_enum_gdk_cap_style[2].symid                 = EcInternSymbol( "round" );
	ec_gdk_enum_gdk_cap_style[3].symid                 = EcInternSymbol( "projecting" );

	/* GdkJoinStyle                   */
	ec_gdk_enum_gdk_join_style[0].symid                = EcInternSymbol( "miter" );
	ec_gdk_enum_gdk_join_style[1].symid                = EcInternSymbol( "round" );
	ec_gdk_enum_gdk_join_style[2].symid                = EcInternSymbol( "bevel" );

	/* GdkCursorType                  */
	ec_gdk_enum_gdk_cursor_type[0].symid               = EcInternSymbol( "cursor" );

	/* GdkEventType                   */
	ec_gdk_enum_gdk_event_type[0].symid                = EcInternSymbol( "nothing" );
	ec_gdk_enum_gdk_event_type[1].symid                = EcInternSymbol( "delete" );
	ec_gdk_enum_gdk_event_type[2].symid                = EcInternSymbol( "destroy" );
	ec_gdk_enum_gdk_event_type[3].symid                = EcInternSymbol( "expose" );
	ec_gdk_enum_gdk_event_type[4].symid                = EcInternSymbol( "motion-notify" );
	ec_gdk_enum_gdk_event_type[5].symid                = EcInternSymbol( "button-press" );
	ec_gdk_enum_gdk_event_type[6].symid                = EcInternSymbol( "button2-press" );
	ec_gdk_enum_gdk_event_type[7].symid                = EcInternSymbol( "button3-press" );
	ec_gdk_enum_gdk_event_type[8].symid                = EcInternSymbol( "button-release" );
	ec_gdk_enum_gdk_event_type[9].symid                = EcInternSymbol( "key-press" );
	ec_gdk_enum_gdk_event_type[10].symid               = EcInternSymbol( "key-release" );
	ec_gdk_enum_gdk_event_type[11].symid               = EcInternSymbol( "enter-notify" );
	ec_gdk_enum_gdk_event_type[12].symid               = EcInternSymbol( "leave-notify" );
	ec_gdk_enum_gdk_event_type[13].symid               = EcInternSymbol( "focus-change" );
	ec_gdk_enum_gdk_event_type[14].symid               = EcInternSymbol( "configure" );
	ec_gdk_enum_gdk_event_type[15].symid               = EcInternSymbol( "map" );
	ec_gdk_enum_gdk_event_type[16].symid               = EcInternSymbol( "unmap" );
	ec_gdk_enum_gdk_event_type[17].symid               = EcInternSymbol( "property-notify" );
	ec_gdk_enum_gdk_event_type[18].symid               = EcInternSymbol( "selection-clear" );
	ec_gdk_enum_gdk_event_type[19].symid               = EcInternSymbol( "selection-request" );
	ec_gdk_enum_gdk_event_type[20].symid               = EcInternSymbol( "selection-notify" );
	ec_gdk_enum_gdk_event_type[21].symid               = EcInternSymbol( "proximity-in" );
	ec_gdk_enum_gdk_event_type[22].symid               = EcInternSymbol( "proximity-out" );
	ec_gdk_enum_gdk_event_type[23].symid               = EcInternSymbol( "drag-enter" );
	ec_gdk_enum_gdk_event_type[24].symid               = EcInternSymbol( "drag-leave" );
	ec_gdk_enum_gdk_event_type[25].symid               = EcInternSymbol( "drag-motion" );
	ec_gdk_enum_gdk_event_type[26].symid               = EcInternSymbol( "drag-status" );
	ec_gdk_enum_gdk_event_type[27].symid               = EcInternSymbol( "drop-start" );
	ec_gdk_enum_gdk_event_type[28].symid               = EcInternSymbol( "drop-finished" );
	ec_gdk_enum_gdk_event_type[29].symid               = EcInternSymbol( "client-event" );
	ec_gdk_enum_gdk_event_type[30].symid               = EcInternSymbol( "visibility-notify" );
	ec_gdk_enum_gdk_event_type[31].symid               = EcInternSymbol( "no-expose" );

	/* GdkEventMask                   */
	ec_gdk_enum_gdk_event_mask[0].symid                = EcInternSymbol( "exposure-mask" );
	ec_gdk_enum_gdk_event_mask[1].symid                = EcInternSymbol( "pointer-motion-mask" );
	ec_gdk_enum_gdk_event_mask[2].symid                = EcInternSymbol( "pointer-motion-hint-mask" );
	ec_gdk_enum_gdk_event_mask[3].symid                = EcInternSymbol( "button-motion-mask" );
	ec_gdk_enum_gdk_event_mask[4].symid                = EcInternSymbol( "button1-motion-mask" );
	ec_gdk_enum_gdk_event_mask[5].symid                = EcInternSymbol( "button2-motion-mask" );
	ec_gdk_enum_gdk_event_mask[6].symid                = EcInternSymbol( "button3-motion-mask" );
	ec_gdk_enum_gdk_event_mask[7].symid                = EcInternSymbol( "button-press-mask" );
	ec_gdk_enum_gdk_event_mask[8].symid                = EcInternSymbol( "button-release-mask" );
	ec_gdk_enum_gdk_event_mask[9].symid                = EcInternSymbol( "key-press-mask" );
	ec_gdk_enum_gdk_event_mask[10].symid               = EcInternSymbol( "key-release-mask" );
	ec_gdk_enum_gdk_event_mask[11].symid               = EcInternSymbol( "enter-notify-mask" );
	ec_gdk_enum_gdk_event_mask[12].symid               = EcInternSymbol( "leave-notify-mask" );
	ec_gdk_enum_gdk_event_mask[13].symid               = EcInternSymbol( "focus-change-mask" );
	ec_gdk_enum_gdk_event_mask[14].symid               = EcInternSymbol( "structure-mask" );
	ec_gdk_enum_gdk_event_mask[15].symid               = EcInternSymbol( "all-events-mask" );

	/* GdkNotifyType                  */
	ec_gdk_enum_gdk_notify_type[0].symid               = EcInternSymbol( "ancestor" );
	ec_gdk_enum_gdk_notify_type[1].symid               = EcInternSymbol( "virtual" );
	ec_gdk_enum_gdk_notify_type[2].symid               = EcInternSymbol( "inferior" );
	ec_gdk_enum_gdk_notify_type[3].symid               = EcInternSymbol( "nonlinear" );
	ec_gdk_enum_gdk_notify_type[4].symid               = EcInternSymbol( "nonlinear-virtual" );
	ec_gdk_enum_gdk_notify_type[5].symid               = EcInternSymbol( "unknown" );

	/* GdkModifierType                */
	ec_gdk_enum_gdk_modifier_type[0].symid             = EcInternSymbol( "shift-mask" );
	ec_gdk_enum_gdk_modifier_type[1].symid             = EcInternSymbol( "lock-mask" );
	ec_gdk_enum_gdk_modifier_type[2].symid             = EcInternSymbol( "control-mask" );
	ec_gdk_enum_gdk_modifier_type[3].symid             = EcInternSymbol( "mod1-mask" );
	ec_gdk_enum_gdk_modifier_type[4].symid             = EcInternSymbol( "mod2-mask" );
	ec_gdk_enum_gdk_modifier_type[5].symid             = EcInternSymbol( "mod3-mask" );
	ec_gdk_enum_gdk_modifier_type[6].symid             = EcInternSymbol( "mod4-mask" );
	ec_gdk_enum_gdk_modifier_type[7].symid             = EcInternSymbol( "mod5-mask" );
	ec_gdk_enum_gdk_modifier_type[8].symid             = EcInternSymbol( "button1-mask" );
	ec_gdk_enum_gdk_modifier_type[9].symid             = EcInternSymbol( "button2-mask" );
	ec_gdk_enum_gdk_modifier_type[10].symid            = EcInternSymbol( "button3-mask" );
	ec_gdk_enum_gdk_modifier_type[11].symid            = EcInternSymbol( "button4-mask" );
	ec_gdk_enum_gdk_modifier_type[12].symid            = EcInternSymbol( "button5-mask" );

	/* GdkSubwindowMode               */
	ec_gdk_enum_gdk_subwindow_mode[0].symid            = EcInternSymbol( "clip-by-children" );
	ec_gdk_enum_gdk_subwindow_mode[1].symid            = EcInternSymbol( "include-inferiors" );

	/* GdkInputCondition              */
	ec_gdk_enum_gdk_input_condition[0].symid           = EcInternSymbol( "read" );
	ec_gdk_enum_gdk_input_condition[1].symid           = EcInternSymbol( "write" );
	ec_gdk_enum_gdk_input_condition[2].symid           = EcInternSymbol( "exception" );

	/* GdkStatus                      */
	ec_gdk_enum_gdk_status[0].symid                    = EcInternSymbol( "ok" );
	ec_gdk_enum_gdk_status[1].symid                    = EcInternSymbol( "error" );
	ec_gdk_enum_gdk_status[2].symid                    = EcInternSymbol( "error-param" );
	ec_gdk_enum_gdk_status[3].symid                    = EcInternSymbol( "error-file" );
	ec_gdk_enum_gdk_status[4].symid                    = EcInternSymbol( "error-mem" );

	/* GdkByteOrder                   */
	ec_gdk_enum_gdk_byte_order[0].symid                = EcInternSymbol( "lsb-first" );
	ec_gdk_enum_gdk_byte_order[1].symid                = EcInternSymbol( "msb-first" );

	/* GdkGCValuesMask                */
	ec_gdk_enum_gdk_gcvalues_mask[0].symid             = EcInternSymbol( "foreground" );
	ec_gdk_enum_gdk_gcvalues_mask[1].symid             = EcInternSymbol( "background" );
	ec_gdk_enum_gdk_gcvalues_mask[2].symid             = EcInternSymbol( "font" );
	ec_gdk_enum_gdk_gcvalues_mask[3].symid             = EcInternSymbol( "function" );
	ec_gdk_enum_gdk_gcvalues_mask[4].symid             = EcInternSymbol( "fill" );
	ec_gdk_enum_gdk_gcvalues_mask[5].symid             = EcInternSymbol( "tile" );
	ec_gdk_enum_gdk_gcvalues_mask[6].symid             = EcInternSymbol( "stipple" );
	ec_gdk_enum_gdk_gcvalues_mask[7].symid             = EcInternSymbol( "clip-mask" );
	ec_gdk_enum_gdk_gcvalues_mask[8].symid             = EcInternSymbol( "subwindow" );
	ec_gdk_enum_gdk_gcvalues_mask[9].symid             = EcInternSymbol( "ts-x-origin" );
	ec_gdk_enum_gdk_gcvalues_mask[10].symid            = EcInternSymbol( "ts-y-origin" );
	ec_gdk_enum_gdk_gcvalues_mask[11].symid            = EcInternSymbol( "clip-x-origin" );
	ec_gdk_enum_gdk_gcvalues_mask[12].symid            = EcInternSymbol( "clip-y-origin" );
	ec_gdk_enum_gdk_gcvalues_mask[13].symid            = EcInternSymbol( "exposures" );
	ec_gdk_enum_gdk_gcvalues_mask[14].symid            = EcInternSymbol( "line-width" );
	ec_gdk_enum_gdk_gcvalues_mask[15].symid            = EcInternSymbol( "line-style" );
	ec_gdk_enum_gdk_gcvalues_mask[16].symid            = EcInternSymbol( "cap-style" );
	ec_gdk_enum_gdk_gcvalues_mask[17].symid            = EcInternSymbol( "join-style" );

	/* GdkSelection                   */
	ec_gdk_enum_gdk_selection[0].symid                 = EcInternSymbol( "primary" );
	ec_gdk_enum_gdk_selection[1].symid                 = EcInternSymbol( "secondary" );

	/* GdkPropertyState               */
	ec_gdk_enum_gdk_property_state[0].symid            = EcInternSymbol( "new-value" );
	ec_gdk_enum_gdk_property_state[1].symid            = EcInternSymbol( "delete" );

	/* GdkPropMode                    */
	ec_gdk_enum_gdk_prop_mode[0].symid                 = EcInternSymbol( "replace" );
	ec_gdk_enum_gdk_prop_mode[1].symid                 = EcInternSymbol( "prepend" );
	ec_gdk_enum_gdk_prop_mode[2].symid                 = EcInternSymbol( "append" );

	/* GdkOverlapType                 */
	ec_gdk_enum_gdk_overlap_type[0].symid              = EcInternSymbol( "in" );
	ec_gdk_enum_gdk_overlap_type[1].symid              = EcInternSymbol( "out" );
	ec_gdk_enum_gdk_overlap_type[2].symid              = EcInternSymbol( "part" );

	/* GdkWMDecoration                */
	ec_gdk_enum_gdk_wmdecoration[0].symid              = EcInternSymbol( "all" );
	ec_gdk_enum_gdk_wmdecoration[1].symid              = EcInternSymbol( "border" );
	ec_gdk_enum_gdk_wmdecoration[2].symid              = EcInternSymbol( "resizeh" );
	ec_gdk_enum_gdk_wmdecoration[3].symid              = EcInternSymbol( "title" );
	ec_gdk_enum_gdk_wmdecoration[4].symid              = EcInternSymbol( "menu" );
	ec_gdk_enum_gdk_wmdecoration[5].symid              = EcInternSymbol( "minimize" );
	ec_gdk_enum_gdk_wmdecoration[6].symid              = EcInternSymbol( "maximize" );

	/* GdkWMFunction                  */
	ec_gdk_enum_gdk_wmfunction[0].symid                = EcInternSymbol( "all" );
	ec_gdk_enum_gdk_wmfunction[1].symid                = EcInternSymbol( "resize" );
	ec_gdk_enum_gdk_wmfunction[2].symid                = EcInternSymbol( "move" );
	ec_gdk_enum_gdk_wmfunction[3].symid                = EcInternSymbol( "minimize" );
	ec_gdk_enum_gdk_wmfunction[4].symid                = EcInternSymbol( "maximize" );
	ec_gdk_enum_gdk_wmfunction[5].symid                = EcInternSymbol( "close" );

	/* GdkVisibilityState             */
	ec_gdk_enum_gdk_visibility_state[0].symid          = EcInternSymbol( "unobscured" );
	ec_gdk_enum_gdk_visibility_state[1].symid          = EcInternSymbol( "partial" );
	ec_gdk_enum_gdk_visibility_state[2].symid          = EcInternSymbol( "fully-obscured" );

	/* GdkInputSource                 */
	ec_gdk_enum_gdk_input_source[0].symid              = EcInternSymbol( "mouse" );
	ec_gdk_enum_gdk_input_source[1].symid              = EcInternSymbol( "pen" );
	ec_gdk_enum_gdk_input_source[2].symid              = EcInternSymbol( "eraser" );
	ec_gdk_enum_gdk_input_source[3].symid              = EcInternSymbol( "cursor" );


	/* Boxed/Object fields */

	ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_BLUE]    = EcInternSymbol( "blue" );
	ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_RED]     = EcInternSymbol( "red" );
	ec_gdk_compoundfield[EC_GDK_COMPOUNDFIELD_GREEN]   = EcInternSymbol( "green" );
}
#endif /* EC_GDK_MODULE */

#endif /* __GDKGLUE_H */
