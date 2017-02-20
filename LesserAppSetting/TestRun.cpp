#include "stdafx.h"

#include "LesserAppSetting.h"

void TestMethod01()
{
    LesserAppSetting s;
    s.Init("");
    s.Parse();
    cout << s.ValueOfAttr("", "", "") << "\n";
    cout << "\n";
}

int main(int argc, char * argv[])
{
    TestMethod01();

    string prompt;
    cin >> prompt;
    return 0;
}