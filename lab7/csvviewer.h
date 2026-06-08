#ifndef CSVVIEWER_H
#define CSVVIEWER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>

class CsvViewer : public QMainWindow {
    Q_OBJECT
private:
    QTableWidget *m_tableWidget;

public:
    explicit CsvViewer(QWidget *parent = nullptr) : QMainWindow(parent) {
        m_tableWidget = new QTableWidget(this);

        // A táblázat NE legyen módosítható
        m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        setCentralWidget(m_tableWidget);

        // Menüsáv felépítése
        QMenu *fileMenu = menuBar()->addMenu("File");
        QAction *openAction = fileMenu->addAction("Open File...");

        connect(openAction, &QAction::triggered, this, &CsvViewer::openCsvFile);
        resize(600, 400);
    }

private slots:
    void openCsvFile() {
        // Csak .csv fájlok szűrése
        QString fileName = QFileDialog::getOpenFileName(this, "CSV megnyitása", "", "CSV Files (*.csv)");
        if (fileName.isEmpty()) return;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Hiba", "Nem sikerült megnyitni a fájlt!");
            return;
        }

        m_tableWidget->clear();
        m_tableWidget->setRowCount(0);
        m_tableWidget->setColumnCount(0);

        QTextStream in(&file);
        bool isFirstLine = true;
        int row = 0;

        while (!in.atEnd()) {
            QString line = in.readLine();
            // Támogatja a vesszővel vagy pontosvesszővel elválasztott CSV-ket is
            QStringList fields = line.split(line.contains(';') ? ';' : ',');

            if (isFirstLine) {
                // Első sor: Oszlopok beállítása
                m_tableWidget->setColumnCount(fields.size());
                m_tableWidget->setHorizontalHeaderLabels(fields);
                isFirstLine = false;
            } else {
                // Többi sor: Adatok feltöltése
                m_tableWidget->insertRow(row);
                for (int col = 0; col < fields.size(); ++col) {
                    m_tableWidget->setItem(row, col, new QTableWidgetItem(fields.at(col).trimmed()));
                }
                row++;
            }
        }
        file.close();
        m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
};

#endif // CSVVIEWER_H
