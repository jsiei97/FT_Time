/*
 * Copyright (C) 2013 Johan Simonsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore>
#include <QtTest>

#include "DateTime.h"

class Test_DateTime : public QObject
{
    Q_OBJECT

    private:
    public:

    private slots:
        void test_bcd2uint();
        void test_bcd2uint_data();

        void test_str2bcd();
        void test_str2bcd_data();

        void test_setTime();
        void test_setTime_data();
};

void Test_DateTime::test_bcd2uint_data()
{
    QTest::addColumn<unsigned int>("bcd");
    QTest::addColumn<unsigned int>("uint");

    QTest::newRow("test") << (unsigned int)0x01 << (unsigned int)1;
    QTest::newRow("test") << (unsigned int)0x43 << (unsigned int)43;
    QTest::newRow("test") << (unsigned int)0x92 << (unsigned int)92;
}

void Test_DateTime::test_bcd2uint()
{
    QFETCH(unsigned int, uint);
    QFETCH(unsigned int, bcd);

    DateTime dt;
    unsigned int result = dt.bcd2uint((uint8_t)bcd);

    //qDebug() << result << uint << bcd;
    QCOMPARE(result, uint);
}

void Test_DateTime::test_str2bcd_data()
{
    QTest::addColumn<unsigned int>("bcd");
    QTest::addColumn<char>("ch0");
    QTest::addColumn<char>("ch1");

    QTest::newRow("test") << (unsigned int)0x01 << '0' << '1';
    QTest::newRow("test") << (unsigned int)0x10 << '1' << '0';
    QTest::newRow("test") << (unsigned int)0x36 << '3' << '6';

    //Non in values
    QTest::newRow("test") << (unsigned int)0 << 'E' << 'D';
}

void Test_DateTime::test_str2bcd()
{
    QFETCH(unsigned int, bcd);
    QFETCH(char, ch0);
    QFETCH(char, ch1);

    DateTime dt;
    unsigned int result = dt.str2bcd(ch0, ch1);

    //qDebug() << result << uint << bcd;
    QCOMPARE(result, bcd);
}

void Test_DateTime::test_setTime_data()
{
    QTest::addColumn<QString>("quickDateString");

    QTest::newRow("test") << "2013-12-30T23:01:56Z_1";
    QTest::newRow("test") << "2013-12-31T16:11:42Z_2";

    QTest::newRow("test") << "2013-12-29T20:24:45Z_7";
    QTest::newRow("test") << "2013-12-30T12:12:09Z_1";
    QTest::newRow("test") << "2014-01-06T09:05:03Z_1";

    /// @todo some negative tests that bad string fail!!!
}

void Test_DateTime::test_setTime()
{
    QFETCH(QString, quickDateString);
    DateTime dt;
    quickDateString = quickDateString.trimmed();

    char* str = quickDateString.toAscii().data();
    //str[22]='\0'; //Fix test input!!!
    //qDebug() << str;

    QVERIFY(dt.setTime(str));

    // compare y-m-d
    QCOMPARE(quickDateString.mid(2,2).toInt(), (int)dt.bcd2uint(dt.year));
    QCOMPARE(quickDateString.mid(5,2).toInt(), (int)dt.bcd2uint(dt.month));
    QCOMPARE(quickDateString.mid(8,2).toInt(), (int)dt.bcd2uint(dt.day));

    // compare h:m:s
    QCOMPARE(quickDateString.mid(11,2).toInt(), (int)dt.bcd2uint(dt.hour));
    QCOMPARE(quickDateString.mid(14,2).toInt(), (int)dt.bcd2uint(dt.min));
    QCOMPARE(quickDateString.mid(17,2).toInt(), (int)dt.bcd2uint(dt.sec));

    // compare weekday
    QCOMPARE(quickDateString.mid(21,1).toInt(), (int)dt.bcd2uint(dt.dow));
}


QTEST_MAIN(Test_DateTime)
#include "Test_DateTime.moc"
