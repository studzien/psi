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
		
		//metoda wysy³aj¹ca wiadomoœæ 
		void send() {
			/*
			 * Informacja dla osoby, która bêdzie tworzyæ wiadomoœæi xmpp:
			 * oldLineList zawiera poprzedni¹ listê linii
			 * newLineList zawiera now¹ listê linii
			 * nale¿y porównaæ te dwa pliki i stworzyæ odpowiednie wiadomoœci xmpp, czyli de facto wykryæ zmiany
			 * trzeba bêdzie tu jeszcze przekazaæ te jakieœ pierdo³y lub inaczej ich u¿yæ do samego wysy³ania, 
			 * z tym ju¿ siê nie bawi³em, wiêc nie pomogê
			 * jeœli oldLinesList jest NULL, to znaczy, ¿e trzeba wys³aæ nowy dokument, czyli ktoœ zrobi³ load nowego pliku
			 * po wszystkim bardzo prosi³bym o
			 * oldLinesList->clear();
			 * newLinesList->clear();
			 * delete w sumie te¿ mo¿esz :)
			 * w przypadku b³êdu mo¿na wyœwietliæ komunikat:
			 * QMessageBox msgBox;
			 * msgBox.setText("send");
			 * msgBox.exec();*/
			
		}
		
	protected:
		//w¹tek, sprawdza flagi, jeœli siê zgadzaj¹ w³¹cza w¹tek wysy³ania
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