
#define APP_VERSION "    Ver 1.0.1.1"
#define MAINWNDNAME "VNN NOK"
#define PGM_NAME	"Warehouse System"

// target PC macros
#define IPNAME "server_addr\0"
#define PORTNAME "port_no\0"


#define CMD_GET_STAFF "M00"		// M00 staff master


//----------------------------------------------------------------------------
//	 staff info.(PC->BCR)
//----------------------------------------------------------------------------
typedef	struct{
		char	Name[30];
		char	AdminSign[1];
		char	Location[3];		
		char	UserId[10];
		char	None;
}	BM_USER;

//----------------------------------------------------------------------------
//	 for error message(PC=>BCR)
//----------------------------------------------------------------------------
typedef	struct{
		char	msg1[20];	// ‚Ps–Ú
		char	msg2[20];	// ‚Qs–Ú
		char	msg3[20];	// ‚Rs–Ú
		char	msg4[20];	// ‚Ss–Ú
		char	msg5[20];	// ‚Ts–Ú
}	BCR_MSG;
