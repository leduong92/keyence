#if 0
// *******************************************************************************
// **		Copyright(c) 2013, Keyence Corporation. All rights reserved
// **		File Name	: btLib.h
// **		Cpu			: 
// **		Compiler	: 
// **
// ** 		Function	: Terminal library header file 
// ** 		Limitations	:
// **		Note		:
// ** 		History		: 
// **
// ********************************************************************************
#endif
#if !defined(__BTLIB_H__)
#define __BTLIB_H__

#ifdef __cplusplus
extern "C" {
#endif


//********************************************************************************
//			Include Files
//********************************************************************************
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <malloc.h>

//********************************************************************************
//			Local macro declarations
//********************************************************************************

//==================================
// Shared information
//==================================
typedef unsigned char				UCHAR;			// Unsigned, 8-bit integer format
typedef signed short				SHORT;			// Signed, 16-bit integer format
typedef unsigned short				USHORT;			// Unsigned, 16-bit integer format
typedef signed long					LONG;			// Signed, 32-bit integer format
typedef unsigned long				ULONG;			// Unsigned, 32-bit integer format
typedef double						DOUBLE;			// Floating point format

#ifndef VOID
typedef void						VOID;			// Void format
typedef char						CHAR;			// Signed, 8-bit integer format
#ifndef ITRON_H
typedef int							BOOL;			// Boolean format
#endif
#endif

#ifndef TRUE
#define TRUE						(1)
#define FALSE						(0)
#endif

#ifndef NULL
#define NULL						(0)
#endif

#define	DECIMAL						long			// Storage multiple 1000 
													// Decimal 123.456 is storage in 123456

#define	BT_OK						(0)
#define	BT_NG						(-1)

#define	BT_TYPE_950W				(1)				// Wireless 2D HT
#define	BT_TYPE_950B				(2)				// Batch-type 2D HT
#define	BT_TYPE_910					(3)				// Wireless barcode HT
#define	BT_TYPE_3000W				(10)			// BT-3000W: Wireless barcode HT with a big screen
#define	BT_TYPE_3000WB				(11)			// BT-3000WB: Wireless barcode HT (Bluetooth) with a big screen
#define	BT_TYPE_3000WC				(12)			// BT-3000WC: Wireless barcode HT with a big screen[Chinese]

#define	BT_TYPE_1000				(20)			// BT-1000: Barcode HT with a high-definition screen
#define	BT_TYPE_1000B				(21)			// BT-1000B: Barcode HT (Bluetooth) with a high-definition screen
#define	BT_TYPE_1000W				(22)			// BT-1000W: Wireless barcode HT with a high-definition screen
#define	BT_TYPE_1000WB				(23)			// BT-1000WB: Wireless barcode HT (Bluetooth) with a high-definition screen
#define	BT_TYPE_1000C				(24)			// BT-1000C: Barcode HT with a high-definition screen[Chinese]
#define	BT_TYPE_1000WC				(25)			// BT-1000WC: Wireless barcode HT with a high-definition screen[Chinese]
#define	BT_TYPE_1000WBG				(26)			// BT-1000WBG: Wireless barcode HT (Bluetooth) with a high-definition screen[global]
#define	BT_TYPE_1000WBGM			(27)			// BT-1000WBGM: Wireless barcode HT (Bluetooth) with a high-definition screen[global]

#define	BT_TYPE_1500				(30)			// BT-1500: 2D code HT with a high-definition screen
#define	BT_TYPE_1500B				(31)			// BT-1500B: 2D code HT (Bluetooth) with a high-definition screen
#define	BT_TYPE_1500W				(32)			// BT-1500W: Wireless 2D code HT with a high-definition screen
#define	BT_TYPE_1500WB				(33)			// BT-1500WB: Wireless 2D code HT (Bluetooth) with a high-definition screen
#define	BT_TYPE_1500C				(34)			// BT-1500C: 2D code HT with a high-definition screen[Chinese]
#define	BT_TYPE_1500WC				(35)			// BT-1500WC: Wireless 2D code HT with a high-definition screen[Chinese]
#define	BT_TYPE_1500WBG				(36)			// BT-1500WBG: Wireless 2D code HT (Bluetooth) with a high-definition screen[global]
#define	BT_TYPE_1500WBGM			(37)			// BT-1500WBGM: Wireless 2D code HT (Bluetooth) with a high-definition screen[global]

#define	BT_TYPE_600					(40)			// BT-600: Barcode HT with a color screen
#define	BT_TYPE_600B				(41)			// BT-600B: Barcode HT (Bluetooth) with a color screen
#define	BT_TYPE_600C				(42)			// BT-600C: Barcode HT with a color screen[Chinese]


// Maximum character string length
#define	BT_MAX_STRLEN				(8192)			// Maximum character string length
//Processing result
#define	NO_ERR						(0)				// Normal completion
// Minimum and maximum values of a value
#define	BT_CHAR_MIN					(-128)			// Minimum CHAR value
#define	BT_CHAR_MAX					(127)			// Maximum CHAR value
#define	BT_UCHAR_MIN				(0)				// Minimum UCHAR value
#define	BT_UCHAR_MAX				(255)			// Maximum UCHAR value
#define	BT_SHORT_MIN				(-32768)		// Minimum SHORT value
#define	BT_SHORT_MAX				(32767)			// Maximum SHORT value
#define	BT_USHORT_MIN				(0)				// Minimum USHORT value
#define	BT_USHORT_MAX				(65535)			// Maximum USHORT value
#define	BT_LONG_MIN					(-2147483648)	// Minimum LONG value
#define	BT_LONG_MAX					(2147483647)	// Maximum LONG value
#define	BT_ULONG_MIN				(0)				// Minimum ULONG value
#define	BT_ULONG_MAX				(4294967295)	// Maximum ULONG value

#define	BT_YEAR_MIN					(1980)			// Minimum year value
#define	BT_YEAR_MAX					(2079)			// Maximum year value
#define	BT_ADD_DATE_MIN				(-100)			// Minimum addition day value
#define	BT_ADD_DATE_MAX				(100)			// Maximum addition day value

//==================================
// Error information
//==================================
#define	NO_ERR						(0)				// Normal completion
#define	ERR_BCR_TIMEOUT				(-1)			// Time-out
#define	ERR_BCR_LASEROFF			(-2)			// Turning off the laser
#define	ERR_BCR_READERROR			(-3)			// Reading error
#define	ERR_BCR_DATA				(-4)			// Illegal barcode data
#define	ERR_BCR_INPUT				(-5)			// Barcode input failure
#define	ERR_FILE_DRIVENAME			(-6)			// Drive name error
#define	ERR_FILE_FILENAME			(-7)			// File name error
#define	ERR_FILE_NONAME				(-8)			// The file name has not been specified.
#define	ERR_FILE_MODE				(-9)			// This cannot be opened in the corresponding mode.
#define	ERR_FILE_NOT_EXIST			(-10)			// File does not exist
#define	ERR_FILE_ALREADY_EXIST		(-11)			// The file already exists.
#define	ERR_FILE_ALREADY_OPEN		(-12)			// The file is already open.
#define	ERR_FILE_NOT_OPEN			(-13)			// The file has not been opened.
#define	ERR_FILE_FORMAT				(-14)			// File format error
#define	ERR_FILE_ENTRY_OVER			(-15)			// Number of file entries exceeded
#define	ERR_FILE_NOSPACE			(-16)			// Insufficient free space
#define	ERR_FILE_MAXOPEN			(-17)			// Number of open files exceeded
#define	ERR_FILE_SIZE				(-18)			// The address of a file whose size exceeds the file size was specified.
#define	ERR_FILE_RECNO_NOT_EXIST	(-20)			// The record number does not exist.
#define	ERR_FILE_INDEX_NOT_EXIST	(-21)			// The index number does not exist.
#define	ERR_FILE_NOT_FOUND			(-22)			// Cannot find the corresponding file.
#define	ERR_FILE_ADDRESS			(-23)			// Address error
#define	ERR_FILE_ADRS_ALIGN			(-24)			// Address border value error
#define	ERR_FILE_SUM				(-25)			// File sum value error
#define	ERR_FILE_ROMERASE			(-27)			// ROM clear failure
#define	ERR_SRCH_NOT_FOUND			(-29)			// A search was performed, but the corresponding data could not be found.
#define	ERR_MENU_CAN				(-30)			// The menu display was canceled.
#define	ERR_MENU_DATAERR			(-31)			// The menu information structure is illegal.
#define	ERR_KEYBUF_OVERFLOW			(-32)			// Key entry buffer overflow
#define	ERR_DEBUGPORT_DISABLE		(-34)			// Debug port disabled
#define	ERR_FILEREC_OVERFLOW		(-35)			// The maximum number of file records has been exceeded.
#define	ERR_INPUT_INIT_STRING		(-36)			// Illegal default character input value
#define	ERR_DEBUGPORT_ENABLE		(-37)			// Debug port enabled
#define	ERR_VAL_RANGE_UPPER			(-38)			// Upper limit error
#define	ERR_VAL_RANGE_LOWER			(-39)			// Lower limit error
#define	ERR_LCD_OUT_RANGE			(-40)			// The input field does not fit on the LCD.
#define	ERR_LOGREC_RECORDLEN		(-41)			// Log record length error
#define	ERR_LOGREC_FIELDLEN			(-42)			// Log field length error
#define	ERR_LOGREC_FIELDCNT			(-43)			// Log field count error
#define	ERR_DEBUGPORT_REFUSED		(-44)			// The use of the debug port has been prohibited from the system menu.
#define	ERR_SERIAL_NOT_OPEN			(-45)			// The serial port has not been opened.
#define	ERR_SERIAL_ALREADY_OPEN		(-46)			// A device that cannot be used at the same time with this one is already in use.
#define	ERR_WINDOW_NO_SPACE			(-47)			// There is no window screen save area.
#define	ERR_LCDRANGE				(-48)			// This does not fit on the LCD screen.
#define	ERR_VRAMRANGE				(-49)			// This does not fit in the VRAM area.
#define	ERR_MASTER_NOT_FOUND 		(-50)			// Cannot find the master file.
#define	ERR_MASTER_ALREADY_OPEN 	(-51)			// The file is already open.
#define	ERR_MASTER_NOT_OPEN			(-52)			// The master file has not been opened.
#define	ERR_MASTER_MAXOPEN 			(-53)			// Number of open files exceeded
#define	ERR_MASTER_FORMAT			(-54)			// The file format is illegal.
#define	ERR_MASTER_RECORD_OVER		(-55)			// There are too many records.
#define	ERR_MASTER_NOT_MATCH		(-56)			// No records that match the search conditions can be found.
#define	ERR_MASTER_NOT_NEXT			(-57)			// GetFirst has not been executed.
#define	ERR_MASTER_RECNO_NOT_FOUND	(-58)			// The record number does not exist.
#define	ERR_MASTER_RECNO_NOT_EXIST	(-59)			// The record number cannot be found because it has been deleted.
#define	ERR_OUTSIDE_OF_SUPPORT		(-60)			// Unsupported
#define	ERR_TIMEOUT					(-61)			// Time-out
#define	ERR_FILE_DIRECTORYNAME		(-62)			// Directory name error
#define	ERR_FILE_DRIVEACESS			(-63)			// The drive cannot be accessed (the media is not inserted).
#define BT_RESULT_EVENTNOTIFY		(-64)			// The function ended because of the specified event notification.
#define BT_ERROR_CANCELED			(-65)			// The operation has ended because it was canceled.

#define	ERR_IRDA_NOT_OPEN			ERR_SERIAL_NOT_OPEN			// The IrDA port has not been opened.
#define	ERR_IRDA_ALREADY_OPEN		ERR_SERIAL_ALREADY_OPEN		// The IrDA port is already open.


// Argument range error
#define	ERR_ARG_RANGE_1				(-101)			// Argument range error (argument 1)
#define	ERR_ARG_RANGE_2				(-102)			// Argument range error (argument 2)
#define	ERR_ARG_RANGE_3				(-103)			// Argument range error (argument 3)
#define	ERR_ARG_RANGE_4				(-104)			// Argument range error (argument 4)
#define	ERR_ARG_RANGE_5				(-105)			// Argument range error (argument 5)
#define	ERR_ARG_RANGE_6				(-106)			// Argument range error (argument 6)

// BCR settings error
#define	ERR_ARG_RANGE_JAN			(-111)			// JAN range error
#define	ERR_ARG_RANGE_C39			(-112)			// CODE39 range error
#define	ERR_ARG_RANGE_E128			(-113)			// EAN128 range error
#define	ERR_ARG_RANGE_C128			(-114)			// CODE128 range error
#define	ERR_ARG_RANGE_ITF			(-115)			// ITF range error
#define	ERR_ARG_RANGE_NW7			(-116)			// NW-7 range error
#define	ERR_ARG_RANGE_C93			(-117)			// CODE93 range error
#define	ERR_ARG_RANGE_TOF			(-118)			// Industrial 2 of 5 range error
#define	ERR_ARG_RANGE_COOP			(-119)			// COOP range error
#define	ERR_ARG_RANGE_QR			(-120)			// QR settings range error
#define	ERR_ARG_RANGE_PDF			(-121)			// PDF settings range error
#define	ERR_ARG_RANGE_DM			(-122)			// DM settings range error
#define	ERR_ARG_RANGE_MC			(-123)			// MC settings range error
#define	ERR_ARG_RANGE_RSS			(-124)			// RSS settings range error
#define	ERR_ARG_RANGE_COMPOSITE		(-127)			// COMPOSITE settings range error
#define	ERR_ARG_RANGE_GS1_DATABAR	ERR_ARG_RANGE_RSS	// GS1 Databar settings range error
#define	ERR_ARG_RANGE_GS1_128		ERR_ARG_RANGE_E128	// GS1-128 settings range error


//==================================
// Settings during operations
//==================================
#define BT_CRADLE_WAKEUP_ON			(1)				// Communication unit (RS232C or USB) wakeup on
#define	BT_RF_WAKEUP_ON				(2)				// RF wakeup on
#define BT_LANCRADLE_WAKEUP_ON		(4)				// Communication unit (LAN) wakeup on
#define BT_RF_WAKEUP_ALWAYS			(16)			// RF wakeup: Enabled on a unit other than a communication unit
#define	BT_IRDA_WAKEUP_ON			BT_CRADLE_WAKEUP_ON		// IrDA wakeup on -> communication unit (RS232C or USB) wakeup on

//==================================
// Display
//==================================
#define	BT_FNT_12					(1)				// 12 Å~ 12 size font
#define	BT_FNT_16					(2)				// 16 Å~ 16 size font
#define BT_FNT_24					(3)				// 24 Å~ 24 size font


#define BT_FNTSIZE_SMALL			(16)			// Width of the small-size font
#define BT_FNTSIZE_NORMAL			(24)			// Width of the normal-size font
#define BT_FNTSIZE_BIG				(32)			// Width of the large-size font

#define BT_FNT_SMALL				(BT_FNTSIZE_SMALL)	// Small-size font
#define BT_FNT_NORMAL				(BT_FNTSIZE_NORMAL)	// Normal-size font
#define BT_FNT_BIG					(BT_FNTSIZE_BIG)	// Large-size font

#define	BT_ATRB_NORM				(0x01)			// Normal attribute
#define	BT_ATRB_REV					(0x02)			// Inversion attribute
#define	BT_ATRB_SEL					(0x04)			// Selection attribute
#define	BT_ATRB_NOPAINT				(1)				// Without fill
#define	BT_ATRB_PAINT				(2)				// With fill

#define	BT_MAG_NORM					(1)				// Monospace
#define	BT_MAG_WIDE					(2)				// Å~2, horizontal
#define	BT_MAG_TALL					(3)				// Å~2, vertical
#define	BT_MAG_4					(4)				// Å~4 (Å~2 in both the vertical and horizontal directions)
#define	BT_MAG_9					(5)				// Å~9 (Å~3 in both the vertical and horizontal directions)
#define	BT_MAG_16					(6)				// Å~16 (Å~4 in both the vertical and horizontal directions)
#define	BT_MAG_25					(7)				// Å~25 (Å~5 in both the vertical and horizontal directions)

#define	BT_FOLD_OFF					(1)				// No line wrap
#define	BT_FOLD_ON					(2)				// Line wrap supported (without indents)
#define	BT_FOLD_INDENT				(3)				// Line wrap supported (with indents)

#define	BT_CURSOR_HIDE				(0)				// Cursor display off
#define	BT_CURSOR_SHOW				(6)				// Cursor display on (box)
#define	BT_CURSOR_SHOW_BLINK		(7)				// Blinking cursor display on (box)

#define	BT_STATUS_OFF				(0)				// Status display off
#define	BT_STATUS_NONE				(0)				// Status display off
#define	BT_STATUS_LOWER				(1)				// Showing the status display in the lower part of the screen
#define	BT_STATUS_UPPER				(2)				// Showing the status display in the upper part of the screen
#define	BT_STATUS_CLOCK_LOWER		(3)				// Showing the status display in the lower part of the screen (displaying the time)
#define	BT_STATUS_CLOCK_UPPER		(4)				// Showing the status display in the upper part of the screen (displaying the time)

// Updating the contents shown on the LCD
#define	BT_UPDATE_ON				(1)				// Display updating enabled
#define	BT_UPDATE_OFF				(2)				// Display updating disabled

#define	BT_SCROLL_OFF				(0x00)			// Scroll guide hidden
#define	BT_SCROLL_UP				(0x01)			// Scroll guide displayed in the up direction
#define	BT_SCROLL_DOWN				(0x02)			// Scroll guide displayed in the down direction
#define	BT_SCROLL_LEFT				(0x04)			// Scroll guide displayed in the left direction
#define	BT_SCROLL_RIGHT				(0x08)			// Scroll guide displayed in the right direction

//----------------------------------------------------------------------
// For the QVGA LCD (normally use this)
#define	BT_LCD_HEIGHT_QVGA			(320)			// LCD height
#define	BT_LCD_WIDTH_QVGA			(240)			// LCD width
#define	BT_VRAM_HEIGHT_QVGA			(1280)			// VRAM height (LCD height Å~ 4 screens)
#define	BT_VRAM_WIDTH_QVGA			(240)			// VRAM width
// For the window for compatibility with older models (normally do not use this)
#define	BT_LCD_HEIGHT				(160)			// LCD height
#define	BT_LCD_WIDTH				(120)			// LCD width
#define	BT_VRAM_HEIGHT				(BT_LCD_HEIGHT * 4)		// VRAM height (LCD height Å~ 4 screens)
#define	BT_VRAM_WIDTH				(120)			// VRAM width

//----------------------------------------------------------------------

#define	BT_STATUS_HEIGHT			(28)			// Status height

// Menu
#define	BT_MENU_TKEYDIRECT_OFF		(0)				// Turning off direct input from the numeric keypad
#define	BT_MENU_TKEYDIRECT_ON1		(1)				// Turning on direct input from the numeric keypad (only for the items displayed on the current screen)
#define	BT_MENU_TKEYDIRECT_ON2		(2)				// Turning on direct input from the numeric keypad (for items displayed on other screens as well)
#define	BT_MENU_SELMAX				(16)			// Maximum number of selected items
#define	BT_MENU_CAN					(ERR_MENU_CAN)	// The menu display was canceled.

#define	BT_MB_OKONLY				(0x0001)		// OK only
#define	BT_MB_CONFONLY				(0x0002)		// Confirmation only
#define	BT_MB_OKCANCEL				(0x0004)		// OK and Cancel
#define	BT_MB_YESNO					(0x0008)		// Yes and No
#define	BT_MB_OK					(0x0010)		// OK
#define	BT_MB_CONF					(0x0020)		// Confirmation
#define	BT_MB_CANCEL				(0x0040)		// Cancel
#define	BT_MB_YES					(0x0080)		// YES
#define	BT_MB_NO					(0x0100)		// NO
#define	BT_MB_FRAME1				(0x0200)		// 1-dot-wide frame
#define	BT_MB_FRAME2				(0x0400)		// 2-dot-wide frame
#define	BT_MB_FRAME3				(0x0800)		// 3D (1-dot wide)
#define	BT_MB_FRAME4				(0x1000)		// 3D (2-dots wide)
#define	BT_MB_ASYNC_SHOW			(0x2000)		// Asynchronous display
#define	BT_MB_ASYNC_HIDE			(0x4000)		// Clearing an asynchronous display

#define	BT_MB_ICONNONE				(0x0000)		// Information icon display (fixed value in MessageBox)
#define	BT_MB_ICONINFORMATION		(0x1000)		// Information icon display (control value in MessageBoxEx)
#define	BT_MB_ICONEXCLAMATION		(0x2000)		// Exclamation mark (!) icon display
#define	BT_MB_ICONQUESTION			(0x4000)		// Question mark icon display
#define	BT_MB_ICONSTOP				(0x8000)		// Stop icon display

// Box display color
#define	BT_COLOR_BLACK				(1)				// Black
#define	BT_COLOR_WHITE				(2)				// White
#define	BT_COLOR_REVERSE			(3)				// Inversion (XOR)
#define	BT_COLOR_CURRENT			(4)				// Dot setting color set with btSetColor
#define	BT_COLOR_CURRENT_SELECT		(5)				// Selection setting set with btSetColor
// Fill
#define	BT_ATRB_NOPAINT				(1)				// Without fill
#define	BT_ATRB_PAINT				(2)				// With fill


//==================================
// Device control
//==================================
#define	BT_OFF						(0)				// OFF
#define	BT_ON						(1)				// ON
#define	BT_ON_ASYNC					(1)				// Operation 1 (do not wait for completion)
#define	BT_ON_SYNC					(2)				// Operation 2 (wait for completion)
#define	BT_ON_ALWAYS				(2)				// Normally on

//Buzzer
#define	BT_BUZZER_INSTANT			(1050)			// Short buzzer

//LED
#define	BT_LED_GREEN				(1)				// Green LED (GREEN_ON)
#define	BT_LED_RED					(2)				// Red LED (RED_ON)
#define	BT_LED_ORANGE				(3)				// Orange LED (green + red; ORANGE_ON)
#define	BT_LED_OFF					(0)				// LED off (OFF)

//Backlight
#define	BT_BACKLIGHT_OFF			(0)				// Backlight off
#define	BT_BACKLIGHT_ON1			(2)				// Backlight on (dark)
#define	BT_BACKLIGHT_ON2			(4)				// Backlight on (bright)
#define	BT_BACKLIGHT_ON3			(7)				// Backlight on (brightest)
#define	BT_BACKLIGHT_AUTO			(100)			// Operate in automatic dimmer mode
#define	BT_BACKLIGHT_TIME_MIN		(1)				// Lights off time (units of 1 s; minimum value)
#define	BT_BACKLIGHT_TIME_MAX		(127)			// Lights off time (units of 1 s; maximum value)
#define	BT_BACKLIGHT_ON_NOW			(1)				// Perform operations by executing this function.
#define	BT_BACKLIGHT_ON_EVT			(2)				// The time when the next cause of lighting occurs
#define	BT_BACKLIGHT_TIME_DEF		(10)			// Lights off time control value


//==================================
// Key settings
//==================================
// Key table
#define	KEY_F1						(0x00000001)	// F1 key
#define	KEY_F2						(0x00000002)	// F2 key
#define	KEY_F3						(0x00000004)	// F3 key
#define	KEY_PERIOD					(0x00000008)	// '.' key
#define	KEY_MINUS					(0x00000010)	// '-' key
#define	KEY_0						(0x00000020)	// 0 key
#define	KEY_1						(0x00000040)	// 1 key
#define	KEY_2						(0x00000080)	// 2 key
#define	KEY_3						(0x00000100)	// 3 key
#define	KEY_4						(0x00000200)	// 4 key
#define	KEY_5						(0x00000400)	// 5 key
#define	KEY_6						(0x00000800)	// 6 key
#define	KEY_7						(0x00001000)	// 7 key
#define	KEY_8						(0x00002000)	// 8 key
#define	KEY_9						(0x00004000)	// 9 key
#define	KEY_CLR						(0x00008000)	// Clear key (BS key)
#define	KEY_UP						(0x00010000)	// Up key
#define	KEY_DOWN					(0x00020000)	// Down key
#define	KEY_LEFT					(0x00040000)	// Left key
#define	KEY_RIGHT					(0x00080000)	// Right key
#define	KEY_TRG						(0x00100000)	// Trigger key
#define	KEY_L						(0x00200000)	// L key
#define	KEY_R						(0x00400000)	// R key
#define	KEY_ENT						(0x00800000)	// ENT key
#define	KEY_SCAN					(0x10000000)	// During scanning
#define	KEY_LONG					(0x20000000)	// While a key is being held down
#define	KEY_SHIFT					(0x40000000)	// Shift enabled

// Key code
#define	KEYCODE_NOTHING				(0x00)		
#define	KEYCODE_F1					(0x11)			// F1 key
#define	KEYCODE_F2					(0x12)			// F2 key
#define	KEYCODE_F3					(0x13)			// F3 key
#define	KEYCODE_PERIOD				('.')			// '.' key
#define	KEYCODE_MINUS				('-')			// '-' key
#define	KEYCODE_0					('0')			// 0 key
#define	KEYCODE_1					('1')			// 1 key
#define	KEYCODE_2					('2')			// 2 key
#define	KEYCODE_3					('3')			// 3 key
#define	KEYCODE_4					('4')			// 4 key
#define	KEYCODE_5					('5')			// 5 key
#define	KEYCODE_6					('6')			// 6 key
#define	KEYCODE_7					('7')			// 7 key
#define	KEYCODE_8					('8')			// 8 key
#define	KEYCODE_9					('9')			// 9 key
#define	KEYCODE_CLR					(0x18)			// Clear key
#define	KEYCODE_BS					(0x19)			// BS key
#define	KEYCODE_UP					(0x14)			// Up key
#define	KEYCODE_DOWN				(0x15)			// Down key
#define	KEYCODE_LEFT				(0x16)			// Left key
#define	KEYCODE_RIGHT				(0x17)			// Right key
#define	KEYCODE_TRG					(0x10)			// Trigger key
#define	KEYCODE_L					(0x0e)			// L key
#define	KEYCODE_R					(0x0f)			// R key
#define	KEYCODE_ENT					(0x0d)			// ENT key
#define	KEYCODE_SHIFT				(0x1a)			// Shift key

#define	BT_KEYbufFMAX				(256)			// Key buffer size


//==================================
// Input processing
//==================================
// Input target classification (a value that is used within the terminal library)
#define	BT_KEYINPUT					(0x0001)		// Key input
#define	BT_BCRINPUT					(0x0002)		// Barcode input
#define	BT_SHIFT_ENABLE				(0x0004)		// Shift enabled or disabled
#define	BT_SHIFT_ON					(0x0008)		// Shift on
#define	BT_SHIFT_OFF				(0x0010)		// Shift off
#define	BT_KEYCALCINPUT				(0x0200)		// Calculator input
#define	BT_ESCAPE_ON				(0x0400)		// Exit from input when F1, F2, L, or R is pressed.
#define	BT_INIT_ERASE_OFF			(0x0800)		// The default value is not cleared even when a direction key is pressed.

// Value input classification: Used with btInputInteger() and btInputDecimal()
#define	BT_INPUT_NUM				(BT_KEYINPUT)											// Value input
#define	BT_INPUT_NUMCALC			(BT_KEYCALCINPUT)										// Value input (with calculator function)
// Character string and barcode input classification: Used with btInputString()
#define	BT_INPUT_KEY_SFT_ON			(BT_KEYINPUT|BT_SHIFT_ENABLE|BT_SHIFT_ON)				// Character string input (shift on)
#define	BT_INPUT_KEY_SFT_OFF		(BT_KEYINPUT|BT_SHIFT_ENABLE|BT_SHIFT_OFF)				// Character string input (shift off)
#define	BT_INPUT_KEY				(BT_KEYINPUT|BT_SHIFT_OFF)								// Character string input (shift disabled)
#define	BT_INPUT_BCR				(BT_BCRINPUT|BT_SHIFT_OFF)								// Barcode input
#define	BT_INPUT_KEYBCR_SFT_ON		(BT_KEYINPUT|BT_BCRINPUT|BT_SHIFT_ENABLE|BT_SHIFT_ON)	// Character string and barcode input (shift on)
#define	BT_INPUT_KEYBCR_SFT_OFF		(BT_KEYINPUT|BT_BCRINPUT|BT_SHIFT_ENABLE|BT_SHIFT_OFF)	// Character string and barcode input (shift off)
#define	BT_INPUT_KEYBCR				(BT_KEYINPUT|BT_BCRINPUT|BT_SHIFT_OFF)					// Character string and barcode input (shift disabled)
// Used with btInputString(), btInputInteger(), and btInputDecimal
#define	BT_INPUT_ESCAPE_ON			(BT_ESCAPE_ON)											// Exit from input when F1, F2, L, or R is pressed.
#define	BT_INPUT_INIT_ERASE_OFF		(BT_INIT_ERASE_OFF)										// The default value is not cleared even when a direction key is pressed.

// Shift status (off: BT_OFF, on: BT_ON)
#define	BT_SHIFT_KANA				(2)				// Hiragana
#define	BT_SHIFT_WIDE_ALPHA			(3)				// Full-width alphabet
#define	BT_SHIFT_NARROW_ALPHA		(4)				// Half-width alphabet

#define	BT_MAX_EDITLEN				(1024)			// Maximum edit length


//==================================
// File
//==================================
// Drive
#define	BT_DRV0						(0)				// Drive 0 (flash ROM)
#define	BT_DRV1						(1)				// Drive 1 (flash ROM)
#define	BT_DRV2						(2)				// Drive 2 (DRAM)
#define	BT_DRV3						(3)				// Drive 3 (flash ROM)
#define	BT_DRV5						(5)				// Drive 5 (external media [miniSD])
// Maximum length of file name (prepared for compatibility with older versions)
#define	BT_FILE_BASEMAX				(8)				// Maximum length of file base name
#define	BT_FILE_EXTMAX				(3)				// Maximum length of extension
#define	BT_FILE_FNAMEMAX			(BT_FILE_BASEMAX + 1 + BT_FILE_EXTMAX)	// Maximum length of file name
// Maximum length of file name
#define	BT_FILE_NAMEMAX				(253)					// Maximum size of file name
#define	BT_PATH_NAMEMAX				(BT_FILE_NAMEMAX + 2)	// Maximum size of path name
#define	BT_LOGFILE_BASEMAX			(8)						// Length of log file base name
#define	BT_LOGFILE_EXTMAX			(3)						// Length of log file extension
#define	BT_LOGFILE_NAMEMAX			(BT_LOGFILE_BASEMAX + 1 + BT_LOGFILE_EXTMAX)

// Maximum number of files to open
#define	BT_FILE_OPENMAX				(8)				///< Maximum number of files that can be opened at the same time
#define	BT_SRCH_OPENMAX				(4)				///< Maximum number of files that the search master file can open at the same time

// Seek standard position
#define	BT_SEEK_SET					(0)				// Start of file
#define	BT_SEEK_CUR					(1)				// Current position
#define	BT_SEEK_END					(2)				// End of file

// Log file search method
#define	BT_FILE_TOP					(1)				// Use the start of the log file as the reference.
#define	BT_FILE_END					(2)				// Use the end of the log file as the reference.
// Correcting and deleting log files
#define	BT_FILE_RECORD_DELETE		(1)				// Record deletion
#define	BT_FILE_RECORD_MODIFY		(2)				// Record correction
// ID classification to record in the log record
#define	BT_LOG_ID_TERMID			(1)				// Record the terminal ID in the log record.
#define	BT_LOG_ID_IP				(2)				// Record the IP address in the log record


//==================================
// Master
//==================================
#define	BT_MASTER_FIELD_NUM			(10)			// Number of master fields
#define	BT_MASTER_OPERATOR_AND		(1)				// Search for the records that match all the fields.
#define	BT_MASTER_OPERATOR_OR		(2)				// Search for the records that match at least one of the fields.
#define	BT_MASTER_FIELD_SIZE		(BT_MAX_STRLEN)	// Maximum length of one field
#define	BT_MASTER_RECORD_SIZE		(BT_MAX_STRLEN)	// Maximum length of the record
#define	BT_MASTER_REC_DELETED		(0)				// Already deleted (data that has been deleted)
#define	BT_MASTER_REC_ORIGINAL		(1)				// Original (valid data saved in the master file)
#define	BT_MASTER_REC_MODIFIED		(2)				// Already corrected (valid data saved in the addition master file)


//==================================
// BCR
//==================================
// Code classification
#define	BT_BCR_JAN					(1)				// JAN
#define	BT_BCR_C39					(2)				// CODE39
#define	BT_BCR_E128					(3)				// EAN128
#define	BT_BCR_C128					(4)				// CODE128
#define	BT_BCR_ITF					(5)				// ITF
#define	BT_BCR_NW7					(6)				// NW-7
#define	BT_BCR_C93					(7)				// CODE93
#define	BT_BCR_TOF					(8)				// Industrial 2 of 5
#define	BT_BCR_COOP					(9)				// COOP
#define BT_BCR_RSS					(10)			// RSS
#define BT_BCR_QR					(11)			// QR
#define BT_BCR_PDF					(12)			// PDF
#define BT_BCR_DM					(13)			// DM
#define BT_BCR_MC					(14)			// MC
#define BT_BCR_COMPOSITE			(16)			// COMPOSITE
#define BT_BCR_GS1_DATABAR			BT_BCR_RSS		// GS1 DataBar
#define BT_BCR_GS1_128				BT_BCR_E128		// GS1-128

// Reading mode
#define	BT_BCR_AUTO_OFF				(1)				// Trigger mode (auto off)
#define	BT_BCR_CONTINUOUS			(2)				// Trigger mode (continuous reading mode)
#define	BT_BCR_MOMENTARY			(3)				// Trigger mode (momentary mode)
#define BT_BCR_SOFT_TRIGGER			(4)				// Trigger mode (software trigger)
#define	BT_BCR_RELEASE				(5)				// Trigger mode (released-trigger reading)
#define	BT_BCR_DOUBLECLICK			(6)				// Trigger mode (double-click)

// Return value of BcrInput (different from BcrInputEx)
#define	BT_BCR_SCANNING				(1)				// Reading (the laser beam is being generated)
#define	BT_BCR_SCAN_ERR				(2)				// Reading error
#define	BT_BCR_LASER_OFF			(3)				// Turning off the laser

// Reading range
#define	BT_RANGE_FIRST				(1)				// Decode only the first found code.
#define	BT_RANGE_CENTER				(2)				// Decoding only the code at the center position of the marker
#define	BT_RANGE_FULL				(3)				// Dedicated for use with multistep reading
#define	BT_SCAN_RANGE_LINE			(4)				// Decode only the level line is contained.

// Inverted or not inverted (2D)
#define	BT_INVERSE_POSITIVE			(1)														// Positive image (black on a white background)
#define	BT_INVERSE_NEGATIVE			(2)														// Negative image (white on a black background)
#define	BT_INVERSE_NEGAPOSI			(BT_INVERSE_POSITIVE | BT_INVERSE_NEGATIVE)				// Both negative and positive are possible (only in 2D mode).
#define	BT_INVERSE_UNMIRRORD		(4)														// Normal label (only in 2D mode)
#define	BT_INVERSE_MIRRORD			(8)														// Mirror image inversion label (only in 2D mode)

// Reading mode control (BT_PROP_SCAN_MODE_2D)
#define	BT_BCR_SCAN_SPEED			(0)
#define BT_BCR_SCAN_BALANCE			(1)
#define	BT_BCR_SCAN_HARD			(2)
#define	BT_BCR_SCAN_ROBUST_DEF		(3)
#define	BT_BCR_SCAN_ROBUST_INF		(4)
#define	BT_BCR_SCAN_ROBUST_GAU		(5)
#define	BT_BCR_SCAN_HIGH1			(6)
#define	BT_BCR_SCAN_HIGH2			(7)

// Maximum values of the multistep reading settings
#define	BT_FULLDETECTION_CODESET_MAX	(4)			// Number of codes that can be specified with multistep reading
#define	BT_FULLDETECTION_COLUMN_MAX		(4)			// Number of steps per code that can be specified with multistep reading

// Composite symbol link information
#define	BT_BCR_NOT_LINKED			(0)
#define	BT_BCR_LINKED_C128			(1)
#define	BT_BCR_LINKED_RSS			(2)
#define	BT_BCR_LINKED_CC_A			(3)
#define	BT_BCR_LINKED_CC_B			(4)
#define	BT_BCR_LINKED_CC_C			(5)
#define	BT_BCR_LINKED_GS1_DATABAR		BT_BCR_LINKED_RSS

// Details of the read GS1 Databar (RSS) code
#define BT_TYPE_RSS_EXP				(0)				// RSS Expanded
#define BT_TYPE_RSS_EXP_ST			(1)				// RSS Expand Stackd
#define BT_TYPE_RSS_14_LIM			(2)				// RSS Limited
#define BT_TYPE_RSS_14				(3)				// RSS-14
#define BT_TYPE_RSS_14_ST			(4)				// RSS-14 Stackd
#define BT_TYPE_GS1_DATABAR_EXP			BT_TYPE_RSS_EXP			// GS1 DataBar Expanded
#define BT_TYPE_GS1_DATABAR_EXP_ST		BT_TYPE_RSS_EXP_ST		// GS1 DataBar Expand Stackd
#define BT_TYPE_GS1_DATABAR_14_LIM		BT_TYPE_RSS_14_LIM		// GS1 DataBar Limited
#define BT_TYPE_GS1_DATABAR_14			BT_TYPE_RSS_14			// GS1 DataBar-14
#define BT_TYPE_GS1_DATABAR_14_ST		BT_TYPE_RSS_14_ST		// GS1 DataBar-14 Stackd

// Property IDs
#define	BT_PROP_TRIGGER_MODE		(0x00000100)	// Trigger mode
#define	BT_PROP_SCAN_TIMEOUT		(0x00000200)	// Scan time-out
#define	BT_PROP_DECODE_TIMEOUT		(0x00000300)	// Decode time-out
#define	BT_PROP_CONTINUOUS_TIME		(0x00000400)	// Duplicate reading prevention time
#define	BT_PROP_SUCCESS_TIMES		(0x00000500)	// Number of reading matches
#define	BT_PROP_SCAN_TYPE			(0x00000700)	// Reading method
#define	BT_PROP_FILTER_LEVEL		(0x00000800)	// Filter settings
#define	BT_PROP_INVERSE				(0x00000900)	// Inverted code reading
#define BT_PROP_SCAN_LEVEL			(0x00000B00)	// Reading enhancement settings
#define	BT_PROP_ENABLE_SYMBOLS		(0x00000C00)	// Reading permission code
#define	BT_PROP_FULLDETECTION		(0x00000D00)	// Multistep reading settings
#define	BT_PROP_SCAN_CODELINK		(0x00000F01)	// Code composition function settings
#define	BT_PROP_SCAN_ILLUMINATION	(0x00000F02)	// Lighting control (on/off) during decoding
#define	BT_PROP_SCAN_MODE_2D		(0x00000800)	// Reading mode control

#define	BT_PROP_JAN					(0x00001000)	// JAN code properties
#define	BT_PROP_CODE39				(0x00001100)	// Code39 code properties
#define	BT_PROP_CODE128				(0x00001200)	// Code128 code properties
#define	BT_PROP_EAN128				(0x00001300)	// Ean128 code properties
#define	BT_PROP_NW7					(0x00001400)	// NW7 code properties
#define	BT_PROP_ITF					(0x00001500)	// ITF code properties
#define	BT_PROP_CODE93				(0x00001600)	// Code93 code properties
#define	BT_PROP_TOF					(0x00001700)	// TOF code properties
#define	BT_PROP_COOP				(0x00001800)	// COOP code properties
#define	BT_PROP_QR					(0x00002000)	// QR code properties
#define	BT_PROP_PDF					(0x00002100)	// PDF417 code properties
#define	BT_PROP_DM					(0x00002200)	// DataMatrix code properties
#define	BT_PROP_MC					(0x00002300)	// MaxiCode code properties
#define	BT_PROP_RSS					(0x00002400)	// RSS code properties
#define	BT_PROP_COMPOSITE			(0x00002500)	// Composite symbol properties
#define	BT_PROP_GS1_DATABAR			BT_PROP_RSS		// GS1 Databar code properties
#define	BT_PROP_GS1_128				BT_PROP_EAN128	// GS1-128 code properties

#define BT_PROP_BUZZER_STOP			(0x00004300)	// Stop the buzzer when reading starts (2D only).
#define BT_PROP_COMPLETE_BUZ		(0x00004400)	// Sound the buzzer when reading is successful.
#define BT_PROP_TIMEOUT_BUZ			(0x00004401)	// Sound the buzzer when a reading time-out occurs.
#define BT_PROP_ERROR_BUZ			(0x00004402)	// Sound the buzzer when a reading error occurs.
#define BT_PROP_COMPLETE_LED		(0x00004500)	// Light the LED when reading is successful.
#define BT_PROP_TIMEOUT_LED			(0x00004501)	// Light the LED when a reading time-out occurs.
#define BT_PROP_ERROR_LED			(0x00004502)	// Light the LED when a reading error occurs.
#define BT_PROP_COMPLETE_VIB		(0x00004600)	// Activate the vibrator when reading is successful.
#define BT_PROP_TIMEOUT_VIB			(0x00004601)	// Activate the vibrator when a reading time-out occurs.
#define BT_PROP_ERROR_VIB			(0x00004602)	// Activate the vibrator when a reading error occurs.

// Live view control
#define BT_LV_STOP					(0)				// Stop
#define BT_LV_START					(1)				// Start

// Saving images
#define	BT_PHOTO_FTYPE_BMP			(1)				// Bitmap file
#define	BT_PHOTO_USE_CAM			(1)				// Take a photograph.
#define	BT_PHOTO_USE_MEM			(2)				// Save the image in the internal buffer without taking a photograph.
#define	BT_PHOTO_LIGHT_ON			(1)				// Lighting on
#define	BT_PHOTO_LIGHT_OFF			(2)				// Lighting off

//==================================
// Utilities
//==================================
#define	BT_BATTERY_MAIN					(1)				// Main battery
#define	BT_BATTERY_SUB					(2)				// Sub battery
#define	BT_DEF_DEBUGPORT_JACK_ENABLE	(1)				// Debug port enabled (jack)
#define	BT_DEF_DEBUGPORT_DISABLE		(2)				// Debug port disabled
#define	BT_DEF_DEBUGPORT_USB_ENABLE		(3)				// Debug port enabled (USB)
#define	BT_DEF_DEBUGPORT_ENABLE			BT_DEF_DEBUGPORT_JACK_ENABLE				// Debug port enabled (jack)


// Display language
#define	BT_LANG_JAPANESE			(0)				// Japanese
#define	BT_LANG_ENGLISH				(1)				// English
#define	BT_LANG_SIMPLE_CHINESE		(2)				// Chinese (simplified)
#define	BT_LANG_TRADITIONAL_CHINESE	(3)				// Chinese (not used)

//==================================
// Shared functions
//==================================
#define	BT_CHAR_ASCII				(1)				// ASCII characters
#define	BT_CHAR_MB1					(2)				// First byte of a two-byte character
#define	BT_CHAR_MB2					(3)				// Second byte of a two-byte character
#define	BT_CHAR_MB4_2				(4)				// Second byte of a four-byte character (simplified Chinese characters only)
#define	BT_CHAR_MB4_3				(5)				// Third byte of a four-byte character (simplified Chinese characters only)
#define	BT_CHAR_MB4_4				(6)				// Fourth byte of a four-byte character (simplified Chinese characters only)
#define	BT_CHAR_NONE				(0)				// The search for the judgment character in a character string reached the end of the character string.
#define	BT_CHAR_ERR					(-1)			// An illegal character code was detected.

#define	BT_COMPARE_MATCH			(0)				// Comparison result: match
// A mismatch is not necessarily indicated by -1 (it is indicated by a value other than 0).
//#define	BT_COMPARE_MISMATCH		(-1)			// Comparison result: mismatch

//==================================
// File compression and decompression
//==================================
#define	BT_COMPRESS_ZIP				(1)				// ZIP file compression and decompression
#define	BT_COMPRESS_ZIP2			(2)				// ZIP file compression and decompression (with decompressed file name specification)

//==================================
// Related to properties
//==================================
#define	PP_SMENU_PASSbufLEN		(32+1)			// System menu password buffer size
#define	PP_SMENU_PASS_LEN			(16)			// System menu password size

// Key click sound
#define	PP_KEYCLICK_OFF				(0)				// Key click sound off
#define	PP_KEYCLICK_S				(1)				// Key click sound low
#define	PP_KEYCLICK_M				(2)				// Key click sound medium
#define	PP_KEYCLICK_L				(3)				// Key click sound high
// Display language
#define	PP_LANG_JPN					(BT_LANG_JAPANESE)				// Japanese
#define	PP_LANG_ENG					(BT_LANG_ENGLISH)				// English
#define	PP_LANG_CHINA				(BT_LANG_SIMPLE_CHINESE)		// Chinese (simplified)
#define	PP_LANG_TW					(BT_LANG_TRADITIONAL_CHINESE)	// Chinese (not used)
// Auto power off setting
#define	PP_AUTOPOWEROFF_FOREVER		(-1)			// Auto power off disabled
// Backlight
#define	PP_BKLIGHT_OFF				(0)				// Backlight off
#define	PP_BKLIGHT_MID				(1)				// Backlight medium brightness
#define	PP_BKLIGHT_LIGHT			(2)				// Backlight bright
// File communication port
#define	PP_PORT_RF_PC				(0)				// RF(PC)
#define	PP_PORT_RF_TERM				(1)				// RF(BT)
#define	PP_PORT_IRDA				(2)				// IrDA
#define	PP_PORT_COM					(3)				// COM

//-----------------------------------------------------------------------
#define MSG_MAX						(8)
#define EV_MAX						(12)
// Event message
#define MSG_SYSTEM					(1)				// System
#define MSG_KEY						(2)				// Key
#define MSG_NETWORK					(4)				// Network
#define MSG_CODEREADER				(5)				// Code reader
#define MSG_SOCKET					(6)				// Socket communication
#define MSG_WIRELESS				MSG_NETWORK		// Wireless

// System event
#define EV_TIMER					(1)				// Timer ID
#define EV_RESUME					(2)				// Turning power on by resuming
#define EV_CRADLE					(3)				// Installing in and removing from communication units
#define EV_BATTERY					(4)				// Change in the remaining power of the main battery
#define EV_USBCABLE					(5)				// Connecting and disconnecting the USB cable
// Key event
#define EV_KEY_PRESS				(1)				// A key has been pressed.
// Wireless event
#define EV_W_CONNECT				(1)				// Connection established
#define EV_W_OUTRANGE				(2)				// Out of range
#define EV_W_REFUSED				(3)				// Connection denied
// Network event
#define EV_NETWORK_CONNECT			(1)				// Connection established
#define EV_NETWORK_OUTRANGE			(2)				// Out of range
#define EV_NETWORK_REFUSED			(3)				// Connection denied
#define EV_NETWORK_DHCP_IP_CHANGED	(4)				// The IP address acquired through the use of DHCP has changed from the previous IP address.
// Code reader event
#define EV_COMPLETE					(1)				// Reading completion
#define EV_TIMEOUT					(2)				// Reading time-out
#define EV_ERROR					(3)				// Reading error
// Socket communication event
#define EV_SOCK_WRITE				(1)				// Change in the write flag
#define EV_SOCK_READ				(2)				// Change in the read flag

#define EV_KEY_PRESS_STR			"1"
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// System
#define	BT_EVENTTIMER_MIN			(1)				// Minimum event timer value
#define	BT_EVENTTIMER_MAX			(300)			// Maximum event timer value
#define	BT_VERSIONINFO_MAX			(32)			// Maximum size of the version information character string

//-----------------------------------------------------------------------
// HandyGetParameter,HandySetParameter
// Auto power off time
#define PRM_AUTOPOWEROFF			(1)
// Resume or do not resume
#define PRM_RESUME					(2)
// Wakeup settings
#define PRM_WAKEUP					(3)
// Terminal ID
#define PRM_ID						(4)
// Remaining battery power (main)
#define PRM_MAINBATTERYSTAT			(5)
// Remaining battery power (sub)
#define PRM_SUBBATTERYSTAT			(6)
// Terminal version (terminal library)
#define PRM_HANDYVERSION			(7)
// Terminal model
#define PRM_HANDYTYPECODE			(10)
// Language
#define PRM_LANGUAGE				(11)
// Serial number
#define PRM_SERIALNO				(12)
// MAC address
#define PRM_MACADDRESS				(13)
// Touch panel events: enabled or disabled
#define PRM_TOUCH_EVENT				(14)
// Display of dialog boxes during communication: enabled or disabled
#define PRM_DISP_TRANSDIALOG		(15)
// Power supplied from communication unit
#define PRM_CRADLESTAT				(16)
// Connecting to the built-in USB port
#define PRM_USBSTAT					(17)
// File information acquisition mode: normal or accelerating (folder size: fixed to -1)
#define PRM_FILEINFO_MODE			(18)
// ZIP password specification (character string up to 63 bytes in length)
#define PRM_ZIP_PASSWORD			(20)
// In addition to the ENT key, the up arrow, down arrow, L, and R keys can also be used
// to confirm and end the btInputString(g), btInputStringEx(g), btInputInteger(g), and btInputDecimal(g) functions.
#define PRM_BTINPUT_FIXKEY			(21)
// A serial number can be acquired (the character string of 9 figures & only in case of acquisition ).  
#define PRM_SERIALCHAR				(32)
// When using 2 GB or more of media, it enables it to treat the size data overflowed by long. 
#define PRM_DFINFO_MODE				(46)
// Form of a terminal (character string) 
#define PRM_HANDYTYPECHAR			(47)
//-----------------------------------------------------------------------


//********************************************************************************
//			Structure declarations
//********************************************************************************
// Date information structure
typedef	struct	_dateinfo	{
	short		year;										// Year
	char		month;										// Month
	char		day;										// Day
} DATEINFO;

// Time information structure
typedef	struct	_timeinfo	{
	char		hour;										// Hours
	char		min;										// Minutes
	char		sec;										// Seconds
	char		reserve;									// Reserved
} TIMEINFO;

//----------------------------------
// Screen display
//----------------------------------
// Text coordinates structure
typedef struct	_tposition {
	short		x;											// X coordinate
	short		y;											// Y coordinate
} TPOSITION;

// Graphic coordinates structure
typedef struct	_gposition {
	short		x;											// X coordinate
	short		y;											// Y coordinate
} GPOSITION;

// Display attribute structure
typedef struct	_dspattr {
	TPOSITION	tposition;									// Display text coordinates
	short		attribute;									// Attributes
	short		mag;										// Display magnification
	short		fold;										// Line wrap (multiple-line display)
} DSPATTR;

// Display attribute structure (graphic coordinates)
typedef	struct	_dspattrg	{
	GPOSITION	gposition;									// Display graphic text coordinates
	short		attribute;									// Display attributes
	short		mag;										// Display magnification
	short		fold;										// Multiple-line display (line wrap)
} DSPATTRG;

// Menu information structure
typedef	struct	_menuinfo {
	short		rsv0;										// Reserve 0
	short		tenkeydirect;								// Direct input from the numeric keypad
	short		totalitem;									// Number of selected items
	short		inititem;									// Initial selection position
	char		*menuitem[BT_MENU_SELMAX];					// Selected item character string pointer
	short		rsv1;										// Reserved 1
	short		rsv2;										// Reserved 2
	short		rsv3;										// Reserved 3
} MENUINFO;


//----------------------------------
// Key settings
//----------------------------------
// Function key information structure
typedef	struct	_keyfunc	{
	char		F1;											// Function 1
	char		F2;											// Function 2
	char		F3;											// Function 3
	char		L;											// L key
	char		R;											// R key
	char		Trg;										// Trigger key
	char		minus[3];									// Key assignment character
} KEYFUNC;


//----------------------------------
// Input processing
//----------------------------------
// Integer entry attribute structure
typedef	struct	_inputintegerattr	{
	short		inputtype;									// Input classification
	short		inputarea;									// Input field size (size of the input field that is cleared with the initial display)
	long		minvalue;									// Minimum value
	long		maxvalue;									// Maximum value
} INPUTINTEGERATTR;

// Decimal fraction entry attribute structure
typedef	struct	_inputdecimalattr	{
	short		inputtype;									// Input classification
	short		inputarea;									// Input field size (size of the input field that is cleared with the initial display)
	DECIMAL		minvalue;									// Minimum value
	DECIMAL		maxvalue;									// Maximum value
	short		digit;										// Number of decimal fraction digits that can be entered
} INPUTDECIMALATTR;

// Character string input attribute structure
typedef	struct	_inputstrattr	{
	short		inputtype;									// Input classification
	short		inputarea;									// Input field size (size of the input field that is cleared with the initial display)
	short		mindigit;									// Minimum number of digits to enter
	short		maxdigit;									// Maximum number of digits to enter
} INPUTSTRATTR;

//----------------------------------
// Barcode processing
//----------------------------------
// Barcode information structure
typedef	struct	_bcrconf	{
	short		mode;										// Reading mode
	short		trial;										// Number of reading attempts (per 100 ms)
	short		overlap;									// Duplicate reading prevention time (in units of 100 ms)
	short		comparecnt;									// Number of reading matches
	short		rsv1;										// Reserved 1
	short		rsv2;										// Reserved 2
	short		rsv3;										// Reserved 3
	short		rsv4;										// Reserved 4
} BCRCONF;

typedef	struct	_jancodeset {								// JAN
	unsigned short	flgset;									// Reading options
	unsigned char	AdGap_min;								// Minimum add-on gap value
	unsigned char	AdGap_max;								// Maximum add-on gap value
} jancodeset;

typedef	struct	_c39codeset {								// CODE39
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	short	rsv1;
} c39codeset;

typedef	struct	_e128codeset {								// EAN128
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	unsigned char	separator;								// Separator character code
	unsigned char	rsv;									// Reserved
} e128codeset;

typedef	struct	_c128codeset {								// CODE128
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	short	rsv1;
} c128codeset;

typedef	struct	_itfcodeset {								// ITF
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	short	rsv1;
} itfcodeset;

typedef	struct	_nw7codeset {								// NW-7
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	short	rsv1;
} nw7codeset;

typedef	struct	_c93codeset {								// CODE93
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
	unsigned short	flgset;									// Reading options
	short	rsv1;
} c93codeset;

typedef	struct	_tofcodeset {								// Industrial 2 of 5
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
} tofcodeset;

typedef	struct	_coopcodeset {								// COOP
	unsigned short	min;									// Minimum number of digits
	unsigned short	max;									// Maximum number of digits
} coopcodeset;

typedef struct _qrcodeset {									// QR
	unsigned short min;										// Minimum number of digits
	unsigned short max;										// Maximum number of digits
	unsigned short flgset;									// Flag set
} qrcodeset;

typedef struct _pdfcodeset {
	unsigned short min;										// Minimum number of digits
	unsigned short max;										// Maximum number of digits
	unsigned short flgset;									// Flag set
} pdfcodeset;

typedef struct _dmcodeset {
	unsigned short min;										// Minimum number of digits
	unsigned short max;										// Maximum number of digits
	unsigned short flgset;									// Flag set
} dmcodeset;

typedef struct _mccodeset {
	unsigned short min;										// Minimum number of digits
	unsigned short max;										// Maximum number of digits
	unsigned short flgset;									// Flag set
} mccodeset;

typedef struct _rsscodeset {
	unsigned short min;										// Minimum number of digits
	unsigned short max;										// Maximum number of digits
	unsigned short flgset;									// Flag set
} rsscodeset;

typedef struct _compositeset {
	unsigned short flgset;									// Flag set
	unsigned char separator;								// Separator character code
	unsigned char rsv;										// Reserved
} compositeset;

// Code classification setting
typedef	struct		_bcrcodeset	{
	jancodeset			JAN;								// JAN
	c39codeset			CODE39;								// CODE39
	e128codeset			EAN128;								// EAN128
	c128codeset			CODE128;							// CODE128
	itfcodeset			ITF;								// ITF
	nw7codeset			NW7;								// NW-7
	c93codeset			CODE93;								// CODE93
	tofcodeset			TOF;								// Industrial 2 of 5
	coopcodeset			COOP;								// COOP
} BCRCODESET;

// Barcode reading result structure
typedef	struct		_bcrinput	{
	short		keta;										// Number of digits
	short		codetype;									// Code classification
	char		bcrdata[128];								// Reading data
} BCRINPUTDATA;

// Reading result report structure
typedef struct _bcr_result_report {
	short		keta;										// Number of digits
	short		codetype;									// Code classification
	short		composite;									// Whether this is composite
	short		quality;									// Reading quality
	char		extraType;									// Detailed information (such as add-on information) related to the code classification
	char		codelink;									// Whether the code composition function was used
} BCR_RESULT_REPORT;

/// Report of the result of reading concatenated QR codes
typedef struct _bcr_result_qr_report {
	short		pos;										// Reading result position
	short		parity;										// Parity
	short		count;										// Total number of concatenations
} BCR_RESULT_QR_REPORT;

typedef struct _bcr_fulldetection_column {
	unsigned short min;										///< the limit on the minimum number of digits (1 to 7089)
	unsigned short max;										///< the limit on the maximum number of digits (1 to 7089)
} BCR_FULLDETECTION_COLUMN;

typedef struct _bcr_fulldetection_codeset {
	short		type;										///< the supported code classification (SCM_BCR_*)
	short		codecount;									///< the number of codes to compare (> 0)
	BCR_FULLDETECTION_COLUMN columneset[BT_FULLDETECTION_COLUMN_MAX];		///< the number of steps of the code limit
} BCR_FULLDETECTION_CODESET;

typedef struct _bcr_fulldetection_set {
	short		typecount;									///< the number of valid column sets
	short		rsv1;										///< reserved
	BCR_FULLDETECTION_CODESET codeset[BT_FULLDETECTION_CODESET_MAX];		///< the sets of the number of steps of each code
} BCR_FULLDETECTION_SET;

/// Buzzer action when reading is successful
typedef struct _bcr_buzzer_action {
	short sw;
	short ontime;
	short offtime;
	short cnt;
	short pitch;
	short rsv1;
} BCR_BUZZER_ACTION;

/// LED action when reading is successful
typedef struct _bcr_led_action {
	short sw;
	short ontime;
	short offtime;
	short cnt;
	short color;
	short rsv1;
} BCR_LED_ACTION;

/// Vibrator action when reading is successful
typedef struct _bcr_vibrator_action {
	short sw;
	short ontime;
	short offtime;
	short cnt;
	short rsv1;
	short rsv2;
} BCR_VIBRATOR_ACTION;

//----------------------------------
// File processing
//----------------------------------

// File information structure
typedef	struct	_file_info {
	char drive[2];						// Drive number
	char filename[BT_FILE_NAMEMAX+1];	// File name
	char isDirectory;					// Directory flag (a file if this is 0)
	long filesize;						// File size
	short year;							// Year
	char month;							// Month
	char day;							// Day
	char hour;							// Hours
	char min;							// Minutes
	char rsv2;
	char rsv3;
} FILE_INFO;

// Drive information structure
typedef	struct	_drive_info {
	long totalsize;						// Total space
	long freesize;						// Free space
	long invalidsize;					// Invalid space (drive 1 and drive 3 only)
	short filecnt;						// Number of files (including directories)
} DRIVE_INFO;



//----------------------------------
// Master
//----------------------------------
// Master file index information
typedef	struct _master_index {
	short		datatype;								// Data type (0: fixed length, 1: variable length)
	char		separator;								// Separator (valid for the variable-length data type)
	signed char	fieldcnt;								// Number of fields
	short		fieldlen[BT_MASTER_FIELD_NUM];			// Field length (valid when the length is fixed)
	char		rsv1;									// Reserved
	char		rsv2;									// Reserved
} MASTER_INDEX;

// Master file search conditions
typedef	struct _master_search {
	char		ope;									// Operator (1: and, 2: or)
	char		*searchdata[BT_MASTER_FIELD_NUM];		// Search data (a null field indicates that searching will not be performed)
	char		incdeldata;								// Select whether to include the data to be deleted in the search (BT_ON: include the data, BT_OFF: do not include the data).
	char		rsv1;									// Reserved
} MASTER_SEARCH;


//----------------------------------
// Related to the alarm
//----------------------------------
typedef struct _alarm_pattern {
	short ontime;				// Device on time (I) [1 to 50 (Å~ 100 ms)]
	short offtime;				// Device off time (I) [0 to 50 (Å~ 100 ms)]
	short swBuzzer;				// Buzzer musical scale (1 to 15 or BT_OFF) 
	short swVibration;			// Vibration (BT_ON or BT_OFF) 
	short swLed;				// LED (three colors or BT_OFF)
	short count;				// Number of operations (1 to 100)
	char message[64];			// Message to display (up to 63 bytes)
} ALARM_PATTERN;

typedef struct _lost_alarm {
	short	sw;				// Lost alarm: enabled or disabled
	short	watchtime;			// Lost alarm monitor time (1 to 24 [h])
	TIMEINFO	timestart;		// Lost alarm start time
	TIMEINFO	timestop;		// Lost alarm stop time
	short	interval;			// Lost alarm rumble interval (1 to 120 [min])
	char	crsv1[2];			// reserve
	ALARM_PATTERN	pattern;	// Lost alarm notification pattern
} LOST_ALARM;


typedef struct _alarm {
	short	sw;				// Alarm: enabled or disabled
	char	crsv1[2];			// reserve
	TIMEINFO	time;			// Alarm time
	ALARM_PATTERN	pattern;	// Alarm notification pattern
} ALARM;

typedef struct _schedule {
	short	sw;				// Application schedule start: enabled or disabled
	char	crsv1[2];			// reserve
	TIMEINFO	time;			// Application schedule time
	char	filename[260];		// Application schedule execution file name
} SCHEDULE;

//----------------------------------
// SEARCH
//----------------------------------
// Search index information structure
//   Used in the functions for creating the terminal master file (search)
typedef	struct	_SEARCH_INDEX {
	short	recordFormat;		// 0: fixed length, 1: variable length
	short	recordLength;		// Fixed length: record length, variable length: number of fields (when this is 0, the number of fields on the first line is automatically selected)
	unsigned char	separator;	// This can only be specified when the length is variable; 0 to 255.
	char	dmy;
	short	sortType;			// 0: quick sort, 1: bubble sort
	short	indexMode;			// The number of hashes in relation to the number of records
								// 0: approximate square root, 1: 5% approximation, 2: 10% approximation, 3: 20% approximation, 4: 30% approximation, 5: 50% approximation
	short	indexNum;			// Number of indexes (1 to 9)
	short	indexPosition[9];	// The number of bytes from the start of the record when the length is fixed (1 to the record length)
								// The item number when the length is variable (1 to the number of fields)
	short	indexLength[9];		// This can only be specified when the length is fixed (1 to 255). This is ignored when the length is variable.
} SEARCH_INDEX;


//********************************************************************************
//			Include Files
//********************************************************************************
#include "btLib_comm.h"		// Shared header related to communication
#include "btLib_newbt.h"	// Shared header related to widgets


#ifdef __cplusplus
}
#endif


#endif // !defined(__BTLIB_H__)
