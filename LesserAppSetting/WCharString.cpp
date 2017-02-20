#include "stdafx.h"

#include "WCharString.h"

void WCharString::Assign(char * arg) {
    value.assign(arg);
}

void WCharString::Assign(wchar_t * arg) {
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.assign(mbArg.get());
}

void WCharString::Assign(string arg) {
    value.assign(arg);
}

void WCharString::Assign(string * arg) {
    value.assign(*arg);
}

void WCharString::Assign(const char * arg) {
    value.assign(arg);
}

void WCharString::Assign(const wchar_t * arg) {
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.assign(mbArg.get());
}

void WCharString::Assign(const string * arg) {
    value.assign(*arg);
}

WCharString WCharString::Append(char * arg) {
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(wchar_t * arg) {
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.append(mbArg.get());
    return *this;
}

WCharString WCharString::Append(string arg) {
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(string * arg) {
    value.append(*arg);
    return *this;
}

WCharString WCharString::Append(const char * arg) {
    value.append(arg);
    return *this;
}

WCharString WCharString::Append(const wchar_t * arg) {
    wstring castedArg = arg;
    size_t argSize = castedArg.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<char> mbArg(new char[argSize]);
    wcstombs_s(&cnvSize, mbArg.get(), argSize, arg, _TRUNCATE);
    value.append(mbArg.get());
    return *this;
}

WCharString WCharString::Append(const string * arg) {
    value.append(*arg);
    return *this;
}

WCharString WCharString::Value(char * arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(wchar_t * arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(string arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(string * arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const char * arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const wchar_t * arg) {
    Assign(arg);
    return *this;
}

WCharString WCharString::Value(const string * arg) {
    Assign(arg);
    return *this;
}

unique_ptr<wchar_t> WCharString::ToWChar() {
    size_t retSize = value.length() + 1;
    size_t cnvSize = 0;
    unique_ptr<wchar_t> ret(new wchar_t[retSize]);
    mbstowcs_s(&cnvSize, ret.get(), retSize, value.c_str(), _TRUNCATE);
    return ret;
}

string WCharString::ToString() {
    return value;
}

bool WCharString::WChar_tStartsWith(wchar_t * arg1eval, string arg2test) {
    wstring castedArg1 = arg1eval;
    if (castedArg1.length() < arg2test.length()) {
        return false;
    }
    for (size_t i = 0; i < arg2test.length(); i++) {
        if (castedArg1.at(i) != arg2test.at(i)) {
            return false;
        }
    }
    return true;
}

string WCharString::SysErrMessage() {
    LPVOID msg = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&msg,
        0,
        NULL);
    string out = this->Value((const wchar_t *)msg).ToString();
    if (out.substr(out.length() - 2, 2) == string("\r\n")) {
        out = out.substr(0, out.length() - 2);
    }
    cout << out << "\n";
    return out;
}

WCharString::WCharString() {
}

WCharString::~WCharString() {
}