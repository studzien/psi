#include <iostream>
#include <vector>
#include "RemoveMessage.h"

using namespace std;

string RemoveMessage::buildMessage()
{
    string result = "<message from='";
    result += from;
    result += "' to='";\
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < lines.size() ; i++)
    {
        result += "\t\t<remove target='line-";
        result += lines[i];
        result += "'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;;
}

RemoveMessage::RemoveMessage(string _message)
{
    message = _message;
}
RemoveMessage::RemoveMessage(string _from, string _to, vector<string> _lines)
{
    from = _from;
    to = _to;
    lines = _lines;
}
