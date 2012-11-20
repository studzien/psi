#include "rapidxml.h"
#include "rapidxml_utils.h"

using namespace std;
using namespace rapidxml;

enum ACTION_TYPE
{
    INIT_MSG = 1,
    ADD_LINE = 2,
    EDIT_LINE = 3,
    REMOVE_LINE = 4
};

class XmlExtractor
{
    private:
    string message;
    vector< pair<string, string> > linesAddChange;
    vector<string> linesRemove;
    ACTION_TYPE type;

    public:
    XmlExtractor(string);
    void parse();

    ACTION_TYPE getType()
    {
        return type;
    }

    vector< pair<string, string> > getChangeLines()
    {
        return linesAddChange;
    }

    vector< pair<string, string> > getAddLines()
    {
        return linesAddChange;
    }

    vector<string> getRemoveLines()
    {
        return linesRemove;
    }
};
