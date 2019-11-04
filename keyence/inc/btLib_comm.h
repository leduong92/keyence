#if 0
// *******************************************************************************
// **		Copyright(c) 2013, Keyence Corporation. All rights reserved
// **		File Name	: btLib_comm.h
// **		Cpu			: 
// **		Compiler	: 
// **
// ** 		Function	: Terminal library header file (communication)
// ** 		Limitations	:
// **		Note		:
// ** 		History		: 
// **
// ********************************************************************************
#endif
#if !defined(__BTLIB_COMM_H__)
#define __BTLIB_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif


//********************************************************************************
//			Include Files
//********************************************************************************

//********************************************************************************
//			Local macro declarations
//********************************************************************************

//==================================
// Error information
//==================================
// Communication return value setting for the btComm function
#define	BTCOMM_OK					(0)				// Normal completion
#define	BTCOMM_OTHER				(-306)			// An error that does not correspond to any of the errors written below
#define	BTCOMM_CONVERT_FAILED		(-307)			// Error in converting data to the master file or the application
#define	BTCOMM_INCOMPLETE			(-308)			// Processing did not complete.
#define	BTCOMM_FILENOTFOUND			(-309)			// Cannot find the file.
#define	BTCOMM_BIGDATA				(-310)			// The file size is too large.
#define	BTCOMM_NETDOWN				(-311)			// The communication path has been disconnected.
#define	BTCOMM_TIMEOUT				(-312)			// Time-out
#define	BTCOMM_CANCELED				(-313)			// Canceled
#define	BTCOMM_REFUSED				(-314)			// Rejected
#define BTCOMM_NOTFOUND				(-315)			// The specified terminal could not be found.
#define BTCOMM_WOULDBLOCK			(-316)			// An asynchronous function has been called. An event will be used to return the result of processing.
#define	BTCOMM_NOTCONNECT			(-317)			// Not connected
#define	BTCOMM_INUSE				(-318)			// The device is in use.
#define BTCOMM_NOTINITIALISED		(-319)			// This has not been initialized. This is an illegal handle.
#define BTCOMM_INVALID_VALUE		(-320)			// Internal use

#define BT_SYSVERSION_OLD			(1)				// The system file on the server is older than the version of the terminal system.
#define BT_SYSVERSION_SAME			(2)				// The system file on the server is the same version as that of the terminal system.
#define BT_SYSVERSION_NEW			(3)				// The system file on the server is newer than the version of the terminal system (an update may be necessary).


// Communication return value setting for the BT900-compatible functions
#define	ERR_COMM_OK					(BTCOMM_OK)				// Normal completion
#define	ERR_COMM_OTHER				(BTCOMM_OTHER)			// An error that does not correspond to any of the errors written below
#define	ERR_COMM_CONVERT_FAILED		(BTCOMM_CONVERT_FAILED)	// Error in converting data to the master file or the application
#define	ERR_COMM_INCOMPLETE			(BTCOMM_INCOMPLETE)		// Processing did not complete.
#define	ERR_COMM_FILENOTFOUND		(BTCOMM_FILENOTFOUND)	// Cannot find the file.
#define	ERR_COMM_BIGDATA			(BTCOMM_BIGDATA)		// The file size is too large.
#define	ERR_COMM_NETDOWN			(BTCOMM_NETDOWN)		// The communication path has been disconnected.
#define	ERR_COMM_TIMEOUT			(BTCOMM_TIMEOUT)		// Time-out
#define	ERR_COMM_CANCELED			(BTCOMM_CANCELED)		// Canceled
#define	ERR_COMM_REFUSED			(BTCOMM_REFUSED)		// Rejected
#define	ERR_COMM_NOTCONNECT			(BTCOMM_NOTCONNECT)		// Not connected

#define	CMN_ERR_ID					(-1)
#define	CMN_ERR_TMOUT				(-2)
#define	CMN_ERR_DLT					(-3)
#define	CMN_ERR_NOEXS				(-4)

#define	COMM_NOERR					(0)
#define	COMM_ERRRANGE				(-101)

// Socket communication return value setting
#define	BTSOCK_SOCKET_OK			(0)
#define	BTSOCK_INVALID_SOCKET		(BTSOCKET)(~0)
#define	BTSOCK_INADDR_NONE			(-1)
#define	BTSOCK_SOCKET_ERROR			(-1)

// FTP communication transmission mode setting
#define	FTP_BINARYMODE				(1)
#define	FTP_ASCIIMODE				(2)

// DIR command mode of FTP (a simple list mode / detailed list mode)   
#define	FTP_NLSTMODE				(1)				// NLST mode
#define	FTP_LISTMODE				(2)				// LIST mode

// PUT mode of FTP (overwrite/append)   
#define	FTP_STORMODE				(1)				// STOR mode
#define	FTP_APPEMODE				(2)				// APPE mode

// FTP communication return value setting
#define	BT_FTP_OK					(0)				//: The file communication has completed.
#define	BT_FTP_NG					(-11)			//: The file communication operation failed.
#define	BT_FTP_TIMEOUT				(-12)			//: A time-out occurred.
#define	BT_FTP_CANCELED				(-13)			//: Canceled
#define	BT_FTP_FILEERR				(-14)			//: Cannot find the local file.
#define	BT_FTP_NETDOWN				(-15)			//: The communication path has been disconnected.

