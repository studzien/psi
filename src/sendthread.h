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
		
		//metoda wysy³aj¹ca wiadomoœæ 
		void send() {
			/*
			 * Informacja dla osoby, która bêdzie tworzyæ wiadomoœæi xmpp:
			 * oldLineList zawiera poprzedni¹ listê linii
			 * newLineList zawiera now¹ listê linii
			 * nale¿y porównaæ te dwa pliki i stworzyæ odpowiednie wiadomoœci xmpp, czyli de facto wykryæ zmiany
			 * trzeba bêdzie tu jeszcze przekazaæ te jakieœ pierdo³y lub inaczej ich u¿yæ do samego wysy³ania, 
			 * z tym ju¿ siê nie bawi³em, wiêc nie pomogê
			 * w przypadku b³êdu mo¿na wyœwietliæ komunikat:
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
		//w¹tek, sprawdza flagi, jeœli siê zgadzaj¹ w³¹cza w¹tek wysy³ania
		void run()
		{
			msleep(2500);
			if (flag == *sendFlag) {
				send();
			}
		}
};