using namespace std;

class RemoveMessage
{
    public:
    string message;

    private:
    string from, to;
    vector<string> lines;

    public:
    RemoveMessage(string);
    RemoveMessage(string, string, vector<string>);
    string buildMessage();
};
