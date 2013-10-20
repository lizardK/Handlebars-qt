#include "test_handlebars.h"
#include <QJsonDocument>
#include <QDebug>

void TestHandlebars::initTestCase()
{
  // Build & Write template
  QFile fTemplate("template.hbr");
  fTemplate.open(QIODevice::WriteOnly);
  QTextStream out (&fTemplate);
  out << "{{#doors}} - {{name}}{{/doors}}";
  fTemplate.close();

  // Build & Write data file
  QJsonDocument data;
  QJsonObject root;
  QJsonArray array;
  QJsonObject o1,o2,o3,o4;
  o1.insert("name",QString("Morrison"));
  o2.insert("name",QString("Manzarek"));
  o3.insert("name",QString("Kriegger"));
  o4.insert("name",QString("Densmore"));

  array.append(o1);
  array.append(o2);
  array.append(o3);
  array.append(o4);
  root.insert("doors",array);
  data.setObject(root);
  QFile fData ("data.json");
  fData.open(QIODevice::WriteOnly);
  out.setDevice(&fData);
  out << data.toJson();
  fData.close();
}

void TestHandlebars::testInit()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QCOMPARE(ret, true);
}

void TestHandlebars::testLoadTemplateFromString()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  ret = hbr.loadTemplate("{{#doors}}- {{name}}{{/doors}}");
  QCOMPARE(ret, true);
}

void TestHandlebars::testLoadTemplateFromFile()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  QFile f ("template.hbr");
  ret = hbr.loadTemplate(f);
  QCOMPARE(ret, true);
}

void TestHandlebars::testRenderFromString()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  ret = hbr.loadTemplate("{{#doors}} - {{name}}{{/doors}}");
  QVERIFY(ret == true);
  QFile fData ("data.json");
  fData.open(QIODevice::ReadOnly);
  QString context = fData.readAll();
  fData.close();
  QString outputExpected = " - Morrison - Manzarek - Kriegger - Densmore";
  QString output = hbr.render(context);
  QCOMPARE(output, outputExpected);
}

void TestHandlebars::testRenderFromStringToFile()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  ret = hbr.loadTemplate("{{#doors}} - {{name}}{{/doors}}");
  QVERIFY(ret == true);
  QFile fData ("data.json");
  fData.open(QIODevice::ReadOnly);
  QString context = fData.readAll();
  fData.close();
  QString outputExpected = " - Morrison - Manzarek - Kriegger - Densmore";
  QString output = hbr.render(context,"render.txt");
  QCOMPARE(output, outputExpected);
  QCOMPARE(QFile::exists("render.txt"), true);

}

void TestHandlebars::testRenderFromJson()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  ret = hbr.loadTemplate("{{#doors}} - {{name}}{{/doors}}");
  QVERIFY(ret == true);
  QFile fData ("data.json");
  fData.open(QIODevice::ReadOnly);
  QJsonDocument context = QJsonDocument::fromJson(fData.readAll());
  fData.close();
  QString outputExpected = " - Morrison - Manzarek - Kriegger - Densmore";
  QString output = hbr.render(context);
  QCOMPARE(output, outputExpected);
}

void TestHandlebars::testRenderFromJsonToFile()
{
  Handlebars hbr;
  bool ret = hbr.init();
  QVERIFY(ret == true);
  ret = hbr.loadTemplate("{{#doors}} - {{name}}{{/doors}}");
  QVERIFY(ret == true);
  QFile fData ("data.json");
  fData.open(QIODevice::ReadOnly);
  QJsonDocument context = QJsonDocument::fromJson(fData.readAll());
  fData.close();
  QString outputExpected = " - Morrison - Manzarek - Kriegger - Densmore";
  QString output = hbr.render(context,"render.txt");
  QCOMPARE(output, outputExpected);
  QCOMPARE(QFile::exists("render.txt"), true);
}

void TestHandlebars::cleanupTestCase()
{
  QFile fTemplate("template.hbr");
  fTemplate.remove();
  QFile fData ("data.json");
  fData.remove();
  QFile fRender ("render.txt");
  fRender.remove();
}

QTEST_GUILESS_MAIN(TestHandlebars)
