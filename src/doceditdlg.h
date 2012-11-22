/*
 * Shared Document Editing Tool
 * Copyright (C) 2012 Krzysiek Rusnarczyk
 */

#ifndef DOCEDITDLG_H
#define DOCEDITDLG_H

#include <QtGui>
#include <QString>
#include <vector>

#include "ui_doceditdlg.h"

using namespace std;

class PsiAccount;

class doceditdlg : public QWidget, private Ui::docEditWidget
{

	Q_OBJECT

	public:
		doceditdlg(QWidget *parent=0, PsiAccount* pa=0, QString to="");
		
		bool changeLine(pair<QString, QString> _line);
		bool addLine(pair<QString, QString> _line);
		bool removeLine(QString _num);
		bool setDoc(vector<pair<QString, QString> > _linesList);
		int compareNums(QString newNum, QString oldNum);
		
		void refresh();
		void createLinesList(QString text, vector<pair<QString,QString> > *_linesList);
		
	public slots:
		void load();
		void save();
		void checkChange();
		
	private:
		int changeFlag;
		vector<pair<QString, QString> > linesList;
		PsiAccount* psiAccount;
		QString to;
};

#endif