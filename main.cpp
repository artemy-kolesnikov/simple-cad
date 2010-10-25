#include <QTextCodec>
#include <QPlastiqueStyle>

#include <locale.h>

#include "mainwindow.h"
#include "cadapplication.h"

int main(int argc, char *argv[])
{
	CADApplication a(argc, argv);

	CADApplication::setStyle(new QPlastiqueStyle());

	setlocale(LC_ALL, "rus_rus");
	setlocale(LC_NUMERIC, "English");
	setlocale(LC_TIME, "English");

	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));

	MainWindow w;
	w.show();

	return a.exec();
}