// RF communication return value setting
#define	BT_RF_API_SET_OK			(0)				// Processing has completed successfully.
#define	BT_RF_API_RF_ERR			(-1)			// The btRfOpen function has not been executed.
#define	BT_RF_API_PARAM_ERR			(-2)			// Parameter error
#define	BT_RF_API_SET_ERR			(-3)			// Communication condition error

#define	BT_RF_API_OK				(0)				// Processing has completed successfully.
#define	BT_RF_API_NG				(-1)			// The notification could not be performed.
#define	BT_RF_API_NOT_OPEN			(-2)			// The wireless port has not been opened.

#define	BT_RF_API_SND_OK			(0)				// Processing has completed successfully.
#define	BT_RF_API_SND_NG			(1)				// Transmission failure caused by the wireless communication part
#define	BT_RF_API_CONTINUE			(-4)			// Processing is incomplete (still continuing).
#define	BT_RF_API_NOT_SND			(-5)			// Data is not transmitted. 
#define	BT_RF_API_OUTRANGE			(-6)			// Transmission failure caused by the range being exceeded
#define	BT_RF_API_RCV_WAIT			(0)				// Data has not been received.
#define	BT_RF_API_RCV_OK			(1)				// Data has been received.
#define	BT_RF_API_TMOUT				(5)				// Time-out

//-----------------------------------------------------------
#define	BT_NETWORK_OK				(0)				// The wireless connection succeeded.
#define	BT_NETWORK_NG				(-1)			// The wireless connection failed.
#define	BT_NETWORK_ALREADYOPEN		(-2)			// btNetworkOpen has already been called.
#define	BT_NETWORK_WOULDBLOCK		(-3)			// This was called in asynchronous mode.
#define	BT_TIMEOUT					(-4)			// There is no response.
#define	BT_NETWORK_ALREADYCLOSE		(-5)			// The port has already been closed.

#define	BT_WIRELESS_OK				BT_NETWORK_OK					// The wireless connection succeeded.
#define	BT_WIRELESS_NG				BT_NETWORK_NG					// The wireless connection failed.
#define	BT_WIRELESS_ALREADYOPEN		BT_NETWORK_ALREADYOPEN			// btWirelessOpen has already been called.
#define	BT_WIRELESS_WOULDBLOCK		BT_NETWORK_WOULDBLOCK			// This was called in asynchronous mode.

#define	BT_NETWORK_NOTOPEN			(0)				// There are no devices that have been opened.
#define	BT_NETWORK_WLAN_ADHOC		(1)				// IEEE802.11g Ad Hoc
#define	BT_NETWORK_WLAN_INFRA		(2)				// IEEE802.11g infrastructure
#define	BT_NETWORK_BLUE_SPP			(3)				// Bluetooth(SPP)
#define	BT_NETWORK_BLUE_PPP			(4)				// Bluetooth(DUN+PPP)
#define	BT_NETWORK_LANCRADLE		(5)				// LAN communication unit
#define	BT_NETWORK_MODEM_SERIAL		(6)				// Modem unit (serial)
#define	BT_NETWORK_MODEM_PPP		(7)				// Modem unit (PPP)
#define	BT_NETWORK_MODEM_CRADLE_SERIAL	(8)			// Modem communication (serial) through a communication unit
#define	BT_NETWORK_MODEM_CRADLE_PPP		(9)			// Modem communication (PPP) through a communication unit
#define	BT_NETWORK_BLUE_HID			(10)			// Bluetooth(HID)

#define	BT_WIRELESS_NOTOPEN			BT_NETWORK_NOTOPEN				// The wireless port has not been opened.
#define	BT_WIRELESS_11G_ADHOC		BT_NETWORK_WLAN_ADHOC			// IEEE802.11g Ad Hoc
#define	BT_WIRELESS_11G_INFRA		BT_NETWORK_WLAN_INFRA			// IEEE802.11g infrastructure
#define	BT_WIRELESS_SPP				BT_NETWORK_BLUE_SPP				// Bluetooth(SPP)
#define	BT_WIRELESS_PPP				BT_NETWORK_BLUE_PPP				// Bluetooth(DUN+PPP)

#define	BT_TARGET_WLAN				(1)				// WLAN
#define	BT_TARGET_PPP				(2)				// PPP
#define	BT_TARGET_BLUETOOTH			(3)				// BLUETOOTH
#define	BT_TARGET_NETWORK			(4)				// NETWORK
#define	BT_TARGET_MODEM				(5)				// MODEM

#define BT_SERIALCOMM_NOTOPEN		(-10)			// The port has not been opened.

#define BT_BLUETOOTH_SECURITY_DISABLE	(0)			// No security
#define BT_BLUETOOTH_SECURITY_AUTH		(1)			// Authentication
#define BT_BLUETOOTH_SECURITY_BOTH		(2)			// Authentication + encryption

#define BT_MODEM_DIAL_PULSE			(0)			// Pulse circuit
#define BT_MODEM_DIAL_TONE			(1)			// Tone circuit

#define BT_BLUETOOTH_MASTER_MODE			(1)		// SPP, HID connection mode :Master
#define BT_BLUETOOTH_SLAVE_MODE				(2)		// SPP, HID connection mode :Slave
#define BT_BLUETOOTH_MASTER_PAIRING_MODE	(3)		// HID connection mode :Master

#define BT_BLUETOOTH_SPP_MASTER_MODE	(1)				// SPP connection mode :Master
#define BT_BLUETOOTH_SPP_SLAVE_MODE		(2)				// SPP connection mode :Slave

