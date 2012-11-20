#include<vector>

using namespace std;

class EditMessage
{
    public:
    string message;

    private:
    string from, to;
    vector<pair<string, string> > lines;

    public:
    EditMessage(string);
    EditMessage(string, string, vector<pair<string, string> >);
    string buildMessage();
};
