#include "CalculatorEngine.h" // "..." will priority local files first, <...> will do opposite
#include <qhashfunctions.h>
#include <spdlog/spdlog.h>
#include <string>

HistoryModel::HistoryModel(QObject *parent) : QAbstractListModel(parent) {
  // The internal m_steps list is automatically initialized as empty.
  // No further setup is required here.
}

/**
 * ATTENTION:
 *
 * In QT underline engine, all data structure (list, table, tree, etc) will be
 * represented in tree structure.
 *
 * For 1D list (QList), it will be a tree with one hidden root node and all
 * elements in the list will be child nodes of that root node (they will be leaf
 * node). That means if parent is invalid (QModelIndex()), it will be the root
 * node, and we need to return the amount of child nodes of that node (all
 * elements in the list). And if parent is valid, it must be one of the child
 * node, which is leaf node in list, and leaf node does not have any children,
 * so we return 0.
 */
int HistoryModel::rowCount(const QModelIndex &parent) const {
  if (parent.isValid()) {
    return 0;
  }

  return m_steps.count();
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }

  const auto &data = m_steps.at(index.row());
  switch (role) {
  case PrefixRole:
    return data.prefix;
  case ValueRole:
    return data.value;
  default:
    return QVariant();
  }
}

QHash<int, QByteArray> HistoryModel::roleNames() const {
  // When a variable inside a function is declared as static, it is initialized
  // only once and retains its value between multiple function calls.
  static QHash<int, QByteArray> mapping{
      {PrefixRole, "prefix"},
      {ValueRole, "value"},
  };

  return mapping;
}

void HistoryModel::addStep(const OperationStep &step) {
  int newRowIndex = m_steps.count();

  beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);

  m_steps.append(step);

  endInsertRows();
}

void HistoryModel::modifyLastStep(const OperationStep &step) {
  if (m_steps.count() == 0) {
    return;
  }

  int lastRow = m_steps.count() - 1;

  auto *lastStep = &m_steps[lastRow];
  lastStep->value = step.value;
  lastStep->prefix = step.prefix;

  // List models are 1D, so the column is always 0
  QModelIndex index = createIndex(lastRow, 0);

  // We pass the same index twice because we are only updating one row (TopLeft
  // to BottomRight)
  emit dataChanged(index, index);
}

void HistoryModel::updateLastStepOp(const Operator &op) {
  if (m_steps.count() == 0) {
    return;
  }

  int lastRow = m_steps.count() - 1;
  auto *lastStep = &m_steps[lastRow];
  if (lastStep->op == op) {
    return;
  }

  auto prefix = OperatorMapper::toTok(op);
  auto prec = OperatorMapper::toPrec(op);

  lastStep->prefix = prefix;
  lastStep->precedence = prec;
  lastStep->op = op;

  QModelIndex index = createIndex(lastRow, 0);

  emit dataChanged(index, index, {PrefixRole});
}

void HistoryModel::updateLastStepVal(const QString &val) {
  if (m_steps.count() == 0) {
    return;
  }

  int lastRow = m_steps.count() - 1;
  auto *lastStep = &m_steps[lastRow];
  if (lastStep->value == val) {
    return;
  }

  lastStep->value = val;

  QModelIndex index = createIndex(lastRow, 0);

  emit dataChanged(index, index, {ValueRole});
}

std::optional<std::reference_wrapper<const OperationStep>>
HistoryModel::getLastStep() const noexcept {
  if (m_steps.count() == 0) {
    return std::nullopt;
  }

  return m_steps.at(m_steps.count() - 1);
}

void HistoryModel::clear() {
  if (m_steps.count() == 0) {
    return;
  }

  beginRemoveRows(QModelIndex(), 0, m_steps.count() - 1);
  m_steps.clear();
  endRemoveRows();
}

QList<OperationStep> HistoryModel::getSteps() { return m_steps; }

void HistoryModel::printDebug() const {
  spdlog::info("--- HistoryModel C++ Debug ({} items) ---", m_steps.size());

  int i = 0;
  for (const auto &step : m_steps) {
    // If step.prefix is QString, use .toStdString()
    // If step.value is a numeric type, it works directly
    spdlog::info("[{}] Prefix: {} | Value: {}", i++, step.prefix.toStdString(),
                 step.value.toStdString());
  }

  spdlog::info("--- End C++ Debug ---");
}

