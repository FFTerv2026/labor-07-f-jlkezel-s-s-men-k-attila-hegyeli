#ifndef FILESEARCHER_H
#define FILESEARCHER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDirIterator>
#include <QProgressDialog>
#include <QTextStream>
#include <QHeaderView>

class FileSearcher : public QWidget {
    Q_OBJECT
private:
    QLineEdit *m_searchEdit;
    QLabel *m_pathLabel;
    QTableWidget *m_resultTable;
    QString m_currentDir;

public:
    explicit FileSearcher(QWidget *parent = nullptr) : QWidget(parent) {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Alapértelmezett mappa az aktuális könyvtár
        m_currentDir = QDir::currentPath();

        QHBoxLayout *topLayout = new QHBoxLayout();
        m_pathLabel = new QLabel("Mappa: " + m_currentDir, this);
        QPushButton *browseButton = new QPushButton("Mappa kiválasztása...", this);
        topLayout->addWidget(m_pathLabel, 1);
        topLayout->addWidget(browseButton);

        QHBoxLayout *searchLayout = new QHBoxLayout();
        m_searchEdit = new QLineEdit(this);
        m_searchEdit->setPlaceholderText("Keresendő szöveg (üresen hagyva minden fájl)...");
        QPushButton *searchButton = new QPushButton("Keresés", this);
        searchLayout->addWidget(m_searchEdit);
        searchLayout->addWidget(searchButton);

        // Táblázat beállítása
        m_resultTable = new QTableWidget(this);
        m_resultTable->setColumnCount(2);
        m_resultTable->setHorizontalHeaderLabels({"Fájlnév", "Méret (Bytes)"});
        m_resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        m_resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

        mainLayout->addLayout(topLayout);
        mainLayout->addLayout(searchLayout);
        mainLayout->addWidget(m_resultTable);

        connect(browseButton, &QPushButton::clicked, this, &FileSearcher::selectDirectory);
        connect(searchButton, &QPushButton::clicked, this, &FileSearcher::startSearch);
    }

private slots:
    void selectDirectory() {
        QString dir = QFileDialog::getExistingDirectory(this, "Válassz könyvtárat", m_currentDir);
        if (!dir.isEmpty()) {
            m_currentDir = dir;
            m_pathLabel->setText("Mappa: " + m_currentDir);
        }
    }

    void startSearch() {
        m_resultTable->setRowCount(0);
        QString filterText = m_searchEdit->text();

        // 1. Összes fájl összegyűjtése a progress bar maximalizálásához
        QStringList fileList;
        QDirIterator countIt(m_currentDir, QDir::Files, QDirIterator::Subdirectories);
        while (countIt.hasNext()) {
            countIt.next();
            fileList.append(countIt.filePath());
        }

        if (fileList.isEmpty()) return;

        // Progress Dialog inicializálása
        QProgressDialog progress("Fájlok feldolgozása...", "Mégse", 0, fileList.size(), this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setMinimumDuration(0); // Azonnal jelenjen meg, ha tart a folyamat

        // 2. Fájlok tartalmának átfésülése
        for (int i = 0; i < fileList.size(); ++i) {
            progress.setValue(i);
            if (progress.wasCanceled()) break; // Ha a felhasználó a Mégse gombra nyom

            QString filePath = fileList.at(i);
            QFile file(filePath);
            bool isMatch = false;

            if (filterText.isEmpty()) {
                isMatch = true; // Ha üres, minden fájl találat
            } else if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString content = in.readAll();
                if (content.contains(filterText, Qt::CaseInsensitive)) {
                    isMatch = true;
                }
                file.close();
            }

            if (isMatch) {
                QFileInfo fileInfo(filePath);
                int row = m_resultTable->rowCount();
                m_resultTable->insertRow(row);
                m_resultTable->setItem(row, 0, new QTableWidgetItem(fileInfo.fileName()));
                m_resultTable->setItem(row, 1, new QTableWidgetItem(QString::number(fileInfo.size())));
            }
        }
        progress.setValue(fileList.size());
    }
};

#endif // FILESEARCHER_H
