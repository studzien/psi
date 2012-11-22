#include <iostream>
#include <vector>
#include "EditMessage.h"

using namespace std;

void EditMessage::buildMessage()
{
    string result = "<message to='";
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < (int)lines.size() ; i++)
    {
        result += "\t\t<set chdata='";
        result += lines.at(i).second;
        result += "' target='line-";
        result += lines.at(i).first;
        result += "'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;
}

EditMessage::EditMessage(string _message)
{
    message = _message;
}
EditMessage::EditMessage(string _to, vector<pair<string, string> > _lines)
{
    to = _to;
    for (int i=0;i<(int)_lines.size();++i) {
        lines.push_back(_lines.at(i));
    }
}
