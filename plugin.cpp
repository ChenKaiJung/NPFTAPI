/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// CPlugin class implementation
//
#ifdef XP_WIN
#include <windows.h>
#include <stdio.h>
#include <windowsx.h>
#endif

#ifdef XP_MAC
#include <TextEdit.h>
#endif

#ifdef XP_UNIX
#include <string.h>
#endif

#include "Greta\regexpr2.h"
#include "plugin.h"
#include "npfunctions.h"

//kk: convinience 
#include "NPString.h"
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>


const char LAUNCHERREG[]="LAUNCHER";
const char LAUNCHERExec[]="exec";
const char LAUNCHERPath[]="path";
#define ALLOW_EXEC "(\\FTLauncher\\.exe)"
#define ALLOW_DOWNLOAD_URL "^(http://download.avaonline.com.tw|https:download.avaonline.com.tw)"
#define ALLOW_INPUT "^[A-Za-z0-9_=]+$"
#define DEFAULT_URL "http://download.avaonline.com.tw"
#ifndef SERVICE_ACCOUNT_LENGTH
	#define SERVICE_ACCOUNT_LENGTH 129
#endif
#ifndef CIPHERTEXT_LENGTH
	#define CIPHERTEXT_LENGTH 1025
#endif


void* NPN_MemAlloc(LPCTSTR psz)
{
	int size = strlen( psz );

	void * rv = NPN_MemAlloc(size);

	strcpy( (char*) rv, psz );

	return rv;
}

#ifdef XP_WIN
bool VerifyEmbeddedSignature(char *file)
{
	LONG lStatus;
    DWORD dwLastError;

	typedef LONG (WINAPI *pWinVerifyTrust)(HWND hWnd,GUID* ActionID,LPVOID ActionData); 
	pWinVerifyTrust iWinVerifyTrust=NULL;
	
    char dllpath[MAX_PATH]={0};
    GetSystemDirectory(dllpath,MAX_PATH);
    sprintf(dllpath,"%s\\%s",dllpath,"WINTRUST.DLL");
    HINSTANCE hWintrust = LoadLibrary(dllpath);	
	  
	if(hWintrust==NULL) return TRUE;
	iWinVerifyTrust = (pWinVerifyTrust)GetProcAddress(hWintrust, "WinVerifyTrust");	
		
    if(iWinVerifyTrust==NULL) return TRUE;
	WCHAR sourcefile[MAX_PATH]={0};
	
    int len=strlen(file);
	MultiByteToWideChar( CP_ACP, 0, file, -1, 
		sourcefile, len );
	
    // Initialize the WINTRUST_FILE_INFO structure.
    WINTRUST_FILE_INFO FileData;
    memset(&FileData, 0, sizeof(FileData));
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath =sourcefile;
    FileData.hFile = NULL;
    FileData.pgKnownSubject = NULL;
	
    /*
    WVTPolicyGUID specifies the policy to apply on the file
    WINTRUST_ACTION_GENERIC_VERIFY_V2 policy checks:
    
	  1) The certificate used to sign the file chains up to a root 
	  certificate located in the trusted root certificate store. This 
	  implies that the identity of the publisher has been verified by 
	  a certification authority.
	  
		2) In cases where user interface is displayed (which this example
		does not do), WinVerifyTrust will check for whether the  
		end entity certificate is stored in the trusted publisher store,  
		implying that the user trusts content from this publisher.
		
		  3) The end entity certificate has sufficient permission to sign 
		  code, as indicated by the presence of a code signing EKU or no 
		  EKU.
    */
	
    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    WINTRUST_DATA WinTrustData;
	
    // Initialize the WinVerifyTrust input data structure.
	
    // Default all fields to 0.
    memset(&WinTrustData, 0, sizeof(WinTrustData));
	
    WinTrustData.cbStruct = sizeof(WinTrustData);
    
    // Use default code signing EKU.
    WinTrustData.pPolicyCallbackData = NULL;
	
    // No data to pass to SIP.
    WinTrustData.pSIPClientData = NULL;
	
    // Disable WVT UI.
    WinTrustData.dwUIChoice = WTD_UI_NONE;
	//	WinTrustData.dwUIChoice = WTD_UI_ALL;
	
    // No revocation checking.
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE; 
	//WinTrustData.fdwRevocationChecks =WTD_REVOCATION_CHECK_NONE;
	
    // Verify an embedded signature on a file.
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
	
    // Default verification.
    WinTrustData.dwStateAction = 0;
	
    // Not applicable for default verification of embedded signature.
    WinTrustData.hWVTStateData = NULL;
	
    // Not used.
    WinTrustData.pwszURLReference = NULL;
	
    // Default.
	//    WinTrustData.dwProvFlags = WTD_SAFER_FLAG;
    WinTrustData.dwProvFlags = WTD_SAFER_FLAG;
    // This is not applicable if there is no UI because it changes 
    // the UI to accommodate running applications instead of 
    // installing applications.
    WinTrustData.dwUIChoice = 2;
	
    // Set pFile.
    WinTrustData.pFile = &FileData;
	
    lStatus = WinVerifyTrust(
        NULL,
        &WVTPolicyGUID,
        &WinTrustData);
	
	//CString sMsg;
	//CString sErrorCode;
    switch (lStatus) 
    {
	case ERROR_SUCCESS:
		break;
        
	case TRUST_E_NOSIGNATURE:
		{
            // The file was not signed or had a signature 
            // that was not valid.
			
            // Get the reason for no signature.
            dwLastError = GetLastError();
            if (TRUST_E_NOSIGNATURE == dwLastError ||
				TRUST_E_SUBJECT_FORM_UNKNOWN == dwLastError ||
				TRUST_E_PROVIDER_UNKNOWN == dwLastError) 
            {
            } 
            else 
            {
            }
		}
		return false;
	case TRUST_E_SUBJECT_NOT_TRUSTED:
		{
		}
		return false;
	case CRYPT_E_SECURITY_SETTINGS:
		{
		}
		return false;
	case TRUST_E_SUBJECT_FORM_UNKNOWN:
		{	
		}
		return false;
	default:
		return false;
    }
	
    return true;
}


