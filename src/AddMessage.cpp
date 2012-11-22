#include <iostream>
#include <vector>
#include "AddMessage.h"

using namespace std;

void AddMessage::buildMessage()
{
    string result = "<message to='";
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < (int)lines.size() ; i++)
    {
        result += "\t\t<new chdata='";
        result += lines.at(i).second;
        result += "' rid='line-";
        result += lines.at(i).first;
        result += "' parent='document' name='line' type='text'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;
}

AddMessage::AddMessage(string _message)
{
    message = _message;
}
AddMessage::AddMessage(string _to, vector<pair<string, string> > _lines)
{
    to = _to;
    for (int i=0;i<(int)_lines.size();++i) {
        lines.push_back(_lines.at(i));
    }
}
