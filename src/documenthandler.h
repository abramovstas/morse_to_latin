#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include <QQuickTextDocument>

#include <QtGui/QTextCharFormat>
#include <QtCore/QTextCodec>

#include <qqmlfile.h>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class DocumentHandler : public QObject
{
    Q_OBJECT

    Q_ENUMS(HAlignment)

    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString textMorze READ textMorze WRITE setTextMorze NOTIFY textChanged)
    Q_PROPERTY(QString documentTitle READ documentTitle WRITE setDocumentTitle NOTIFY documentTitleChanged)

public:
    DocumentHandler();

    bool isLatinEncoding(const QString &source) const;
    Q_INVOKABLE QString translate(const QString &source) const;
    Q_INVOKABLE QString toMorse(const QString &source) const;
    Q_INVOKABLE QString fromMorse(const QString &source) const;

    QQuickItem *target() { return m_target; }

    void setTarget(QQuickItem *target);

    void setCursorPosition(int position);
    void setSelectionStart(int position);
    void setSelectionEnd(int position);

    int cursorPosition() const { return m_cursorPosition; }
    int selectionStart() const { return m_selectionStart; }
    int selectionEnd() const { return m_selectionEnd; }

    QUrl fileUrl() const;
    QString text() const;
    QString textMorze() const;

    QString documentTitle() const;

public Q_SLOTS:
    void setFileUrl(const QUrl &arg);
    void setText(const QString &arg);
    void setTextMorze(const QString &arg);
    void saveAs(const QUrl &arg, const QString &fileType);
    void saveTranslateAs(const QUrl &arg, const QString &fileType, const QString &data);

    void setDocumentTitle(QString arg);

Q_SIGNALS:
    void targetChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();
    void fileUrlChanged();
    void textChanged();
    void documentTitleChanged();
    void error(QString message);

private:
    QTextCursor textCursor() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    QQuickItem *m_target;
    QTextDocument *m_doc;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;

    QFont m_font;
    QUrl m_fileUrl;
    QString m_text;
    QString m_documentTitle;

    QMap<QString, QString> morse;
};

#endif
