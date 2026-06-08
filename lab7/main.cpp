#include <QApplication>
#include "filesearcher.h"
#include "csvviewer.h"
#include "quotesapp.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // 1. Feladat: Fájlkereső
    FileSearcher searcherWindow;
    searcherWindow.setWindowTitle("1. Feladat: Fájlkereső");
    searcherWindow.resize(500, 400);
    searcherWindow.show();

    // 2. Feladat: CSV Megtekintő
    CsvViewer csvWindow;
    csvWindow.setWindowTitle("2. Feladat: CSV Megtekintő");
    csvWindow.show();

    // 3. Feladat: Motivációs idézetek
    QuotesApp quotesWindow;
    quotesWindow.setWindowTitle("3. Feladat: Motiváció");
    quotesWindow.resize(400, 250);
    quotesWindow.show();

    return a.exec();
}