#endif

static NPIdentifier sFoo_id;
static NPIdentifier sBar_id;
static NPIdentifier sDocument_id;
static NPIdentifier sBody_id;
static NPIdentifier sCreateElement_id;
static NPIdentifier sCreateTextNode_id;
static NPIdentifier sAppendChild_id;
static NPIdentifier sPluginType_id;
static NPObject *sWindowObj;

// Helper class that can be used to map calls to the NPObject hooks
// into virtual methods on instances of classes that derive from thisclass.
class ScriptablePluginObjectBase : public NPObject
{
public:
  ScriptablePluginObjectBase(NPP npp) : mNpp(npp)
  {
  }

  virtual ~ScriptablePluginObjectBase()
  {
  }

  // Virtual NPObject hooks called through this base class. Override as you see fit.
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);
  virtual bool Enumerate(NPIdentifier **identifier, uint32_t *count);
  virtual bool Construct(const NPVariant *args, uint32_t argCount, NPVariant *result);

public:
  static void _Deallocate(NPObject *npobj);
  static void _Invalidate(NPObject *npobj);
  static bool _HasMethod(NPObject *npobj, NPIdentifier name);
  static bool _Invoke(NPObject *npobj, NPIdentifier name,
                      const NPVariant *args, uint32_t argCount,
                      NPVariant *result);
  static bool _InvokeDefault(NPObject *npobj, const NPVariant *args,
                             uint32_t argCount, NPVariant *result);
  static bool _HasProperty(NPObject * npobj, NPIdentifier name);
  static bool _GetProperty(NPObject *npobj, NPIdentifier name,
                           NPVariant *result);
  static bool _SetProperty(NPObject *npobj, NPIdentifier name,
                           const NPVariant *value);
  static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);
  static bool _Enumerate(NPObject *npobj, NPIdentifier **identifier,
                         uint32_t *count);
  static bool _Construct(NPObject *npobj, const NPVariant *args,
                         uint32_t argCount, NPVariant *result);

protected:
  NPP mNpp;
};

#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
  NP_CLASS_STRUCT_VERSION_CTOR,                                               \
  ctor,                                                                       \
  ScriptablePluginObjectBase::_Deallocate,                                    \
  ScriptablePluginObjectBase::_Invalidate,                                    \
  ScriptablePluginObjectBase::_HasMethod,                                     \
  ScriptablePluginObjectBase::_Invoke,                                        \
  ScriptablePluginObjectBase::_InvokeDefault,                                 \
  ScriptablePluginObjectBase::_HasProperty,                                   \
  ScriptablePluginObjectBase::_GetProperty,                                   \
  ScriptablePluginObjectBase::_SetProperty,                                   \
  ScriptablePluginObjectBase::_RemoveProperty,                                \
  ScriptablePluginObjectBase::_Enumerate,                                     \
  ScriptablePluginObjectBase::_Construct                                      \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

