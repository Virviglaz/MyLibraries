#ifndef KALMAN_H
#define KALMAN_H

typedef struct
{
  float Result;                 //�������������� ��������
  float Value;                  //�������� ��������
  float Previous;               //��������� ���������� � ���������� ���������
  float K;                      //����������� ������������, ��-��������� = 0.1
}KalmanFloatStructTypeDef;

typedef struct
{
  unsigned int Result;          //�������������� ��������
  unsigned int Value;           //�������� ��������
  unsigned int Previous;        //��������� ���������� � ���������� ���������
  float K;                      //����������� ������������, ��-��������� = 0.1
}KalmanStructTypeDef;

void KalmanFloatCalc (KalmanFloatStructTypeDef * KalmanFloatStruct);
void KalmanCalc (KalmanStructTypeDef * KalmanFloatStruct);
#endif
