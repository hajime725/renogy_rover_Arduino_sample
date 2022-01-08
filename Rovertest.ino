//RJ12のピン配列
//1 白 TX
//2 黒 RX
//3 赤 GND
//4 緑 GND
//5 黄 +15V
//6 青 +15V


#define MODBUS_SERIAL Serial2   //renogy Roverをつないでいるポート
#include "RenogyRover.h"        //なぜかライブラリとして認識しなかったので。
                                //ほかにModbusMaster.hが必要。Arduinoのライブラリを管理から入手可能
RenogyRover rover;
PanelState panel;
BatteryState battery;
DayStatistics daystat;
HistStatistics histstat;
ChargingState chargestat;
char* model="0000000000000000";//16バイトなので15文字
FaultCode* faultcode;
int numerr = 0;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);   //roverの純正通信ユニット(BT-1)は9600bps
  rover.begin(9600);
  
  if(rover.getProductModel(model))      //型番取得（RNG-CTRL-RVR20）とか
  {
    Serial.println(model);
  }
  else
  {
    Serial.println("modelgetERROR");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("------------------------------------------------");
  if(rover.getPanelState(&panel))      //パネル状態取得
  {
    Serial.print("PV=,");
    Serial.print(panel.voltage);
    Serial.print(",V, PI=,");
    Serial.print(panel.current);
    Serial.print(",A, PP=,");
    Serial.print(panel.chargingPower);
    Serial.println(",W");
  }
  else
  {
    Serial.println("panelgetERROR");
  }

  if(rover.getBatteryState(&battery))      //バッテリ状態取得
  {
    Serial.print("BV=,");
    Serial.print(battery.batteryVoltage);
    Serial.print(",V, BchgI=,");
    Serial.print(battery.chargingCurrent);
    Serial.print(",A, CHGCap=,");
    Serial.print(battery.stateOfCharge);
    Serial.print(",%, CTRLTMP=,");
    Serial.print(battery.controllerTemperature);
    Serial.print(",C, BTMP=,");
    Serial.print(battery.batteryTemperature);
    Serial.println(",C");
  }
  else
  {
    Serial.println("batterygetERROR");
  }

  if(rover.getDayStatistics(&daystat))      //今日の統計
  {
    Serial.print("DAY:minBV=,");
    Serial.print(daystat.batteryVoltageMinForDay);
    Serial.print(",V, maxBV=,");
    Serial.print(daystat.batteryVoltageMaxForDay);
    Serial.print(",V, maxchgI=,");
    Serial.print(daystat.maxChargeCurrentForDay);
    Serial.print(",A, maxDischgI=,");
    Serial.print(daystat.maxDischargeCurrentForDay);
    Serial.print(",A, maxchgP=,");
    Serial.print(daystat.maxChargePowerForDay);
    Serial.print(",W, maxDischgP=,");
    Serial.print(daystat.maxDischargePowerForDay);
    Serial.print(",W, chgAH=,");
    Serial.print(daystat.chargingAmpHoursForDay);
    Serial.print(",AH, DischgAH=,");
    Serial.print(daystat.dischargingAmpHoursForDay);
    Serial.print(",AH, GenWH=,");
    Serial.print(daystat.powerGenerationForDay);
    Serial.print(",WH, 消費電力量=,");
    Serial.print(daystat.powerConsumptionForDay);
    Serial.println(",WH");
  }
  else
  {
    Serial.println("daystatgetERROR");
  }

  if(rover.getHistoricalStatistics(&histstat))      //歴代統計
  {
    Serial.print("稼働日数=,");
    Serial.print(histstat.operatingDays);
    Serial.print(",d, 過放電=,");
    Serial.print(histstat.batOverDischarges);
    Serial.print(", 満充電=,");
    Serial.print(histstat.batFullCharges);
    Serial.print(", TotalCHGAH=,");
    Serial.print(histstat.batChargingAmpHours);
    Serial.print(",AH, TotalDISCHGAH=,");
    Serial.print(histstat.batDischargingAmpHours);
    Serial.print(",AH, 総発電量=,");
    Serial.print(histstat.powerGenerated);
    Serial.print(",kWH, 総消費電力量=,");
    Serial.print(histstat.powerConsumed);
    Serial.println(",kWH");
  }
  else
  {
    Serial.println("histstatgetERROR");
  }

  if(rover.getChargingState(&chargestat))      //充電状況
  {
    Serial.print("日光状態=,");
    Serial.print(chargestat.streetLightState);
    Serial.print(", 日光明度=,");
    Serial.print(chargestat.streetLightBrightness);
    Serial.print(", 充電モード=,");
    switch(chargestat.chargingMode)
    {
      case UNDEFINED:
        Serial.print("未定義");
        break;
      case DEACTIVATED:
        Serial.print("DEACTIVATED");
        break;
      case ACTIVATED:
        Serial.print("ACTIVATED");
        break;
      case MPPT:
        Serial.print("MPPT");
        break;
      case EQUALIZING:
        Serial.print("EQUALIZING");
        break;
      case BOOST:
        Serial.print("BOOST");
        break;
      case FLOATING:
        Serial.print("FLOATING");
        break;
      case OVERPOWER:
        Serial.print("OVERPOWER");
        break;
      default:
        Serial.print("未定義");
    }
    
    Serial.println("");
  }
  else
  {
    Serial.println("chargestatgetERROR");
  }
/*
 * よくわからなかった
 * int getErrors(FaultCode*& errors, int& numErrors);
 * 
  if(rover.getErrors(*faultcode,&numerr))      //エラー取得
  {
    
  }
  else
  {
    Serial.println("Error get ERROR");
  }
*/
  //int setStreetLight(int state);    //こんなのもあるよ
  delay(500);
}