void
ScriptablePluginObjectBase::Invalidate()
{
}

bool
ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
                                   uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
                                          uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
  if (name == sBar_id) {
    printf ("bar set\n");

    return true;
  }

	char *pProp = NPN_UTF8FromIdentifier(name);

  return false;
}

bool
ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::Enumerate(NPIdentifier **identifier,
                                      uint32_t *count)
{
  return false;
}

bool
ScriptablePluginObjectBase::Construct(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  return false;
}

// static
void
ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
  // Call the virtual destructor.
  delete (ScriptablePluginObjectBase *)npobj;
}

// static
void
ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
  ((ScriptablePluginObjectBase *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
                                    const NPVariant *args, uint32_t argCount,
                                    NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->Invoke(name, args, argCount,
                                                       result);
}

// static
bool
ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
                                           const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->InvokeDefault(args, argCount,
                                                              result);
}

// static
bool
ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
                                         NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *value)
{
  return ((ScriptablePluginObjectBase *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->RemoveProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_Enumerate(NPObject *npobj,
                                       NPIdentifier **identifier,
                                       uint32_t *count)
{
  return ((ScriptablePluginObjectBase *)npobj)->Enumerate(identifier, count);
}

// static
bool
ScriptablePluginObjectBase::_Construct(NPObject *npobj, const NPVariant *args,
                                       uint32_t argCount, NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->Construct(args, argCount,
                                                          result);
}


class ConstructablePluginObject : public ScriptablePluginObjectBase
{
public:
  ConstructablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
  }

  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result);
};

static NPObject *
AllocateConstructablePluginObject(NPP npp, NPClass *aClass)
{
  return new ConstructablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ConstructablePluginObject,
                                 AllocateConstructablePluginObject);

bool
ConstructablePluginObject::Construct(const NPVariant *args, uint32_t argCount,
                                     NPVariant *result)
{
  printf("Creating new ConstructablePluginObject!\n");

  NPObject *myobj =
    NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
  if (!myobj)
    return false;

  OBJECT_TO_NPVARIANT(myobj, *result);

  return true;
}

using namespace std;
using namespace regex;

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    //: ScriptablePluginObjectBase(npp), m_pszName(0), m_hWnd(0)
	: ScriptablePluginObjectBase(npp), m_hWnd(0)
  {
		//memset( m_szTextGui, 0, sizeof(m_szTextGui) );

		//m_pszName = new char[200];
		//memset( m_pszName, 0, 200 );

		//DWORD dw = 200;
		//::GetUserName( m_pszName, &dw );
	  m_OnOk = NULL;
	  m_OnError = NULL;
	  m_LauncherExec = new char[MAX_PATH];
	  m_LauncherPath = new char[MAX_PATH];
	  m_GameExec = new char[MAX_PATH];
	  m_GamePath = new char[MAX_PATH];
  }

  ~ScriptablePluginObject();

  virtual bool HasMethod(NPIdentifier name);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
	virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);

//	char m_szTextGui[200];
	HWND m_hWnd;
private:
	bool GetRegInfo();
	//kk some implementation
//	char *m_pszName;
	char *m_OnOk;
	char *m_OnError;
	char *m_LauncherPath;
	char *m_LauncherExec;
	char *m_GameExec;
	char *m_GamePath;
	int m_ErrorCode;
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

ScriptablePluginObject::~ScriptablePluginObject()
{
	  if(m_OnOk != NULL) delete m_OnOk;
	  if(m_OnError != NULL) delete m_OnError;
	  if(m_LauncherExec != NULL) delete m_LauncherExec;
	  if(m_LauncherPath != NULL) delete m_LauncherPath;
	  if(m_GameExec != NULL) delete m_GameExec;
	  if(m_GamePath != NULL)  delete m_GamePath;

}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);

bool
ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	//kk
  //return name == sFoo_id;
	char *pFunc = NPN_UTF8FromIdentifier(name);
	//is function implemented
	if(strcmp( "Launch", pFunc ) == 0)
	{
		return true;
	}
	if(strcmp( "AddOkEvent", pFunc ) == 0)
	{
		return true;
	}
	if(strcmp( "AddErrorEvent", pFunc ) == 0)
	{
		return true;
	}
	return false;
}

