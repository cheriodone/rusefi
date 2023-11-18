#include "pch.h"

#include "defaults.h"
#include "vr_pwm.h"
#include "kline.h"
#if HW_PROTEUS
#include "proteus_meta.h"
#endif // HW_PROTEUS

#if EFI_ALTERNATOR_CONTROL
static void setDefaultAlternatorParameters() {
	engineConfiguration->targetVBatt = 14;

	engineConfiguration->alternatorControl.offset = 0;
	engineConfiguration->alternatorControl.pFactor = 30;
	engineConfiguration->alternatorControl.periodMs = 100;
}
#endif // EFI_ALTERNATOR_CONTROL

void setGDIFueling() {
	// Use high pressure sensor
	engineConfiguration->injectorPressureType = IPT_High;
	// Automatic compensation of injector flow based on rail pressure
	engineConfiguration->injectorCompensationMode = ICM_SensedRailPressure;
	// Reference rail pressure is 10 000 kPa = 100 bar
	engineConfiguration->fuelReferencePressure = 10000;
	//setting "flat" 0.2 ms injector's lag time
	setArrayValues(engineConfiguration->injector.battLagCorr, 0.2);

	setTable(config->injectionPhase, -200.0f);
	engineConfiguration->injectionTimingMode = InjectionTimingMode::Center;
    engineConfiguration->isPhaseSyncRequiredForIgnition = true;
}

/* Cylinder to bank mapping */
void setLeftRightBanksNeedBetterName() {
    for (size_t i = 0; i < engineConfiguration->cylindersCount; i++) {
	    engineConfiguration->cylinderBankSelect[i] = i % 2;
    }
}

static void setDefaultHPFP() {
#if ! EFI_UNIT_TEST
    // unit tests rely on 'hpfpCamLobes' for isGdiEngine() and we need not-GDI by default for unit tests
	engineConfiguration->hpfpCamLobes = 3;
#endif

// todo: would be nice for unit tests to be happy about these defaults
#if EFI_PROD_CODE
	engineConfiguration->hpfpPumpVolume = 0.290;
#endif
	engineConfiguration->hpfpMinAngle = 10;
	engineConfiguration->hpfpActivationAngle = 30;
	engineConfiguration->hpfpTargetDecay = 2000;
	engineConfiguration->hpfpPidP = 0.01;
	engineConfiguration->hpfpPidI = 0.0003;
	engineConfiguration->hpfpPeakPos = 10;
}

