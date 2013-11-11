#include "handlebars.h"
#include <QTextStream>
#include <QFileInfo>

Handlebars::Handlebars(QObject *parent):
  QObject(parent)
{
  m_JsEngine = new QJSEngine(this);
}

Handlebars::~Handlebars()
{
  delete m_JsEngine;
}

bool Handlebars::init()
{
  QFile fHandlebars(":/js/handlebars.js");
  if(!fHandlebars.open(QIODevice::ReadOnly))
    {
      m_errorString = QString("No such file %1").arg(fHandlebars.fileName());
      return false;
    }
  QTextStream in (&fHandlebars);
  QString hbrSrc = in.readAll();
  fHandlebars.close();

  QJSValue ret = m_JsEngine->evaluate(hbrSrc);
  if(ret.isError())
    {
      m_errorString = QString("Unable to load handlebars.js : %1").arg(ret.toString());
      return false;
    }

  return true;
}

bool Handlebars::compile(const QString &hbrTemplate)
{
  m_errorString = "";
  if(hbrTemplate.isEmpty())
    {
      m_errorString = QString("Template is empty");
      return false;
    }

  m_JsEngine->globalObject().setProperty("tpl",hbrTemplate);
  QJSValue ret = m_JsEngine->evaluate(QString("var template = Handlebars.compile(tpl);"));
  if(ret.isError())
    {
      m_errorString = QString("Unable to load the template : %1").arg(ret.toString());
      return false;
    }

  return true;
}

bool Handlebars::compile(const QFile &hbrTemplateFile)
{
  m_errorString = "";
  QFile fHbrTpl (QFileInfo(hbrTemplateFile).absoluteFilePath());
  if(!fHbrTpl.exists() || !fHbrTpl.open(QIODevice::ReadOnly))
    {
      m_errorString = QString("Unable to open file %1").arg(fHbrTpl.fileName());
      return false;
    }
  QTextStream in (&fHbrTpl);
  QString hbrTpl = in.readAll();
  fHbrTpl.close();

  if(hbrTpl.isEmpty())
    {
      m_errorString = QString("File %1 is empty").arg(fHbrTpl.fileName());
      return false;
    }

  return this->compile(hbrTpl);
}

bool Handlebars::registerHelper(const QString &helper)
{
  m_errorString = "";
  if(helper.isEmpty())
    {
      m_errorString = QString("Heleper is empty");
      return false;
    }

  QJSValue ret = m_JsEngine->evaluate(QString(helper));
  if(ret.isError())
    {
      m_errorString = QString("Unable to load the helper : %1").arg(ret.toString());
      return false;
    }

  return true;
}

bool Handlebars::registerHelper(const QFile &helperFile)
{
  m_errorString = "";
  QFile fHbrHelper (QFileInfo(helperFile).absoluteFilePath());
  if(!fHbrHelper.exists() || !fHbrHelper.open(QIODevice::ReadOnly))
    {
      m_errorString = QString("Unable to open file %1").arg(fHbrHelper.fileName());
      return false;
    }
  QTextStream in (&fHbrHelper);
  QString hbrHelper = in.readAll();
  fHbrHelper.close();

  if(hbrHelper.isEmpty())
    {
      m_errorString = QString("File %1 is empty").arg(fHbrHelper.fileName());
      return false;
    }

  return this->registerHelper(hbrHelper);
}

bool Handlebars::registerPartial(const QString &name, const QString &partial)
{
  m_errorString = "";
  if(name.isEmpty())
    {
      m_errorString = QString("Partial name is empty");
      return false;
    }
  if(partial.isEmpty())
    {
      m_errorString = QString("Partial is empty");
      return false;
    }

  QJSValue ret = m_JsEngine->evaluate(QString("Handlebars.registerPartial('%1','%2');").arg(name).arg(partial));
  if(ret.isError())
    {
      m_errorString = QString("Unable to register the partial %1 : %2").arg(name).arg(ret.toString());
      return false;
    }

  return true;
}

bool Handlebars::registerPartial(const QString & name, const QFile &partialFile)
{
  m_errorString = "";
  QFile fHbrPartial(QFileInfo(partialFile).absoluteFilePath());
  if(!fHbrPartial.exists() || !fHbrPartial.open(QIODevice::ReadOnly))
    {
      m_errorString = QString("Unable to open file %1").arg(fHbrPartial.fileName());
      return false;
    }
  QTextStream in (&fHbrPartial);
  QString hbrPartial = in.readAll();
  fHbrPartial.close();

  if(hbrPartial.isEmpty())
    {
      m_errorString = QString("File %1 is empty").arg(fHbrPartial.fileName());
      return false;
    }

  return this->registerPartial(name,hbrPartial);
}

QString Handlebars::render(const QJsonDocument &context, const QString &file)
{
  m_errorString = "";
  QJsonDocument doc = context;
  QString ctxData = doc.toJson(QJsonDocument::Compact);

  QJSValue ret = m_JsEngine->evaluate(QString("template(%1);").arg(ctxData));

  if(ret.isError())
    {
      m_errorString = QString("Unable to execute render : %1").arg(ret.toString());
      return QString("");
    }

  if(file.isEmpty())
    return ret.toString();

  QFile fTemplateRendered (file);
  if(!fTemplateRendered.open(QIODevice::WriteOnly))
    {
      m_errorString = QString("Unable to open file %1 : %2").arg(file).arg(fTemplateRendered.errorString());
      return QString("");
    }
  QTextStream out (&fTemplateRendered);
  out << ret.toString();
  fTemplateRendered.close();
  return ret.toString();
}

QString Handlebars::render(const QString &context, const QString &file)
{
  m_errorString = "";
  QJsonParseError * err = new QJsonParseError;
  QJsonDocument doc = QJsonDocument::fromJson(QByteArray(context.toStdString().c_str()),err);

  if(err->error != QJsonParseError::NoError)
    {
      m_errorString = QString("Context is not a valid JSON : %1").arg(err->errorString());
      return QString("");
    }
  return this->render(doc,file);
}

QString Handlebars::errorString() const
{
  return m_errorString;
}
