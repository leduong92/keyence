#if 0
// *******************************************************************************
// **		Copyright(c) 2013, Keyence Corporation. All rights reserved
// **		File Name	: btLib_newbt.h
// **		Cpu			: 
// **		Compiler	: 
// **
// ** 		Function	: Terminal library header file (newbt)
// ** 		Limitations	:
// **		Note		:
// ** 		History		: 
// ********************************************************************************
#endif
#if !defined(__BTLIB_NEWBT_H__)
#define __BTLIB_NEWBT_H__

#ifdef __cplusplus
extern "C" {
#endif


//********************************************************************************
//			Local macro declarations
//********************************************************************************
#define BT_GUI_ID_ROOT_WINDOW		(10001)			// WidgetID of the root window
#define BT_GUI_ID_PAST_COMPATI		(10002)			// WidgetID of the window for compatibility with older models
#define BT_GUI_MAX_WIDGET_COUNT		(1024)			// Maximum number of widgets
#define BT_GUI_MAX_LIST_ITEMS		(128)			// Maximum number of list box or combo box items

#define BT_MACRO_STARTCUI \
{ \
	long lShow_STARTCUI = BT_ON; \
	btWidgetSetParameter(BT_GUI_ID_PAST_COMPATI, BT_GUI_PT_VISIBLE, &lShow_STARTCUI, sizeof(lShow_STARTCUI)); \
	btClear(); \
}
#define BT_MACRO_ENDCUI \
{ \
	long lShow_ENDCUI = BT_OFF; \
	btClear(); \
	btWidgetSetParameter(BT_GUI_ID_PAST_COMPATI, BT_GUI_PT_VISIBLE, &lShow_ENDCUI, sizeof(lShow_ENDCUI)); \
}

// BT-Widget type
enum{
	// NewBT GUI Widget
	BT_GUI_WT_CANVAS = 0,			// Canvas Window
	BT_GUI_WT_EDIT_FIELD,			// EditField Widget
	BT_GUI_WT_ML_EDIT,				// MultiLineEdit Widget
	BT_GUI_WT_TEXTBOX,				// TextBox Widget
	BT_GUI_WT_GROUPBOX,				// GroupBox Widget
	BT_GUI_WT_LISTBOX,				// ListBox Widget
	BT_GUI_WT_COMBOBOX,				// ComboBox Widget
	BT_GUI_WT_CHECKBOX,				// CheckBox Widget
	BT_GUI_WT_RADIOBUTTON,			// RadioButton Widget
	BT_GUI_WT_BUTTON,				// Button Widget
	BT_GUI_WT_WINDOW,				// Window Widget
};

// BT-Widget setting value
enum{
	BT_GUI_PT_VISIBLE = 0,			// Show or hide
	BT_GUI_PT_ENABLE,				// Enable or disable
	BT_GUI_PT_FRAME,				// Frame shape
	BT_GUI_PT_BGCOLOR,				// Background color
	BT_GUI_PT_TEXTCOLOR,			// Character color
	BT_GUI_PT_SEL_BGCOLOR,			// Selected background color
	BT_GUI_PT_SEL_TEXTCOLOR,		// Selected character color
	BT_GUI_PT_EXCLUSIVE,			// Group exclusion
	BT_GUI_PT_TEXTDATA,				// Text data
	BT_GUI_PT_IMAGE_FILE,			// Image file (setting only)
	BT_GUI_PT_FONTSIZE,				// Font size
	BT_GUI_PT_TEXTALIGN,			// Text alignment (left-aligned, centered, or right-aligned)
	BT_GUI_PT_HSCROLLBAR,			// Showing or hiding the (horizontal) scroll bar
	BT_GUI_PT_VSCROLLBAR,			// Showing or hiding the (vertical) scroll bar
	BT_GUI_PT_STRLEN,				// Acquiring the number of characters in the text (acquisition only)
	BT_GUI_PT_CHECKED,				// Check box or radio button selection status
	BT_GUI_PT_INPUTMAX,				// Character entry limit (maximum) of edit fields and multi-line edit fields
	BT_GUI_PT_TRG_MODE,				// Trigger mode of edit fields and multi-line edit fields
	BT_GUI_PT_F3KEY_MODE,			// F3 key mode of edit fields and multi-line edit fields
	BT_GUI_PT_FOCUSIN_EDIT,			// Editing permission of edit fields and multi-line edit fields when they attain the focus
	BT_GUI_PT_AUTOREAD_ENABLE,		// Whether code reading is possible when edit fields and multi-line edit fields attain the focus
	BT_GUI_PT_KEYINPUT_ENABLE,		// Whether key entry is possible in edit fields and multi-line edit fields
	BT_GUI_PT_FOCUSINDICATOR,		// Frame color of the widget that has the focus
};

// Widget frame type
#define BT_FRAME_NONE			(0x0001)		// No frame
#define BT_FRAME_THIN			(0x0002)		// Border line
#define BT_FRAME_RAISED			(0x0004)		// Emboss
#define BT_FRAME_RECESSED		(0x0008)		// Engrave

// Character string display position
#define BT_ALIGN_RIGHT			(0x0020)		// Right-aligned
#define BT_ALIGN_LEFT			(0x0040)		// Left-aligned
#define BT_ALIGN_CENTER			(0x0080)		// Centered

// Operation of the F3 key in edit-type widgets
#define BT_F3MODE_NONE			(0)				// None
#define BT_F3MODE_EDIT			(1)				// Start Japanese input.
#define BT_F3MODE_CALC			(2)				// Calculator
#define BT_F3MODE_SHIFT_OFF		(3)				// Shift mode switching (disabled by default)
#define BT_F3MODE_SHIFT_ON		(4)				// Shift mode switching (enabled by default)



//********************************************************************************
//			Structure declarations
//********************************************************************************
// NewBT-GUI: Font information structure
typedef struct _bt_gui_fontinfo{
	short sFontSize;		// Font size
	short sReserve1;
	ULONG ulTextColor;		// Character color
	ULONG ulBkgColor;		// Background color
}BT_GUI_FONTINFO;

// NewBT-GUI: DrawEx information structure
typedef struct _bt_gui_drawex{
	// Font and color information
	BT_GUI_FONTINFO fontinfo;
	short gapx;				// X gap
	short gapy;				// Y gap
	// Character decoration, etc. (specifiable at the same time)
	short type;				// 0x0001: Underline (drawn at a position two dots below the bottom of the character; enabled when the Y gap is two or more)
							// 0x0002: Strikethrough (single)
							// 0x0004: Strikethrough (double)
							// 0x0008: Bold (shift characters by one dot in the X direction; enabled when the X gap is one or more)
							// 0x0010: Transparency (do not draw the background color; the display may not work when the characters have a Å~n magnification)
	short sReserve1;
	// Character format
	DSPATTRG attrg;			// GPOSITION gposition: Display coordinates
							// short attribute; display attributes (BT_ATRB_NORM(1), BT_ATRB_REV(2))
							// short mag; display magnification (BT_MAG_NORM(1) - BT_MAG_25(7))
							// short fold; multi-line display (line wrap); BT_FOLD_OFF (1) - BT_FOLD_INDENT(3))
}BT_GUI_DRAWEX;

#ifdef __cplusplus
}
#endif


#endif // !defined(__BTLIB_NEWBT_H__)
