#include "XmlExtractor.h"

XmlExtractor::XmlExtractor(string xmlMessage)
{
    message = xmlMessage;
}

void XmlExtractor::parse()
{
    vector<char> data(message.begin(), message.end());
    data.push_back('\0');

    xml_document<> doc;
    doc.parse<parse_declaration_node | parse_no_data_nodes>(&data[0]);

    xml_node<> *root = doc.first_node() -> first_node() -> first_node();

    string actionName = root -> name();

    if(actionName.compare("new") == 0 )
    {
        type = ADD_LINE;

        while(root)
        {
            string data, line;
            data = root->first_attribute("chdata")->value();
            line = root->first_attribute("rid")->value();

            line = line.substr(5, line.size());

            linesAddChange.push_back(make_pair(line, data));

            root = root->next_sibling();
        }
    }
    else if(actionName.compare("remove") == 0)
    {
        type = REMOVE_LINE;

        while(root)
        {
            string line;
            if(root->first_attribute("target"))
            {
                line = root->first_attribute("target")->value();

                line = line.substr(5, line.size());
            }
            linesRemove.push_back(line);

            root = root->next_sibling();
        }
    }
    else if(actionName.compare("set") == 0)
    {
        type = EDIT_LINE;

        while(root)
        {
            string data, line;
            if(root->first_attribute("chdata"))
            {
                data = root->first_attribute("chdata")->value();
            }

            if(root->first_attribute("target"))
            {
                line = root->first_attribute("target")->value();

                line = line.substr(5, line.size());
            }
            linesAddChange.push_back(make_pair(line, data));

            root = root->next_sibling();
        }
    }
    else if(actionName.compare("state") == 0)
    {
        type = INIT_MSG;

        root = root -> first_node();

        while(root)
        {
            string tagName = root -> name();

            if(tagName.compare("new") == 0)
            {
                string data, line;
                if(root->first_attribute("chdata"))
                {
                    data = root->first_attribute("chdata")->value();
                }
                if(root->first_attribute("rid"))
                {
                    line = root->first_attribute("rid")->value();

                    line = line.substr(5, line.size());
                }
                linesAddChange.push_back(make_pair(line, data));
            }

            root = root->next_sibling();
        }
    }
}
