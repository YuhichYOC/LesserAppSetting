#pragma once

#include "stdafx.h"

class WCharString
{
private:

    string value;

    void Assign(char * arg);

    void Assign(wchar_t * arg);

    void Assign(string arg);

    void Assign(string * arg);

    void Assign(const char * arg);

    void Assign(const wchar_t * arg);

    void Assign(const string * arg);

public:

    WCharString Append(char * arg);

    WCharString Append(wchar_t * arg);

    WCharString Append(string arg);

    WCharString Append(string * arg);

    WCharString Append(const char * arg);

    WCharString Append(const wchar_t * arg);

    WCharString Append(const string * arg);

    WCharString Value(char * arg);

    WCharString Value(wchar_t * arg);

    WCharString Value(string arg);

    WCharString Value(string * arg);

    WCharString Value(const char * arg);

    WCharString Value(const wchar_t * arg);

    WCharString Value(const string * arg);

    unique_ptr<wchar_t> ToWChar();

    string ToString();

    bool WChar_tStartsWith(wchar_t * arg1eval, string arg2test);

    string SysErrMessage();

    WCharString();

    ~WCharString();
};
