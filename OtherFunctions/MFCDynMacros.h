/*============================================================================*/
// Streamlined full runtime helper macros for declaring CRuntimeClass compatible classes
//(based off https://stackoverflow.com/questions/1491971/mfc-implement-dyncreate-with-template)
//Varient of DECLARE_DYNCREATE/DECLARE_DYNAMIC macro from 
#ifndef MFCDynMacros_IncludeGuard
#define MFCDynMacros_IncludeGuard
#pragma once

#include <string>
/*
class classType: public baseClass
{
    classType(){}
    ~classType(){}
    //And other class stuff
    MFC_DynExtPart01(classType, baseClass)
    //Any message map messages here
    MFC_DynExtPart02()
    //MFC_DynExtClassName(classType)//If need classType info
};

MFC_RuntimeImplimentation(classType)
//MFC_RuntimeClassImplimentation(classType)//If need classType info
*/
#define MFC_DynExtPart01(classType, baseClass)\
private:\
    typedef baseClass TheBaseClass;\
    typedef classType ThisClass;\
protected:\
    static CRuntimeClass* PASCAL _GetBaseClass() { return TheBaseClass::GetThisClass(); } \
public:\
    static const CRuntimeClass class##classType;\
    static CRuntimeClass* PASCAL GetThisClass() { return _RUNTIME_CLASS(classType); }\
    virtual CRuntimeClass* GetRuntimeClass() const { return _RUNTIME_CLASS(classType); }\
protected:\
    static const AFX_MSGMAP* PASCAL GetThisMessageMap()\
    {\
        __pragma(warning(push))\
        __pragma(warning(disable: 4640))\
        static const AFX_MSGMAP_ENTRY _messageEntries[] =\
        {

#define MFC_DynExtPart02()\
            { 0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }\
        };\
        __pragma(warning(pop))\
            static const AFX_MSGMAP messageMap =\
        { &TheBaseClass::GetThisMessageMap, &_messageEntries[0] };\
        return &messageMap;\
    }\
public:\
    /// <summary>\
    /// Gets the message map.\
    /// </summary>\
    /// <returns>const AFX_MSGMAP *</returns>\
    virtual const AFX_MSGMAP* GetMessageMap() const\
    {\
        return GetThisMessageMap();\
    }

#define MFC_DynExtClassName(classType)\
public:\
    static std::string ClassString()\
    {\
        return "classType"; \
    }\
    static const std::string classNameStr;\
    static LPCSTR ClassName() { return classNameStr.c_str(); }

#define MFC_RuntimeClassImplimentation(classType)\
const std::string classType::classNameStr = classType::ClassString();

#define MFC_RuntimeImplimentation(classType)\
const CRuntimeClass classType::class##classType = { "classType", sizeof(classType), 0xFFFF, NULL,&classType::_GetBaseClass, NULL, NULL };

#endif
