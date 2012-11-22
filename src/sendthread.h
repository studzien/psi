/*
 * Shared Document Editing Tool
 * Copyright (C) 2012 Krzysiek Rusnarczyk
 */

#include <QtGui>
#include <vector>
#include <sstream>
#include <utility>
#include <string>
#include <iostream>
#include <stdio.h>

#include "psiaccount.h"
#include "xmpp_client.h"
#include "mucmanager.h"

#include "AddMessage.h"
#include "EditMessage.h"
#include "RemoveMessage.h"

class PsiAccount;

using namespace std;
//using namespace XMPP;

class sendThread : public QThread
{
	Q_OBJECT
		
	private:
		int *sendFlag;
		int flag;
		vector <pair <QString, QString> > oldLinesList;
		vector <pair <QString, QString> > newLinesList;
		PsiAccount* psiAccount;
		QString to;
		
	public:
		//konstruktor
		sendThread(int _flag, int *_sendFlag, vector <pair <QString, QString> >* _oldLinesList, vector <pair <QString, QString> >* _newLinesList, PsiAccount* pa, QString _to)
		{
			flag=_flag;
			sendFlag=_sendFlag;
			psiAccount=pa;
			to=_to;
			
			for (int i=0;i<(int)_oldLinesList->size();++i) {
				oldLinesList.push_back(make_pair(_oldLinesList->at(i).first,_oldLinesList->at(i).second));
			}
			
			for (int i=0;i<(int)_newLinesList->size();++i) {
				newLinesList.push_back(make_pair(_newLinesList->at(i).first,_newLinesList->at(i).second));
			}
			
		}
		
		//metoda wysy³aj¹ca wiadomoœæ 
		void send() {
			
			vector <pair <string, string> > oldLinesListString;
			vector <pair <string, string> > newLinesListString;
			
			for (int i=0;i<(int)oldLinesList.size();++i) {
				oldLinesListString.push_back(make_pair(oldLinesList.at(i).first.toUtf8().constData(),oldLinesList.at(i).second.toUtf8().constData()));
			}
			
			for (int i=0;i<(int)newLinesList.size();++i) {
				newLinesListString.push_back(make_pair(newLinesList.at(i).first.toUtf8().constData(),newLinesList.at(i).second.toUtf8().constData()));
			}
			
			vector <pair <string, string> > add, edit;
			vector <string>	remove;
			
			find_diff(&oldLinesListString, &newLinesListString, &add, &edit, &remove);
			
			/*
			//tests
			QMessageBox msgBox;
			msgBox.setText("edit");
			msgBox.exec();
			for (int i=0;i<(int)edit.size();++i) {
				msgBox.setText(QString::fromUtf8(edit.at(i).first.c_str())+" "+QString::fromUtf8(edit.at(i).second.c_str()));
				msgBox.exec();
			}
			
			msgBox.setText("add");
			msgBox.exec();
			for (int i=0;i<(int)add.size();++i) {
				msgBox.setText(QString::fromUtf8(add.at(i).first.c_str())+" "+QString::fromUtf8(add.at(i).second.c_str()));
				msgBox.exec();
			}
			
			msgBox.setText("remove");
			msgBox.exec();
			for (int i=0;i<(int)remove.size();++i) {
				msgBox.setText(QString::fromUtf8(remove.at(i).c_str()));
				msgBox.exec();
			}
			*/
			QMessageBox msgBox;
			if (edit.size()!=0) {
				EditMessage *xmlEdit = new EditMessage(to.toUtf8().constData(), edit);
				xmlEdit->buildMessage();

				psiAccount->client()->send(QString::fromUtf8(xmlEdit->message.c_str()));
			}
			
			if (remove.size()!=0) {
				RemoveMessage *xmlRemove = new RemoveMessage(to.toUtf8().constData(), remove);
				xmlRemove->buildMessage();
				
				psiAccount->client()->send(QString::fromUtf8(xmlRemove->message.c_str()));
			}
			
			if (add.size()!=0) {
				AddMessage *xmlAdd = new AddMessage(to.toUtf8().constData(), add);
				xmlAdd->buildMessage();
				
				psiAccount->client()->send(QString::fromUtf8(xmlAdd->message.c_str()));
			}
		}
		
		//szukanie ró¿nic, by M.Pa³ka
		void find_diff(vector <pair <string, string> >* server, 
					vector <pair <string, string> >* client, 
					vector <pair <string, string> >* add, 
					vector <pair <string, string> >* edit, 
					vector <string>* remove)
		{

			int c = client->size();
			int s = server->size();
			
			int d = c > s ? c : s;
			
			string current_line = server->at(s-1).first;
			
			for(int i = 0; i < d; i++){
			
				// so far we are iterating over lines which exist in both files
				if(i < c && i < s){
					if(server->at(i).second != client->at(i).second){
					edit->push_back(make_pair(server->at(i).first, client->at(i).second));
					}
				} 
				// now only one of them has a line
				else {
					//server
					if(i < s) {
						remove->push_back(server->at(i).first);
					} 
					// or client
					else {
						add->push_back(make_pair(get_next(current_line), client->at(i).second));
						current_line = get_next(current_line);
					}
				}
			}
		}
		
		string get_next(string s){
			int i;
			istringstream iss(s);
			iss >> i;
			i++;
			stringstream ss;//create a stringstream
			ss << i;//add number to the stream
			return ss.str();
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