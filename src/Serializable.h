#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H
#include <Arduino.h>
#include <ArduinoJson.h>

template <class T>
class Serializable
{
public:
  virtual void toJson(T json) = 0;
  virtual void fromJson(T json) = 0;
};


#endif
