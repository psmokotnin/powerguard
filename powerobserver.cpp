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
#include "powerobserver.h"

#include <IOKitLib.h>
#include <ps/IOPowerSources.h>
#include <ps/IOPSKeys.h>

PowerObserver::PowerObserver(QObject *parent) : QObject(parent), m_timer(new QTimer(this)),
    m_charge(0), m_threshold(80)
{
    update();
    connect(m_timer, &QTimer::timeout, this, &PowerObserver::update);
    m_timer->start(1000);
}

void PowerObserver::update()
{
    CFTypeRef info = IOPSCopyPowerSourcesInfo();
    if (auto sl = IOPSCopyPowerSourcesList(info)) {
        auto sd = IOPSGetPowerSourceDescription(info, CFArrayGetValueAtIndex(sl, 0));

        auto sourceState = (CFStringRef)CFDictionaryGetValue(sd, CFSTR(kIOPSPowerSourceStateKey));
        if (kCFCompareEqualTo == CFStringCompare(sourceState, CFSTR(kIOPSACPowerValue), kCFCompareEqualTo)) {
            setSource(AC);
        } else {
            setSource(BATTERY);
        }

        auto key = CFSTR(kIOPSCurrentCapacityKey);
        auto value = (CFNumberRef)CFDictionaryGetValue(sd, key);
        int charge(0);
        CFNumberGetValue(value, kCFNumberIntType, &charge);
        setCharge(charge);
        CFRelease(sl);
    }
    CFRelease(info);
}

void PowerObserver::setSource(const POWER_SOURCE &power)
{
    if (m_source != power) {
        m_source = power;
        emit sourceChanged();
    }
}

void PowerObserver::setCharge(int charge)
{
    if (m_charge != charge) {
        m_lastCharge = m_charge;
        m_charge = charge;

        if (m_charge <= m_threshold && m_lastCharge > m_threshold) {
            emit thresholdPassed();
        }
        emit chargeChanged();
    }
}

void PowerObserver::setThreshold(int threshold)
{
    if (m_threshold != threshold) {
        m_threshold = threshold;
        emit thresholdChanged();
    }
}