#define PRM_HID_MODE					(2102)	// Master(1)/Slave(2)
#define PRM_HID_LOCALCOD				(2103)	// LocalClassOfDevice(0xXXXX)

enum WLanParam {
	// NETWORK
	PRM_TERMINALIP = 1			,	// Terminal IP address
	PRM_SUBNETMASK				,	// Terminal subnet mask
	PRM_GATEWAYIP				,	// Terminal gateway address
	PRM_HOSTIP					,	// Host IP address
	PRM_DHCPENABLE				,	// DHCP: enable or disable
	PRM_TERMINALNAME			,	// Nickname (DNS)
	PRM_PRI_DNS_ADDR			,	// Primary DNS address
	PRM_SEC_DNS_ADDR			,	// Secondary DNS address
	PRM_FTP_PASV				,	// Connection method of FTP communication data (PORT mode or PASV mode)
	PRM_FTP_PORTMODE			,	// When data is received from the FTP client by way of a Get command, wait for the data port to be disconnected.
	PRM_USE_FTPS				,	// Use FTPS when performing FTP communication.
	PRM_SOCKET_NOTIFY			,	// Asynchronous socket notification
	PRM_FTP_TRANSMODE			,	// Transmission mode of FTP communication data (BINARY mode or ASCII mode)
	PRM_FTP_EPHEMERAL_PORT		,	// Number of the primary port to use during FTP communication
	PRM_FTP_SIZECHECK			,	// When data is received from the FTP client by way of a Get command, if a size information notification from the server is present, check the size.
	PRM_FTP_TEMPDRIVE			,	// Temporary drive to use for transmitting and receiving files during FTP communication (drive 2 or drive 5)
	PRM_COMM_TRANSMODE			,	// Switching the mode for connecting to the LC communication library
	// WLAN
	PRM_PREAMBLETYPE = 101		,	// Preamble type
	PRM_RFRATE					,	// Transmission rate
	PRM_WEPAUTHENTICATIONTYPE	,	// WEP authentication type
	PRM_WPAAUTHENTICATIONMODE	,	// WPA mode and authentication mode
	PRM_BEACONLOSTCOUNT			,	// BeaconLostcount
	PRM_PWMODE					,	// Power management mode
	PRM_POWERSAVETIME			,	// Automatic power save monitor time
	PRM_INFRASSID				,	// Infrastructure SSID (with size)
	PRM_INFRAWEPINDEX			,	// Infrastructure WEP index to select (0 to 3)
	PRM_INFRAWEP_1				,	// Infrastructure WEP character string 1 (with size)
	PRM_INFRAWEP_2				,	// Infrastructure WEP character string 2 (with size)
	PRM_INFRAWEP_3				,	// Infrastructure WEP character string 3 (with size)
	PRM_INFRAWEP_4				,	// Infrastructure WEP character string 4 (with size)
	PRM_ADHOCCHANNEL			,	// Ad Hoc mode channel
	PRM_ADHOCSSID				,	// Ad Hoc SSID (with size)
	PRM_ADHOCWEPINDEX			,	// Ad Hoc WEP index to select (0 to 3)
	PRM_ADHOCWEP_1				,	// Ad Hoc WEP character string 1 (with size)
	PRM_ADHOCWEP_2				,	// Ad Hoc WEP character string 2 (with size)
	PRM_ADHOCWEP_3				,	// Ad Hoc WEP character string 3 (with size)
	PRM_ADHOCWEP_4				,	// Ad Hoc WEP character string 4 (with size)
	PRM_WPACIPHERMODE			,	// Encryption method
	PRM_PRESHAREDKEY			,	// PSK character string (with size)
	PRM_USERNAME				,	// User ID (with size)
	PRM_PASSWORD				,	// Password (with size)
	PRM_ROOTCAFILENAME			,	// Root (server) certificate file name
	PRM_CERTFILENAME			,	// Client certificate file name
	PRM_PRIVATEKEYFILENAME		,	// Private key file name
	PRM_MAC_ADDR				,	// MAC address (6-byte array)
	PRM_NETWORKMODE				,	// Network mode (infrastructure or Ad Hoc)
	PRM_WLAN_READY				,	// WLAN connection status check
	PRM_RSSI_THRESH				,	// Roaming threshold (60 to 90 dBm)
	PRM_INFRACHANNEL			,	// Infrastructure mode channel
	PRM_RADIUS_LOGOFF			,	// Perform a LogOff transmission to a Radius server.
	PRM_WLAN_INFRA_802d11_MODE	,	// Radio mode of infra(2.4G)
	PRM_INFRACHANNEL_5G			,	// Radio mode of infra(5G)
	// Bluetooth
	PRM_PINCODE = 201			,	// PIN code (0 to 16 bytes)
	PRM_LOCALDEVICENAME			,	// Local device name (0 to 30 bytes)
	PRM_BD_ADDR					,	// BD address (replacement for the MAC address)
	PRM_CON_BD_ADDR				,	// Connection destination BD address (replacement for the MAC address)
	PRM_SECURITYMODE			,	// Security mode (mode 0, mode 1, or mode 2)
	PRM_BD_DTRMODE				,	// DTR
	PRM_BLUE_SPPMODE			,	// SPP connection mode (1: Master or 2: Slave)
	PRM_LAST_TARGET_BD_ADDR		,	// BD address of the last other party device connected to
	// PPP
	PRM_TELNUMBER = 301			,	// Connection destination phone number
	PRM_PPP_USERNAME			,	// User name
	PRM_PPP_PASSWORD			,	// Password
	PRM_USE_PAP					,	// PAP authentication support
	PRM_USE_CHAP				,	// CHAP authentication support
	PRM_PPP_TIMEOUT				,	// PPP connection attempt time-out (seconds)
	PRM_PPP_HANGUP				,	// PPP HANGUP
	// Modem
	PRM_MODEM_ATCOMMAND = 401	,	// AT command to add when performing a modem connection
	PRM_MODEM_TELNUMBER			,	// Connection destination phone number (used when performing serial communication through a modem)
	PRM_MODEM_DIALTYPE			,	// Circuit type
	PRM_MODEM_DIALTIMEOUT		,	// Dial response time-out (seconds)
	PRM_MODEM_REDIALCOUNT		,	// Number of redials
	PRM_MODEM_REDIALINTERVAL	,	// Redial interval
	PRM_MODEM_CRADLERATE		,	// Baud rate when communicating through a communication unit
};
//-----------------------------------------------------------


