#include "Logger.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[]) {
  Logger::init();

#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
  // Qt 6.6+ mouse wheel hack
  qputenv("QT_QUICK_FLICKABLE_WHEEL_DECELERATION", "10000");
#endif

  // qputenv("QT_QPA_PLATFORM", "wayland");

  QGuiApplication app(argc, argv);

  app.setApplicationName("Calculator");
  app.setOrganizationName("MyCompany");
  app.setOrganizationDomain("mycompany.com");

  QQmlApplicationEngine engine;

  // CalculatorEngine calcEngine; // call constructor and create on stack
  //
  // engine.setInitialProperties(
  //     {{"calcEngine", QVariant::fromValue(&calcEngine)}});

  engine.loadFromModule("Calculator", "Application");

  if (engine.rootObjects().isEmpty()) {
    qWarning() << "Cannot init QmlApplicationEngine!";
    return EXIT_FAILURE;
  }

  int result = app.exec();

  Logger::shutdown();

  return result;
}
