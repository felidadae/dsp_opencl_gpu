//
//  language_pl.h
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 08.10.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef DSPOpenCLGPUTester_language_pl_h
#define DSPOpenCLGPUTester_language_pl_h



#define PROPERTIE_NAME__EXPERIMENT_TITLE "Tytuł eksperymentu"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_AUDIO_TEST "Test odsłuchowy implementacji filtrujących"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_GPU_GENERAL "Ogólny test implementacji filtrującej OpenCL GPU"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_COMPARISON "Porównanie implementacji filtrujących OpenCL GPU i CPU"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_MAX "Maksymalne osiągi implementacji filtrującej"
//---
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__AUDIO_TEST "testy odsłuchowe implementacji kopiującej"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__GANGED_OR_MULTI_CHANNEL "testy porównawcze implementacji kopiujących"
#define PROPERTIE_VALUE__EXPERIMENT_TITLE__MINIMUM_BUFFER_SIZE "testy czasowe lepszej implementacji kopiującej"
//---
#define PROPERTIE_NAME__WORK_NAME "Praca wykonana przez DSP"
#define PROPERTIE_VALUE__WORK_NAME__SIMPLY_DSP "kopiowanie wejścia do wyjścia"
#define PROPERTIE_VALUE__WORK_NAME__FILTERING "filtracja"
//---
#define PROPERTIE_NAME__CONVOLUTIONER "Nazwa splotera"
#define PROPERTIE_VALUE__CONVOLUTIONER__CPU__DEFINITION_TIME_DOMAIN "procesor CPU jednordzeniowe/dziedzina czasu (splot z definicji)"
#define PROPERTIE_VALUE__CONVOLUTIONER__CPU__UNIFORMLY_PARTITIONED "procesor CPU, jeden rdzeń/jednorodny podział odpowiedzi impulsowej"
#define PROPERTIE_VALUE__CONVOLUTIONER__OPENCL_GPU__UNIFORMLY_PARTITIONED "GPU OpenCL/jednorodny podział odpowiedzi impulsowej"
//---
#define PROPERTIE_NAME__IRFC "Typ dostarczyciela odpowiedzi impulsowej"
#define PROPERTIE_VALUE__IRFC__LOW_PASS "dolnoprzepustowy"
#define PROPERTIE_VALUE__IRFC__FROM_FILE "pobieranie z pliku"
#define PROPERTIE_VALUE__IRFC__TESTING_RANDOM "testowy, dane losowe"
//---
#define PROPERTIE_NAME__GANGED_OR_MULTI_BUFFER "Buffor OpenCL zbiorczy czy oddzielny na każdy kanał"
#define PROPERTIE_VALUE__GANGED_OR_MULTI_BUFFER__MULTI "oddzielny"
#define PROPERTIE_VALUE__GANGED_OR_MULTI_BUFFER__GANGED "zbiorczy"
//---
#define PROPERTIE_NAME__LENGTH_IN_SECONDS "Długość pliku wejściowego[s]"
#define PROPERTIE_NAME__REPETITION "Ilość bloków przetworzonych"
#define PROPERTIE_NAME__BLOCKS_SIZE "Wielkość bloku"
#define PROPERTIE_NAME__SAMPLE_RATE "Częstotliwość próbkowania"
#define PROPERTIE_NAME__NUMBER_OF_CHANNELS "Ilość kanałów pliku wejściowego"
#define PROPERTIE_NAME__LATENCY "Latencja[ms]"
//---
#define PROPERTIE_NAME__GPU_THREADS_NUMBER "Ilość wątków GPU"
#define PROPERTIE_NAME__ALLOCATED_GPU_MEMORY "Zaalokowana pamięć GPU"
//-
#define PROPERTIE_NAME__AVERAGE_TIME_PER_SAMPLE "Średni czas na jeden sampel[micros]"
#define PROPERTIE_NAME__AVERAGE "Średni czas[ms]"
#define PROPERTIE_NAME__MINIMUM "Wartość minimalna[ms]"
#define PROPERTIE_NAME__MEDIAN "Mediana[ms]"
#define PROPERTIE_NAME__99PROCENT "99% wartości poniżej[ms]"
#define PROPERTIE_NAME__MAXIMUM "Wartość maksymalna[ms]"
#define PROPERTIE_NAME__ON_TIME "Ilość bloków przetworzonych na czas"
#define PROPERTIE_NAME__PERCENTAGE_ON_TIME "Ilość bloków na czas [%]"
//-
#define PROPERTIE_NAME__IMPULSE_RESPONSE_N "Długość odpowiedzi impulsowej[s]"
#define PROPERTIE_NAME__IMPULSE_RESPONSE_C "Ilość kanałów odpowiedzi impulsowej"
#define PROPERTIE_NAME__MAIN_KERNEL_LOOP_ITERATION "Ilość obiegów pętli"



#endif
