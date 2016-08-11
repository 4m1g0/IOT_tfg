#include <Arduino.h>
#include "History.h"
#include "../Clock.h"
#include <ArduinoJson.h>

template <class T>
JsonObject& Record<T>::toJson()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<long>("t", timestamp);
  root.set<T>("v", value);
  return root;
}

// This tells the compiler which types are going to be used so that it can
// compile a instance of this class for that especific type.
// Thanks for this to Aaron McDaid (http://stackoverflow.com/questions/8752837)
template class Record<float>;

template <class T>
void Record<T>::fromJson(JsonObject& json)
{
  timestamp = json.get<long>("t");
  value = json.get<float>("v");
}

template <class T>
int History<T>::_next(int n)
{
  return ++n/N;
}

template <class T>
void History<T>::addValue(T value)
{
  Record<T> record;
  record.timestamp = Clock::getUnixTime();
  record.value = value;

  _records[_last] = record;
  _last = _next(_last);
}
