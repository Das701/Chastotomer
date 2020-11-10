#pragma once


namespace FreqMeter
{
    // Загрузка режима измерения частоты
    void LoadModeMeasureFrequency();

    // Загрузка режима измерения периода
    void LoadModeMeasurePeriod();

    // Загрузка режима измерения длительности
    void LoadModeMeasureDuration();

    // Загрузка режима измерения счёта импульсов
    void LoadModeMeasureCountPulse();

    // Загрузка периода меток времени
    void LoadPeriodTimeLabels();

    // Загрузка времени измерения
    void LoadTimeMeasure();

    // Загрузка числа периодов измерения
    void LoadNumerPeriodsMeasure();

    // Загрузка входного сопротивления по текущему каналу
    void LoadImpedance();

    void LoadModeFront();

    void LoadDivider();

    void LoadTypeSynch();
    
    void LoadChannel();
    
    void LoadMemoryMode();
    
    void LoadCalibration();
    
    void LoadLaunchSource();
    
    void LoadRefGenerator();
    
    void LoadDisplayTime();
    
    // Переключить режим "Тест"
    void SwitchModeTest();
    
    void LoadAuto();
    
    void LoadMeasure();
    
    void UnloadAuto();
    
    void LoadOneTime();
    
    void LoadStartStop();
}
