#ifndef _VYSDKDefines_h_
#define _VYSDKDefines_h_


#define VY_SDK_DECL __declspec(dllexport)
#define VY_SDK_METHOD __cdecl
//#define VY_SDK_METHOD __stdcall


#ifdef __cplusplus
#define VY_SDK_API extern "C" VY_SDK_DECL
#else
	#define VY_SDK_API VY_SDK_DECL
#endif

#endif