void tftShow(){

  if(updateMenu){
    // reset variable
    updateMenu = false;

    // do not reset screen while switchingColor 
    if(!(inData[3]==228)){
      tft.fillScreen(TFT_BLACK);    
    }
    if(inSetupMode){
       
    }
    else{
        tft.drawFastVLine(82,0,128, TFT_WHITE);
        tft.drawFastHLine(0,70,164, TFT_WHITE);
        if(switchColor){
           switch(menu){
            case 1:
              tft.drawBitmap( 58, 25, cup, cup_width,  cup_height, TFT_WHITE);
              break;
            case 2:
              tft.drawBitmap( 50, 21, cups, cups_width,  cups_height, TFT_WHITE);
              break;
            case 3:
              tft.drawBitmap( 30, 40, portalfilter, portalfilter_width,  portalfilter_height, TFT_WHITE);
              break;  
            }          
        }
        else{
           switch(menu){
            case 1:
              tft.drawBitmap( 58, 25, cup, cup_width,  cup_height, TFT_BLUE);
              break;
            case 2:
              tft.drawBitmap( 50, 21, cups, cups_width,  cups_height, TFT_BLUE);
              break;
            case 3:
              tft.drawBitmap( 30, 40, portalfilter, portalfilter_width,  portalfilter_height, TFT_BLUE);
              break;  
            }          
          
        }



    }
  }
  else if(updateTime){
    // reset variable
    updateTime = false;
    tft.setTextPadding(60);
    tft.setTextDatum(TR_DATUM);
    int poX = 100;
    int poY = 85;
    int font = 4;
    
    if(!inEditMode){
      tft.setTextColor( TFT_BLUE, TFT_BLACK);
      if(menu==3){
        tft.drawFloat((18000-t)/1000, 1, poX, poY, font);            
      }
      else{
        tft.drawFloat(t/1000, 1, poX, poY, font);  
        tft.drawNumber(t, 90, 115, 1);          
      }
    }
    else{
      tft.setTextColor( TFT_WHITE, TFT_BLACK );
      volatile float dT = doseTime[menu];
      tft.drawFloat(dT /100, 1, poX, poY, font);
    }
    
      tft.setTextDatum(TR_DATUM);
      tft.setTextPadding(40);
      
      tft.drawNumber(inData[0], 20, 3, 1);
      tft.drawNumber(inData[1], 20, 12, 1);
      tft.drawNumber(inData[2], 20, 21, 1);
      tft.drawNumber(inData[3], 20, 30, 1);
      tft.drawNumber(inData[4], 20, 39, 1);
      
      tft.drawNumber(inData[5], 20, 48, 1);
      tft.drawNumber(inData[6], 20, 57, 1);
      tft.drawNumber(inData[7], 20, 68, 1);
      tft.drawNumber(inData[8], 20, 77, 1);
      tft.drawNumber(inData[9], 20, 86, 1);

  }
  else if(updateButtons){
    if(!inEditMode){
      tft.drawBitmap( 12, 90, cup_small, cup_small_width,  cup_small_height, TFT_WHITE);
        tft.drawBitmap( 130, 90, cups_small, cups_small_width,  cups_small_height, TFT_WHITE);
      }
    else{
        tft.drawBitmap( 10, 90, minus, minus_width,  minus_height, TFT_WHITE);
        tft.drawBitmap( 130, 90, plus, plus_width,  plus_height, TFT_WHITE);
      }
  }
  
}