//==================================
// Communication processing
//==================================
// Serial communication
#define	BT_IRDA_SENDBUF				(1)				// IrDA transmission buffer
#define	BT_IRDA_RECVBUF				(2)				// IrDA receive buffer
#define	BT_COM_SENDBUF				(1)				// 232c transmission buffer
#define	BT_COM_RECVBUF				(2)				// 232c receive buffer
#define	BT_SERIAL_SENDBUF			(1)				// Serial transmission buffer
#define	BT_SERIAL_RECVBUF			(2)				// Serial receive buffer
// Baud rate setting
#define	BT_BAUD_2400				(2400)			// 2400bps
#define	BT_BAUD_4800				(4800)			// 4800bps
#define	BT_BAUD_9600				(9600)			// 9600bps
#define	BT_BAUD_19200				(19200)			// 19200bps
#define	BT_BAUD_38400				(38400)			// 38400bps
#define	BT_BAUD_57600				(57600)			// 57600bps
#define	BT_BAUD_115200				(115200)		// 115200bps
#define BT_BAUD_230400				(230400)		// 230400bps
#define	BT_BAUD_921600				(921600)		// 921600bps
// Data length setting
#define	BT_DATA_7BIT				(7)				// 7bit
#define	BT_DATA_8BIT				(8)				// 8bit
// Stop bit setting
#define	BT_STOP_1BIT				(1)				// 1bit
#define	BT_STOP_2BIT				(2)				// 2bit
// Parity setting
#define	BT_PARITY_NONE				(0)				// None
#define	BT_PARITY_ODD				(1)				// Odd
#define	BT_PARITY_EVEN				(2)				// Even

// Communication destination setting
#define	COMM_DEVICE_IRDA			(1)
#define	COMM_DEVICE_RF				(2)
#define	COMM_DEVICE_RS232C			(3)
#define	COMM_DEVICE_SOCK_COMPATIBLE	(4)
#define	COMM_DEVICE_SOCK_PASSIVE	(5)
#define	COMM_DEVICE_CRADLE			(6)
#define	COMM_DEVICE_USB				(7)
#define	COMM_DEVICE_IRDA2			(8)
#define	COMM_DEVICE_BLUETOOTH		(11)
#define	COMM_DEVICE_BLUETOOTH_PPP	(12)
#define	COMM_DEVICE_MODEM_SERIAL	(13)

// Zengin TCP/IP communication destination setting
#define	BT_ZGN_BASIC				(0x00)
#define	BT_ZGN_PC					(0x10)
#define	BT_ZGN_PUT					(0)
#define	BT_ZGN_GET					(1)
#define	BT_ZGN_APPEND				(2)
#define	BT_ZGN_START				(0)
#define	BT_ZGN_RESTART_FILE			(1)
#define	BT_ZGN_RESTART_TEXT			(2)
#define	BT_ZGN_CR					(1)
#define	BT_ZGN_CRLF					(2)
#define	BT_ZGN_EOF					(3)
#define	BT_ZGN_LF					(4)
#define	BT_ZGN_EBCDIC2JIS8			(1)
#define	BT_ZGN_JIS82EBCDIC			(2)

// Socket communication settings
#define	BTSOCK_AF_INET				(2)
#define	BTSOCK_STREAM				(1)
#define	BTSOCK_DGRAM				(2)
#define	BTSOCK_IPPROTO_TCP			(6)
#define	BTSOCK_MSG_PEEK				(2)

#define	BTSOCK_INADDR_ANY           (0)
#define	BTSOCK_ADDR_ANY             (0)

#define	BTSOCK_EINVAL				(10022)
#define	BTSOCK_EWOULDBLOCK			(10035)
#define	BTSOCK_ENOTSOCK				(10038)
#define	BTSOCK_ENETDOWN				(10050)
#define	BTSOCK_EISCONN				(10056)
#define	BTSOCK_ENOTCONN				(10057)

#define	BTSOCK_SD_RECV				(0x00)
#define	BTSOCK_SD_SEND				(0x01)
#define	BTSOCK_SD_BOTH				(0x02)

#define	BTSOCK_SO_BROADCAST			(1)
#define	BTSOCK_SO_SYNRETRY			(2)
#define	BTSOCK_SO_DATARETRY			(3)
#define	BTSOCK_SO_RETRYTIMEOUT		(4)
#define	BTSOCK_SO_KEEPALIVE			(5)

