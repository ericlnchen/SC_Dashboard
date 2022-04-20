  // if(RPM == 11500){
  //   if(gear == 5){
  //     isUp = false;
  //   }
  //   else if(gear == 0){
  //     isUp = true;
  //   }
  //   if(isUp) gear += 1;
  //   else gear -= 1;
  //   driver.drawGear(gears[gear]);
  //   lastGearUpdate = millis();
  //   RPM = 0;
  // }
  // else {
  //  RPM+=100;    
  // }
  // driver.drawBoxGauge(RPM, 12000,cutoff,10000);
  
  // if (millis()-lastspeedUpdate>500){
  //   if (speed==99) {
  //     speed = 0;
  //   } else {
  //     speed++;
  //   }
  //   driver.drawMph(speed);
  //   lastspeedUpdate = millis();
  // }

  // if(millis()-lastMainCurrentUpdate > 2000){
  //   driver.drawElectricDiagnostics(0, mc, false);
  //   lastMainCurrentUpdate = millis();
  // }
  
  // if(millis()-lastFuelCurrentUpdate > 4000){
  //   driver.drawElectricDiagnostics(0, f, true);
  //   lastFuelCurrentUpdate = millis();
  // }

  // if(millis()-lastH20CurrentUpdate > 3000){
  //   driver.drawElectricDiagnostics(0, h, true);
  //   lastH20CurrentUpdate = millis();
  // }

  // if(millis()-lastFanCurrentUpdate > 5000){
  //   driver.drawElectricDiagnostics(0, fa, true);
  //   lastFanCurrentUpdate = millis();
  // }

  // if (millis()-lastLapTimeUpdate>=1000){
  //   int seconds = millis()/1000;
  //   // lapTime[2] = millis();
  //   lapTime[1] = seconds%60;
  //   lapTime[0] = (seconds/60)%60;
  //   driver.drawLapTime(lapTime);
  //   lastLapTimeUpdate = millis();
  // }

  // if (millis()-lastoil_pressureUpdate>3000){
  //   if(oil_pressure == 0){
  //     oil_pressure = 90;
  //   }
  //   if(oil_pressure >= 90){
  //     isWorking[3] = false;
  //     driver.functioning(oil_pressure, oP, false);
  //   }
  //   else if(oil_pressure <= 30){
  //     isWorking[3] = false;
  //     driver.functioning(oil_pressure, oP, false);
  //   }
  //   else{
  //     isWorking[3] = true;
  //     driver.functioning(oil_pressure, oP, true);
  //   }
  //   --oil_pressure;
  //   lastoil_pressureUpdate = millis();
  // }

  // if (millis()-lastVoltUpdate>1000){
  //   voltage=voltage+.1;
  //   if(voltage <= 0.0){
  //     voltage = 12.0;
  //   }
  //   else if(voltage >= 14.5){
  //     voltage = 10;
  //   }
  //   if(voltage <= 11.5){
  //     isWorking[1] = false;
  //     driver.drawElectricDiagnostics(voltage, b, false);
  //   }
  //   else{
  //     isWorking[1] = true;
  //     driver.drawElectricDiagnostics(voltage, b, true);
  //   }
  //   lastVoltUpdate = millis();
  // }

  // if (millis()-lastCTempUpdate>4000){
  //   if(coolant_temp == 0){
  //     coolant_temp = 99;
  //   }
  //   if(coolant_temp >= 100){
  //     isWorking[0] = false;
  //     driver.drawTempDiagnostics(coolant_temp, cT, false);
  //   }
  //   else{
  //     isWorking[0] = true;
  //     driver.drawTempDiagnostics(coolant_temp, cT, true);
  //   }
  //    --coolant_temp;
  //   lastCTempUpdate = millis();
  // }

  // if (millis()-lastOTempUpdate>4500){
  //   if(oil_temp == 0){
  //     oil_temp = 99;
  //   }
  //   if(oil_temp >= 100){
  //     isWorking[2] = false;
  //     driver.drawTempDiagnostics(oil_temp, oT, false);
  //   }
  //   else{
  //     isWorking[2] = true;
  //     driver.drawTempDiagnostics(oil_temp, oT, true);
  //   }
  //   --oil_temp;
  //   lastOTempUpdate = millis();
  // }