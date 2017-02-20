#include "stdafx.h"

#include "LesserAppSetting.h"

void ValueEntity::SetID(int arg)
{
    itemId = arg;
}

void ValueEntity::SetDepth(int arg)
{
    depth = arg;
}

void ValueEntity::SetKey(string arg)
{
    key = arg;
}

void ValueEntity::SetValue(string arg)
{
    value = arg;
}

void ValueEntity::AddAttr(int id, string key, string value)
{
    unique_ptr<ValueEntity> add(new ValueEntity());
    add->SetID(id);
    add->SetKey(key);
    add->SetValue(value);
    attributes.push_back(move(add));
}

int ValueEntity::GetID()
{
    return itemId;
}

string ValueEntity::GetKey()
{
    return key;
}

string ValueEntity::GetValue()
{
    return value;
}

string ValueEntity::GetAttrValue(string attrname)
{
    int last = attributes.size();
    for (int i = 0; i < last; i++) {
        string iattrname = attributes.at(i)->GetKey();
        if (iattrname.compare(attrname) == 0) {
            string iattrvalue = attributes.at(i)->GetValue();
            return iattrvalue;
        }
    }
    return string();
}

ValueEntity::ValueEntity()
{
    itemId = 0;
    depth = 0;
    key = string();
    value = string();
}

ValueEntity::~ValueEntity()
{
}

void LesserAppSetting::Init(string arg)
{
    if (arg.length() == 0) {
        return;
    }
    if (FAILED(SHCreateStreamOnFile((LPCWSTR)(wc.Value(arg).ToWChar().get()), STGM_READ, &stream))) {
        return;
    }
    if (FAILED(CreateXmlReader(__uuidof(IXmlReader), (void**)&reader, NULL))) {
        return;
    }
    if (FAILED(reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit))) {
        return;
    }
    if (FAILED(reader->SetInput(stream))) {
        return;
    }
}

void LesserAppSetting::Parse()
{
    XmlNodeType nodeType;
    int id = 0;
    int depth = 0;
    int attrid = 0;
    unique_ptr<ValueEntity> add;
    while (reader->Read(&nodeType) == S_OK)
    {
        switch (nodeType)
        {
        case XmlNodeType_Element:
            id++;
            depth++;
            const wchar_t * prefix;
            const wchar_t * localName;
            uint32_t prefixLength;
            if (FAILED(reader->GetPrefix(&prefix, &prefixLength))) {
                break;
            }
            if (FAILED(reader->GetLocalName(&localName, NULL))) {
                break;
            }
            add.reset(new ValueEntity());
            add->SetID(id);
            add->SetDepth(depth);
            add->SetKey(wc.Value(localName).ToString());
            const wchar_t * attrvalue;
            if (FAILED(reader->MoveToFirstAttribute())) {
                break;
            }
            attrid = 0;
            while (true) {
                if (!reader->IsDefault()) {
                    if (FAILED(reader->GetPrefix(&prefix, &prefixLength))) {
                        continue;
                    }
                    if (FAILED(reader->GetLocalName(&localName, NULL))) {
                        continue;
                    }
                    if (FAILED(reader->GetValue(&attrvalue, NULL))) {
                        continue;
                    }
                    attrid++;
                    add->AddAttr(attrid, wc.Value(localName).ToString(), wc.Value(attrvalue).ToString());
                }
                if (S_OK != reader->MoveToNextAttribute()) {
                    break;
                }
            }
            value.push_back(move(add));
            break;
        case XmlNodeType_Attribute:
            break;
        case XmlNodeType_Text:
            const wchar_t * nodetextText;
            if (FAILED(reader->GetValue(&nodetextText, NULL))) {
                break;
            }
            value.at(id - 1)->SetValue(wc.Value(nodetextText).ToString());
            break;
        case XmlNodeType_CDATA:
            const wchar_t * nodetextCDATA;
            if (FAILED(reader->GetValue(&nodetextCDATA, NULL))) {
                break;
            }
            value.at(id - 1)->SetValue(wc.Value(nodetextCDATA).ToString());
            break;
        case XmlNodeType_EndElement:
            depth--;
            break;
        default:
            break;
        }
    }
}

