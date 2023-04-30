#include "EEGStatus.h"

/**
 * @brief EEGStatus::EEGStatus
 * @param parent
 */
EEGStatus::EEGStatus(QWidget *parent) :
    QChartView(parent)
  , m_chart(new QChart)
  , m_attention(new QBarSeries)
  , m_meditaion(new QBarSeries)
  , m_timer(new QTimer(this))
  // , m_poorsignal(new QBarSeries)
{
    QBarSet *attention  = new QBarSet(tr("Attention"));
    QBarSet *meditaion  = new QBarSet(tr("Meditaion"));
    // QBarSet *poorsignal = new QBarSet(tr("Poor Signal"));

    *attention  << 10;
    *meditaion  << 20;
    // *poorsignal << 30;

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 100);
    QBarCategoryAxis *axisX = new QBarCategoryAxis;
    QStringList names;
    names << tr("Attention") << tr("Meditaion");
    axisX->append(names);
    m_chart->addAxis(axisY, Qt::AlignLeft);
    m_chart->addAxis(axisX, Qt::AlignBottom);

    m_attention->setBarWidth(.9);
    m_meditaion->setBarWidth(.9);
    // m_poorsignal->setBarWidth(.5);

    m_attention->append(attention);
    m_meditaion->append(meditaion);
    // m_poorsignal->append(poorsignal);

    m_chart->addSeries(m_attention);
    m_chart->addSeries(m_meditaion);
    // m_chart->addSeries(m_poorsignal);

    m_attention->attachAxis(axisY);
    m_meditaion->attachAxis(axisY);
    // m_poorsignal->attachAxis(axisY);

    this->setChart(m_chart);
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    this->setRenderHint(QPainter::Antialiasing);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(reRead()));
    m_timer->start(2000);
}

/**
 * @brief EEGStatus::~EEGStatus
 */
EEGStatus::~EEGStatus() {
    delete m_attention;
    delete m_meditaion;
    delete m_chart;
}

/**
 * @brief EEGStatus::setAttention
 * @param attention
 */
void EEGStatus::setAttention(uint8_t attention) {
    qDebug() << "Attention" << attention;
    m_queueAttention.push(attention);

}

/**
 * @brief EEGStatus::setMeditation
 * @param meditation
 */
void EEGStatus::setMeditation(uint8_t meditation) {
    qDebug() << "Meditation" << meditation;
    m_queueMeditation.push(meditation);
}

/**
 * @brief EEGStatus::setPoorSignal
 * @param signal
 */
void EEGStatus::setPoorSignal(uint8_t signal) {
    qDebug() << "Poor Signal" << signal;
    // m_poorsignal->barSets().at(0)->replace(0, signal);
}

void EEGStatus::setBlinkStrength(uint8_t blinkStrength) {
    qDebug() << "Blink Strength" << blinkStrength;
}

void EEGStatus::reRead(void) {
    if (!m_queueAttention.empty()) {
        m_attention->barSets().at(0)->replace(0, m_queueAttention.front());
        while(!m_queueAttention.empty()) {
            m_queueAttention.pop();
        }

    }

    if (!m_queueMeditation.empty()) {
        m_meditaion->barSets().at(0)->replace(0, m_queueMeditation.front());
        while(!m_queueMeditation.empty()) {
            m_queueMeditation.pop();
        }
    }
}
