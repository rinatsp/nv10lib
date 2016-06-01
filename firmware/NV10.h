enum NV10State
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_BUSY,
    STATE_HOLDING
};

enum MoneyType
{                   //pulse count
    MONEY_ERROR     =0,
    MONEY_CHF_10    =1,
    MONEY_CHF_20    =2,
    MONEY_CHF_50    =3,
    MONEY_CHF_100   =4,
    MONEY_EUR_5     =5,
    MONEY_EUR_10    =6,
    MONEY_EUR_20    =7,
    MONEY_EUR_50    =8,
    MONEY_EUR_100   =9
};

class NV10
{
    public:
        NV10(int vend, int inhibit, int escrow);
        void            begin();
        void            enabled(bool enabled);
        bool            accept();
        void            reject();
        void            handleInterrupt();
        NV10State     getState();
        MoneyType       getMoneyType();
        void            getMoneyName(char* name);
        float           getMoneyValue();
    private:
        int             _vend;
        int             _inhibit;
        int             _escrow;
        unsigned long   _lastInterrupt;
        int             _pulsecount;
        NV10State     _currentState;
};