#ifndef QUOTESAPP_H
#define QUOTESAPP_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>

class QuotesApp : public QWidget {
    Q_OBJECT
private:
    QLabel *m_quoteLabel;
    QPushButton *m_nextButton;
    QStringList m_quotes;
    int m_lastIndex;

public:
    explicit QuotesApp(QWidget *parent = nullptr) : QWidget(parent), m_lastIndex(-1) {
        QVBoxLayout *layout = new QVBoxLayout(this);

        m_quoteLabel = new QLabel("Idézet betöltése...", this);
        m_quoteLabel->setWordWrap(true);
        m_quoteLabel->setAlignment(Qt::AlignCenter);

        m_nextButton = new QPushButton("Új idézet", this);

        layout->addWidget(m_quoteLabel, 1);
        layout->addWidget(m_nextButton);

        // Idézetek betöltése és az első megjelenítése
        loadQuotes();
        generateNewQuote();

        connect(m_nextButton, &QPushButton::clicked, this, &QuotesApp::generateNewQuote);

        // --- STYLESHEET (DÍSZÍTÉS) ---
        this->setStyleSheet(
            "QWidget { background-color: #2c3e50; }"
            "QLabel { color: #ecf0f1; font-size: 16px; font-style: italic; padding: 20px; font-family: 'Georgia'; }"
            "QPushButton { background-color: #e74c3c; color: white; border-radius: 8px; padding: 10px; font-weight: bold; font-size: 14px; }"
            "QPushButton:hover { background-color: #c0392b; }"
            "QPushButton:pressed { background-color: #962d22; }"
            );
    }

private:
    void loadQuotes() {
        QFile file("quotes.txt");
        // Ha nincs fájl, létrehozunk egyet alapértelmezett mintákkal a teszteléshez
        if (!file.exists()) {
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "A siker nem döntő, a kudarc nem végzetes: a folytatáshoz való bátorság az, ami számít.\n"
                    << "Ne várd meg a tökéletes pillanatot. Ragadd meg a pillanatot, és tedd tökéletessé!\n"
                    << "A legnagyobb dicsőségünk nem az, hogy soha nem bukunk el, hanem hogy minden bukás után felállunk.\n"
                    << "Az egyetlen módja annak, hogy nagyszerű munkát végezz, ha szereted, amit csinálsz.\n";
                file.close();
            }
        }

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty()) {
                    m_quotes.append(line);
                }
            }
            file.close();
        }
    }

private slots:
    void generateNewQuote() {
        if (m_quotes.isEmpty()) {
            m_quoteLabel->setText("Nem találhatók idézetek a quotes.txt fájlban!");
            return;
        }

        if (m_quotes.size() == 1) {
            m_quoteLabel->setText(m_quotes.first());
            return;
        }

        int newIndex = m_lastIndex;
        // Addig generálunk új indexet, amíg az meggyezik az előzővel
        while (newIndex == m_lastIndex) {
            newIndex = QRandomGenerator::global()->bounded(m_quotes.size());
        }

        m_lastIndex = newIndex;
        m_quoteLabel->setText("\"" + m_quotes.at(m_lastIndex) + "\"");
    }
};

#endif // QUOTESAPP_H