// Switching the mode for connecting to the LC communication library
#define	COMM_TRANSMODE_NORMAL		(0)
#define	COMM_TRANSMODE_LC			(1)

//----------------------------------
// Wireless communication settings
//----------------------------------
// Transmission rate
#define	BT_RF_RATE_1M				(0x01)			// Fixed to 1 Mbps
#define	BT_RF_RATE_2M				(0x02)			// Fixed to 2 Mbps (setting to use in normal situations)
#define	BT_RF_RATE_5M				(0x04)			// Fixed to 5.5 Mbps
#define	BT_RF_RATE_11M				(0x08)			// Fixed to 11 Mbps
#define	BT_RF_RATE_AUTO				(0x0F)			// Automatic, RATE802_11[] = {0x02, 0x04, 0x0b, 0x16}
#define	BT_RF_RATE_BT900COMPATI		(0x03)			// BT900 compatibility mode, RATE802_11[] = {0x02, 0x04}

// Preamble type
#define	BT_RF_PREAMBLE_S			(0)				// Short preamble
#define	BT_RF_PREAMBLE_L			(1)				// Long preamble (setting to use in normal situations)

// Authentication classification
#define	BT_RF_AUTH_OPEN				(0)				// Open System
#define	BT_RF_AUTH_SHARED			(1)				// Shared Key

// Power management mode
#define	BT_RF_POWER_ACTIVE			(0)				// Always active mode
#define	BT_RF_POWER_AUTO			(2)				// Automatic (automatic switching between active and power save)

// Automatic power save monitor time
#define	BT_RF_PS_TIME_DFLT			(2)

// BeaconLostCount
#define	BT_RF_BEACONLOST_DFLT		(10)

// Out-of-range scan interval
#define	BT_RF_SCAN_INTERVAL			(10)			// 10 seconds

// WEP function
#define	BT_RF_WEP_OFF				(0)				// WEP OFF
#define	BT_RF_CIPH_WEP64			(1)				// WEP ON 64bit
#define	BT_RF_CIPH_WEP128			(2)				// WEP ON 128bit

// Maximum value of the channel detection result
#define	BT_RF_AP_LIST_MAX			(12)

// Maximum number of channels
#define	BT_RF_CH_MAX				(14)

// Connection status
#define	BT_RF_WLAN_DISCONNECT		(0)				// Disconnection
#define	BT_RF_WLAN_CONNECT			(1)				// Connection
#define	BT_RF_WLAN_OUTAREA			(2)				// Out of range

// Authentication result
#define	BT_RF_STS_WPA_NOT_ENA		(0)				// WPA function disabled
#define	BT_RF_STS_AUTH_INIT			(1)				// Unauthenticated status
#define	BT_RF_STS_AUTH_OK			(2)				// Normal completion of authentication
#define	BT_RF_STS_AUTH_NG			(3)				// Authentication failure

// WLAN infra mode
#define	BT_WLAN_11B					(1)				// 802.11b only
#define	BT_WLAN_11BGN_2d4G			(2)				// 802.11bgn(2.4G)
#define	BT_WLAN_11AN_5G				(3)				// 802.11an(5G)

//----------------------------------
// Wireless security
//----------------------------------
// Authentication method
#define	BT_RF_SECUR_WPA_OFF			(0)				// WEP
#define	BT_RF_SECUR_WPA_PSK			(1)				// WPA-PSK
#define	BT_RF_SECUR_WPA_1X_PEAP		(2)				// WPA-PEAP
#define	BT_RF_SECUR_WPA_1X_EAPTLS	(3)				// WPA-TLS
#define	BT_RF_SECUR_WPA2_PSK		(4)				// WPA2-PSK
#define	BT_RF_SECUR_WPA2_1X_PEAP	(5)				// WPA2-PEAP
#define	BT_RF_SECUR_WPA2_1X_EAPTLS	(6)				// WPA2-TLS
// Encryption method
#define	BT_RF_SECUR_CIPH_TKIP		(1)				// WPA-TKIP
#define	BT_RF_SECUR_CIPH_AES		(2)				// WPA-AES
#define	BT_RF_SECUR_CIPH_NONE		(3)				// 

// WEP key settings
#define	BT_RF_SECUR_WEP_NOTUSE		(0)
#define	BT_RF_SECUR_WEP_64BIT		(5)
#define	BT_RF_SECUR_WEP_128BIT		(13)

#define	BT_RF_SECUR_AUTH_NOTSET		(-1)

//----------------------------------
// Wireless connection information
//----------------------------------
// Network mode
#define	BT_RF_ADHOCMODE				(1)				// Ad Hoc mode
#define	BT_RF_INFRAMODE				(2)				// Infrastructure mode

//----------------------------------
// Socket
//----------------------------------
#ifndef BTSOCKFD_SETSIZE
#define BTSOCKFD_SETSIZE      64
#endif // BTSOCKFD_SETSIZE

