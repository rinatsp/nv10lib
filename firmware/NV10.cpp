#include "NV10.h"
#include "application.h"

NV10::NV10(int vend, int inhibit, int escrow)
{
    _vend=vend;
    _inhibit=inhibit;
    _escrow=escrow;
    _pulsecount=0;
    _lastInterrupt = 0;
}

void NV10::begin()
{
    pinMode(_vend, INPUT);
    pinMode(_escrow,OUTPUT);
    pinMode(_inhibit,OUTPUT);
    
    digitalWrite(_escrow,HIGH);
    digitalWrite(_inhibit,LOW);
}

void NV10::enabled(bool enabled)
{
   digitalWrite(_inhibit,enabled);
   if(enabled)
   {
       _currentState=STATE_ENABLED;
   }else
   {
       _currentState=STATE_DISABLED;
   }
}

void NV10::handleInterrupt()
{
    unsigned long now = millis();
    if((now-_lastInterrupt)>1000)// länger als eine Sekunde seit dem letzten Puls => neue Note
    {
        _pulsecount=1;
    }else
    {
        _pulsecount++;
    }
    _lastInterrupt=now;
    _currentState=STATE_BUSY;
}

NV10State NV10::getState()
{
    unsigned long now = millis();
    
    if(_currentState==STATE_BUSY)
    {
        
        if((now-_lastInterrupt)>500) //no more pulse arrived => holding the money
        {
            _currentState = STATE_HOLDING;
        }
    }
    if(_currentState==STATE_HOLDING)
    {
        if((now-_lastInterrupt)>20000) //timeout, reject note
        {
            reject();
            _currentState = STATE_ENABLED;
        }
    }
    
    return _currentState;
}

MoneyType NV10::getMoneyType()
{
    if(getState()==STATE_HOLDING)
    {
        return (MoneyType)_pulsecount;
    }else
    {
        return MONEY_ERROR;
    }
}

bool NV10::accept()
{
    //nur ausführen, wenn er auch eine Note hat
    if(getState()==STATE_HOLDING)
    {
        digitalWrite(_escrow,LOW);
        delay(20);
        digitalWrite(_escrow,HIGH);
        return true;
    }else
    {
        //keine Note, oder bereits Timeout
        return false;
    }
}

void NV10::reject()
{
    digitalWrite(_inhibit,LOW);
    delay(20);
    digitalWrite(_inhibit,HIGH);
}

void NV10::getMoneyName(char* name)
{

   if(_currentState==STATE_HOLDING)
   {
       switch(_pulsecount)
       {
            case MONEY_ERROR:sprintf(name,"0CHF");break;
            case MONEY_CHF_10:sprintf(name,"10CHF");break;
            case MONEY_CHF_20:sprintf(name,"20CHF");break;
            case MONEY_CHF_50:sprintf(name,"50CHF");break;
            case MONEY_CHF_100:sprintf(name,"100CHF");break;
            case MONEY_EUR_5:sprintf(name,"5EUR");break;
            case MONEY_EUR_10:sprintf(name,"10EUR");break;
            case MONEY_EUR_20:sprintf(name,"20EUR");break;
            case MONEY_EUR_50:sprintf(name,"50EUR");break;
            case MONEY_EUR_100 :sprintf(name,"100EUR");break;
       }
   }else
   {
       sprintf(name,"0CHF");
   }
}

float NV10::getMoneyValue()
{
    if(_currentState==STATE_HOLDING)
   {
       switch(_pulsecount)
       {
            case MONEY_ERROR:   return 0;
            case MONEY_CHF_10:  return 10;
            case MONEY_CHF_20:  return 20;
            case MONEY_CHF_50:  return 50;
            case MONEY_CHF_100: return 100;
            case MONEY_EUR_5:   return 5.5;
            case MONEY_EUR_10:  return 11;
            case MONEY_EUR_20:  return 22;
            case MONEY_EUR_50:  return 55;
            case MONEY_EUR_100: return 110;
       }
   }else
   {
       return 0;
   }
}