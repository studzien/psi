#include<vector>

using namespace std;

class AddMessage
{
    public:
    string message;

    private:
    string from, to;
    vector<pair<string, string> > lines;

    public:
    AddMessage(string);
    AddMessage(string, string, vector<pair<string, string> >);
    string buildMessage();
};