bool
ScriptablePluginObject::HasProperty(NPIdentifier name)
{
	//kk
  /*return (name == sBar_id ||
          name == sPluginType_id);*/
	char *pProp = NPN_UTF8FromIdentifier(name);
	//Check which Properties are available
	if( !strcmp( "Version", pProp ) )
	{
		return true;
	}
	
	if( !strcmp( "ErrorCode", pProp ) )
	{
		return true;
	}

	return false;
}

bool
ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
  VOID_TO_NPVARIANT(*result);

	//kk
  /*if (name == sBar_id) {
    static int a = 17;

    INT32_TO_NPVARIANT(a, *result);

    a += 5;

    return true;
  }*/
	char *pProp = NPN_UTF8FromIdentifier(name);
			
	if( strcmp( "Version", pProp ) ==0 )
	{
		char *p = (char*) NPN_MemAlloc( "Version 1.0" );//allocating Memory for the string with invocation of Browser-API
		STRINGZ_TO_NPVARIANT( p, *result);
		return true;
	}

	if( strcmp( "ErrorCode", pProp ) ==0 )
	{
		INT32_TO_NPVARIANT(m_ErrorCode, *result);
		return true;
	}


//	if( !strcmp( "Name", pProp ) )
//	{
//		char *p = (char*) NPN_MemAlloc( m_pszName );//allocating Memory for the string with invocation of Browser-API
//		STRINGZ_TO_NPVARIANT( p, *result);
//		return true;
//	}

  if (name == sPluginType_id) {
    NPObject *myobj =
      NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
    if (!myobj) {
      return false;
    }

    OBJECT_TO_NPVARIANT(myobj, *result);

    return true;	
  }

  return true;
}
//kk: implemented
bool
ScriptablePluginObject::SetProperty(NPIdentifier name,
                                        const NPVariant *value)
{
	//kk
	char *pProp = NPN_UTF8FromIdentifier(name);

//	if( !strcmp( "Name", pProp ) )
//	{
//		if( m_pszName ) delete m_pszName;
//		m_pszName = new char[ value->value.stringValue.UTF8Length + 1 ];//allocating Memory for the string with local memory
//		memset( m_pszName, 0, value->value.stringValue.UTF8Length + 1 );
//		strncpy( m_pszName, value->value.stringValue.UTF8Characters, value->value.stringValue.UTF8Length );//copy data

//		sprintf( m_szTextGui, "Name set. Value = %s", m_pszName );
//		return true;
//	}

  return false;
}

