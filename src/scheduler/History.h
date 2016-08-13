#ifndef  HISTORY_H
#define HISTORY_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "../Serializable.h"

struct myJsonObject
{
  const JsonObject& refence;
};

struct myJsonArray
{
  myJsonArray(const JsonArray& r) : refence(r) { }
  const JsonArray& refence;
};

template <class T>
class Record
{
  // We don't have enough memory to hold this object in memory
public:
  Record();
  Record(unsigned long timestamp, T value);
  void fromJson(JsonObject& json);
  JsonObject& toJson();
  unsigned long timestamp;
  T value;
};

class History
{
public:
  History();
  void fromJson(JsonArray& json);
  myJsonArray toJson(Print &stream);
  void addValue(unsigned short value);

protected:
  static const int N = 60;
  //Record<T> _records[N];
  unsigned short _records[N];
  int _last;
  int _next(int n);
};
#endif
