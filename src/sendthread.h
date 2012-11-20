/*
 * Shared Document Editing Tool
 * Copyright (C) 2012 Krzysiek Rusnarczyk
 */

#include <QtGui>
#include <vector>

#include "AddMessage.h"
#include "EditMessage.h"
#include "RemoveMessage.h"

using namespace std;

class sendThread : public QThread
{
	Q_OBJECT
		
	private:
		int *sendFlag;
		int flag;
		vector<pair<QString, QString> > *oldLinesList;
		vector<pair<QString, QString> > *newLinesList;
		
	public:
		//konstruktor
		sendThread(int _flag, int *_sendFlag, vector<pair<QString, QString> > *_oldLinesList, vector<pair<QString, QString> > *_newLinesList)
		{
			flag=_flag;
			sendFlag=_sendFlag;
			oldLinesList=_oldLinesList;
			newLinesList=_newLinesList;
		}
		
		//metoda wysy�aj�ca wiadomo�� 
		void send() {
			/*
			 * Informacja dla osoby, kt�ra b�dzie tworzy� wiadomo��i xmpp:
			 * oldLineList zawiera poprzedni� list� linii
			 * newLineList zawiera now� list� linii
			 * nale�y por�wna� te dwa pliki i stworzy� odpowiednie wiadomo�ci xmpp, czyli de facto wykry� zmiany
			 * trzeba b�dzie tu jeszcze przekaza� te jakie� pierdo�y lub inaczej ich u�y� do samego wysy�ania, 
			 * z tym ju� si� nie bawi�em, wi�c nie pomog�
			 * w przypadku b��du mo�na wy�wietli� komunikat:
			 * QMessageBox msgBox;
			 * msgBox.setText("send");
			 * msgBox.exec();*/
			 
			//usuwanie linii
			/*
			vector<string> linesToRemove;
			linesToRemove.push_back("3");
			linesToRemove.push_back("4.2.3");

			RemoveMessage *xmlRemove = new RemoveMessage("www.from", "www.to", linesToRemove);
			xmlRemove->buildMessage();
			cout << xmlRemove->message;
			*/
			
			//dodawanie linii
			/*
			vector<pair<string, string> > linesToAdd;
			linesToAdd.push_back(make_pair("5", "Tresc linii"));

			AddMessage *xmlAdd = new AddMessage("www.from", "www.to", linesToAdd);
			xmlAdd->buildMessage();
			xmlAdd->message;
			*/
			
			//edycja linii
			/*
			vector<pair<string, string> > linesToEdit;
			linesToEdit.push_back(make_pair("8", "%% factorial"));

			EditMessage *xmlEdit = new EditMessage("www.from", "www.to", linesToEdit);
			xmlEdit->buildMessage();
			cout << xmlEdit->message;
			*/
		}
		
	protected:
		//w�tek, sprawdza flagi, je�li si� zgadzaj� w��cza w�tek wysy�ania
		void run()
		{
			msleep(2500);
			if (flag == *sendFlag) {
				send();
			}
		}
};