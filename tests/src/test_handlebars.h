#include "handlebars.h"
#include <QtTest/QtTest>

class TestHandlebars : public QObject
{
  Q_OBJECT

private Q_SLOTS:
  void initTestCase();

  void testInit();
  void testLoadTemplateFromString();
  void testLoadTemplateFromFile();
  void testRenderFromString();
  void testRegisterPartialFromString();
  void testRegisterPartialFromFile();
  void testRenderFromStringToFile();
  void testRenderFromJson();
  void testRenderFromJsonToFile();
  void testRenderWithPartial();
  void testErrorLoadTemplateFromEmptyString();

  void cleanupTestCase();
};
