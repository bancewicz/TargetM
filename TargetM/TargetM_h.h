

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Jun 28 12:31:01 2018
 */
/* Compiler settings for TargetM.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __TargetM_h_h__
#define __TargetM_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITargetM_FWD_DEFINED__
#define __ITargetM_FWD_DEFINED__
typedef interface ITargetM ITargetM;
#endif 	/* __ITargetM_FWD_DEFINED__ */


#ifndef __TargetM_FWD_DEFINED__
#define __TargetM_FWD_DEFINED__

#ifdef __cplusplus
typedef class TargetM TargetM;
#else
typedef struct TargetM TargetM;
#endif /* __cplusplus */

#endif 	/* __TargetM_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TargetM_LIBRARY_DEFINED__
#define __TargetM_LIBRARY_DEFINED__

/* library TargetM */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TargetM;

#ifndef __ITargetM_DISPINTERFACE_DEFINED__
#define __ITargetM_DISPINTERFACE_DEFINED__

/* dispinterface ITargetM */
/* [uuid] */ 


EXTERN_C const IID DIID_ITargetM;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5B753AAF-3787-4CE8-9B7E-702C3C22DDE7")
    ITargetM : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ITargetMVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITargetM * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITargetM * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITargetM * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITargetM * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITargetM * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITargetM * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITargetM * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITargetMVtbl;

    interface ITargetM
    {
        CONST_VTBL struct ITargetMVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITargetM_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITargetM_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITargetM_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITargetM_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITargetM_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITargetM_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITargetM_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ITargetM_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TargetM;

#ifdef __cplusplus

class DECLSPEC_UUID("AD700F4B-8CFB-4FAB-9F44-DCE4640C7B9E")
TargetM;
#endif
#endif /* __TargetM_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


