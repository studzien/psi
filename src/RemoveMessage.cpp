#include <iostream>
#include <vector>
#include "RemoveMessage.h"

using namespace std;

void RemoveMessage::buildMessage()
{
    string result = "<message to='";
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < (int)lines.size() ; i++)
    {
        result += "\t\t<remove target='line-";
        result += lines.at(i);
        result += "'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;
}

RemoveMessage::RemoveMessage(string _message)
{
    message = _message;
}
RemoveMessage::RemoveMessage(string _to, vector<string> _lines)
{
    to = _to;
    for (int i=0;i<(int)_lines.size();++i) {
        lines.push_back(_lines.at(i));
    }
}
