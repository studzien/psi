/*
 * Shared Document Editing Tool
 * Copyright (C) 2012 Krzysiek Rusnarczyk
 */

#include <QtGui>
#include <string>
#include "doceditdlg.h"
#include "psiaccount.h"
#include "sendthread.h"

using namespace XMPP;

//konstruktor
doceditdlg::doceditdlg(QWidget *parent)
{
    setupUi(this);
	
	changeFlag = 0;
 
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save())); 
    connect(loadButton, SIGNAL(clicked()), this, SLOT(load()));
	connect(textEditor, SIGNAL(textChanged()), this, SLOT(checkChange()));
}

//slot dla wczytywania pliku z dysku
//wywo³uje automatycznie w¹tek wysy³ania pe³nego dokumentu
//wczytuje text do textarea
//podmienia lokaln¹ zmienn¹ dokumentu
void doceditdlg::load()
{  
    QString path = QFileDialog::getOpenFileName(
        this,
        "Choose a file to open",
        QString::null,
        QString::null);
 
	textEditor->clear();
	
	QFile file(path);
	QString line;
	
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		while (!in.atEnd()) {
			line = in.readLine();
			textEditor->appendPlainText(line);
		}
	}
	
	file.close();
	
	vector<pair<QString,QString> > newLinesList;
	createLinesList(textEditor->toPlainText(), &newLinesList);
	
	int _flag = 1;
	sendThread *sThread = new sendThread(_flag, &_flag, &linesList, &newLinesList);
	sThread->start();
	
	//zmiana linesList nast¹pi po otrzymaniu odpowiedzi z serwera
}

//zapisuje obecny stan dokumentu na dysku lokalnym
void doceditdlg::save()
{
	QString filename = QFileDialog::getSaveFileName(
		this,
		"Save file as",
		QString::null,
		QString::null);
	
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
	QTextStream out(&file);
	out << textEditor->toPlainText();
    file.close();
}

//tworzy w¹tek zmiany tekstu
//przekazuje w¹tkowi flagi
//przekazuje w¹tkowi referencje do tekstu
void doceditdlg::checkChange()
{	
	if (changeFlag == 1000) changeFlag=0;
	
	++changeFlag;
	
	vector<pair<QString, QString> > oldLinesList(linesList);
	vector<pair<QString, QString> > newLinesList;
	createLinesList(textEditor->toPlainText(), &newLinesList);
	
	sendThread *sThread = new sendThread(changeFlag, &changeFlag, &oldLinesList, &newLinesList);
	sThread->start();

	//zmiana linesList nast¹pi po otrzymaniu odpowiedzi z serwera
}

//podmienia liniê tekstu o okreœlonych parametrach
//odœwie¿a pole tekstowe nowym tekstem
bool doceditdlg::changeLine(pair<QString, QString> _line)
{
	for (int i=0;i<(int)linesList.size();++i) {
		if (QString::compare(_line.first,linesList.at(i).first,Qt::CaseSensitive) == 0) {
			linesList.at(i).second=_line.second;
			refresh();
			return true;
		}
	}
	return false;
}

//dodaje liniê tekstu o okreœlonych parametrach
//odœwie¿a pole tekstowe nowym tekstem
bool doceditdlg::addLine(pair<QString, QString> _line)
{
	//wykrywanie przypadku <0,"">, ale nie powinien siê przydarzyæ
	if (_line.first=="0" && _line.second=="") {
		return true;
	}

	//parsowanie numeru
	QStringList nums = _line.first.split(".");
	QString num="";
	
	if ((int)nums.size()!=1) {
		for (int i=0;i<=(int)nums.size()-2;++i) {
			num+=nums.at(i);
			if (i!=(int)nums.size()-2) {
				num+=".";
			}
		}
	} else {
		num+=nums.at(0);
	}
	
	//ostatnia linia, wykorzystujemy tu fakt, ¿e ostatnia linia, zawsze jest ca³kowita
	int lastnum = linesList.back().first.toInt();
	++lastnum;
	
	if (QString::compare(num,QString::number(lastnum),Qt::CaseSensitive)==0) {
		linesList.push_back(_line);
		refresh();
		return true;
	}
	
	//przypadek dodawania po zero
	if (_line.first.startsWith("0.") && (int)nums.size()==2) {
		linesList.insert(linesList.begin(),_line);
		refresh();
		return true;
	}
	
	//przypadki zwyk³e, czyli wstawianie gdzieœ w œrodku tekstu
	//zasada jest prosta
	//myœlimy w kategorii wstawiania linii po:
	//jeœli mam wstawiæ 2.-1.x to wrzucam po 2.-1
	//jeœli mam wrzuciæ 2.-1.4.x to wrzucam po 2.-1.4
	for (int i=0;i<(int)linesList.size();++i) {
		if (QString::compare(linesList.at(i).first,num,Qt::CaseSensitive)==0) {
			linesList.insert(linesList.begin()+i+1,_line);
			refresh();
			return true;
		}
	}
	return false;
}

