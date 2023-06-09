// ***********************************************************************
// Code Created by James Michael Armstrong (https://github.com/BlazesRus)
// Latest Code Release at https://github.com/BlazesRus/BlazesRusSharedCode
// ***********************************************************************
#pragma once
#ifndef IndexedPDictionary_IncludeGuard
#define IndexedPDictionary_IncludeGuard

#ifdef BlazesSharedCode_LocalLayout
#ifndef DLL_API
#ifdef UsingBlazesSharedCodeDLL
#define DLL_API __declspec(dllimport)
#elif defined(BLAZESSharedCode_LIBRARY)
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif
#endif
#else
#include "..\DLLAPI.h"
#endif

#include "OrderedPointerDictionary.h"
#include "PointerDictionary.h"

/// <summary>
/// Class named IndexedPDictionary.
/// Implements the <see cref="PointerDictionary{unsigned int, ValueType}" />
/// </summary>
/// <seealso cref="PointerDictionary{unsigned int, ValueType}" />
template <typename ValueType>
class DLL_API IndexedPDictionary:public PointerDictionary<unsigned int, ValueType>
{
private:
    /// <summary>
    /// The next index
    /// </summary>
    unsigned int NextIndex = 0;
    //bool ReverseIndexSearch = true;//Later might add code to force to search indexes in reverse by default if this is set
public:

    /// <summary>
    /// Use insert if doesn't Already exist, otherwise set the value
    /// </summary>
    /// <param name="Value">The value.</param>
    void Add(ValueType Value)
    {
        //if(ReverseIndexSearch){}else{}
        bool Success = AddOnlyNew(NextIndex,Value);
        if(Success){NextIndex++;return;}
        int LastIndexSlot = NextIndex;//Check higher indexes first so save information of last index to check in reverse later
        for(Success=false;!Success||NextIndex==4294967295;Success=AddOnlyNew(NextIndex,Value))
        {
            NextIndex++;
        }
        if(Success){NextIndex++;return;}
        Success = AddOnlyNew(4294967295,Value);
        //ReverseIndexSearch = true;
        if(Success){return;}
        NextIndex = LastIndexSlot;
        for (Success = false; !Success || NextIndex == 0; Success = AddOnlyNew(NextIndex, Value))//Search in reverse for slots now
        {
            NextIndex--;
        }
    }
    /// <summary>
    /// Removes the specified key.
    /// </summary>
    /// <param name="Key">The key.</param>
    void Remove(unsigned int Key)
    {
        NextIndex = Key;
    }
    /// <summary>
    /// Clears this instance.
    /// </summary>
    void clear()
    {
        NextIndex = 0;//Free up all index slots usage as well
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="IndexedPDictionary"/> class.
    /// </summary>
    IndexedPDictionary() {}
    /// <summary>
    /// Finalizes an instance of the <see cref="IndexedPDictionary"/> class.
    /// </summary>
    ~IndexedPDictionary() {}
};

/// <summary>
/// Class named IndexedOrderedPDictionary.
/// Implements the <see cref="OrderedPointerDictionary{unsigned int, ValueType}" />
/// </summary>
/// <seealso cref="OrderedPointerDictionary{unsigned int, ValueType}" />
template <typename ValueType>
class DLL_API IndexedOrderedPDictionary : public OrderedPointerDictionary<unsigned int, ValueType>
{
private:
    /// <summary>
    /// The next index
    /// </summary>
    unsigned int NextIndex = 0;
public:

    /// <summary>
    /// Use insert if doesn't Already exist, otherwise set the value
    /// </summary>
    /// <param name="Value">The value.</param>
    void Add(ValueType Value)
    {
        //if(ReverseIndexSearch){}else{}
        bool Success = AddOnlyNew(NextIndex,Value);
        if(Success){NextIndex++;return;}
        int LastIndexSlot = NextIndex;//Check higher indexes first so save information of last index to check in reverse later
        for(Success=false;!Success||NextIndex==4294967295;Success=AddOnlyNew(NextIndex,Value))
        {
            NextIndex++;
        }
        if(Success){NextIndex++;return;}
        Success = AddOnlyNew(4294967295,Value);
        //ReverseIndexSearch = true;
        if(Success){return;}
        NextIndex = LastIndexSlot;
        for (Success = false; !Success || NextIndex == 0; Success = AddOnlyNew(NextIndex, Value))//Search in reverse for slots now
        {
            NextIndex--;
        }
    }
    /// <summary>
    /// Removes the specified key.
    /// </summary>
    /// <param name="Key">The key.</param>
    void Remove(unsigned int Key)
    {
        NextIndex = Key;
    }
    /// <summary>
    /// Clears this instance.
    /// </summary>
    void clear()
    {
        NextIndex = 0;//Free up all index slots usage as well
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="IndexedOrderedPDictionary"/> class.
    /// </summary>
    IndexedOrderedPDictionary() {}
    /// <summary>
    /// Finalizes an instance of the <see cref="IndexedOrderedPDictionary"/> class.
    /// </summary>
    ~IndexedOrderedPDictionary() {}
};

/// <summary>
/// Class named IndexedLongPDictionary.
/// Implements the <see cref="PointerDictionary{unsigned _int64, ValueType}" />
/// </summary>
/// <seealso cref="PointerDictionary{unsigned _int64, ValueType}" />
template <typename ValueType>
class DLL_API IndexedLongPDictionary: public PointerDictionary<unsigned _int64, ValueType>
{
private:
    /// <summary>
    /// The next index
    /// </summary>
    unsigned _int64 NextIndex = 0;
public:

