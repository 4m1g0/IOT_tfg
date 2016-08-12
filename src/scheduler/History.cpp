#include <Arduino.h>
#include "History.h"
#include "../Clock.h"
#include <ArduinoJson.h>

template <class T>
Record<T>::Record()
: timestamp(0)
, value(0)
{ }

template <class T>
Record<T>::Record(unsigned long timestamp, T value)
: timestamp(timestamp)
, value(value)
{ }

template <class T>
JsonObject& Record<T>::toJson()
{
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root.set<long>("t", timestamp);
  root.set<T>("v", value);
  return root;
}

template <class T>
void Record<T>::fromJson(JsonObject& json)
{
  timestamp = json.get<unsigned long>("t");
  value = json.get<float>("v");
}

History::History()
: _last(0)
{
  for (int i = 0; i < N; i++)
    _records[i] = 0;
}

void History::addValue(unsigned short value)
{
  _last = _next(_last);
  _records[_last] = value;
}

JsonArray& History::toJson()
{
  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();
  for (int i = _next(_last); i != _last; i = _next(i))
    root.add<unsigned short>(_records[i]);

  root.add<unsigned short>(_records[_last]);
  return root;
}

int History::_next(int n)
{
  return ++n%N;
}

// This tells the compiler which types are going to be used so that it can
// compile a instance of this class for that especific type.
// Thanks for this to Aaron McDaid (http://stackoverflow.com/questions/8752837)
//template class History<float>;
template class Record<float>;
