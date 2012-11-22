using namespace std;

class RemoveMessage
{
    public:
    string message;

    private:
    string to;
    vector<string> lines;

    public:
    RemoveMessage(string);
    RemoveMessage(string, vector<string>);
    void buildMessage();
};