CalculatorEngine::CalculatorEngine(QObject *parent)
    : QObject(parent), m_resultStep(OperationStep("0", Operator::NONE)) {
  // Passing 'this' as the parent ensures that when CalculatorEngine
  // is destroyed, it automatically deletes the HistoryModel to prevent memory
  // leaks.
  // NO NEED FOR SMART POINTER HERE BECAUSE QT CAN HANDLE IT
  m_historyModel = new HistoryModel(this);
}

HistoryModel *CalculatorEngine::historyModel() const { return m_historyModel; }

OperationStep CalculatorEngine::resultStep() const { return m_resultStep; }

void CalculatorEngine::processInput(const QString &input, bool isOperator) {
  if (input.isNull()) {
    spdlog::warn("No input.");
    return;
  }

  spdlog::debug("Input: {}, isOperator: {}", input.toStdString(), isOperator);

  auto lastStepWrapper = m_historyModel->getLastStep();
  auto op = OperatorMapper::toOp(input);

  if (!lastStepWrapper.has_value()) {
    handleInitialInput(input, isOperator);
  } else {
    auto lastStep = lastStepWrapper.value().get();
    if (isOperator) {
      applyOperator(op, lastStep);
    } else {
      appendOperand(input, lastStep);
    }
  }

  calculate();
}

void CalculatorEngine::assignNewValue() {
  if (m_historyModel->rowCount() == 0) {
    spdlog::debug("List is empty, skipping.");
    return;
  }

  auto currentResult = m_resultStep.value;
  if (!convertToNumber(currentResult)) {
    spdlog::debug("Invalid result.");
    return;
  }

  m_historyModel->clear();
  m_historyModel->addStep(OperationStep(currentResult, Operator::NONE));
}

void CalculatorEngine::handleInitialInput(const QString &input,
                                          bool isOperator) {
  if (isOperator) {
    spdlog::debug("The first value must be a number, not an operator.");
    return;
  }

  if (input == '.') {
    m_historyModel->addStep(OperationStep(
        QString(), "0.", OperationPrecedence::NONE, Operator::NONE));
  } else {
    m_historyModel->addStep(OperationStep(
        QString(), input, OperationPrecedence::NONE, Operator::NONE));
  }
}

void CalculatorEngine::applyOperator(const Operator &op,
                                     const OperationStep &lastStep) {
  auto token = OperatorMapper::toTok(op);
  auto prec = OperatorMapper::toPrec(op);
  if (lastStep.value == "") {
    m_historyModel->modifyLastStep(
        OperationStep(QString(token), lastStep.value, prec, op));
  } else {
    m_historyModel->addStep(OperationStep(token, QString(), prec, op));
  }
}

void CalculatorEngine::appendOperand(const QString &digit,
                                     const OperationStep &lastStep) {
  if (digit == '.') {
    if (lastStep.value.contains(".")) {
      spdlog::debug("Already have dot, skipping.");
      return;
    }

    if (lastStep.value.isEmpty()) {
      m_historyModel->updateLastStepVal("0.");
      return;
    }
  }

  QString newVal = lastStep.value; // make a copy first
  newVal.append(digit);
  m_historyModel->updateLastStepVal(newVal);
}

void CalculatorEngine::clear() {
  m_historyModel->clear();
  calculate();
}

void CalculatorEngine::deleteLastDigit() {
  auto lastStepWrapper = m_historyModel->getLastStep();
  if (!lastStepWrapper.has_value()) {
    spdlog::debug("No digit to delete.");
    return;
  }

  auto lastStep = lastStepWrapper.value().get();
  if (lastStep.value.isEmpty()) {
    spdlog::debug("No digit to delete.");
    return;
  }

  QString val = lastStep.value;
  QString stripped = val.left(val.size() - 1);

  spdlog::debug("Before deleted last digit: {}. After delete last digit: {}",
                val.toStdString(), stripped.toStdString());
  m_historyModel->updateLastStepVal(stripped);

  calculate();
}

