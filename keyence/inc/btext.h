#if 0
// *******************************************************************************
// **		Copyright(c) 2013, Keyence Corporation. All rights reserved
// **		File Name	: btext.h
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
#if !defined(__BTEXT_H__)
#define __BTEXT_H__


#include "btlib.h"

#ifdef __cplusplus
extern "C" {
#endif


//===============================================================================
// Setting devices
//===============================================================================
extern short btBackLight(short sw, short timeout, short factor);
extern short btBackLightStandby(short sw);
extern short btBlinkBackLight(short sw, short ontime, short offtime, short cnt, short onbrightness, short offbrightness);
extern short btKeyBackLight(short sw);
extern short btBuzzer(short buzsw, short ontime, short offtime, short cnt, short pitch);
extern short btLed(short ledsw, short ontime, short offtime, short cnt, short color);
extern short btVibration(short vibsw, short ontime, short offtime, short cnt);
extern short btContrast(short contrast);
extern short btVolume(short volume);
extern short btKeyVolume(short volume);
extern short btScreenContrast(char *pTitle);
extern short btScreenVolume(char *pTitle);
extern short btScreenClick(char *pTitle);


//===============================================================================
// Input
//===============================================================================
extern short btGetTouchmode(short* mode, short* click);
extern short btSetTouchmode(short mode, short click);
extern short btGetKeyFunction(KEYFUNC *pKeyFunc);
extern short btSetKeyFunction(KEYFUNC *pKeyFunc);
extern long btGetPowerKeyValid(void);
extern short btSetPowerKeyValid(long sw);
extern short btSetShift(short valid, short sw);
extern short btGetShift(short *valid, short *sw);
extern short btTbl2Code(long keytbl, char *pKeycode);
extern long btKeySense(long keytbl);
extern long btKeyWait(long keytbl);
extern long btKeyWaitEx(long keytbl, short timeout);
extern void btKeyBufClear(void);
extern short btKeyBufSense(void);
extern short btKeyBufRead(char *pBuffer, short size);
extern short btStatus(short statustype);
extern short btScrollGuide(short scrollguide);
extern short btSetGaiji(unsigned short code, char *pGaijiData);


//===============================================================================
// Dialog box
//===============================================================================
extern short btMenu(char *title, MENUINFO *menuinfo);
extern short btMenu2(char *title, MENUINFO *menuinfo);
extern short btMessageBox(char *pMsg, short type);
extern short btMessageBoxEx(char *pMsg, short type, char *pTitle, short fontsize);
extern short btLineMessageBox(short posy, char *pMsg, char *pSel1, char *pSel2, short InitSel, short fontsize);
extern short btLineMessageBoxg(short posy, char *pMsg, char *pSel1, char *pSel2, short InitSel, short fontsize);
extern short btCalc(DECIMAL *inival);
extern short btScreenSetDateTime(char *title);
extern short btCalendar(DATEINFO* dt);
extern short btEdit(char *pTitle, char *pBuffer, short maxsize);
extern short btScreenPassword(char *pTitle, char *pPassword, short inputtype, short maxdigit);


//===============================================================================
// Low-rank file
//===============================================================================
extern short btDriveInfo(short driveno, DRIVE_INFO *pDriveInfo);
extern short btFileInfo(char *pFilename, FILE_INFO *pFileInfo);
extern short btFileInfoNext(FILE_INFO *pFileInfo);
extern short btRename(char *pOldFilename, char *pNewFilename);
extern short btCopy(char *pSrcFilename, char *pDstFilename, short overwrite);
extern short btRemove(char *pFilename);
extern short btDirectoryCreate (char *pDirectoryName);
extern short btFileOpen(char *pFilename, char *pMode);
extern short btFileClose(short fileno);
extern long btFileRead(short fileno, char *pBuffer, long count);
extern long btFileWrite(short fileno, char *pBuffer, long count);
extern long btFileGets(short fileno, char *pBuffer, long count);
extern long btFilePuts(short fileno, char *pString);
extern short btFileSeek(short fileno, long offset, short origin);
extern long btFileTell(short fileno);
extern long btFileCountLine(char *filename);
extern short btDriveMount(short driveno, short sw);
extern short btDriveIsMount(short driveno);
extern short btUnCompress (short type ,char *pSrcFilename, char * pDstFilename, short overwrite);
extern short btCompress (short type ,char *pSrcFilename, char *pDstFilename, short overwrite);


//===============================================================================
// System
//===============================================================================
extern short btSetDateTime(DATEINFO *pDateInfo, TIMEINFO *pTimeInfo);
extern short btGetDateTime(DATEINFO *pDateInfo, TIMEINFO *pTimeInfo);
extern unsigned long btGetTickCount(void);
extern long btGetResource(long *total_size, long *global_size, long *stack_size);
extern long btHandyGetParameter(short parameter, void* value);
extern long btHandySetParameter(short parameter, void* value);
extern long btWait(short timeout, long dwCancelKey);
extern void btReset(void);
extern short btSetTimer(short timeid, long cycle, short repeat);
extern short btKillTimer(short timeid);
extern short btWaitNextEvent(short timeout, unsigned long* GenerateTick, unsigned long* uMsg, unsigned long* wParam, unsigned long* lParam );
extern short btCountEventQueue(void);
extern short btSetEventMode(short enable,unsigned long Msg, unsigned long wParam, unsigned long lParam);
extern short btGetEventMode(unsigned long Msg, unsigned long wParam, unsigned long lParam);
extern short btDebugPort(short sw);
extern short btDebugPrint(char *pBuffer);
extern short btDebugInput(char *pBuffer, short length, short timeout, short sw);
extern long btGetLostAlarm(LOST_ALARM* pLostAlarm);
extern long btSetLostAlarm(LOST_ALARM* pLostAlarm);
extern long btGetAlarm(short id, ALARM* pAlarm);
extern long btSetAlarm(short id, ALARM* pAlarm);
extern long btGetApplicationSchedule(short id, SCHEDULE* pSchedule);
extern long btSetApplicationSchedule(short id, SCHEDULE* pSchedule);

extern long btPropertySave(void);
extern short btChangeApplication(char* pFilename);

extern char *btstrcpy(char *dst, const char *src);
extern long btstrcmp(const char *string1, const char *string2);
extern long btstrlen(const char *string);
extern void btstrcat(char *first, char *second);
extern char *btstrstr(const char *first, const char *second);
extern void *btmemcpy(void *dst, const void *src, long len);
extern long btmemcmp(const void *mem1, const void *mem2, long len);
extern void *btmemset(void *dst, const char set, long len);
extern void *btmalloc(long size);
extern void *btcalloc(long num, long size);
extern void *btrealloc( void *memblock, long size);
extern void btfree( void *memblock );
extern short btnum2char(long num, char *pBuf);
extern short btnum2hexchar(long num, char *pBuf, short keta, short type);
extern short btdec2char(DECIMAL decimal, char *pBuf);
extern short btchar2num(char *pBuf, long *pNum);
extern short bthexchar2num(char *pBuf, long *pNum);
extern long btsprintf(char *pcstring, char *pcformat, ...);
extern long btsscanf(char *pcstring, char *pcformat, ...);
extern char *btstrtok(char *pcstring, char *pcdelimiter);
extern long btmbcheck(char *string, char *checkchar);
extern short btAddDate(DATEINFO *pDateInfo, short add, DATEINFO *pDateResult);
extern short btDiffDate(DATEINFO *pDateInfo1, DATEINFO *pDateInfo2);
extern short btHan2Zen(char *pHankaku, char *pZenkaku);

extern short btAutoPowerOff(short time);
extern short btResume(short sw);
extern short btWakeup(short sw);
extern short btSetTermid(unsigned short TerminalId);
extern unsigned short btGetTermid(void);
extern short btBatteryLevel(short type);
extern void btVersion(char *pVersion);
extern long btGetTermType(void);
extern long btGetSerialNo(void);
extern long btGetMacAddress(unsigned char *mac_address);
extern unsigned long btTickCount10(void);


//===============================================================================
// Screen display settings
//===============================================================================
extern short btFont(short fontnum, short gapx, short gapy);
extern short btViewer(char *pTitle, short top, short bottom, short step, short *pos);
extern short btPrint(char *pString, DSPATTR *pDspAttr);
extern short btPrintg(char *pString, DSPATTRG *pDspAttrg);
extern short btClear(void);
extern short btSetWindow(short lcdposy, short height, short vramposy);
extern short btGetWindow(short *plcdposy, short *pheight, short *pvramposy);
extern short btReadWindow(char *readptr);
extern short btWriteWindow(char *writeptr);
extern short btScroll(short offset, short top, short bottom);
extern short btLine(GPOSITION *pGpos1, GPOSITION *pGpos2, short color);
extern short btBox(GPOSITION *pGpos1, GPOSITION *pGpos2, short color, short paint);
extern short btArc (GPOSITION *cposition, short radius, short startangle, short endangle,short linecolor, short paint);
extern short btBitmap (GPOSITION *pGpos1, GPOSITION *pGpos2, char* filename, unsigned long* transparentcolor);
extern short btCursor(short cursortype, TPOSITION *pTpos, short mag);
extern short btCursorg(short cursortype, GPOSITION *pGpos1, GPOSITION *pGpos2);
extern short btGetCursorPos(TPOSITION *pTpos);
extern short btGetCursorPosg(GPOSITION *pGpos1, GPOSITION *pGpos2);
extern short btLcdUpdate(short sw);
extern short btGraphic2Text(GPOSITION *pGpos, TPOSITION *pTpos);
extern short btText2Graphic(TPOSITION *pTpos, GPOSITION *pGpos);
extern short btSetColor (unsigned long dotcolor, unsigned long backcolor, unsigned long seldotcolor, unsigned long selbackcolor);
extern short btGetColor (unsigned long* dotcolor, unsigned long* backcolor, unsigned long* seldotcolor, unsigned long* selbackcolor);
extern short btAxisMode (short sw);


//===============================================================================
// Input processing
//===============================================================================
extern short btInputInteger(long *pLValue, DSPATTR *pDspAttr, INPUTINTEGERATTR *pInputNumAttr);
extern short btInputDecimal(DECIMAL *pDValue, DSPATTR *pDspAttr, INPUTDECIMALATTR *pInputDecimalAttr);
extern short btInputString(char *pBuffer, DSPATTR *pDspAttr, INPUTSTRATTR *pInputStrAttr);
extern short btInputStringEx(char *pBuffer, DSPATTR *pDspAttr, INPUTSTRATTR *pInputStrAttr);
extern short btInputDate(DATEINFO *pDateInfo, DSPATTR *pDspAttr, short step);
extern short btChoice(char *filename, DSPATTR *pDspAttr, char separator, long *index, char *seldata);
extern short btInputIntegerg(long *pLValue, DSPATTRG *pDspAttr, INPUTINTEGERATTR *pInputNumAttr);
extern short btInputDecimalg(DECIMAL *pDValue, DSPATTRG *pDspAttr, INPUTDECIMALATTR *pInputDecimalAttr);
extern short btInputStringg(char *pBuffer, DSPATTRG *pDspAttr, INPUTSTRATTR *pInputStrAttr);
extern short btInputStringgEx(char *pBuffer, DSPATTRG *pDspAttr, INPUTSTRATTR *pInputStrAttr);
extern short btInputDateg(DATEINFO *pDateInfo, DSPATTRG *pDspAttr, short step);
extern short btChoiceg(char *filename, DSPATTRG *pDspAttr, char separator, long *index, char *seldata);
extern short btInputStringResult(void);


//===============================================================================
// Barcode reader
//===============================================================================
extern short btSetBcrCommonOption ( BCRCONF *pBcrConf );
extern short btGetBcrCommonOption ( BCRCONF *pBcrConf );
extern short btSetBcrCodeOption ( BCRCODESET *pBcrCodeSet );
extern short btGetBcrCodeOption ( BCRCODESET *pBcrCodeSet );
extern short btSetBcrProperty (long id, const void* pProperty );
extern short btGetBcrProperty (long id, void* pProperty );
extern short btSetBcrCode ( short codetype );
extern short btGetBcrCode ( short *pCodetype );
extern void btBcrBufClear ( void );
extern short btBcrTrigger ( short sw );
extern short btBcrMarker ( short sw );
extern short btBcrInput ( BCRINPUTDATA *pBcrInput );
extern short btBcrGetData ( short index, char* pBuffer );
extern short btBcrGetReport ( short index, BCR_RESULT_REPORT *pReport );
extern short btBcrInputEx ( short *pCount );
extern short btBcrQRReport ( short index, BCR_RESULT_QR_REPORT *pQRReport );
extern short btBcrSaveImage ( char *filename );
extern short btIllumination (short sw);
extern short btLiveViewControl( short operation );
extern short btPhotoSave( char *filepath, short ftype, short mode, short light );

//===============================================================================
// File operations
//===============================================================================
extern short btLogRecordSetIdType( short idtype );
extern short btLogRecordGetIdType( void );
extern long btLogRecordAppend (short driveno, char *pFilename, char *pString, char separator);
extern long btLogRecordCount( short driveno, char *pFilename );
extern long btLogRecordRead( short driveno, char *pFilename, short origin, short recno, short *pIndexNo, char *pRecData);
extern long btLogRecordModify (short driveno, char *pFilename, short control, short indexno,  char *pModify, char separator);
extern long btLogFileDelete( short driveno);


//===============================================================================
// Search
//===============================================================================
extern short btSearchOpen (char *pFilename);
extern short btSearchClose (short searchno);
extern short btSearchFirst (short searchno, short indexno, char *pString, char *pRecData);
extern short btSearchNext(short searchno, char *pRecData);
extern short btSearchCreate( char* pMstFilename, char* pIdxFilename, char* pSrchFilename );
extern short btSearchCreateIndex( char* pMstFilename, char* pSrchFilename, SEARCH_INDEX* pSrchIndex );


//===============================================================================
// Master
//===============================================================================
extern short btMasterOpen(char *filename, char *subfilename, char *addfilename, MASTER_INDEX *mstindex);
extern short btMasterClose(short mstno);
extern long btMasterGetFirst(short mstno, MASTER_SEARCH *mstsearch);
extern long btMasterGetNext(short mstno);
extern short btMasterRead(short mstno, long recno, char incdeldata, char *readdata);
extern short btMasterModify(short mstno, long recno, char *newdata);
extern short btMasterDelete(short mstno, long recno, short delattr);
extern long btMasterAdd(short mstno, char *newdata);
extern long btMasterRestore(short mstno, char *filename);
extern long btMasterRecordStatus(short mstno, long recno);
extern long btMasterGetCount(short mstno, char incdeldata);


//===============================================================================
// RF device API
//===============================================================================
extern long btRfOpen ( void );
extern void btRfClose ( void );
extern long	btRfGetConfig (ST_RF_CONFIGSET* pStRfConfigSet);
extern long	btRfSetConfig (ST_RF_CONFIGSET* pStRfConfigSet);
extern long	btRfPing (long *taraget_ip);
extern long	btRfGetConnectionInfo (ST_RF_CONNECTIONINFO *pStRfConnectionInfo);
extern long	btRfSntpRequest (long *taraget_ip);


//===============================================================================
// RF communication settings
//===============================================================================
extern long btRfGetIpInfo ( RF_STATUS *rf_stat );
extern long btRfSetIpInfo ( RF_STATUS *rf_stat );
extern long btRfGetHostIpInfo ( HOST_STATUS *host_stat );
extern long btRfSetHostIpInfo ( HOST_STATUS *host_stat );
extern long	btRfGetSecurityInfo (ST_RF_SECURITYINFOSET *pStRfSecurInfoSet, ST_RF_SECURITYINFOSET *pStRfSecurInfoSet_Adohoc);
extern long	btRfSetSecurityInfo (ST_RF_SECURITYINFOSET *pStRfSecurInfoSet, ST_RF_SECURITYINFOSET *pStRfSecurInfoSet_Adohoc);
extern long btRfGetSecurityMode ( long* wpa_mode, long* cipher);
extern long btRfSetSecurityMode ( long wpa_mode, long cipher);
extern long btRfSetSecurityAuth (ST_RF_SECURITYAUTHSET *pStRfSecurAuthSet);


//===============================================================================
// File communication
//===============================================================================
extern long btCommSetDev ( long CommDeviceId );
extern long btCommGetDev ( void );
extern long btCommListen ( long timeout, long cancelKey, char* filename );
extern long btCommConnect ( long timeout, long cancelKey );
extern long btCommDisconnect ( long timeout, long cancelKey );
extern long btCommPutFile (const char *pLocalFile, const char *pRemoteFile,  long timeout, long cancelKey);
extern long btCommGetFile (const char *pRemoteFile, const char *pLocalFile,  long timeout, long cancelKey);
extern long btCommPutLogFiles (long dwTimeOut, long cancelKey);
extern long btCommRemoveFile( const char *pstrFileName, long timeout, long cancelKey );
extern long btCommRenameFile( const char *pstrOrgFileName, const char *pstrNewFileName, long timeout, long cancelKey );
extern long btCommPutFileCompress( const char *pLocalFile, const char *pRemoteFile, long timeout, long cancelKey );
extern long btCommGetFileCompress( const char *pRemoteFile, const char *pLocalFile, long timeout, long cancelKey );
extern long btCommSystemUpdate( const char *pSystemProgram, long drive_no, const char *pApplicationFile, long timeout, long cancelKey );
extern long btCommSystemUpdateCheck(const char *pSystemProgram, long timeout, long cancelKey);
extern long btCommQuQuery( const COMM_APP_INFO* myInfo, COMM_APP_INFO* setInfo, long timeout, long cancelKey );

extern long btCommCompressFile(const char *pRemoteFile, const char *pCompressFile, long overwrite, long timeout, long cancelKey);
extern long btCommUnCompressFile(const char *pCompressFile, char *pOutFileName, long overwrite, long timeout, long cancelKey);


// BT-500 compatibility
extern long btIrdaListen (long timeout, long cancelKey);
extern long btIrdaConnect (long timeout, long cancelKey);
extern long btIrdaDisconnect (long timeout, long cancelKey);
extern long btIrdaPutFile (const char *pLocalFile, const char *pRemoteFile,  long timeout, long cancelKey);
extern long btIrdaGetFile (const char *pRemoteFile, const char *pLocalFile,  long timeout, long cancelKey);
extern long btIrdaPutLogFiles (short driveno, long timeout, long cancelKey);

//===============================================================================
// File communication (Zengin TCP/IP procedure)
//===============================================================================
extern short btZgnTrans( ST_ZGNCONFIG *pStZgnconfig, ST_ZGNFCONFIG *pStZgnFConfig, unsigned long cancelkey, char* resultfilename );

//===============================================================================
// ñ≥ê¸ê⁄ë±
//===============================================================================
extern long btWirelessOpen(short target, short asyncmode);
extern short btWirelessIsOpen(void);
extern void btWirelessClose(void);
extern long btWirelessGetParameter(short target, short parameter, void* value);
extern long btWirelessSetParameter(short target, short parameter, void* value);

//===============================================================================
// Network connection
//===============================================================================
extern long btNetworkOpen(short target, short asyncmode);
extern long btNetworkIsOpen(short target);
extern long btNetworkClose(short target);
extern long btNetworkGetParameter(short target, short parameter, void* value);
extern long btNetworkSetParameter(short target, short parameter, void* value);

//===============================================================================
// Network utilities
//===============================================================================
extern long btPing(unsigned long target_ip,short timeout);
extern long btSntpRequest(unsigned long target_ip,short timeout);
extern long btChannelScan( T_APINFO *papinfo, long mode );
extern long btBluetoothSearch(T_BLUETOOTHINFO* pbluetoothinfo, long cancelKey);
extern long btNIResolverRequest(unsigned long server_ip, short timeout);


//===============================================================================
// Socket communication
//===============================================================================
// Socket sharing
extern BTSOCKET btSocketOpen (int af, int type, int protocol);
extern int btSocketClose (BTSOCKET s);
extern int btSocketBind (BTSOCKET s, const BTSOCKADDR *addr, int namelen);
extern int btSocketSelect (int nfds, BTSOCKFD_SET *readfds, BTSOCKFD_SET *writefds,BTSOCKFD_SET *exceptfds, const BTSOCKTIMEVAL *timeout);
extern long btSocketGetOpt(long s, long level, long optname, char *optval, long *optlen);
extern long btSocketSetOpt(long s, long level, long optname, char *optval, long *optlen);
extern int btSocketShutdown (BTSOCKET s, int how);
// Connected API (TCP)
extern int btSocketConnect (BTSOCKET s, const BTSOCKADDR *name, int namelen);
extern int btSocketListen (BTSOCKET s, int backlog);
extern BTSOCKET btSocketAccept (BTSOCKET s, BTSOCKADDR *addr,int *addrlen);
extern int btSocketSend (BTSOCKET s, const char * buf, int len, int flags);
extern int btSocketRecv (BTSOCKET s, char * buf, int len, int flags);
// Disconnected API (UDP)
extern int btSocketSendto (BTSOCKET s, const char * buf, int len, int flags,const BTSOCKADDR *to, int tolen);
extern int btSocketRecvfrom (BTSOCKET s, char * buf, int len, int flags,BTSOCKADDR *from, int * fromlen);
// Utilities
extern long btSocketGetLastError(void);
extern int btSocketGetpeername (BTSOCKET s, BTSOCKADDR *name,int * namelen);
extern int btSocketGetsockname (BTSOCKET s, BTSOCKADDR *name,int * namelen);
extern BTSOCKHOSTENTL* btSocketGethostbyname(const char *name);
extern unsigned long btSocketInet_addr (const char * cp);
extern unsigned long btSocketHtonl (unsigned long hostlong);
extern unsigned short btSocketHtons (unsigned short hostshort);
extern unsigned long btSocketNtohl (unsigned long netlong);
extern unsigned short btSocketNtohs (unsigned short netshort);
extern long btFD_ISSET(long s, BTSOCKFD_SET* sock_fd);
extern long btFD_CLR(long s, BTSOCKFD_SET* sock_fd);
extern long btFD_SET(long s, BTSOCKFD_SET* sock_fd);
extern long btFD_ZERO(BTSOCKFD_SET* sock_fd);


//===============================================================================
// FTP communication
//===============================================================================
extern long btFtpGetOpt(ST_FTPCONFIG *pStFtpConfig);
extern long btFtpSetOpt(ST_FTPCONFIG *pStFtpConfig);
extern long btFtpServer(void);
extern long btFtpOpen(void);
extern long btFtpQuit(void);
extern long btFtpPut(char* localfilename, char* remotefilename);
extern long btFtpGet(char* remotefilename ,char* localfilename);
extern long btFtpMove(char* originalfilename,char* targetfile);
extern long btFtpRemove(char* targetfilename);
extern long btFtpPwd(char* remotepath);
extern long btFtpCwd(char* remotepath);
extern long btFtpDir(char* resultfilename);
extern long btFtpsGetOpt(ST_FTPSCONFIG *pStFtpsConfig);
extern long btFtpsSetOpt(ST_FTPSCONFIG *pStFtpsConfig);
extern long btFtpSendCommand(char* command,char* response);


//===============================================================================
// Quick update
//===============================================================================
extern long btQuNotify (char* myAppName,unsigned char* myVersion);
extern long btQuCheck (char* setAppName,char* setfilename,unsigned char* setVersion);
extern long btQuRequest (char* AppFileName);


//===============================================================================
// RF data communication (BT-900 compatibility)
//===============================================================================
extern long btRfGetRcvLevel ( void );
extern long btRfGetEssidInfo ( long ess_mode, ESS_STATUS *ess_stat );
extern long btRfGetRetryInfo ( RETRY_STATUS *retry_info );
extern long btRfSetRetryInfo ( RETRY_STATUS *retry_info );
extern long btRfGetSndMode ( void );
extern long btRfSetSndMode ( long wait );
extern long btRfPingRequest ( void );
extern long btRfSndData ( RFDATIF *rf_dat );
extern long btRfChkSndFinish ( void );
extern long btRfRcvData ( RFDATIF *rf_dat );
extern long btRfGetBroadcastMode ( void );
extern long btRfSetBroadcastMode ( long mode );
extern long btRfRcvBroadcast ( RFDATIF *rf_dat );
extern long btRfSetDateTime ( void );


//===============================================================================
// RF file communication (BT-900 compatibility)
//===============================================================================
extern long btRfFileConnect ( ST_FILEUP_STATUS *pStFileStat, long transmode, long timeout, long cancelKey );
extern long btRfFileDisconnect ( long timeout, long cancelKey );
extern long btRfPutFile ( const char *pLocalFile, const char *pRemoteFile, long cancelKey );
extern long btRfPutLogFiles ( short drive,  long cancelKey );
extern long btRfListenFile ( long transmode, long timeout, long cancelKey );


//===============================================================================
// Communication (IrDA)
//===============================================================================
extern long btIrdaSetEnv (ST_COMENV *pComenv);
extern long btIrdaGetEnv (ST_COMENV *pComenv);
extern long btIrdaOpen( void );
extern long btIrdaIsOpen( void );
extern long btIrdaClose( void );
extern long btIrdaRead (char *pBuffer, long length, long timeout, long cancelKey);
extern long btIrdaWrite (char *pBuffer, long length, long cancelKey);
extern void btIrdaClear( short buftype );
extern long btIrdaBufSense( void );


//===============================================================================
// Communication (RS-232C)
//===============================================================================
extern long btRs232cSetEnv (ST_COMENV *pComenv);
extern long btRs232cGetEnv (ST_COMENV *pComenv);
extern long btRs232cOpen( void );
extern long btRs232cIsOpen( void );
extern long btRs232cClose( void );
extern long btRs232cRead (char *pBuffer, long length, long timeout, long cancelKey);
extern long btRs232cWrite (char *pBuffer, long length, long cancelKey);
extern void btRs232cClear( short buftype );
extern long btRs232cBufSense( void );


//===============================================================================
// Communication (IrDA, RS-232C, Bluetooth)
//===============================================================================
extern long btSerialcommSetEnv( long SerialDevice, ST_COMENV *pComenv );
extern long btSerialcommGetEnv( long SerialDevice, ST_COMENV *pComenv );
extern long btSerialcommOpen( long SerialDevice );
extern long btSerialcommIsOpen( long SerialDevice );
extern long btSerialcommClose( long SerialDevice );
extern long btSerialcommRead( long SerialDevice, char *pBuffer, long length, long timeout, long cancelKey );
extern long btSerialcommWrite( long SerialDevice, char *pBuffer, long length, long cancelKey );
extern long btSerialcommClear( long SerialDevice, short buftype );
extern long btSerialcommBufSense( long SerialDevice );


//===============================================================================
// GUI (BT-3000 only)
//===============================================================================
extern short btWidgetCreate(short parentwidgethandle, short aimedid, short type);
extern short btWidgetDelete(short widgethandle);
extern short btWidgetCopy(short sourcewidgethandle, short parentwidgethandle, short aimedid);
extern short btWidgetSetSize(short widgethandle, short x, short y, short width, short height);
extern short btWidgetGetSize(short widgethandle, short *x, short *y, short *width, short *height);
extern short btWidgetSetFocus(short widgethandle);
extern short btWidgetGetFocus(void);
extern short btWidgetGetParent(short widgethandle);
extern short btWidgetSetScroll(short widgethandle, short posx, short posy);
extern short btWidgetGetScroll(short widgethandle, short* posx, short* posy);
extern short btWidgetSetParameter(short widgethandle, short parameter, void* value, long size);
extern short btWidgetGetParameter(short widgethandle, short parameter, void* value, long size);
extern short btWidgetListAdd(short widgethandle, char *pString);
extern short btWidgetListInsert(short widgethandle, short index, char*pString);
extern short btWidgetListDelete(short widgethandle, short index);
extern short btWidgetListCount(short widgethandle);
extern short btWidgetListSetSel(short widgethandle, short index);
extern short btWidgetListGetSel(short widgethandle);
extern short btWidgetListGetText(short widgethandle, short index, char *pString);
extern short btWidgetListGetTextLen(short widgethandle, short index);
extern short btWidgetListClear(short widgethandle);
extern short btWidgetDrawText(short widgethandle, char *pString, GPOSITION *pPosition, BT_GUI_FONTINFO *pFontInfo);
extern short btWidgetDrawTextEx(short widgethandle, char *pString, BT_GUI_DRAWEX* pDrawEx);
extern short btWidgetDrawLine(short widgethandle, GPOSITION *sposition, GPOSITION *eposition, unsigned long linecolor);
extern short btWidgetDrawRectangle(short widgethandle, GPOSITION *sposition, GPOSITION *eposition, unsigned long linecolor, unsigned long paintcolor);
extern short btWidgetDrawArc(short widgethandle, GPOSITION *cposition, short radius, short startangle, short endangle, unsigned long linecolor, unsigned long paintcolor);
extern short btWidgetUpdate(short widgethandle);


#ifdef __cplusplus
}
#endif


#endif // !defined(__BTEXT_H__)
