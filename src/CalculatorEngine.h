#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QtQml>
#include <expected>
#include <stack>

enum class Operator { NONE, PLUS, MINUS, MUL, DIV, EQUAL, DOT };

enum class OperationPrecedence {
  NONE = 0,
  PLUS = 9,
  MINUS = 9,
  MUL = 10,
  DIV = 10
};

class OperatorMapper {
public:
  static Operator toOp(QString op) {
    static const std::unordered_map<std::string_view, Operator> mapping = {
        {"+", Operator::PLUS}, {"-", Operator::MINUS}, {"×", Operator::MUL},
        {"÷", Operator::DIV},  {"=", Operator::EQUAL}, {".", Operator::DOT},
    };

    auto it = mapping.find(op.toStdString());
    if (it != mapping.end()) {
      return it->second;
    }
    return Operator::NONE;
  }

  static QString toTok(const Operator &op) {
    switch (op) {
    case Operator::NONE:
      return "";
    case Operator::PLUS:
      return "+";
    case Operator::MINUS:
      return "-";
    case Operator::MUL:
      return "×";
    case Operator::DIV:
      return "÷";
    case Operator::EQUAL:
      return "=";
    case Operator::DOT:
      return ".";
    default:
      return "";
    }
  }

  static OperationPrecedence toPrec(const Operator &op) {
    switch (op) {
    case Operator::NONE:
      return OperationPrecedence::NONE;
    case Operator::PLUS:
      return OperationPrecedence::PLUS;
    case Operator::MINUS:
      return OperationPrecedence::MINUS;
    case Operator::MUL:
      return OperationPrecedence::MUL;
    case Operator::DIV:
      return OperationPrecedence::DIV;
    case Operator::EQUAL:
      return OperationPrecedence::NONE;
    case Operator::DOT:
      return OperationPrecedence::NONE;
    default:
      return OperationPrecedence::NONE;
    }
  }
};

struct OperationStep {
  Q_GADGET
  QML_ANONYMOUS
  Q_PROPERTY(QString prefix MEMBER prefix)
  Q_PROPERTY(QString value MEMBER value)

public:
  OperationStep()
      : prefix(""), value(""), precedence(OperationPrecedence::NONE),
        op(Operator::NONE) {}
  OperationStep(const QString &value, const Operator &op)
      : value(value), op(op) {
    auto pref = OperatorMapper::toTok(op);
    auto prec = OperatorMapper::toPrec(op);

    prefix = pref;
    precedence = prec;
  }
  OperationStep(QString prefix, QString value, OperationPrecedence precedence,
                Operator op)
      : prefix(prefix), value(value), precedence(precedence), op(op) {}

  QString prefix;
  QString value;
  Operator op;
  OperationPrecedence precedence;
};

Q_DECLARE_METATYPE(OperationStep)

class HistoryModel : public QAbstractListModel {
  Q_OBJECT
  QML_ELEMENT

public:
  // These roles act as the "keys" that QML uses to ask for specific data
  enum StepRoles { PrefixRole = Qt::UserRole + 1, ValueRole };

  //  Most C++ style guides, including the C++ Core Guidelines, recommend
  //  marking all single-argument constructors as explicit by default to
  //  disallow "magic" behavior.
  explicit HistoryModel(QObject *parent = nullptr);

  // The 3 required overrides for QAbstractListModel
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  void addStep(const OperationStep &step);
  std::optional<std::reference_wrapper<const OperationStep>>
  getLastStep() const noexcept;
  void modifyLastStep(const OperationStep &step);
  void updateLastStepOp(const Operator &op);
  void updateLastStepVal(const QString &val);
  void clear();
  void printDebug() const;
  QList<OperationStep> getSteps();

private:
  QList<OperationStep> m_steps;
};

class CalculatorEngine : public QObject {
  Q_OBJECT
  // Tells QML this type exists
  QML_ELEMENT
  QML_SINGLETON
  Q_PROPERTY(HistoryModel *historyModel READ historyModel CONSTANT)
  Q_PROPERTY(OperationStep resultStep READ resultStep NOTIFY resultStepChanged)

public:
  explicit CalculatorEngine(QObject *parent = nullptr);

  HistoryModel *historyModel() const;
  OperationStep resultStep() const;

  Q_INVOKABLE void processInput(const QString &input, bool isOperator);
  Q_INVOKABLE void clear();
  Q_INVOKABLE void deleteLastDigit();
  Q_INVOKABLE void flipValueSign();
  Q_INVOKABLE void debug();
  Q_INVOKABLE void assignNewValue();

signals:
  void resultStepChanged();

private:
  HistoryModel *m_historyModel;
  OperationStep m_resultStep;

  void calculate();
  void handleInitialInput(const QString &input, bool isOperator);
  void applyOperator(const Operator &op, const OperationStep &lastStep);
  std::expected<double, std::string>
  convertToNumber(const QString &input) const;
  void appendOperand(const QString &digit, const OperationStep &lastStep);
  std::expected<double, std::string> doMath(const double &first,
                                            const double &second, Operator op);
  bool collapseTopOperation(std::stack<double> &nums,
                            std::stack<OperationStep> &steps);
};
