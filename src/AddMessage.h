#include<vector>

using namespace std;

class AddMessage
{
    public:
    string message;

    private:
    string to;
    vector<pair<string, string> > lines;

    public:
    AddMessage(string);
    AddMessage(string, vector<pair<string, string> >);
    void buildMessage();
};