    /// <summary>
    /// Use insert if doesn't Already exist, otherwise set the value
    /// </summary>
    /// <param name="Value">The value.</param>
    void Add(ValueType Value)
    {
        bool Success = AddOnlyNew(NextIndex,Value);
        if(Success){NextIndex++;return;}
        int LastIndexSlot = NextIndex;//Check higher indexes first so save information of last index to check in reverse later
        for(Success=false;!Success||NextIndex==18446744073709551615;Success=AddOnlyNew(NextIndex,Value))
        {
            NextIndex++;
        }
        if (Success) { NextIndex++; return; }
        Success = AddOnlyNew(18446744073709551615,Value);//Unlikely to require getting past this point but just in case
        if (Success) { return; }
        NextIndex = LastIndexSlot;
        for (Success = false; !Success || NextIndex == 0; Success = AddOnlyNew(NextIndex, Value))//Search in reverse for slots now
        {
            NextIndex--;
        }
    }
    /// <summary>
    /// Removes the specified key.
    /// </summary>
    /// <param name="Key">The key.</param>
    void Remove(unsigned _int64 Key)
    {
        NextIndex = Key;
    }
    /// <summary>
    /// Clears this instance.
    /// </summary>
    void clear()
    {
        NextIndex = 0;//Free up all index slots usage as well
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="IndexedLongPDictionary"/> class.
    /// </summary>
    IndexedLongPDictionary() {}
    /// <summary>
    /// Finalizes an instance of the <see cref="IndexedLongPDictionary"/> class.
    /// </summary>
    ~IndexedLongPDictionary() {}
};

/// <summary>
/// Class named IndexedOrderedLDictionary.
/// Implements the <see cref="OrderedPointerDictionary{unsigned _int64, ValueType}" />
/// </summary>
/// <seealso cref="OrderedPointerDictionary{unsigned _int64, ValueType}" />
template <typename ValueType>
class DLL_API IndexedOrderedLDictionary: public OrderedPointerDictionary<unsigned _int64, ValueType>
{
private:
    /// <summary>
    /// The next index
    /// </summary>
    unsigned _int64 NextIndex = 0;
public:

    /// <summary>
    /// Use insert if doesn't Already exist, otherwise set the value
    /// </summary>
    /// <param name="Value">The value.</param>
    void Add(ValueType Value)
    {
        bool Success = AddOnlyNew(NextIndex,Value);
        if (Success) { NextIndex++; return; }
        int LastIndexSlot = NextIndex;//Check higher indexes first so save information of last index to check in reverse later
        for(Success=false;!Success||NextIndex==18446744073709551615;Success=AddOnlyNew(NextIndex,Value))
        {
            NextIndex++;
        }
        if (Success) { NextIndex++; return; }
        Success = AddOnlyNew(18446744073709551615,Value);//Unlikely to require getting past this point but just in case
        if (Success) { return; }
        NextIndex = LastIndexSlot;
        for (Success = false; !Success || NextIndex == 0; Success = AddOnlyNew(NextIndex, Value))//Search in reverse for slots now
        {
            NextIndex--;
        }
    }
    /// <summary>
    /// Removes the specified key.
    /// </summary>
    /// <param name="Key">The key.</param>
    void Remove(unsigned _int64 Key)
    {
        NextIndex = Key;
    }
    /// <summary>
    /// Clears this instance.
    /// </summary>
    void clear()
    {
        NextIndex = 0;//Free up all index slots usage as well
    }
    /// <summary>
    /// Initializes a new instance of the <see cref="IndexedOrderedLDictionary"/> class.
    /// </summary>
    IndexedOrderedLDictionary() {}
    /// <summary>
    /// Finalizes an instance of the <see cref="IndexedOrderedLDictionary"/> class.
    /// </summary>
    ~IndexedOrderedLDictionary() {}
};
#endif