string LesserAppSetting::ValueOfDir(string arg1category, string arg2attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    int end = start + 1;
    for (int j = end; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            end = j;
            break;
        }
    }
    for (int k = start; k < end; k++) {
        if (value.at(k)->GetKey().compare("Item") == 0) {
            if (value.at(k)->GetAttrValue("type").compare("Dir") == 0) {
                if (value.at(k)->GetAttrValue("name").compare(arg2attrname) == 0) {
                    return value.at(k)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfDir(string arg1category, string arg2category, string arg3attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    for (int j = start + 1; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            if (value.at(j)->GetAttrValue("name").compare(arg2category) == 0) {
                start = j;
                break;
            }
        }
    }
    int end = start + 1;
    for (int k = end; k < (int)value.size(); k++) {
        if (value.at(k)->GetKey().compare("Category") == 0) {
            end = k;
            break;
        }
    }
    for (int l = start; l < end; l++) {
        if (value.at(l)->GetKey().compare("Item") == 0) {
            if (value.at(l)->GetAttrValue("type").compare("Dir") == 0) {
                if (value.at(l)->GetAttrValue("name").compare(arg3attrname) == 0) {
                    return value.at(l)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfFile(string arg1category, string arg2attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    int end = start + 1;
    for (int j = end; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            end = j;
            break;
        }
    }
    for (int k = start; k < end; k++) {
        if (value.at(k)->GetKey().compare("Item") == 0) {
            if (value.at(k)->GetAttrValue("type").compare("File") == 0) {
                if (value.at(k)->GetAttrValue("name").compare(arg2attrname) == 0) {
                    return value.at(k)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfFile(string arg1category, string arg2category, string arg3attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    for (int j = start + 1; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            if (value.at(j)->GetAttrValue("name").compare(arg2category) == 0) {
                start = j;
                break;
            }
        }
    }
    int end = start + 1;
    for (int k = end; k < (int)value.size(); k++) {
        if (value.at(k)->GetKey().compare("Category") == 0) {
            end = k;
            break;
        }
    }
    for (int l = start; l < end; l++) {
        if (value.at(l)->GetKey().compare("Item") == 0) {
            if (value.at(l)->GetAttrValue("type").compare("File") == 0) {
                if (value.at(l)->GetAttrValue("name").compare(arg3attrname) == 0) {
                    return value.at(l)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfTable(string arg1category, string arg2attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    int end = start + 1;
    for (int j = end; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            end = j;
            break;
        }
    }
    for (int k = start; k < end; k++) {
        if (value.at(k)->GetKey().compare("Item") == 0) {
            if (value.at(k)->GetAttrValue("type").compare("Table") == 0) {
                if (value.at(k)->GetAttrValue("name").compare(arg2attrname) == 0) {
                    return value.at(k)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfTable(string arg1category, string arg2category, string arg3attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    for (int j = start + 1; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            if (value.at(j)->GetAttrValue("name").compare(arg2category) == 0) {
                start = j;
                break;
            }
        }
    }
    int end = start + 1;
    for (int k = end; k < (int)value.size(); k++) {
        if (value.at(k)->GetKey().compare("Category") == 0) {
            end = k;
            break;
        }
    }
    for (int l = start; l < end; l++) {
        if (value.at(l)->GetKey().compare("Item") == 0) {
            if (value.at(l)->GetAttrValue("type").compare("Table") == 0) {
                if (value.at(l)->GetAttrValue("name").compare(arg3attrname) == 0) {
                    return value.at(l)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfAttr(string arg1category, string arg2attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    int end = start + 1;
    for (int j = end; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            end = j;
            break;
        }
    }
    for (int k = start; k < end; k++) {
        if (value.at(k)->GetKey().compare("Item") == 0) {
            if (value.at(k)->GetAttrValue("type").compare("Attr") == 0) {
                if (value.at(k)->GetAttrValue("name").compare(arg2attrname) == 0) {
                    return value.at(k)->GetValue();
                }
            }
        }
    }
    return string();
}

string LesserAppSetting::ValueOfAttr(string arg1category, string arg2category, string arg3attrname)
{
    int start = 1;
    for (int i = start; i < (int)value.size(); i++) {
        if (value.at(i)->GetKey().compare("Category") == 0) {
            if (value.at(i)->GetAttrValue("name").compare(arg1category) == 0) {
                start = i;
                break;
            }
        }
    }
    for (int j = start + 1; j < (int)value.size(); j++) {
        if (value.at(j)->GetKey().compare("Category") == 0) {
            if (value.at(j)->GetAttrValue("name").compare(arg2category) == 0) {
                start = j;
                break;
            }
        }
    }
    int end = start + 1;
    for (int k = end; k < (int)value.size(); k++) {
        if (value.at(k)->GetKey().compare("Category") == 0) {
            end = k;
            break;
        }
    }
    for (int l = start; l < end; l++) {
        if (value.at(l)->GetKey().compare("Item") == 0) {
            if (value.at(l)->GetAttrValue("type").compare("Attr") == 0) {
                if (value.at(l)->GetAttrValue("name").compare(arg3attrname) == 0) {
                    return value.at(l)->GetValue();
                }
            }
        }
    }
    return string();
}

LesserAppSetting::LesserAppSetting()
{
}

LesserAppSetting::~LesserAppSetting()
{
}