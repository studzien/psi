#include<vector>

using namespace std;

class EditMessage
{
    public:
    string message;

    private:
    string to;
    vector<pair<string, string> > lines;

    public:
    EditMessage(string);
    EditMessage(string, vector<pair<string, string> >);
    void buildMessage();
};
