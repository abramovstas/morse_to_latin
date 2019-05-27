#include "documenthandler.h"

#include <QtGui/QTextDocument>
#include <QtGui/QTextCursor>
#include <QtGui/QFontDatabase>
#include <QtCore/QFileInfo>

DocumentHandler::DocumentHandler()
    : m_target(nullptr)
    , m_doc(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
{
    morse.insert("a", ".-");
    morse.insert("b", "-...");
    morse.insert("w", ".--");
    morse.insert("g", "--.");
    morse.insert("d", "-..");
    morse.insert("e", ".");
    morse.insert("v", "...-");
    morse.insert("z", "--..");
    morse.insert("i", "..");
    morse.insert("j", ".---");
    morse.insert("k", "-.-");
    morse.insert("l", ".-..");
    morse.insert("m", "--");
    morse.insert("n", "-.");
    morse.insert("o", "---");
    morse.insert("p", ".--.");
    morse.insert("r", ".-.");
    morse.insert("s", "...");
    morse.insert("t", "-");
    morse.insert("u", "..-");
    morse.insert("f", "..-.");
    morse.insert("h", "....");
    morse.insert("c", "-.-.");
    morse.insert("q", "--.-");
    morse.insert("y", "-.--");
    morse.insert("x", "-..-");
    morse.insert("1", ".----");
    morse.insert("2", "..---");
    morse.insert("3", "...--");
    morse.insert("4", "....-");
    morse.insert("5", ".....");
    morse.insert("6", "-....");
    morse.insert("7", "--...");
    morse.insert("8", "---..");
    morse.insert("9", "----.");
    morse.insert("0", "-----");
    morse.insert(".", "......");
    morse.insert(",", ".-.-.-");
    morse.insert(":", "---...");
    morse.insert(";", "-.-.-.");
    morse.insert("(", "-.--.-");
    morse.insert(")", "-.--.-");
    morse.insert("\"", ".-..-.");
    morse.insert("-", "-....-");
    morse.insert("/", "-..-.");
    morse.insert("?", "..--..");
    morse.insert("!", "--..--");
    morse.insert(" ", "-...-");
    morse.insert("@", ".--.-.");
}

bool DocumentHandler::isLatinEncoding(const QString &source) const
{
    QString copy_source = source;
    QStringList letters = copy_source.split(" ");
    for(int i = 0; i < letters.count(); i++) {
        qDebug() << letters[i];
        if (morse.values().contains(letters[i]))
            return false;
    }

    return true;
}

QString DocumentHandler::translate(const QString &source) const
{
    //qDebug() << "RESULT " << isLatinEncoding(source);
    if (isLatinEncoding(source))
        return toMorse(source);
    else return fromMorse(source);
}

QString DocumentHandler::toMorse(const QString &source) const
{
    QString out_string;
    for(int i = 0; i < source.length(); i++) {
        QMap<QString, QString>::const_iterator j = morse.find(source[i].toLower());
        while (j != morse.end() && j.key() == source[i].toLower()) {
            if (j.key() != " ")
                out_string += j.value();

            out_string += " ";
            ++j;
        }
    }
    //qDebug() << out_string;
    return out_string;
}

QString DocumentHandler::fromMorse(const QString &source) const
{
    QString copy_source = source;
    copy_source.replace("  ", " * ");
    QString out_string;
    QStringList morse_letters = copy_source.split(" ");
    for(int i = 0; i < morse_letters.length(); i++) {
        if (morse_letters[i] == "*") {
            out_string += " ";
            continue;
        }
        QList<QString> morse_keys = morse.keys(morse_letters[i]);
        if (morse_keys.size() > 0)
            out_string += morse_keys.at(0);
    }
    //qDebug() << out_string;
    return out_string;
}

void DocumentHandler::setTarget(QQuickItem *target)
{
    m_doc = 0;
    m_target = target;
    if (!m_target)
        return;

    QVariant doc = m_target->property("textDocument");
    if (doc.canConvert<QQuickTextDocument*>()) {
        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument*>();
        if (qqdoc)
            m_doc = qqdoc->textDocument();
    }
    emit targetChanged();
}

void DocumentHandler::setFileUrl(const QUrl &arg)
{
    if (m_fileUrl != arg) {
        m_fileUrl = arg;
        QString fileName = QQmlFile::urlToLocalFileOrQrc(arg);
        if (QFile::exists(fileName)) {
            QFile file(fileName);
            if (file.open(QFile::ReadOnly)) {
                QByteArray data = file.readAll();
                QTextCodec *codec = QTextCodec::codecForHtml(data);
                setText(codec->toUnicode(data));
                if (m_doc)
                    m_doc->setModified(false);
                if (fileName.isEmpty())
                    m_documentTitle = QStringLiteral("untitled.txt");
                else
                    m_documentTitle = QFileInfo(fileName).fileName();

                emit textChanged();
                emit documentTitleChanged();
            }
        }
        emit fileUrlChanged();
    }
}

QString DocumentHandler::documentTitle() const
{
    return m_documentTitle;
}

void DocumentHandler::setDocumentTitle(QString arg)
{
    if (m_documentTitle != arg) {
        m_documentTitle = arg;
        emit documentTitleChanged();
    }
}

void DocumentHandler::setText(const QString &arg)
{
    if (m_text != arg) {
        m_text = arg;
        emit textChanged();
    }
}

void DocumentHandler::setTextMorze(const QString &arg)
{
    if (m_text != arg) {
        m_text = arg;
        emit textChanged();
    }
}

void DocumentHandler::saveAs(const QUrl &arg, const QString &fileType)
{
    bool isHtml = fileType.contains(QLatin1String("htm"));
    QLatin1String ext(isHtml ? ".html" : ".txt");
    QString localPath = arg.toLocalFile();
    if (!localPath.endsWith(ext))
        localPath += ext;
    QFile f(localPath);
    if (!f.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + f.errorString());
        return;
    }
    f.write((isHtml ? m_doc->toHtml() : m_doc->toPlainText()).toLocal8Bit());
    f.close();
    setFileUrl(QUrl::fromLocalFile(localPath));
}

void DocumentHandler::saveTranslateAs(const QUrl &arg, const QString &fileType, const QString &data)
{
    bool isHtml = fileType.contains(QLatin1String("htm"));
    QLatin1String ext(isHtml ? ".html" : ".txt");
    QString localPath = arg.toLocalFile();
    if (!localPath.endsWith(ext))
        localPath += ext;
    QFile f(localPath);
    if (!f.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + f.errorString());
        return;
    }
    f.write(data.toLocal8Bit());
    f.close();
    //setFileUrl(QUrl::fromLocalFile(localPath));
}

QUrl DocumentHandler::fileUrl() const
{
    return m_fileUrl;
}

QString DocumentHandler::text() const
{
    return m_text;
}

QString DocumentHandler::textMorze() const
{
    return toMorse(m_text);
}

void DocumentHandler::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
}

QTextCursor DocumentHandler::textCursor() const
{
    if (!m_doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(m_doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

void DocumentHandler::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

void DocumentHandler::setSelectionStart(int position)
{
    m_selectionStart = position;
}

void DocumentHandler::setSelectionEnd(int position)
{
    m_selectionEnd = position;
}
