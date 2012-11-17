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
	
	createLinesList(textEditor->toPlainText(), &linesList);
	
	int _flag = 1;
	sendThread *sThread = new sendThread(_flag, &_flag, NULL, &linesList);
	sThread->start();
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

	//tutaj powinna nast¹piæ zmiana linesList, jednak jest problem implementacyjny, którego krzychowina nie ogarnia, bo:
	//zmiana nastêpuje w w¹tku, musia³bym z w¹tku przes³aæ sygna³, ¿e uda³o mu siê wys³aæ,
	//a tu go odebraæ, a w¹tki œmierdz¹
	//problem jednak sam siê rozwi¹zuje, bo serwer przesy³a zmianê do wszystkich, wiêc i tak linesList zostanie podmieniony.
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

//zmienia linesList na podstawie przekazanej wartoœci
//odœwie¿a pole tekstowe nowym tekstem
bool doceditdlg::setDoc(vector<pair<QString, QString> > _linesList)
{
	linesList.clear();
	linesList=_linesList;
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