bool
ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
	//kk	
	char *pFunc = NPN_UTF8FromIdentifier(name);
	
	if( strcmp( "Launch", pFunc ) == 0)
	{
		char *ServiceAccount=NULL;
		char *Cipher=NULL;
		m_ErrorCode = 115;
		INT32_TO_NPVARIANT( m_ErrorCode,*result);

		if( args[0].type != NPVariantType_String )
		{
			m_ErrorCode = 114;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}
		ServiceAccount = (char *)args[0].value.stringValue.UTF8Characters;

		if( args[1].type != NPVariantType_String )
		{
			m_ErrorCode = 114;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}
		Cipher = (char *)args[1].value.stringValue.UTF8Characters;

		bool retval= GetRegInfo();
		if(retval==false) 
		{
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}

		retval=VerifyEmbeddedSignature((char *)(LPCTSTR)m_LauncherExec);
		if(retval==false)  
		{
			m_ErrorCode=510;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);

			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}

		match_results results;
		rpattern pat(ALLOW_EXEC);  
		rpattern pat_i(ALLOW_INPUT);
		// Match a dollar sign followed by one or more digits,
		// optionally followed by a period and two more digits.
		// The double-escapes are necessary to satisfy the compiler.
		string fromatch = (char *)(LPCTSTR)m_LauncherExec;
		match_results::backref_type br = pat.match( fromatch, results );
		if( ! br.matched ) 
		{
			m_ErrorCode=511;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}
		fromatch = (char *)ServiceAccount;
		br = pat_i.match( fromatch, results );
		if( ! br.matched ) 
		{
			m_ErrorCode=101;
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}

		fromatch = (char *)Cipher;		
		
		br = pat_i.match( fromatch, results );
		if( ! br.matched ) 
		{
			m_ErrorCode=114;
			if(m_OnError!=NULL) NPN_GetURL(mNpp, m_OnError, NULL);
			return false;
		}

		char path[MAX_PATH];
		SetCurrentDirectory((LPCTSTR)m_LauncherPath);
		sprintf(path,"%s -ServiceAccount=%s -Cipher=%s",m_LauncherExec,ServiceAccount,Cipher);
		WinExec( path, SW_SHOW);
	
		m_ErrorCode = 100;
		INT32_TO_NPVARIANT( m_ErrorCode,*result);
		
		if(m_OnOk!=NULL) NPN_GetURL(mNpp, m_OnOk, NULL);
		return true;
	}

	if( strcmp( "AddOkEvent", pFunc ) == 0)
	{
		if( args[0].type != NPVariantType_String )
		{
			m_ErrorCode = 114;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			return false;
		}
		int length= args[0].value.stringValue.UTF8Length;
		m_OnOk = new char[ length + 1 + 30];//allocating Memory for the string with local memory
		memset( m_OnOk, 0, length + 1 + 30);
		//strncpy( m_OnOk, args[0].value.stringValue.UTF8Characters, args[0].value.stringValue.UTF8Length );//copy data
		sprintf(m_OnOk,"javascript:%s()",args[0].value.stringValue.UTF8Characters);
		return true;
	}
	
	if( strcmp( "AddErrorEvent", pFunc ) == 0)
	{	
		if( args[0].type != NPVariantType_String ) 
		{
			m_ErrorCode = 114;
			INT32_TO_NPVARIANT( m_ErrorCode,*result);
			return false;
		}
		int length= args[0].value.stringValue.UTF8Length;
		m_OnError = new char[ length + 1 + 30];//allocating Memory for the string with local memory
		memset( m_OnError, 0, length + 1 + 30);
		//strncpy( m_OnError, args[0].value.stringValue.UTF8Characters, args[0].value.stringValue.UTF8Length );//copy data
		sprintf(m_OnError,"javascript:%s()",args[0].value.stringValue.UTF8Characters);
		return true;
	}

	if(m_OnOk!=NULL) NPN_GetURL(mNpp, m_OnOk, NULL);
	return false;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  printf ("ScriptablePluginObject default method called!\n");

  STRINGZ_TO_NPVARIANT(strdup("default method return val"), *result);

  return true;
}

bool ScriptablePluginObject::GetRegInfo()
{
	// TODO: Add your dispatch handler code here
	HKEY hKey;
	BYTE szBuf[256];
	DWORD lpType, lpDataSize;
	
	lpType = REG_SZ;
	lpDataSize = 256;
	char RegKeyNew[MAX_PATH]={0};
	char RegGame[MAX_PATH]={0};
	char RegGameExec[MAX_PATH]={0};
	char RegGamePath[MAX_PATH]={0};
	char ErrorMsg[MAX_PATH]={0};	
	

	sprintf(RegGame,"%s\\",LAUNCHERREG);
	sprintf(RegGameExec,"%s",LAUNCHERExec);
	sprintf(RegGamePath,"%s",LAUNCHERPath);	
	sprintf(RegKeyNew,"%s%s","SOFTWARE\\Funtown\\",RegGame);
	
	//0x0100 for Windows 64bit
	int retval=::RegOpenKeyEx(HKEY_LOCAL_MACHINE, RegKeyNew, 0, KEY_READ | 0x0100, &hKey);
	if ( retval != ERROR_SUCCESS)
	{
		::RegCloseKey(hKey);
		m_ErrorCode=506;
		return false;
	}
	
	memset(&szBuf,0,sizeof(szBuf));
	lpDataSize = 256;	
	retval=::RegQueryValueEx(hKey, RegGameExec, 0, &lpType, szBuf, &lpDataSize);
	if (retval != ERROR_SUCCESS)	
	{
		::RegCloseKey(hKey);
		m_ErrorCode=507;
		return false;
	}
	sprintf(m_LauncherExec,"%s", szBuf);
	
	memset(&szBuf,0,sizeof(szBuf));
	lpDataSize = 256;
	
	retval=::RegQueryValueEx(hKey, RegGamePath, 0, &lpType, szBuf, &lpDataSize);	
	if (retval != ERROR_SUCCESS)
	{
		::RegCloseKey(hKey);
		m_ErrorCode=507;
		return false;
	}
	sprintf(m_LauncherPath,"%s", szBuf);

	char *cmp=m_LauncherExec+(strlen(m_LauncherExec)-4);
	if(strcmp(cmp,".exe")!=0)
	{
		strcat(m_LauncherExec,".exe");
	}
	
	FILE *fd = NULL;
	if((fd=fopen(m_LauncherExec,"rb"))==NULL)
	{
		m_ErrorCode=508;
		return false;
	}


	fclose(fd);
	return true;
}