void CalculatorEngine::flipValueSign() {
  auto lastStepWrapper = m_historyModel->getLastStep();
  if (!lastStepWrapper.has_value()) {
    spdlog::debug("No number to flip sign.");
    return;
  }

  auto lastStep = lastStepWrapper.value().get();
  QString val = lastStep.value;
  if (val.isEmpty()) {
    spdlog::debug("No number to flip sign.");
    return;
  }

  QChar first = val.at(0);
  QString flipped = (first == '-') ? val.sliced(1) : QString("-").append(val);

  spdlog::debug("Before flipped: {}. After flipped: {}", val.toStdString(),
                flipped.toStdString());
  m_historyModel->updateLastStepVal(flipped);

  calculate();
}

// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
void CalculatorEngine::calculate() {
  if (m_historyModel->getSteps().isEmpty()) {
    if (m_resultStep.value == "0") {
      spdlog::debug("Same result value, skipping.");
      return;
    }

    m_resultStep.value = "0";
    emit resultStepChanged();
    return;
  }

  std::stack<double> nums;
  std::stack<OperationStep> steps;

  bool hasError = false;
  for (const auto &step : m_historyModel->getSteps()) {
    if (step.value == "") {
      continue;
    }

    auto converted = convertToNumber(step.value);
    if (!converted) {
      spdlog::debug("Invalid step value.");
      hasError = true;
      break;
    }
    double val = converted.value();

    if (steps.empty()) {
      nums.push(val);
      if (step.precedence != OperationPrecedence::NONE) {
        steps.push(step);
      }
      continue;
    }

    while (!steps.empty() && step.precedence <= steps.top().precedence) {
      if (!collapseTopOperation(nums, steps)) {
        spdlog::debug("Failed to collapse top operation.");
        hasError = true;
        break;
      }
    }

    if (hasError) {
      spdlog::debug("Had error, breaking.");
      break;
    }

    nums.push(val);
    steps.push(step);
  }

  if (!hasError) {
    while (!steps.empty()) {
      if (!collapseTopOperation(nums, steps)) {
        spdlog::debug("Failed to collapse top operation.");
        hasError = true;
        break;
      }
    }
  }

  QString result = "ERROR";
  spdlog::debug("hasError: {}, numsEmpty: {}, stepsEmpty: {}", hasError,
                nums.empty(), steps.empty());
  if (!hasError && !nums.empty()) {
    double res = nums.top();
    if (std::isinf(res)) {
      result = (res > 0) ? "INFINITY" : "-INFINITY";
    } else {
      result = formatNumber(res);
    }
  }

  if (m_resultStep.value == result) {
    spdlog::debug("Same result value, skipping.");
    return;
  };

  m_resultStep.value = result;
  emit resultStepChanged();
}

bool CalculatorEngine::collapseTopOperation(std::stack<double> &nums,
                                            std::stack<OperationStep> &steps) {
  // Added safety checks to prevent popping from empty stacks
  if (nums.size() < 2 || steps.empty()) {
    return false;
  }

  auto second = nums.top();
  nums.pop();
  auto first = nums.top();
  nums.pop();
  auto op = steps.top().op;
  steps.pop();

  auto calculated = doMath(first, second, op);
  if (!calculated) {
    return false;
  }

  nums.push(calculated.value());
  return true;
}

std::expected<double, std::string>
CalculatorEngine::doMath(const double &first, const double &second,
                         Operator op) {
  try {
    switch (op) {
    case Operator::PLUS:
      return first + second;
    case Operator::MINUS:
      return first - second;
    case Operator::MUL:
      return first * second;
    case Operator::DIV:
      return first / second;
    default:
      return std::unexpected("invalid precedence");
    }
  } catch (std::exception e) {
    return std::unexpected(e.what());
  }
}

void CalculatorEngine::debug() { m_historyModel->printDebug(); }

std::expected<double, std::string>
CalculatorEngine::convertToNumber(const QString &input) const {
  try {
    return std::stod(input.toStdString());
  } catch (std::invalid_argument e) {
    return std::unexpected("invalid value");
  } catch (std::out_of_range e) {
    return std::unexpected("the value is out of range");
  } catch (std::exception e) {
    return std::unexpected(e.what());
  }
}

QString CalculatorEngine::formatNumber(double num) {
  try {
    return QString::number(num, 'g', 30);
  } catch (std::exception e) {
    return QString();
  }
}