//----------------------------------
// RF communication
//----------------------------------
// API setting value constants
#define	BT_RF_WAIT_OFF				(0)				// Move to the next process without waiting for the current process to finish.
#define	BT_RF_WAIT_ON				(1)				// Wait for the current process to finish, and then move to the next process.
#define	BT_RF_MODE_OFF				(0)				// Turn the function off.
#define	BT_RF_MODE_ON				(1)				// Turn the function on.
#define	BT_RF_MODE_INFRA			(0)				// Communicating through AP
#define	BT_RF_MODE_PC				(BT_RF_MODE_INFRA)	//
#define	BT_RF_MODE_ADHOC			(1)					// Communicating between terminals
#define	BT_RF_MODE_HT				(BT_RF_MODE_ADHOC)	//
// File transfer mode
#define	BT_RF_TRANS_HT_FILE			(1)				// File transfer between terminals
#define	BT_RF_TRANS_PC_FILE			(2)				// File transfer between terminals and the PC

//----------------------------------
// Quick update
//----------------------------------
#define	BT_RF_APLIUPDATE_SAME		(1)	
#define	BT_RF_APLIUPDATE_MOST		(2)	
#define	BT_RF_APLIUPDATE_HIGH		(3)	
#define	BT_RF_APLIUPDATE_NORM		(4)	
#define	BT_RF_APLIUPDATE_LOW		(5)	
#define	BT_RF_APLIUPDATE_NONE		(6)	


//==================================
// Related to properties
//==================================
#define	PP_IP_LENGTH				(15+1)			// Size of the area of the IP address field
#define	PP_WEP_LENGTH				(13+1)			// Size of the area of the WEP key field
#define	PP_ESSID_LENGTH				(32+1)			// Size of the area of the ESSID field
#define	PP_PSK_LENGTH				(63+1)			// Size of the area of the PSK field
#define	PP_USERID_LENGTH			(32+1)			// Size of the area of the user ID field
#define	PP_PASSWORD_LENGTH			(32+1)			// Size of the area of the password field
#define	PP_BD_ADDR_LENGTH			(12+1)			// Size of the area of the BD address field

#define	PP_TELNUMBER_MAX			(16)			// Maximum number of bytes of the connection destination phone number
#define	PP_PPP_USERNAME_MAX			(30)			// Maximum number of bytes of the user name
#define	PP_PPP_PASSWORD_MAX			(30)			// Maximum number of bytes of the password
#define	PP_PINCODE_MAX				(16)			// Maximum number of bytes of the PIN code
#define	PP_LOCALDEVICENAME_MAX		(30)			// Maximum number of bytes of the local device name

#define	PP_MODEM_ATCOMMAND_MAX		(255)			// Maximum number of bytes of the AT command to add when performing a modem connection

// Authentication method
#define	PP_SECU_TYPE_WEP			BT_RF_SECUR_WPA_OFF			// WEP
#define	PP_SECU_TYPE_WPA_PSK		BT_RF_SECUR_WPA_PSK			// WPA-PSK
#define	PP_SECU_TYPE_WPA_PEAP		BT_RF_SECUR_WPA_1X_PEAP		// WPA-PEAP
#define	PP_SECU_TYPE_WPA_TLS		BT_RF_SECUR_WPA_1X_EAPTLS	// WPA-TLS
// Encryption method
#define	PP_SECU_WPA_TKIP			BT_RF_SECUR_CIPH_TKIP		// WPA-TKIP
#define	PP_SECU_WPA_AES				BT_RF_SECUR_CIPH_AES		// WPA-AES
#define	PP_SECU_NONE				BT_RF_SECUR_CIPH_NONE		// 

// Communication speed
#define	PP_9600BPS					BT_BAUD_9600	// 9600bps
#define	PP_19200BPS					BT_BAUD_19200	// 19200bps
#define	PP_38400BPS					BT_BAUD_38400	// 38400bps
#define	PP_57600BPS					BT_BAUD_57600	// 57600bps
#define	PP_115200BPS				BT_BAUD_115200	// 115200bps
#define	PP_921600BPS				BT_BAUD_921600	// 921600bps
// Data length
#define	PP_DATA_7BIT				BT_DATA_7BIT	// 7bit
#define	PP_DATA_8BIT				BT_DATA_8BIT	// 8bit
// Stop bit
#define	PP_STOP_1BIT				BT_STOP_1BIT	// 1bit
#define	PP_STOP_2BIT				BT_STOP_2BIT	// 2bit
// Parity
#define	PP_PARITY_NONE				BT_PARITY_NONE	// None
#define	PP_PARITY_ODD				BT_PARITY_ODD	// Odd
#define	PP_PARITY_EVEN				BT_PARITY_EVEN	// Even
// Packet size
#define	PP_PACKET2048				(2048)			// 2048byte
#define	PP_PACKET1024				(1024)			// 1024byte
#define	PP_PACKET512				(512)			// 512byte
#define	PP_PACKET256				(256)			// 256byte

//********************************************************************************
//			Structure declarations
//********************************************************************************
//----------------------------------
// Wireless information
//----------------------------------
typedef struct _st_rf_configset{
	unsigned char	NetworkMode;	// Network mode
	unsigned char	AdhocCh;		// Ad Hoc mode channel
	unsigned char	Preamble;		// Preamble type
	unsigned char	WepAuthtype;	// WEP authentication type
	unsigned char	rate;			// Transmission rate
	unsigned char	PwMode;			// Power management mode
	unsigned char	PowerSavetime;	// Automatic power save monitor time
	unsigned char	BeaconLostcount;// BeaconLostcount
} ST_RF_CONFIGSET;

