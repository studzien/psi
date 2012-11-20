#include <iostream>
#include <vector>
#include "EditMessage.h"

using namespace std;

string EditMessage::buildMessage()
{
    string result = "<message from='";
    result += from;
    result += "' to='";\
    result += to;
    result += "' type='sxe'>\n";
    result += "\t<sxe xmlns='urn:xmpp:sxe:0'>\n";

    for(int i = 0; i < lines.size() ; i++)
    {
        result += "\t\t<set chdata='";
        result += lines[i].second;
        result += "' target='line-";
        result += lines[i].first;
        result += "'/>\n";
    }

    result += "\t</sxe>\n";
    result += "</message>";

    message = result;;
}

EditMessage::EditMessage(string _message)
{
    message = _message;
}
EditMessage::EditMessage(string _from, string _to, vector<pair<string, string> > _lines)
{
    from = _from;
    to = _to;
    lines = _lines;
}
