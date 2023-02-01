#include <napi.h>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")
#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>
#include <string>
#include <vector>
#include <format>
#include <cstdlib> // для system
using namespace std;
#include <iostream>

bool checkUserExists(std::string userName)
{
    LPUSER_INFO_0 pBuf = NULL;
    LPUSER_INFO_0 pTmpBuf;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    DWORD dwResumeHandle = 0;
    DWORD i;
    DWORD dwTotalCount = 0;
    NET_API_STATUS nStatus;
    LPTSTR pszServerName = NULL;
    do
    {
        nStatus = NetUserEnum((LPCWSTR)pszServerName,
            dwLevel,
            FILTER_NORMAL_ACCOUNT,
            (LPBYTE*)&pBuf,
            dwPrefMaxLen,
            &dwEntriesRead,
            &dwTotalEntries,
            &dwResumeHandle);

        if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
        {
            if ((pTmpBuf = pBuf) != NULL)
            {
                for (i = 0; (i < dwEntriesRead); i++)
                {
                    assert(pTmpBuf != NULL);

                    if (pTmpBuf == NULL)
                    {
                        return 0;
                    }

                    wchar_t* txt = (L"\t-- %s\n", pTmpBuf->usri0_name);
                    wstring ws(txt);
                    string str(ws.begin(), ws.end());
                    if (str == userName)
                        return 1;
                    pTmpBuf++;
                    dwTotalCount++;
                }
            }
        }
        else
            return 0;
        if (pBuf != NULL)
        {
            NetApiBufferFree(pBuf);
            pBuf = NULL;
        }
    } while (nStatus == ERROR_MORE_DATA); // end do
    if (pBuf != NULL)
        NetApiBufferFree(pBuf);
    return 0;
}

Napi::Boolean checkUser(Napi::CallbackInfo &args) {
  auto env = args.Env();
  string userName = args[0].As<Napi::String>();
  return Napi::Boolean::New(env, checkUserExists(userName));
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
 exports.Set("checkUser", Napi::Function::New(env, checkUser, "checkUser"));
 return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)