typedef struct _st_rf_connectioninfo{ 
	unsigned char	AuthStatus;		// Authenticated status
	unsigned char	Ch;				// Connection destination channel
	unsigned char	MacAddress[6];	// Connection destination MAC address
	unsigned char	Rssi;			// Signal strength
} ST_RF_CONNECTIONINFO;

typedef struct _st_wep_info{ 
	long			size;				// WEP size (0, 5, 13)
	unsigned char	wep[PP_WEP_LENGTH];	// WEP data
} ST_WEP_INFO;

//----------------------------------
// Security settings
//----------------------------------
typedef struct _st_rf_securityinfoset{ 
	unsigned char ssid[PP_ESSID_LENGTH];
	char		 ssid_size;
	char		 wep_selected;		// 0-3
	char		 wep_size[4];		// WEP key size
	unsigned char wep[4][PP_WEP_LENGTH]; 
} ST_RF_SECURITYINFOSET; 

typedef struct _st_rf_securityauthset{
	short		psk_len;
	char		psk[PP_PSK_LENGTH]; 
	short		usr_id_len;
	char		usr_id[PP_USERID_LENGTH]; 
	short		usr_pass_len;
	char		usr_pass[PP_PASSWORD_LENGTH]; 
	char		rootca_filename[BT_FILE_NAMEMAX+3]; 
	char		cert_filename[BT_FILE_NAMEMAX+3]; 
	char		prvkey_filename[BT_FILE_NAMEMAX+3];
} ST_RF_SECURITYAUTHSET;

//----------------------------------
// Socket communication
//----------------------------------
typedef int	BTSOCKET;

typedef struct btsockaddr {
	unsigned short	sa_family;
	char			sa_data[14];
} BTSOCKADDR;

typedef struct btin_addr {
	union {
		struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { unsigned short s_w1,s_w2; } S_un_w;
		unsigned long S_addr;
	} S_un;
} BTSOCKIN_ADDR;
#define	s_addr	S_un.S_addr
#define	s_host	S_un.S_un_b.s_b2
#define	s_net	S_un.S_un_b.s_b1
#define	s_imp	S_un.S_un_w.s_w2
#define	s_impno	S_un.S_un_b.s_b4
#define	s_lh	S_un.S_un_b.s_b3

typedef struct btsockaddr_in {
	short			sin_family;
	unsigned short	sin_port;
	BTSOCKIN_ADDR	sin_addr;
	char			sin_zero[8];
} BTSOCKADDR_IN;

typedef struct btfd_set {
	unsigned int	fd_count;
	BTSOCKET		fd_array[BTSOCKFD_SETSIZE];
} BTSOCKFD_SET;

typedef struct bttimeval {
	long		tv_sec;
	long		tv_usec;
} BTSOCKTIMEVAL;

#define BTSOCK_MAX_HX_ALIAS		(1)
#define BTSOCK_MAX_HX_ADDRS		(3)
typedef struct bthostent {
    char*				h_name;                           // official name of host
    char*				h_aliases[BTSOCK_MAX_HX_ALIAS];   // alias list
    unsigned short		h_addrtype;                       // host address type
    unsigned short		h_length;                         // length of address
    char*				h_addr_list[BTSOCK_MAX_HX_ADDRS]; // list of addresses
} BTSOCKHOSTENTL;

//----------------------------------
// FTP communication
//----------------------------------
typedef struct _st_ftpconfig{ 
	long			timeout; 
	long			cancelkey;
	unsigned long	target_ip; 
	unsigned short	ftp_portnum;
	unsigned short	rsv1;
	char			usrname[65]; 
	char			password[65]; 
	char			logfilename[BT_PATH_NAMEMAX+1];
} ST_FTPCONFIG;

typedef struct _st_ftpsconfig{ 
	int ftps_protocol;      // Protocol to use: BT_ON (TLS) or BT_OFF (SSLv2v3)
	int ftps_data_encrypt;  // Data port encryption: BT_ON or BT_OFF
	int ftps_verify;		// Certificate authentication: BT_ON or BT_OFF
	char ftps_certfile[BT_PATH_NAMEMAX+1];   // Certificate file name
} ST_FTPSCONFIG;


//----------------------------------
// RF communication
//----------------------------------
// Terminal IP information structure
typedef	struct	_rfstatus	{
	unsigned char	ip_adr[4];									// Terminal IP address (fixed to four bytes)
	unsigned char	subnet[4];									// Subnet mask (fixed to four bytes)
	unsigned char	defgate[4];									// Default gateway (fixed to four bytes)
} RF_STATUS;

// Host IP information structure
typedef	struct	_host_status{
	unsigned char	ip_adr[4];									// Host IP address data (fixed to four bytes)
} HOST_STATUS;

// Data structure
typedef	struct	_rfdatif	{
	unsigned char	ip[4];										// When receiving data, this is the other party's IP address. This has no meaning when transmitting data.
	unsigned long	cnt;										// Length of the received (transmitting) data
	unsigned char	*buf;										// Leading pointer of the received (transmitting) data
} RFDATIF;

//File transmission destination IP information structure
typedef	struct	_fileup_status	{
	unsigned long	len;										// Length of the IP address (0 [not set] or 4 [set])
	unsigned char	ip_adr[4];									// Connection destination IP address (fixed to four bytes)
} ST_FILEUP_STATUS;

// ESSID structure
typedef struct _ess_status {
	unsigned long	ess_len;									// ESSID length
	unsigned char	ess_dat[32];								// ESSID character string data (variable length between 1 and 32 bytes)
} ESS_STATUS;