CPlugin::CPlugin(NPP pNPInstance) :
  m_pNPInstance(pNPInstance),
  m_pNPStream(NULL),
  m_bInitialized(false),
  m_pScriptableObject(NULL)
{
#ifdef XP_WIN
  m_hWnd = NULL;
#endif

  NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &sWindowObj);

  NPIdentifier n = NPN_GetStringIdentifier("foof");

  sFoo_id = NPN_GetStringIdentifier("foo");
  sBar_id = NPN_GetStringIdentifier("bar");
  sDocument_id = NPN_GetStringIdentifier("document");
  sBody_id = NPN_GetStringIdentifier("body");
  sCreateElement_id = NPN_GetStringIdentifier("createElement");
  sCreateTextNode_id = NPN_GetStringIdentifier("createTextNode");
  sAppendChild_id = NPN_GetStringIdentifier("appendChild");
  sPluginType_id = NPN_GetStringIdentifier("PluginType");

  NPVariant v;
  INT32_TO_NPVARIANT(46, v);

  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  NPVariant rval;
  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  if (NPVARIANT_IS_INT32(rval)) {
    printf("rval = %d\n", NPVARIANT_TO_INT32(rval));
  }

  n = NPN_GetStringIdentifier("document");

  if (!NPN_IdentifierIsString(n)) {
    NPString str;
    str.UTF8Characters = "alert('NPN_IdentifierIsString() test failed!');";
    str.UTF8Length = strlen(str.UTF8Characters);

    NPN_Evaluate(m_pNPInstance, sWindowObj, &str, NULL);
  }

  NPObject *doc;

  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  if (NPVARIANT_IS_OBJECT(rval) && (doc = NPVARIANT_TO_OBJECT(rval))) {
    n = NPN_GetStringIdentifier("title");

    NPN_GetProperty(m_pNPInstance, doc, n, &rval);

    if (NPVARIANT_IS_STRING(rval)) {
      printf ("title = %s\n", NPVARIANT_TO_STRING(rval).UTF8Characters);

      NPN_ReleaseVariantValue(&rval);
    }

    n = NPN_GetStringIdentifier("plugindoc");

    OBJECT_TO_NPVARIANT(doc, v);
    NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

    NPString str;
    str.UTF8Characters = "document.getElementById('result').innerHTML += '<p>' + 'NPN_Evaluate() test, document = ' + this + '</p>';";
    str.UTF8Length = strlen(str.UTF8Characters);

    NPN_Evaluate(m_pNPInstance, doc, &str, NULL);

		NPN_ReleaseObject(doc);
  }

  NPVariant barval;
  NPN_GetProperty(m_pNPInstance, sWindowObj, sBar_id, &barval);

  NPVariant arg;
  OBJECT_TO_NPVARIANT(sWindowObj, arg);

  NPN_InvokeDefault(m_pNPInstance, NPVARIANT_TO_OBJECT(barval), &arg, 1,
                    &rval);

  if (NPVARIANT_IS_INT32(rval) && NPVARIANT_TO_INT32(rval) == 4) {
    printf ("Default function call SUCCEEDED!\n");
  } else {
    printf ("Default function call FAILED!\n");
  }

  NPN_ReleaseVariantValue(&barval);
  NPN_ReleaseVariantValue(&rval);


#if 0
  n = NPN_GetStringIdentifier("prompt");

  NPVariant vars[3];
  STRINGZ_TO_NPVARIANT("foo", vars[0]);
  STRINGZ_TO_NPVARIANT("bar", vars[1]);
  STRINGZ_TO_NPVARIANT("foof", vars[2]);

  NPN_Invoke(sWindowObj, n, vars, 3, &rval);

  if (NPVARIANT_IS_STRING(rval)) {
    printf ("prompt returned '%s'\n", NPVARIANT_TO_STRING(rval).UTF8Characters);
  }

  NPN_ReleaseVariantValue(&rval);
