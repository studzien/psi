/*
 * Shared Document Editing Tool
 * Copyright (C) 2012 Krzysiek Rusnarczyk
 */

#include <QtGui>
#include <vector>

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
			 * je�li oldLinesList jest NULL, to znaczy, �e trzeba wys�a� nowy dokument, czyli kto� zrobi� load nowego pliku
			 * po wszystkim bardzo prosi�bym o
			 * oldLinesList->clear();
			 * newLinesList->clear();
			 * delete w sumie te� mo�esz :)
			 * w przypadku b��du mo�na wy�wietli� komunikat:
			 * QMessageBox msgBox;
			 * msgBox.setText("send");
			 * msgBox.exec();*/
			
		}
		
	protected:
		//w�tek, sprawdza flagi, je�li si� zgadzaj� w��cza w�tek wysy�ania
		void run()
		{
			msleep(2500);
			if (flag == *sendFlag) {
				send();
			} else {
				//oldLinesList->clear();
				//newLinesList->clear();
			}
		}
};