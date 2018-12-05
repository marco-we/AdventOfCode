#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <map>
#include <cassert>
using namespace std;

enum Event
{
    FALLS_ASLEEP, BEGINS_SHIFT, WAKES_UP
};

struct GuardEvent
{
    GuardEvent() {}

    GuardEvent(int _action, int _id, int _year, int _month, int _day, int _hour, int _minute) :
        action(_action), id(_id), year(_year), month(_month), day(_day), hour(_hour), minute(_minute) {}

    int16_t action;
    int16_t id;
    int32_t year;
    int8_t month;
    int8_t day;
    int8_t hour;
    int8_t minute;
};

struct GuardDuty
{
    int16_t guardId;
    int16_t totalMinutesAsleep;
    int16_t minutes[60];
};


const char* action_to_string(int16_t action)
{
    switch (action)
    {
        case FALLS_ASLEEP: return "FALLS ASLEEP";
        case BEGINS_SHIFT: return "BEGINS SHIFT";
        case WAKES_UP: return "WAKES UP";
        default: return "ERROR";
    }
}

void print(ofstream& of, const GuardEvent& ev)
{
    of << ev.year << "/" << (int)ev.month << "/" << (int)ev.day <<
        "  " << (int)ev.hour << ":" << (int)ev.minute << "  [" << (int)ev.id << "](" << action_to_string(ev.action) << ")" << endl;
}

bool sort_date(const GuardEvent& a, const GuardEvent& b)
{
    if (a.year < b.year)
        return true;
    if (a.year == b.year)
    {
        if (a.month < b.month)
            return true;
        if (a.month == b.month)
        {
            if (a.day < b.day)
                return true;
            if (a.day == b.day)
            {
                if (a.hour < b.hour)
                    return true;
                if (a.hour == b.hour &&
                    a.minute < b.minute)
                    return true;
            }
        }
    }
    return false;
}



int main(int argc, char** argv)
{
    ifstream file("input.txt");
    if (!file.is_open())
    {
        return EXIT_FAILURE;
    }

    vector<GuardEvent> guard_events;
    while (!file.eof())
    {
        string s;
        getline(file, s);

        if (file)
        {
            int year, month, day, hour, minute;
            GuardEvent ge;

            if (s.find("falls asleep") != string::npos)
            {
                if (sscanf(s.c_str(), "[%d-%d-%d %d:%d]", &year, &month, &day, &hour, &minute) != 5)
                {
                    return EXIT_FAILURE;
                }

                ge = GuardEvent(FALLS_ASLEEP, -1, year, month, day, hour, minute);
            }
            else if (s.find("wakes up") != string::npos)
            {
                if (sscanf(s.c_str(), "[%d-%d-%d %d:%d]", &year, &month, &day, &hour, &minute) != 5)
                {
                    return EXIT_FAILURE;
                }

                ge = GuardEvent(WAKES_UP, -1, year, month, day, hour, minute);
            }
            else if (s.find("begins shift") != string::npos)
            {
                int guard;
                if (sscanf(s.c_str(), "[%d-%d-%d %d:%d] Guard #%d begins shift", &year, &month, &day, &hour, &minute, &guard) != 6)
                {
                    return EXIT_FAILURE;
                }

                ge = GuardEvent(BEGINS_SHIFT, guard, year, month, day, hour, minute);
            }
            else
            {
                return EXIT_FAILURE;
            }

            guard_events.push_back(ge);
        }
    }

    sort(guard_events.begin(), guard_events.end(), sort_date);

    int16_t guard_id = -1;
    int8_t asleep_minute = -1;

    int16_t max_minutes_asleep = 0;
    int16_t sleepiest_guard = -1;

    int16_t sleepiest_minute_counter[60] = { 0 };

    map<int16_t, GuardDuty> duties;

    const size_t num_actions = guard_events.size();
    for (size_t i = 0; i < num_actions; ++i)
    {
        const GuardEvent ge = guard_events[i];
        switch (ge.action)
        {
        case BEGINS_SHIFT:
        {
            guard_id = ge.id;
            break;
        }
        case FALLS_ASLEEP:
        {
            asleep_minute = ge.minute;
            guard_events[i].id = guard_id;
            break;
        }
        case WAKES_UP:
        {
            guard_events[i].id = guard_id;

            GuardDuty duty = { 0 };

            auto iter = duties.find(guard_id);
            if (iter != duties.end())
            {
                duty = iter->second;
                assert(guard_id == duty.guardId);
            }

            duty.guardId = guard_id;
            for (int8_t j = asleep_minute; j < ge.minute; ++j)
            {
                sleepiest_minute_counter[j]++;
                duty.minutes[j]++;
                duty.totalMinutesAsleep++;
            }

            duties[guard_id] = duty;

            if (duty.totalMinutesAsleep > max_minutes_asleep)
            {
                sleepiest_guard = guard_id;
                max_minutes_asleep = duty.totalMinutesAsleep;
            }

            break;
        }
        }
    }

    // part 2

    int8_t sleepiest_minute = -1;
    int16_t max_sleepiest_minute = 0;

    for (int i = 0; i < 60; ++i)
    {
        if (sleepiest_minute_counter[i] > max_sleepiest_minute)
        {
            max_sleepiest_minute = sleepiest_minute_counter[i];
            sleepiest_minute = i;
        }
    }

    sleepiest_guard = -1;
    max_minutes_asleep = 0;
    for (const auto& duty : duties)
    {
        if (duty.second.minutes[sleepiest_minute] > max_minutes_asleep)
        {
            sleepiest_guard = duty.first;
            max_minutes_asleep = duty.second.minutes[sleepiest_minute];
        }
    }

    return EXIT_SUCCESS;
}