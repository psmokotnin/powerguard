/**
 *  Power guard
 *  Copyright (C) 2020  Pavel Smokotnin

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef POWERTRAYICON_H
#define POWERTRAYICON_H

#include <QTimer>

class PowerObserver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(POWER_SOURCE source READ source NOTIFY sourceChanged)
    Q_PROPERTY(int charge READ charge NOTIFY chargeChanged)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)

public:
    explicit PowerObserver(QObject *parent = nullptr);

    enum POWER_SOURCE {
        AC, BATTERY
    };
    Q_ENUMS(POWER_SOURCE)


    Q_INVOKABLE POWER_SOURCE source() const {return  m_source;}
    void setSource(const POWER_SOURCE &power);

    Q_INVOKABLE int charge() const {return m_charge;}
    void setCharge(int charge);

    int threshold() const {return m_threshold;}
    void setThreshold(int threshold);

private:
    QTimer *m_timer;
    POWER_SOURCE m_source;
    int m_charge, m_lastCharge;
    int m_threshold;

public slots:
    void update();

signals:
    void sourceChanged();
    void chargeChanged();
    void thresholdChanged();
    void thresholdPassed();

};

#endif // POWERTRAYICON_H
