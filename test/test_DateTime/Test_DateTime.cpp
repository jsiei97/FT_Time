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

        void test_setTime_fail();
        void test_setTime_fail_data();

        void test_timeSince2000();
        void test_timeSince2000_data();

        void test_isoDateString();
        void test_isoDateString_data();
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

void Test_DateTime::test_setTime_fail_data()
{
    QTest::addColumn<QString>("quickDateString");

    //To short
    QTest::newRow("test") << "";
    QTest::newRow("test") << "2013-12-30";
    QTest::newRow("test") << "2013-12-31T";
    QTest::newRow("test") << "2013-12-29T2";
    QTest::newRow("test") << "2013-12-30T12";
    QTest::newRow("test") << "2014-01-06T09:";
    QTest::newRow("test") << "2014-01-06T09:0";
    QTest::newRow("test") << "2014-01-06T09:05";
    QTest::newRow("test") << "2014-01-06T09:05:";
    QTest::newRow("test") << "2014-01-06T09:05:0";
    QTest::newRow("test") << "2014-01-06T09:05:03";
    QTest::newRow("test") << "2014-01-06T09:05:03Z";
    QTest::newRow("test") << "2014-01-06T09:05:03Z_";

    //To long
    QTest::newRow("long") << "2014-01-06T09:05:03Z_1_";
    QTest::newRow("long") << "2014-01-06T09:05:03Z_1_kugjkhkhuh";


    //Bad syntax
    QTest::newRow("syntax") << "2014 01-06T09:05:03Z_1";
    QTest::newRow("syntax") << "2014-01 06T09:05:03Z_1";
    QTest::newRow("syntax") << "2014-01-06 09:05:03Z_1";
    QTest::newRow("syntax") << "2014-01-06T09 05:03Z_1";
    QTest::newRow("syntax") << "2014-01-06T09:05 03Z_1";
    QTest::newRow("syntax") << "2014-01-06T09:05:03 _1";
    QTest::newRow("syntax") << "2014-01-06T09:05:03Z 1";
    QTest::newRow("syntax") << "2014-01-06T09:05:03Z_ ";
}

/**
 * Test that we fail with bad data.
 */
void Test_DateTime::test_setTime_fail()
{
    QFETCH(QString, quickDateString);
    DateTime dt;
    quickDateString = quickDateString.trimmed();

    char* str = quickDateString.toAscii().data();
    //str[22]='\0'; //Fix test input!!!
    //qDebug() << str;

    QCOMPARE(dt.setTime(str), false);
}

void Test_DateTime::test_timeSince2000_data()
{
    QTest::addColumn<QString>("quickDateString");
    QTest::addColumn<unsigned int>("days");
    QTest::addColumn<unsigned int>("sec");

    //Seconds per day: 24*60*60=86400 (not counting leap seconds)

    QTest::newRow("2000-01-02 00:00:00") << "2000-01-02T00:00:00Z_1"
        << (unsigned int)(1)
        << (unsigned int)(1*24*60*60);

    QTest::newRow("test") << "2000-02-01T00:00:00Z_1"
        << (unsigned int)(31)
        << (unsigned int)(31*24*60*60);

    QTest::newRow("test") << "2000-03-01T00:00:00Z_1"
        << (unsigned int) (31+29)
        << (unsigned int)((31+29)*24*60*60);

    QTest::newRow("test") << "2001-01-01T00:00:00Z_1"
        << (unsigned int)(366)
        << (unsigned int)(366*24*60*60);

    QTest::newRow("test") << "2002-01-01T00:00:00Z_1"
        << (unsigned int) (366+365)
        << (unsigned int)((366+365)*24*60*60);

    unsigned int days = (366*3)+(365*7);
    QTest::newRow("test") << "2010-01-01T00:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)(days*24*60*60);

    //month: 1  2  3  4  5  6  7  8 // 9 10 11 12
    days += 31+28+31+30+31+30+31+31;//+30+31+30+31;
    QTest::newRow("test") << "2010-09-01T00:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)(days*24*60*60);

    days += 10;
    QTest::newRow("test") << "2010-09-11T00:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)(days*24*60*60);

    QTest::newRow("test") << "2010-09-11T14:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)((days*24*60*60)+(14*60*60));

    QTest::newRow("test") << "2010-09-11T14:23:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)((days*24*60*60)+(14*60*60)+(23*60));

    QTest::newRow("test") << "2010-09-11T14:23:12Z_1"
        << (unsigned int)(days)
        << (unsigned int)((days*24*60*60)+(14*60*60)+(23*60)+12);


    //And test with a leap year
    days = (366*3)+(365*9);
    QTest::newRow("test") << "2012-01-01T00:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)(days*24*60*60);

    //month: 1  2  3  4  5  6  7  8  9 10 11 // 12
    days += 31+29+31+30+31+30+31+31+30+31+30;//+31;
    QTest::newRow("test") << "2012-12-01T00:00:00Z_1"
        << (unsigned int)(days)
        << (unsigned int)(days*24*60*60);
}

void Test_DateTime::test_timeSince2000()
{
    QFETCH(QString, quickDateString);
    QFETCH(unsigned int, days);
    QFETCH(unsigned int, sec);

    DateTime dt;
    quickDateString = quickDateString.trimmed();

    char* str = quickDateString.toAscii().data();
    //qDebug() << str;

    QVERIFY(dt.setTime(str));

    QCOMPARE((unsigned int)dt.daySince2000(), days);
    QCOMPARE((unsigned int)dt.secSince2000(), sec);
}


void Test_DateTime::test_isoDateString_data()
{
    QTest::addColumn<QString>("quickDateString");
    QTest::addColumn<QString>("isoString");

    QTest::newRow("date01")
        << "2013-12-30T23:01:56Z_1"
        << "2013-12-30T23:01:56Z";

    QTest::newRow("date01")
        << "2015-09-30T20:05:58Z_1"
        << "2015-09-30T20:05:58Z";
}

void Test_DateTime::test_isoDateString()
{
    QFETCH(QString, quickDateString);
    QFETCH(QString, isoString);

    quickDateString = quickDateString.trimmed();

    DateTime dt;
    char* str = quickDateString.toAscii().data();
    //str[22]='\0'; //Fix test input!!!
    //qDebug() << str;

    char iso[21];

    QCOMPARE(dt.isoDateString(iso), false);
    QVERIFY(dt.setTime(str));
    QVERIFY(dt.isoDateString(iso));
    QCOMPARE(isoString, QString(iso));
}


QTEST_MAIN(Test_DateTime)
#include "Test_DateTime.moc"