void setDefaultBaseEngine() {
	// Base Engine Settings
	engineConfiguration->displacement = 2;
	setInline4();

	engineConfiguration->compressionRatio = 9;
	engineConfiguration->vssFilterReciprocal = VSS_FILTER_MIN;
	engineConfiguration->boardUseCanTerminator = true;

    setDefaultHPFP();

    engineConfiguration->acrRevolutions = 5;

    engineConfiguration->watchOutForLinearTime = true;

	engineConfiguration->turbochargerFilter = 0.01f;

	engineConfiguration->fuelAlgorithm = LM_SPEED_DENSITY;
	// let's have valid default while we still have the field
	engineConfiguration->debugMode = DBG_MAP;

	engineConfiguration->boostCutPressure = 300;
	engineConfiguration->boostCutPressureHyst = 20;

	engineConfiguration->primingDelay = 0.5;
	engineConfiguration->vvtControlMinRpm = 500.0;

    engineConfiguration->camDecoder2jzPosition = 95;
    engineConfiguration->camDecoder2jzPrecision = 40;

	// Limits and Fallbacks
	engineConfiguration->rpmHardLimit = 7000;
	engineConfiguration->rpmHardLimitHyst = 50;
	engineConfiguration->cutFuelOnHardLimit = true;
	engineConfiguration->cutSparkOnHardLimit = true;
	engineConfiguration->etbRevLimitRange = 250;

	engineConfiguration->ALSMinRPM = 400;
	engineConfiguration->ALSMaxRPM = 3200;
	engineConfiguration->ALSMaxDuration = 3.5;
	engineConfiguration->ALSMaxCLT = 105;
//	engineConfiguration->alsMinPps = 10;
	engineConfiguration->alsMinTimeBetween = 5;
	engineConfiguration->alsEtbPosition = 30;
	engineConfiguration->ALSMaxTPS = 5;

	// Trigger
	engineConfiguration->trigger.type = trigger_type_e::TT_TOOTHED_WHEEL_60_2;

#if EFI_SIMULATOR
	engineConfiguration->vvtMode[0] = VVT_SINGLE_TOOTH;
	engineConfiguration->vvtOffsets[0] = 450;
	engineConfiguration->vvtPins[0] = Gpio::I0; // a random unused pin needed to unblock startSimplePwmExt()
#endif // EFI_SIMULATOR

#if EFI_SIMULATOR
	// R
	config->tcuSolenoidTable[0][0] = 99;
	config->tcuSolenoidTable[0][1] = 1;
	config->tcuSolenoidTable[0][2] = 2;
	config->tcuSolenoidTable[0][3] = 3;
	config->tcuSolenoidTable[0][4] = 4;
	config->tcuSolenoidTable[0][5] = 5;
	// P/N
	config->tcuSolenoidTable[1][0] = 10;
	config->tcuSolenoidTable[1][1] = 11;
	config->tcuSolenoidTable[1][2] = 12;
	config->tcuSolenoidTable[1][3] = 13;
	config->tcuSolenoidTable[1][4] = 14;
	config->tcuSolenoidTable[1][5] = 15;
	config->tcuSolenoidTable[1][6] = 16;
	config->tcuSolenoidTable[1][7] = 17;
	config->tcuSolenoidTable[1][8] = 18;
	config->tcuSolenoidTable[1][9] = 19;
	// 1
	config->tcuSolenoidTable[2][0] = 20;
	config->tcuSolenoidTable[2][1] = 21;
	config->tcuSolenoidTable[2][2] = 22;
	// 2
	config->tcuSolenoidTable[3][0] = 30;
	config->tcuSolenoidTable[3][1] = 31;
	config->tcuSolenoidTable[3][3] = 33;
	// 3
	config->tcuSolenoidTable[4][0] = 40;
	config->tcuSolenoidTable[4][1] = 41;
	config->tcuSolenoidTable[4][4] = 44;
	// 4
	config->tcuSolenoidTable[5][0] = 50;
	config->tcuSolenoidTable[5][1] = 51;
	config->tcuSolenoidTable[5][5] = 55;

	config->scriptTable4[0][0] = 140;
	config->scriptTable4[0][1] = 141;
	config->scriptTable4[0][2] = 142;
	config->scriptTable4[0][3] = 143;
	config->scriptTable4[0][4] = 144;
	config->scriptTable4[0][5] = 145;

	config->scriptTable4[1][0] = 240;
   	config->scriptTable4[1][1] = 241;
   	config->scriptTable4[1][2] = 242;
   	config->scriptTable4[1][3] = 243;
   	config->scriptTable4[1][4] = 244;
   	config->scriptTable4[1][5] = 245;

	config->scriptTable4[4][0] = 40;
	config->scriptTable4[4][2] = 41;
	config->scriptTable4[4][3] = 42;
	config->scriptTable4[4][4] = 43;
	config->scriptTable4[4][5] = 44;
	config->scriptTable4[4][5] = 45;

	config->scriptTable4[5][0] = 50;
	config->scriptTable4[5][1] = 51;
	config->scriptTable4[5][2] = 52;
	config->scriptTable4[5][3] = 53;
	config->scriptTable4[5][4] = 54;
	config->scriptTable4[5][5] = 55;

#endif // EFI_SIMULATOR

	engineConfiguration->globalTriggerAngleOffset = 0;

	// Default this to on - if you want to diagnose, turn it off.
	engineConfiguration->silentTriggerError = true;

	engineConfiguration->idleStepperReactionTime = 3;
	engineConfiguration->idleStepperTotalSteps = 200;
	engineConfiguration->stepperForceParkingEveryRestart = true;
	engineConfiguration->iacByTpsTaper = 2;

    engineConfiguration->etbSplit = MAX_TPS_PPS_DISCREPANCY;

	// Advanced Trigger

	// Battery and alternator
	engineConfiguration->vbattDividerCoeff = ((float) (15 + 65)) / 15;

#if EFI_ALTERNATOR_CONTROL
	setDefaultAlternatorParameters();
#endif /* EFI_ALTERNATOR_CONTROL */

	// Fuel pump
	engineConfiguration->startUpFuelPumpDuration = 4;

	engineConfiguration->kLineBaudRate = KLINE_BAUD_RATE;

	engineConfiguration->benchTestOnTime = 4;
	engineConfiguration->benchTestOffTime = 500;
	engineConfiguration->benchTestCount = 3;

	// Fans
	engineConfiguration->fanOnTemperature = 95;
	engineConfiguration->fanOffTemperature = 91;
	engineConfiguration->fan2OnTemperature = 95;
	engineConfiguration->fan2OffTemperature = 91;

	// Tachometer
	// 50% duty cycle is the default for tach signal
	engineConfiguration->tachPulseDurationAsDutyCycle = true;
	engineConfiguration->tachPulseDuractionMs = 0.5;
	engineConfiguration->tachPulsePerRev = 1;

	engineConfiguration->etbMinimumPosition = 1;
	engineConfiguration->etbMaximumPosition = 100;

	engineConfiguration->tcuInputSpeedSensorTeeth = 1;
	engineConfiguration->issFilterReciprocal = 2;

	// Check engine light
#if EFI_PROD_CODE
	engineConfiguration->warningPeriod = 10;
#else
	engineConfiguration->warningPeriod = 0;
#endif /* EFI_PROD_CODE */

	setDefaultVrThresholds();

}

