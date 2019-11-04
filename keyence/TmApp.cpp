// TmApp.cpp : This file defines the entry point of the DLL application.
//
#include <windows.h>

//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// Structure

//////////////////////////////////////////////////////
// Function declarations

//////////////////////////////////////////////////////
// Variable declarations

// DLL module handle
HMODULE	ghThisModule = NULL;


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//	@function DllMain
//	@brief DLL main processing
//	@param
//		[in] HANDLE hModule
//		[in] DWORD ul_reason_for_call
//		[in] LPVOID lpReserved
//	@return BOOL
//	@author Y.YURA
//------------------------------------------------------------------------------
BOOL APIENTRY DllMain( HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved ) {

	if ( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
		// Save module handle when initializing the DLL.
		ghThisModule = (HMODULE)hModule;
	}
    return TRUE;
}




