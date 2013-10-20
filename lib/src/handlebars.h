#ifndef HANDLEBARS_H
#define HANDLEBARS_H

#include "handlebars-qt_global.h"
#include <QObject>
#include <QJSEngine>
#include <QString>
#include <QFile>
#include <QJsonDocument>

class HANDLEBARSQTSHARED_EXPORT Handlebars : public QObject
{

public:
  Handlebars(QObject * parent = 0);
  ~Handlebars();

  bool init();

  bool loadTemplate(const QString & hbrTemplate);
  bool loadTemplate(const QFile & hbrTemplateFile);

  bool registerHelper(const QString & helper);
  bool registerHelper(const QFile & helperFile);

  bool registerPartial(const QString & name, const QString & partial);
  bool registerPartial(const QString & name, const QFile & partialFile);

  QString render(const QJsonDocument & context, const QString & file = QString());
  QString render(const QString & context, const QString & file = QString());

  QString errorString() const;

private:
  QJSEngine * m_JsEngine;
  QString m_template;
  QString m_errorString;
};

#endif // HANDLEBARS_H