#endif

  NPObject *myobj =
    NPN_CreateObject(m_pNPInstance,
                     GET_NPOBJECT_CLASS(ScriptablePluginObject));

  n = NPN_GetStringIdentifier("pluginobj");

  OBJECT_TO_NPVARIANT(myobj, v);
  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

  printf ("Object set/get test ");

  if (NPVARIANT_IS_OBJECT(rval) && NPVARIANT_TO_OBJECT(rval) == myobj) {
    printf ("succeeded!\n");
  } else {
    printf ("FAILED!\n");
  }

  NPN_ReleaseVariantValue(&rval);
  NPN_ReleaseObject(myobj);

  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);
}

CPlugin::~CPlugin()
{
  if (sWindowObj)
    NPN_ReleaseObject(sWindowObj);
  if (m_pScriptableObject)
    NPN_ReleaseObject(m_pScriptableObject);

  sWindowObj = 0;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif

NPBool CPlugin::init(NPWindow* pNPWindow)
{
  if(pNPWindow == NULL)
    return false;

#ifdef XP_WIN
  m_hWnd = (HWND)pNPWindow->window;
  if(m_hWnd == NULL)
    return false;

  // subclass window so we can intercept window messages and
  // do our drawing to it
  lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);

  // associate window with our CPlugin object so we can access 
  // it in the window procedure
  SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
#endif

  m_Window = pNPWindow;

  m_bInitialized = true;
  return true;
}

void CPlugin::shut()
{
#ifdef XP_WIN
  // subclass it back
  SubclassWindow(m_hWnd, lpOldProc);
  m_hWnd = NULL;
#endif

  m_bInitialized = false;
}

NPBool CPlugin::isInitialized()
{
  return m_bInitialized;
}

int16_t CPlugin::handleEvent(void* event)
{
#ifdef XP_MAC
  NPEvent* ev = (NPEvent*)event;
  if (m_Window) {
    Rect box = { m_Window->y, m_Window->x,
                 m_Window->y + m_Window->height, m_Window->x + m_Window->width };
    if (ev->what == updateEvt) {
      ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
    }
  }
#endif
  return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif

  if (m_Window) {
    NPRect r =
      {
        (uint16_t)m_Window->y, (uint16_t)m_Window->x,
        (uint16_t)(m_Window->y + m_Window->height),
        (uint16_t)(m_Window->x + m_Window->width)
      };

    NPN_InvalidateRect(m_pNPInstance, &r);
  }
}

// this will clean the plugin window
void CPlugin::clear()
{
  strcpy(m_String, "");

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif
}

void CPlugin::getVersion(char* *aVersion)
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  char*& version = *aVersion;
  version = (char*)NPN_MemAlloc(1 + strlen(ua));
  if (version)
    strcpy(version, ua);
}

NPObject *
CPlugin::GetScriptableObject()
{
  if (!m_pScriptableObject) {
    m_pScriptableObject =
      NPN_CreateObject(m_pNPInstance,
                       GET_NPOBJECT_CLASS(ScriptablePluginObject));
  }

  if (m_pScriptableObject) {
    NPN_RetainObject(m_pScriptableObject);
  }
	//kk for invalidating the output
	 ( (ScriptablePluginObject*) m_pScriptableObject )->m_hWnd = m_hWnd;

  return m_pScriptableObject;
}

char*
CPlugin::GetGuiText()
{
//	if( m_pScriptableObject->_class == GET_NPOBJECT_CLASS(ScriptablePluginObject))
//	{
//		char* p = ( (ScriptablePluginObject*) m_pScriptableObject )->m_szTextGui;

//		if( *p ) return p;
//	}

	return m_String;
}



#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 /*
	switch (msg) {
    case WM_PAINT:
      {
        // draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
				FillRect( hdc, &rc, (HBRUSH) (COLOR_WINDOW+1));
        FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));
        CPlugin * p = (CPlugin*) GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if(p) {
					//kk: some fancy GUI stuff
          //if (p->m_String[0] == 0) {
          //  strcpy("foo", p->m_String);
          //}
					char *s = p->GetGuiText();

          DrawText(hdc, s, strlen(s), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
      }
      break;
    default:
      break;
  }
 */
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
