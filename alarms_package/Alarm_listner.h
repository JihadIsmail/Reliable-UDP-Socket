#ifndef ALARM_LISTNER_H
#define ALARM_LISTNER_H


class Alarm_listner
{
    public:
        virtual void on_timeout(int alarm_id) = 0;
};

#endif // ALARM_LISTNER_H