//usuwa liniê tekstu o okreœlonych parametrach
//odœwie¿a pole tekstowe nowym tekstem
bool doceditdlg::removeLine(QString _num)
{
	for (int i=0;i<(int)linesList.size();++i) {
		if (QString::compare(_num,linesList.at(i).first,Qt::CaseSensitive) == 0) {
			linesList.erase(linesList.begin()+i);
			refresh();
			return true;
		}
	}
	return false;
}

//inicjacja listy linii
//odœwie¿a pole tekstowe nowym tekstem
bool doceditdlg::setDoc(vector<pair<QString, QString> > _linesList)
{
	linesList.clear();
	
	//pêtla po elementach z _linesList
	for (int i=0;i<(int)_linesList.size();++i) {
	
		bool isInserted = false;
		
		//pêtla po obecnej linesList
		for (int j=0;j<(int)linesList.size();++j) {
			int cmp = compareNums(_linesList.at(i).first,linesList.at(j).first);
			
			//nowy element jest przed obecnym na liœcie
			if (cmp == 0) {
				linesList.insert(linesList.begin()+j,_linesList.at(i));
				isInserted = true;
				break;
			}
			//nowy element jest po obecnym na liœcie
			if (cmp == 1) {
				continue;
			}
		}
		
		//wstawianie na koniec listy
		if (isInserted == false) {
			linesList.push_back(_linesList.at(i));
		}
	}
	
	refresh();
	
	return true;
}

//metoda odœwie¿ania pola textarea obecnym stanem linesList
void doceditdlg::refresh() {
	textEditor->clear();
	for (int i=0;i<(int)linesList.size();++i) {
		textEditor->appendPlainText(linesList.at(i).second);
	}
}

//metoda porównywania numerów
//zwraca 0 - gdy newNum jest mniejszy od oldNum
//zwraca 1 - gdy newNum jest wiêkszy od oldNum
int doceditdlg::compareNums(QString newNum, QString oldNum) {
	QStringList newNums = newNum.split(".");
	QStringList oldNums = oldNum.split(".");
	
	//obieramy ten rozmiar pêtli, który jest mniejszy
	int size;
	int result;
	if (newNums.size() >= oldNums.size()) {
		size=oldNums.size();
		result=1;
	} else {
		size=newNums.size();
		result=0;
	}
	
	//pêtla po elementach
	for (int i=0;i<size;++i) {
		if (newNums.at(i).toInt() == oldNums.at(i).toInt()) {
			continue;
		}
		
		if (newNums.at(i).toInt() < oldNums.at(i).toInt()) {
			result = 0;
		}
		
		if (newNums.at(i).toInt() > oldNums.at(i).toInt()) {
			result = 1;
		}	
	}
	
	return result;
}

//metoda tworzenia linesList na podstawie przekazanej wartoœci
void doceditdlg::createLinesList(QString text, vector<pair<QString,QString> > *_linesList)
{
	QStringList lines = text.split("\n");
	
	_linesList->clear();
	
	for (int i=0;i<lines.size();++i) {
		_linesList->push_back(make_pair(QString::number(i+1),lines.at(i)));
	}
	
	lines.clear();
}
