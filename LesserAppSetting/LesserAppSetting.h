#pragma once

#include "stdafx.h"

#include "WCharString.h"

class ValueEntity
{
private:

    int itemId;

    int depth;

    string key;

    string value;

    vector<unique_ptr<ValueEntity>> attributes;

public:

    void SetID(int arg);

    void SetDepth(int arg);

    void SetKey(string keyname);

    void SetValue(string value);

    void AddAttr(int id, string key, string value);

    int GetID();

    string GetKey();

    string GetValue();

    string GetAttrValue(string attrname);

    ValueEntity();

    ~ValueEntity();
};

class LesserAppSetting
{
private:

    WCharString wc;

    IStream * stream;

    IXmlReader * reader;

    vector<unique_ptr<ValueEntity>> value;

public:

    void Init(string arg);

    void Parse();

    string ValueOfDir(string arg1category, string arg2attrname);

    string ValueOfDir(string arg1category, string arg2category, string arg3attrname);

    string ValueOfFile(string arg1category, string arg2attrname);

    string ValueOfFile(string arg1category, string arg2category, string arg3attrname);

    string ValueOfTable(string arg1category, string arg2attrname);

    string ValueOfTable(string arg1category, string arg2category, string arg3attrname);

    string ValueOfAttr(string arg1category, string arg2attrname);

    string ValueOfAttr(string arg1category, string arg2category, string arg3attrname);

    LesserAppSetting();

    ~LesserAppSetting();
};