// Structure for the number of retransmissions
typedef struct _retry_status {
	unsigned char	retry_count_max;							// Maximum number of retransmissions (0 to 9)
	unsigned char	retry_timer_init;							// Retransmission timer time-out value (1 to 10)
} RETRY_STATUS;

//----------------------------------
// KEYENCE procedure
//----------------------------------
// Terminal application update information structure
typedef struct {
	char major_version;		// Major version
	char minor_version;		// Minor version
	char comment[62];		// Comments related to the application (the title)
	char filepath[260];		// Folder to search for the application in (relative path from the reception folder)
							// Name of the application file when acquisition is performed.
} COMM_APP_INFO;

//----------------------------------
// Zengin TCP/IP procedure
//----------------------------------
typedef struct _st_zgnconfig {
	unsigned short	MaxTextSize;		// Maximum transmission text length (32 to 32768)
	unsigned short	Timeout;			// Non-communication timer (0 to 999)
	unsigned char	DestIP[80];			// Connection destination IP address
	unsigned short	DestPortNo;			// Connection destination port number (1 to 65535)
	unsigned short	LocalPortNo;		// Local port number (0 to 65535)
	unsigned short	TcpConnectRetry;	// Number of TCP connection retries (0 to 99)
	unsigned short	TtcRoundupInit;		// Default value for rounding up the TTC counter (0 or 1)
	unsigned char	DefineMN;			// Number of consecutive receptions (0 to 15)
	unsigned char	ZeroByteFile;		// 0-byte file transmission permission flag (BT_OFF or BT_ON) 
	unsigned char	TransDateCheck;		// Date check of the response file control telegram (BT_OFF or BT_ON)
	unsigned char	TtcBranch;			// TTC telegram subdivision (BT_ZGN_BASIC or BT_ZGN_PC)
	unsigned char	Hostcord[7];		// Confirmation code of the other center
	unsigned char	Termcord[7];		// Confirmation code of this center
	unsigned char	Password[6];		// Password
	unsigned char	Ext[34];			// Communication control expansion area
} ST_ZGNCONFIG; 

typedef struct _st_zgnfconfig {
	unsigned char	FileName[256];		// Name of the file to transmit or receive
	unsigned char	Mode;				// Mode (transmit [BT_ZGN_PUT], overwrite receive [BT_ZGN_GET], or append receive [BT_ZGN_APPEND])
	unsigned char	Branch;				// Telegram subdivision (start request, retransmission request [file], or retransmission request [text])
										// (BT_ZGN_START/BT_ZGN_RESTART_FILE/BT_ZGN_RESTART_TEXT)
	unsigned char	Pack;				// Compression used at the time of transmission (BT_OFF or BT_ON)
	unsigned char	Derimita;			// Editing the delimiter (none, CR, CR+LF, EOF removal, or LF)
										// (BT_OFF/BT_ZGN_CR/BT_ZGN_CRLF/BT_ZGN_EOF/BT_ZGN_LF)
	unsigned char	Blocking;			// Blocking (BT_OFF or BT_ON)
	unsigned char 	ChangeCodePtn;		// Code conversion pattern (none, EBCDIC->JIS8, or JIS8->EBCDIC)
										// (BT_OFF/BT_ZGN_EBCDIC2JIS8/BT_ZGN_JIS82EBCDIC)
	unsigned char	Cycle;				// Cycle number (0 to 99)
	unsigned char	VLRec;				// Variable-length record specification (BT_OFF or BT_ON)
	unsigned short	RecSize;			// Record length (1 to maximum text length - 5)
	unsigned char	ZFileName[12];		// Zengin file name
	unsigned char	FileAccessKey[6];	// File access key
	unsigned char	FileSub[17];		// File name supplementary information
	unsigned char	FileExt[14];		// File control expansion area
} ST_ZGNFCONFIG;

//----------------------------------
// QUT
//----------------------------------
typedef struct _apli_updateinfo{
	char			apptype;
	char			filename[BT_PATH_NAMEMAX+1];
	char			title[65];
	char			version[2];
	char			priority;
} APLI_UPDATEINFO;

//----------------------------------
// Serial communication
//----------------------------------
// Communication setting details structure
typedef struct	_st_comenv {
	unsigned long	baudRate;									// Baud rate
	short			stopBits;									// Stop bit
	short			parity;										// Parity
	short			dataLen;									// Data length
} ST_COMENV;

//----------------------------------
// Channel scan
//----------------------------------
typedef struct {
	unsigned char ch;				// Channel 
	unsigned char signallevel;		// Signal level 
	unsigned char macaddr[6];		// MAC address 
	unsigned char ssidlen;			// SSID length (number of bytes)
	unsigned char ssid[32];			// Connection destination SSID
	unsigned char rsv1[3];			// rsv
} T_APINFO_EACH;

typedef struct {
	T_APINFO_EACH apinfo[12];
} T_APINFO;

//----------------------------------
// BLUETOOTH
//----------------------------------
typedef struct {
	char devicename[32];			// Device name (a character string of up to 29 characters + one null character)
	unsigned char bd_address[12];	// BD address (12 binary digits)
} T_BLUETOOTHINFO_EACH;

typedef struct {
	T_BLUETOOTHINFO_EACH bluetoothinfo[9];
} T_BLUETOOTHINFO;


#ifdef __cplusplus
}
#endif


#endif // !defined(__BTLIB_COMM_H__)