void setPPSInputs(adc_channel_e pps1, adc_channel_e pps2) {
	engineConfiguration->throttlePedalPositionAdcChannel = pps1;
	engineConfiguration->throttlePedalPositionSecondAdcChannel = pps2;
}

void setTPS1Inputs(adc_channel_e tps1, adc_channel_e tps2) {
	engineConfiguration->tps1_1AdcChannel = tps1;
	engineConfiguration->tps1_2AdcChannel = tps2;
}

void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax) {
	engineConfiguration->tpsMin = tpsMin;
	engineConfiguration->tpsMax = tpsMax;
}

void setTPS1Calibration(uint16_t tpsMin, uint16_t tpsMax, uint16_t tps1SecondaryMin, uint16_t tps1SecondaryMax) {
    setTPS1Calibration(tpsMin, tpsMax);


	engineConfiguration->tps1SecondaryMin = tps1SecondaryMin;
	engineConfiguration->tps1SecondaryMax = tps1SecondaryMax;
}

void setCustomMap(float lowValue, float mapLowValueVoltage, float highValue, float mapHighValueVoltage) {
	engineConfiguration->map.sensor.type = MT_CUSTOM;
	engineConfiguration->map.sensor.lowValue = lowValue;
	engineConfiguration->mapLowValueVoltage = mapLowValueVoltage;
	engineConfiguration->map.sensor.highValue = highValue;
	engineConfiguration->mapHighValueVoltage = mapHighValueVoltage;
}

void setPPSCalibration(float primaryUp, float primaryDown, float secondaryUp, float secondaryDown) {
	engineConfiguration->throttlePedalUpVoltage = primaryUp;
	engineConfiguration->throttlePedalWOTVoltage = primaryDown;
	engineConfiguration->throttlePedalSecondaryUpVoltage = secondaryUp;
	engineConfiguration->throttlePedalSecondaryWOTVoltage = secondaryDown;
}

void setEtbPID(float p, float i, float d) {
	engineConfiguration->etb.pFactor = p;
	engineConfiguration->etb.iFactor = i;
	engineConfiguration->etb.dFactor = d;
}

void setInline4() {
	engineConfiguration->cylindersCount = 4;
	engineConfiguration->firingOrder = FO_1_3_4_2;
}

void setProteusEtbIO() {
#if HW_PROTEUS && EFI_PROD_CODE
	engineConfiguration->tps1_2AdcChannel = PROTEUS_IN_TPS1_2;
	setPPSInputs(PROTEUS_IN_PPS, PROTEUS_IN_PPS2);
#endif // HW_PROTEUS
}
