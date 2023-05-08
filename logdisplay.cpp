#include <QScrollBar>
#include <QDebug>
#include <QInputDialog>
#include <QMenu>
#include "logdisplay.h"
#include "mainwindow.h"
#include "global.h"
#include "ui_logdisplay.h"

CLogDisplay::CLogDisplay() :
    QDialog(nullptr, Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint),
    ui(new Ui::CLogDisplay)
{
    ui->setupUi(this);
    firstShow = true;
    syntax = new CSpecLogHighlighter(ui->logView->document());

    updateMessages();
}

CLogDisplay::~CLogDisplay()
{
    savedMessages.clear();
    delete ui;
}

void CLogDisplay::updateMessages()
{
    if (!isVisible()) return;

    int fr = -1;
    int sv = -1;

    if (ui->logView->verticalScrollBar() != nullptr)
        sv = ui->logView->verticalScrollBar()->value();

    if (!savedMessages.isEmpty())
        fr = debugMessages.lastIndexOf(savedMessages.last());

    if (fr >= 0 && fr < debugMessages.count()) {
        for (int i = (fr + 1); i < debugMessages.count(); i++)
            savedMessages << debugMessages.at(i);
    } else
        savedMessages = debugMessages;

    updateText(savedMessages.join('\n'));

    if (ui->logView->verticalScrollBar() != nullptr) {
        if (!ui->checkScrollLock->isChecked())
            ui->logView->verticalScrollBar()->setValue(ui->logView->verticalScrollBar()->maximum());
        else if (sv != -1)
            ui->logView->verticalScrollBar()->setValue(sv);
    }
}

void CLogDisplay::updateText(const QString &text)
{
    ui->logView->setPlainText(text);
}

void CLogDisplay::showEvent(QShowEvent *)
{
    updateMessages();

    if (firstShow && QApplication::activeWindow() != nullptr) {
        QPoint p = QApplication::activeWindow()->pos();
        p.rx() += 200;
        p.ry() += 100;
        move(p);
        firstShow = false;
    }
}

CSpecLogHighlighter::CSpecLogHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{

}

void CSpecLogHighlighter::highlightBlock(const QString &text)
{
    formatBlock(text, QRegularExpression("^\\S{,8}", QRegularExpression::CaseInsensitiveOption), Qt::black, true);
    formatBlock(text, QRegularExpression("\\s(\\S+\\s)?Debug:\\s", QRegularExpression::CaseInsensitiveOption), Qt::black, true);
    formatBlock(text, QRegularExpression("\\s(\\S+\\s)?Warning:\\s", QRegularExpression::CaseInsensitiveOption), Qt::darkRed, true);
    formatBlock(text, QRegularExpression("\\s(\\S+\\s)?Critical:\\s", QRegularExpression::CaseInsensitiveOption), Qt::red, true);
    formatBlock(text, QRegularExpression("\\s(\\S+\\s)?Fatal:\\s", QRegularExpression::CaseInsensitiveOption), Qt::red, true);
    formatBlock(text, QRegularExpression("\\s(\\S+\\s)?Info:\\s", QRegularExpression::CaseInsensitiveOption), Qt::darkBlue, true);
    formatBlock(text, QRegularExpression("\\(\\S+\\)$", QRegularExpression::CaseInsensitiveOption), Qt::gray, false, true);
}

void CSpecLogHighlighter::formatBlock(const QString &text, const QRegularExpression &exp,
                                      const QColor &color,
                                      bool weight,
                                      bool italic,
                                      bool underline,
                                      bool strikeout)
{
    if (text.isEmpty()) return;

    QTextCharFormat fmt;
    fmt.setForeground(color);

    if (weight)
        fmt.setFontWeight(QFont::Bold);
    else
        fmt.setFontWeight(QFont::Normal);

    fmt.setFontItalic(italic);
    fmt.setFontUnderline(underline);
    fmt.setFontStrikeOut(strikeout);

    QRegularExpressionMatchIterator i = exp.globalMatch(text);

    while (i.hasNext()) {
        QRegularExpressionMatch m = i.next();
        int length = m.capturedLength();
        int pos = m.capturedStart();
        setFormat(pos, length, fmt);
    }
}
