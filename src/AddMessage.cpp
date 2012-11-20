#include <iostream>
#include <vector>
#include "AddMessage.h"

using namespace std;

string AddMessage::buildMessage()
{
    string result = "<message from='";
    result += from;
    result += "' to='";\
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < lines.size() ; i++)
    {
        result += "\t\t<new chdata='";
        result += lines[i].second;
        result += "' rid='line-";
        result += lines[i].first;
        result += "' parent='document' name='line' type='text'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;;
}

AddMessage::AddMessage(string _message)
{
    message = _message;
}
AddMessage::AddMessage(string _from, string _to, vector<pair<string, string> > _lines)
{
    from = _from;
    to = _to;
    lines = _lines;
}
